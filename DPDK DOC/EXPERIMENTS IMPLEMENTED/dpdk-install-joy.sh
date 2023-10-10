#!/bin/bash

sudo apt install build-essential meson ninja-build python3-pyelftools libnuma-dev libarchive-dev git -y
sudo apt install libarchive-dev libelf-dev

echo "Install Driver-specific dependencies"
# https://doc.dpdk.org/guides-21.11/nics/index.html

sudo apt install pkg-config # or pkgconf

# You may require linux kernel headers to build dpdk
sudo apt install linux-headers-$(uname -r)

# DPDK can also work with libpcap driver for low speed packet capture without real NIC.
sudo apt install libpcap-dev

# Install DPDK
# https://docs.openvswitch.org/en/latest/intro/install/dpdk/
# https://levelup.gitconnected.com/building-a-high-performance-linux-based-traffic-generator-with-dpdk-93bb9904416c
cd /usr/src/
export DPDK_VER=22.11.2
wget https://fast.dpdk.org/rel/dpdk-${DPDK_VER}.tar.xz
tar xf dpdk-${DPDK_VER}.tar.xz
sudo rm -fr /usr/local/lib/x86_64-linux-gnu 

export DPDK_DIR=/usr/src/dpdk-stable-${DPDK_VER}
cd $DPDK_DIR

# Build DPDK
export DPDK_BUILD=$DPDK_DIR/build
meson setup -Dexamples=all build
ninja -C build
ninja -C build install
ldconfig

export PKG_CONFIG_PATH=/usr/local/lib/x86_64-linux-gnu/pkgconfig
# The above command should return the DPDK version installed.
pkg-config --modversion libdpdk

# Hugepages is required. Ubuntu has it on by default, confirm using
grep -i huge /boot/config-$(uname -r)
echo 2048 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
grep -i HugePages_ /proc/meminfo


# DPDK takes control of full NIC, if you need to share NIC with other programs too, setup SR-IOV. Since we are using a separate NIC (Intel multi-NIC) for DPDK, we are good to go. So for VFIO, do the following steps.
# i.e DPDK requires passthrough (Intel VT-d) and may need SR-IOV. You can skip SR-IOV for now.
sudo nano /etc/default/grub
# add following options to this macro variable: GRUB_CMDLINE_LINUX_DEFAULT="intel_iommu=on iommu=pt"
sudo update-grub
# To confirm above commands are executed correctly and if passthrough can be used,
ls /sys/kernel/iommu_groups # Should have some numbers as directories

# You may need to restart to see effect, and also confirm using
dmesg | grep -i iommu
dmesg | grep "Virtualization Technology for Directed I/O" 
dmesg | grep -e DMAR -e IOMMU


#Load the vfio module
sudo modprobe vfio-pci

#Find out the binding status of the ports
./usertools/dpdk-devbind.py --status

#Unbind the port from kernel(port number should be identified from ip a ) 
./usertools/dpdk-devbind.py -u  0000:01:00.0

#Bind the same port to dpdk vfio driver
./usertools/dpdk-devbind.py --bind=uio_pci_generic 0000:01:00.0


# For pktgen-dpdk
# Pktgen vs MoonGen
# Moongen: Have a warm-up phase https://github.com/emmericp/MoonGen/issues/213
# MoonGen: A Scriptable High-Speed Packet Generator (2015) - Has comparison with Pktgen-DPDK
# Mind the Gap – A Comparison of Software Packet Generators (2017) - Same as above and studies to compare performance of hardware assist. Also talks about supported NICs like Intel X710 which we have.
git clone https://dpdk.org/git/apps/pktgen-dpdk
cd pktgen-dpdk
make build
