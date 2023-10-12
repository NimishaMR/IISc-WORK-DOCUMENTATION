**This file contains the errors I encountered and how I reslved it with relevent links**

1)PROBLEM WITH PERF INSTALLATION :

**A**) The versionon of perf that was getting executed was 
4.15.0-211-generic(do zenlab@tsn-switch2:/usr/lib/linux-tools$ ls)
but linux kernel  version is 4.15.0-041500-generic

Unable to change the version so i created a softlink to another version of perf and associated it there:https://askubuntu.com/questions/56339/how-to-create-a-soft-or-symbolic-link
/usr/lib/linux-tools

https://askubuntu.com/questions/56339/how-to-create-a-soft-or-symbolic-link

```bash
/usr/lib/linux-tools

sudo ln -s /usr/lib/linux-tools/4.15.0-211-generic/perf /usr/bin/perf0>>creates new version of perf:perf0-
```

use this version of perf(perf0)only when you want to execute some other command before perf command like ex: 

```bash
/usr/lib/linux-tools# taskset -c 2 perf0 sched record -- date
```

**B**)Perf is not running properly so to make perf run properly,do this: https://www.swift.org/server/guides/linux-perf.html  

```bash
alias perf=$(find /usr/lib/linux-tools/*/perf | head -1)
```

2)If set filter option isnt available then

```bash
cd proc
cd sys
cd kernel
cat stack_tracer_enabled
echo 1 > stack_tracer_enabled
cat stack_tracer_enabled
```




