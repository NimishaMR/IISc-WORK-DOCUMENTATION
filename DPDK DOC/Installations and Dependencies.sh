#Packages to be installed:

apt-get -y update
apt-get -y upgrade
apt-get -y install python2.7
apt-get -y install build-essential ninja-build meson cmake
apt-get -y install libnuma-dev
apt-get -y install pciutils
apt-get -y install libpcap-dev
apt-get -y install liblua5.3-dev
apt-get -y install python3-pyelftools
apt -y install libelf-dev
apt-get -y install linux-headers-`uname -r` || apt -y install  linux-headers-generic


