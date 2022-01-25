Compile mySysStat.c
    gcc -Wall mySysStat.c -o mySysStat -lm

About printing priority:
    I've used ESCape code to make sure data printed at speicif line,
    but I didn't figure out how to print rest of information while waiting
    memory use and cpu use to be printed.

    Sorry, you have to wait to see the rest information.

Flags: 
    - Flag's priority is always bigger than default.
        Ex:
            ./myprogram --samples=8 --tdelay=2 10 1
            -> myprogram will run 8 times with 2 sec delay.
    - Must follow the order sample tdelay, sample is always excuted first in any order
        Ex:
            ./myprogram -g -s 10 1  -> sample=10, tdelay=1
            ./myprogram -g -s 10 -u 1  -> sample=10, tdelay=1
            ./myprogram -g -s 1 -u 10 -> sample=1, tdelay=10
    - If multiple flags used, will only excute the last one
        Ex:
            ./myprogram -g -s -u    show only user
            ./myprogram -g -u -s    show only system
    - If no sample and tdelay flags, but with one number only, 
        program will set the number to sample.
    - Allow shorcuts for --system, --user, --graphics -> -s, -u, -g
    - Allow typos like --samples -> --sample, --tdelay -> --delay, --graphics -> --graphic

To make it sample, I moved all the general header files to mySysStat.c

How I solved the assignment:

Calculate cpu use % by this logic (see more details at https://stackoverflow.com/a/3017438)
Read first line of "/proc/stat":
    Suppose at 14:00:00 you have:

        cpu 4698 591 262 8953 916 449 531

        total_jiffies_1 = (sum of all values) = 16400

        work_jiffies_1 = (sum of user,nice,system = the first 3 values) = 5551

    and at 14:00:05 you have:

        cpu 4739 591 289 9961 936 449 541

        total_jiffies_2 = 17506

        work_jiffies_2 = 5619

    So the %cpu usage over this period is:

        work_over_period = work_jiffies_2 - work_jiffies_1 = 68

        total_over_period = total_jiffies_2 - total_jiffies_1 = 1106

        %cpu = work_over_period / total_over_period * 100 = 6.1%

Calculate memory use struct sysinfo *info, called from <sys/sysinfo.h>:
    physic memory  = ram, virtual memory = ram + swap

Call sysconf() "unistd.h" to get/print number of cores in the system
Call getrusage() "sys/resource.h" to get/print memory usage
USe struct sysinfo *info "sys/sysinfo.h" to print memory used information
USe struct utmp *buf "utmp.h" to print user information under user usage
Use struct utsname *name "sys/utsname.h" to print system information under system usage
Get/print CPU use rate by reading "/proc/stat", get data at first line to solve the problem

If you want to see my non-copy code -> liujunxu@iits-b473-13:~/cscb09w22_space/assignment1

