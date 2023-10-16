**DISCLAIMER:** This file has the solutions and some links to solve some common errors which coe while setting up or implementing DPDK.
The one's documented here are the errors which I came across , there may be few other errors which are different from mine as well.


Befor anything else:  if your ip a command is not showing the multinic : then:
```bash
1)either SFP Module is not loaded:To solve this issue refer to my POSSIBLE ERRORS AND CORRECTONS FILE:
2)Ixgbe drive module may have to reinstalled:
sudo modprobe -r -v ixgbe
sudo modprobe -v ixgbe
3)Or last case:the pysical driver ,module maybe corrupted ,have to replace it
```

**1)IF A ETHERNET NIC IS NOT VISIBLE ON IP A/IFCONFIG BUT ITS VISIBLE ON LSCPI HOW TO SOLVE IT:**

When I gave  ip -a in the terminal and a particular etheret card was not being identified ,namely:
INTEL NIC X520 -D2 port eth0 and eth1:
00:03:00.0 and 00:03:00.1

To debug the issue execute the following commands:
```bash
1)first check if the particluar device is being recognised by the PCI bus:
```
lspci

if yes:

2)unload adn reload the kernel module by :
```bash
rmmod ixge
sudo modprobe ixgbe
```

3)to check if the module has been loaded 
```bash
lsmod
modinfo ixgbe
```

4)to get more info about network devices:
```bash
sudo lshw -C network
or 
sudo lspci -nnvmm | egrep -A 6 -B 1 -i 'network|ethernet'
(for eternet adapters)
sudo lspci -vvv (gives info about the driver being sued by the card)
```

if kernel module is also non erroneous:

3)Check if the network device is listed in the /sys/class/net folder:
if its not present there then theres a problem because your device isnt getting recognised as
a network devcie

what to do now?

A)tried reloading the firmware:
to get data about the firmware being used by all the cards on the d=cpu execute: sudo dmidecode

B)but for the NIC we were using which is: INTEL X520 D2-The firmware cant be reloaded or updated 
this information is available in the following links:

https://www.intel.com/content/www/us/en/support/articles/000027683/ethernet-products/500-series-network-adapters-up-to-10gbe.html
https://www.reddit.com/r/homelab/comments/8btmo8/updating_intel_x520da2_firmware/
https://community.intel.com/t5/Ethernet-Products/x520-2-new-firmware/td-p/518677  : how to update firmware 

execute the line : 
```bash
ethtool -i 03:00.0 (in /sys/class/net)
```
if this command gives error as : Cannot get driver information: No such device

then follow this link:

https://bobcares.com/blog/ethtool-no-such-device-error/

the solution to this error can be solved by:


The first step to start troubleshooting the issue would be to check the dmesg output. dmesg is a command that Linux 
operating systems use to print the message buffer of the kernel. Since we here need the output related to an interface, 
we can use the following command to collect the required details:

execute : 
```bash
dmesg | grep 03:00.0
```
this says the error is ixgbe 0000:03:00.0: failed to load because an unsupported SFP+ or QSFP module type was detected.
ixgbe 0000:03:00.0: Reload the driver after installing a supported module.


C)so the error is because of an unsupported SFP+ module:
this is solved in this link:

https://www.serveradminz.com/blog/unsupported-sfp-linux/

FINAL SOLN
executed the following lines:
```bash
1) sudo nano /etc/modprobe.d/ixgbe.conf
2)in this text editor: type: options ixgbe allow_unsupported_sfp=1
3)then unload and reload the firmware:
rmmod ixgbe; modprobe ixgbe 

4)now do ip-a (you can see the network card now)


TO MAKE THIS A PERMANENT SOLUTION:
1)sudo nano /etc/default/grub
2)GRUB_CMDLINE_LINUX=”ixgbe.allow_unsupported_sfp=1″ 
3)grub-mkconfig -o /boot/grub/grub.cfg  (to update the grub)
4)rmmod ixgbe && modprobe ixgbe  
5)sudo reboot 

6)then when the computer switches on: ifconfig
```
the ports reoccur

**2)Not able to edit sys/bus/pci/drivers while binding & unbininding ports.**

https://bbs.archlinux.org/viewtopic.php?id=173773

You're getting permission denied because of what you're trying to echo, 
not because of file permissions.  As to why vfio-pci vs pci-stub, vfio is a new userspace driver 
interface where qemu is just a userspace driver using it for device assignment.  
```bash
echo "8086 10b9" > /sys/bus/pci/drivers/pci-stub/new_id
echo 0000:01:00.0 > /sys/bus/pci/devices/0000:01:00.0/driver/unbind
echo 0000:01:00.0 > /sys/bus/pci/drivers/pci-stub/bind

chmod 666 /sys/bus/pci/drivers/pci-stub/new_id
```
-----------------------------------------------------------------------------------------------------------------------------------------------------

