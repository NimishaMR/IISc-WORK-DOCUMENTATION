**CORE ISOLATION USING ISOLCPU:**

1)Link followed:

https://www.baeldung.com/linux/managing-processors-availability


Scroll down to Permanent isolation:
```bash
sudo nano /etc/default/grub
#add this command to the grub file
GRUB_CMDLINE_LINUX="isolcpus=2"
sudo update-grub
sudo reboot
```

This command ensures that CPU 2 will not be used for anything unless assigned to a task using the taskset command

2)Command to verify that cpu 2 is idle whereas other cpus arent is:
```bash
sar -P 0-3 1 70
```

This monitored cores 0,1,2,3 for 70sec and it was observed that cpu 2 was not used at all

3)Next task is to assign our target program to the isolated cpu
this has been done using taskset command
```bash
taskset -c 2 ./timeloggingsw2.sh
```

every time this command is run it ensures that the file timeloggingsw2 runs only under cpu core 2

4)again if the file is run without the taskset command we can observe that it may be using any other core or a combination of different cores

5)This is the command to check which core our target file is using:
```bash
 ps -e -o pid,psr,cpu,cmd | grep -E  "^[[:space:]][[:digit:]]+[[:space:]]" | grep timelog
```

6)The date command logging in SWITCH 2 has been observed for 30 minutes with core isolation and without core isolation and has been plotted

7)To change the grub settings after the experiment :

Type this :
```bash
1)sudo nano /etc/default/grub
2)GRUB_CMDLINE_LINUX="isolcpus=2-3" should be changed to GRUB_CMDLINE_LINUX=""
3)to update the changes run sudo update-grub and reboot the system
4)after system is rebooted again check sudo nano /etc/default/grub to see if the isolcpus is reset to ""
```
