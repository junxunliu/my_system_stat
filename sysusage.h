#ifndef SYSUSAGE_H
#define SYSUSAGE_H

#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/resource.h>

/**
 * @brief print current system information
 * 
 */
void printSystemInfo()
{

    struct utsname buf;

    if (uname(&buf) < 0)
    {
        perror("uname");
    }

    printf("    System name  = %s\n", buf.sysname);
    printf("    Node name    = %s\n", buf.nodename);
    printf("    Release      = %s\n", buf.release);
    printf("    Version      = %s\n", buf.version);
    printf("    Architecture = %s\n", buf.machine);
}

/**
 * @brief generate the bars for memory used, indicate
 * relative increase/decrease memory and curr memory used
 * @param prev previous used virtual memory
 * @param curr current used virtual memory
 */
void generateMemBars(long double prev, long double curr)
{
    // prev = roundl(prev * 100) / 100;
    // curr = roundl(prev * 100) / 100;

    double diff = curr - prev;
    double absDiff = fabs(diff);
    int intDiff = absDiff;
    int hashtags = 0, bars = 0;
    int check;
    char sign;

    if (absDiff >= 1)
    {
        bars += intDiff * 10;
        absDiff = absDiff - intDiff;
    }

    if (absDiff < 0.2)
    {
        hashtags += diff * 100;
    }

    else
    {
        check = absDiff / 0.1;
        if (check % 2 == 0)
        {
            hashtags += fmod(absDiff, 0.1);
            bars += check;
        }

        else
        {
            hashtags += fmod(absDiff, 0.1) + 10;
            bars += check - 1;
        }
    }

    if (diff < 0)
    {
        sign = '-';
    }

    else
    {
        sign = '+';
    }

    printf("   ");
    for (int i = 0; i < bars + 1; i++)
    {
        printf("|");
    }
    for (int i = 0; i < hashtags; i++)
    {
        printf("#");
    }
    printf("%c %.2f (%.2Lf)\n", sign, absDiff, curr);
}

/**
 * @brief a method to print memory usage at one time
 */
void printMemoryUsage()
{
    struct rusage usage;
    int ret = getrusage(RUSAGE_SELF, &usage);

    if (ret == 0)
        printf("    Memory usage: %ld kilobytes\n", usage.ru_maxrss);
    else
        printf("Error in getrusage. errno = %d\n", errno);
}

/**
 * @brief Get the Memory Used Info object *sample times whit *tdelay second, 
 * show relative graphs, print them behind if flag is checked
 * @param sample how many times should be printed
 * @param tdelay how lone to print 1 line
 * @param graphicsFlag check if the graphics flag is on
 */
void getMemoryUsedInfo(long *sample, long *tdelay, bool graphicsFlag)
{
    struct sysinfo info;
    // a constant double help convert byte to GB
    const float d = 1024.00 * 1024.00 * 1024.00;
    long double usedPhys, totalPhys, usedVirt, totalVirt;
    long double prevUsed = 0;
    long times = *sample;
    while (times > 0)
    {
        // get system informatio
        sysinfo(&info);

        totalPhys = info.totalram / d;
        usedPhys = totalPhys - info.freeram / d;
        totalVirt = totalPhys + info.totalswap / d;
        usedVirt = totalVirt - (info.freeram + info.freeswap) / d;

        printf("%.2Lf GB / %.2Lf GB  -- %.2Lf GB / %.2Lf GB",
               usedPhys, totalPhys, usedVirt, totalVirt);

        if (graphicsFlag)
        {
            if (prevUsed != 0)
            {
                generateMemBars(prevUsed, usedVirt);
            }
            else
            {
                generateMemBars(usedVirt, usedVirt);
            }
            prevUsed = usedVirt;
        }

        else
        {
            printf("\n");
        }

        times--;
        sleep(*tdelay);
    }
}

#endif