3)Unable to bind intel NIC 82599ES to vfio-pci, uio_pci_generic

https://mails.dpdk.org/archives/users/2017-February/001542.html
https://mails.dpdk.org/archives/users/2017-February/001544.html

https://inbox.dpdk.org/dev/8fa1aae5-617c-64a7-a647-fd2508e0f662@intel.com/T/
The reason behind this is that I am unable to write in /sys/bus/pci/drivers/vfio-pci/bind. Upon searching solutions
I tried a couple of things such as setting iommu=pt and intel_iommu=on and ensured vt-d is enabled.
Along with this I have made sure that the vfio-pci module is correctly loaded. I have also tried
```bash
chmod 666 /sys/bus/pci/drivers/vfio-pci/bind
```
So that I have permissions to write in this file.

First thing: to check if VT-d is enabled in Linux, you can read MSR 0x3a register. Its lowest bit set will show VT-d works:

(optionally on Ubuntu: apt-get install msr-tools)
```bash
modprobe msr
rdmsr 0x3a
3 or 5 -> OK
```
Other possible way to check for VT-d (IOMMU) working is to view the booting log (dmesg):
```bash
dmesg | grep DMAR
"DMAR: IOMMU enabled" -> OK
```
Both of these should be met in order to use vfio-pci in the next step.
One possible problem that it could be, is that you will need to ensure that
any other ports on the same device are either similarly bound to vfio-pci
or not bound to any driver. You can't have e.g. a 2-port X722 NIC where one
port is bound to the kernel driver, while another is bound to vfio in
userspace.

To me, it starts to sound more and more like some kind of security 
measure. Either it's something like AppArmor/SELinux preventing you from 
binding the drivers, or maybe it's something like Secure Boot, or some 
other security-related feature. A "permission denied" error is usually 
indicative of such things. either Secure  Boot-related settings, or SELinux/AppArmor logs.

try:
```bash
1)dpdk-devbind --status
2)dpdk-devbind -b vfio-pci 01:00.1
3)modprobe vfio-pci
4)confirm via lsmod
5)bind with sudo ./usertools/dpdk-devbind.py -b vfio-pci [PCIe B:D:F]
```

note:




**4)Changing the GRUB settings to enable iommu on:**

   https://www.pugetsystems.com/labs/hpc/how-to-use-linux-kernel-boot-options/

**5)Intel VFIO-PCI Driver Error -22**

   https://inbox.dpdk.org/ci/8c696743-5640-9ad9-aba4-916ca8858f64@intel.com/t/

   have "intel_iommu=on iommu=pt" in their kernel parameters.


**6)BIND/UNBIND PCI device on ubuntu host--all about unbinding and binding**

   https://serverfault.com/questions/1080606/bind-unbind-pci-device-on-the-ubuntu-host

7)**Using lspci on Linux to see display devices and drivers on your linux system:**

  https://opensource.com/article/21/9/lspci-linux-hardware

**8)J)LINKS TO SOLVE THE IOMMU  ISSUE**

https://www.goglides.dev/bkpandey/what-is-iommu-how-to-check-iommu-is-enabled-1p7c
https://forum.proxmox.com/threads/iommu-enabled-but-not-detected.110417/
https://superuser.com/questions/1112238/intel-iommu-on-but-no-iommu-groups
https://ubuntuforums.org/showthread.php?t=2394933
https://www.reddit.com/r/virtualization/comments/bh0y6m/iommu_groups_empty/
https://doc.dpdk.org/guides-21.11/linux_gsg/linux_drivers.html



In certain situations, using dpdk-devbind.py script to bind a device to VFIO driver may fail. The first place to check is the kernel messages:

dmesg | tail
...
[ 1297.875090] vfio-pci: probe of 0000:31:00.0 failed with error -22
...
In most cases, the error -22 indicates that the VFIO subsystem could not be enabled because there is no IOMMU support.
To check whether the kernel has been booted with correct parameters, one can check the kernel command-line:
cat /proc/cmdline

If the kernel is configured correctly, one also has to make sure that the BIOS configuration has virtualization features (such as Intel® VT-d). 
There is no standard way to check if the platform is configured correctly, so please check with your platform documentation to see if it has 
such features, and how to enable them.

In certain distributions, default kernel configuration is such that the no-IOMMU mode is disabled altogether at compile time. T
his can be checked in the boot configuration of your system:
cat /boot/config-$(uname -r) | grep NOIOMMU

# CONFIG_VFIO_NOIOMMU is not set

If CONFIG_VFIO_NOIOMMU is not enabled in the kernel configuration, VFIO driver will not support the no-IOMMU mode, and other alternatives 
(such as UIO drivers) will have to be used.

