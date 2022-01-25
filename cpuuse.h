#ifndef CPUUSE_H
#define CPUUSE_H

#include <unistd.h>

int getNumOfCores()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

/*
 *helper function of totalCpuUse()
 *print bars for one cpu use, total bars is 55
 */
void generateCpuBars(double percent) {
    int bars = percent;
    if (percent > 100) {
        percent = 100;
        bars = percent;
    }

    if (percent < 1 ) {
        bars = 0;
    }

    else if (percent > 20 && percent <= 30) {
        bars = 20 + (percent - 20) * 0.7;
    }

    else if (percent > 35 && percent <= 50) {
        bars = 20 + 15 * 0.8 + (percent - 35) * 0.5;
    }

    else if (percent > 50 ) {
        bars = 20 + 15 * 0.8 + 15 * 0.5
        + (percent - 50) * 0.3;
    }

    for (int i = 0; i < bars + 1; i++) {
        printf("|");
    }
    printf(" %.2f%%\n", percent);

    // build number of | characters, add to a string
    // return a string instead of printing

    // tried to make the bars as a string and then print it
    // but print directly is faster

    // char temp[65] = "";
    // char tail[10];
    // for (int i = 0; i < bars + 1; i++) {
    //     char bar[] = "|";
    //     strcat(temp, bar);
    // }
    // sprintf(tail, "%.2f", percent);
    // strcat(temp, " ");
    // strcat(temp, tail);
    // strcat(temp, "%");

    // // printf("%s\n", res);
    // char* result =  malloc (sizeof (char) * strlen(temp));
    // strcat(result, temp);
    // return result;
}

/**
 * @brief Get the CPU Used Info object *sample times whit *tdelay second, 
 * show relative graphs, print them below if flag is checked
 * @param sample how many times should be printed
 * @param tdelay how lone to print 1 line
 * @param graphicsFlag check if the graphics flag is on
 */
void totalCpuUse(long *sample, long *tdelay, bool graphicsFlag)
{
    char str[100];
    char *token;
    int i = 0;
    int b = 1;
    long currTotal = 0, currWork = 0;
    long prevTotal = 0, prevWork = 0;
    long total = 0, work = 0;
    double percent;
    long times = *sample + 1;

    while (times > 0)
    {
        FILE *fp = fopen("/proc/stat", "r");
        if (!fp)
        {
            perror("File open error!\n");
            return;
        }

        fscanf(fp, "%[^\n]", str);
        // printf("%s\n", str);
        fclose(fp);
        token = strtok(str, " ");
        currTotal = 0;
        currWork = 0;
        i = 0;
        
        while (token != NULL)
        {
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                currTotal += strtol(token, NULL, 10);
                // printf("|%ld| ", currTotal);
                while (i < 3)
                {
                    currWork += strtol(token, NULL, 10);
                    // printf("~%ld~ ", currWork);
                    break;
                }
                i++;
            }
        }

        // use the current time - prev time to get a cpu usage interval
        if (times - 1 == *sample)
        {
            prevWork = currWork;
            prevTotal = currTotal;
        }
        else
        {
            total = currTotal - prevTotal;
            work = currWork - prevWork;
            percent = work * 100.0 / total;
            printf("\033[s total cpu use = %.2f%%\n\r", percent);
            
            // if flag --graphics is excuted. Print the bars for graph
            if (graphicsFlag) {
                printf("\033[%dB \033[8C", b++);
                generateCpuBars(percent);
            }        
            printf("\033[u");
            prevWork = currWork;
            prevTotal = currTotal;
        }
        times--;
        sleep(*tdelay);        
    }
    printf("\033[%dB", b);
}

#endif