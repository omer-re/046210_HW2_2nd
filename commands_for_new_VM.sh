cd /usr/src/linux-2.4.18-14custom
make
make modules
make modules_install
make install
printf('/a')
reboot