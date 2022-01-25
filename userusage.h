#ifndef USERUSAGE_H
#define USERUSAGE_H

#include <utmp.h>

/**
 * @brief print users information, similar to 'w' command in Linux
 * 
 */
void printUserInfo()
{   
    int count = 0;
    struct utmp buf;
    FILE *fp;
    fp = fopen(_PATH_UTMP, "r");
    while (fread(&buf, sizeof(buf), 1, fp))
    {
        if (buf.ut_type == USER_PROCESS)
        {
            printf("    %s  ", buf.ut_user);
            printf("    %s  ", buf.ut_line);
            printf("    %s  ", buf.ut_id);
            printf("    %s  ", buf.ut_host);
            printf("\n");
            count++;
        }
    }
    fclose(fp);
    printf("    %d users are connected\n", count);
    return;
}

#endif