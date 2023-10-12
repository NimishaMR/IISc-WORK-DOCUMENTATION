**This file points you to some super helpful links to read if you're new to F-Trace**

1)About FTrace : https://lwn.net/Articles/365835/
                 https://jvns.ca/blog/2017/03/19/getting-started-with-ftrace/

2)Finding the trace output for only a particular process using the PID feature:

https://unix.stackexchange.com/questions/290467/filter-out-certain-processes-and-or-pids-in-ftrace


3)Track a packet as it goes throught he linux kernel : 

https://www.alibabacloud.com/blog/surftrace-an-open-source-kernel-tracking-tool-that-increased-protocol-package-resolution-efficiency-by-ten_599302

https://www.brendangregg.com/blog/2015-07-08/choosing-a-linux-tracer.html
(**very good read**)

https://jvns.ca/blog/2017/03/19/getting-started-with-ftrace/

udp latency measurement for linnux using ftrace:
https://stackoverflow.com/questions/33259744/udp-latency-measurement-for-linux-using-ftrace

https://lwn.net/Articles/365835/

https://opensource.com/article/21/7/linux-kernel-trace-cmd

https://www.slideshare.net/ScyllaDB/osnoise-tracer-who-is-stealing-my-cpu-time

4)To change clock source in linux:
 ```bash
cd /sys/devices/system/clocksource/clocksource0/
available_clocksource
current_clocksource
echo hpet > current_clocksource
```
5)About GNOMON for tracing: https://github.com/paypal/gnomon

```bash
sudo apt install npm
npm install -g gnomon
#find out the time taken to execute date command using gnomon
date | gnomon --type=elapsed-total --ignore-blank


```

6)About IRQS TRACER:
  https://stackoverflow.com/questions/51504616/why-there-is-no-irqsoff-option-in-the-ftrace

  https://cateee.net/lkddb/web-lkddb/IRQSOFF_TRACER.html

7)About sysdig:
 https://www.appservgrid.com/paw92/index.php/2019/04/06/sysdig-a-powerful-system-monitoring-and-troubleshooting-tool-for-linux/

 https://sysdig.com/blog/sysdig-tracers/

8)About PERF:

https://www.swift.org/server/guides/linux-perf.html

perf command runs in user mode
all the values we are seeing in perf are only scheduler times


**very useful read on perf:**
https://www.brendangregg.com/perf.html


https://github.com/brendangregg/perf-tools

https://www.brendangregg.com/blog/2017-03-16/perf-sched.html

about perf timers: https://stackoverflow.com/questions/66974440/what-timeframe-does-the-perf-sched-record-use

https://perf.wiki.kernel.org/index.php/Tutorial#Hardware_events_2

https://man7.org/linux/man-pages/man1/perf-sched

```bash
perf stat date
perf stat -e r1a8 -a date
perf list
```

```bash
perf ftrace date
perf sched record -- date
perf sched timehist
perf sched latency
```

   
