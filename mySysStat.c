#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

#include "cpuuse.h"
#include "sysusage.h"
#include "userusage.h"

/**
 * @brief a helper function to return number appear
 * in flag --samples=N or --tdelay=T
 * @param s
 * @return int
 */
int getNumber(char *s)
{
    char *token;
    int num;
    token = strtok(s, "=");
    token = strtok(NULL, s);
    num = strtol(token, NULL, 10);
    return num;
}

int main(int argc, char *argv[])
{

    long sample = 10;
    long tdelay = 1;
    int i, check;
    bool systemFlag = false;
    bool userFlag = false;
    bool graphicsFlag = false;

    // iterate through argv[], find the first two nums that
    // represent the sample and tdelay.
    for (i = 1; i < argc; i++)
    {
        if (isdigit(*argv[i]))
        {
            sample = strtol(argv[i], NULL, 10);
            // if (argv[i + 1] != NULL || isdigit(argv[i + 1])) {
            //     tdelay = strtol(argv[i + 1], NULL, 10);
            // }     
            break;
        }
    }
    for (i = i + 1; i < argc; i++)
    {
        if (isdigit(*argv[i]))
        {
            tdelay = strtol(argv[i], NULL, 10);
            // if (argv[i + 1] != NULL || isdigit(argv[i + 1])) {
            //     tdelay = strtol(argv[i + 1], NULL, 10);
            // }     
            break;
        }
    }

    printf("sample=%ld, tdelay=%ld\n", sample, tdelay);

    // iterate through argv[], find the flags need to be excuted
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "--system") == 0 || strcmp(argv[i], "-s") == 0)
            {   
                userFlag = true;
                check = 1;
            }

            else if (strcmp(argv[i], "--user") == 0 || strcmp(argv[i], "-u") == 0)
            {
                userFlag = true;
                check = 0;
            }

            else if (strcmp(argv[i], "--graphics") == 0 || strcmp(argv[i], "--graphic") == 0 || strcmp(argv[i], "-g") == 0)
            {
                graphicsFlag = true;
            }

            else if (strncmp(argv[i], "--samples=", 10) == 0 || strncmp(argv[i], "--samples=", 10) == -54)
            {
                sample = getNumber(argv[i]);
            }

            else if (strncmp(argv[i], "--tdelay=", 9) == 0 || strncmp(argv[i], "--tdelay=", 9) == -16)
            {
                tdelay = getNumber(argv[i]);
            }
        }
    }

    system("clear");

    if (userFlag && check == 0)
    {
        printf("### Sessions/users ###\n");
        printUserInfo();
        return 0;
    }

    if (systemFlag && check == 1)
    {
        printf("Number of samples: %ld -- every %ld secs\n", sample, tdelay);
        printMemoryUsage();
        printf("---------------------------------------\n");
        printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot\n");
        getMemoryUsedInfo(&sample, &tdelay, graphicsFlag);
        printf("---------------------------------------\n");
        printf("Number of cores: %d\n", getNumOfCores());
        totalCpuUse(&sample, &tdelay, graphicsFlag);
        printf("\n---------------------------------------\n");
        printf("### System Information ###\n");
        printSystemInfo();
        printf("---------------------------------------\n");
        return 0;
    }

    printf("\033[1mNumber of samples: %ld -- every %ld secs\033[22m\n", sample, tdelay);
    printMemoryUsage();
    printf("---------------------------------------\n");
    printf("\033[1m### Memory ### (Phys.Used/Tot -- Virtual Used/Tot\033[22m\n");
    getMemoryUsedInfo(&sample, &tdelay, graphicsFlag);
    printf("---------------------------------------\n");
    printf("\033[1mNumber of cores: %d\033[22m\n", getNumOfCores());
    totalCpuUse(&sample, &tdelay, graphicsFlag);
    printf("---------------------------------------\n");
    printf("\033[1m### Sessions/users ###\033[22m\n");
    printUserInfo();
    printf("---------------------------------------\n");
    printf("\033[1m### System Information ###\033[22m\n");
    printSystemInfo();
    printf("---------------------------------------\n");
}