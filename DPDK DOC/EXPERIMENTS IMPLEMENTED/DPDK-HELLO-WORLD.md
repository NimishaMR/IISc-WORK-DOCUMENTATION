**NOTE: Before executing all these make sure that you have installed all the dependencies as mentioned in the "Installations and dependencies folder"**

**RUN EVERYTHING AS ROOT FOR A SMOOTH FLOW WITHOUT ERRORS**

1)Download the latest stable version of DPDK from this link: https://core.dpdk.org/download/

2)Uncompress the archive :
tar xJf dpdk-23.03.tar.xz(or whichever version you download)

3)cd dpdk-23.03

4)sudo apt install meson

5)sudo apt install build-essential

6)pip3 install pyelftools

7)meson -Dexamples=all build
When -Dexamples=all is set as a meson option, meson will check each example application to see if
it can be built, and add all which can be built to the list of tasks in the ninja build configuration file.

9)cd build

10)ninja

11)ninja install

12)sudo ldconfig

13)pkg-config --modversion libdpdk 

The above command should return the DPDK version installed.If not found, export the path to the installed DPDK libraries:
$ export PKG_CONFIG_PATH=/path/to/installed/".pc" file/for/DPDK

14)#command sto reserve hugepages for dpdk

  echo 1024 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
15)dpdk-hugepages.py -s

16)mkdir /mnt/huge

17)mount -t hugetlbfs pagesize=1GB /mnt/huge

  then: sudo nano /etc/fstab #to edit  the fstab file to make the mount point permanent:
  added a line in the end 
  nodev /mnt/huge hugetlbfs pagesize=1GB 0 0 

18)mount -a #command to mount all filesystems listed in fstab file 
  command to verify that mnt/huge has been mounted to the fstab:
  findmnt(here the output should have mnt/huge

19)#finally to read the total amount of hugepages which are allocated:
  grep Huge /proc/meminfo

20)sudo modprobe uio_pci_generic
21)#command to unbind the enp1s0f0 from ixgbe driver before binding it to uio :
./usertools/dpdk-devbind.py -u  0000:01:00.0

22)#command to bind it to uio_pci_generic driver:
./usertools/dpdk-devbind.py --bind=uio_pci_generic 0000:01:00.0

23)next check the devbind status again to verify by runnning:
./usertools/dpdk-devbind.py --status

24)#commands to run helloworld application: (prints helloworld from different cores ,just to verify if EAL has been set up properly)
   cd build
   cd examples
   ./dpdk-helloworld -l 0-3 -n 4

   **The output looks like this**
   

25)If you want you can also allocate certain cores for dpdk use only, this can be done by:
   isolcpus=2,3,4,5,6,7,8
