/**
 * @file info.c
 * @brief Code for information maintenance system calls
 * @author Joel Schellinger
 * Compile instructions: make
 * Assignement: Lab 9
 * Course: CPE-2600
 * Section: 111
 */
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <features.h>
#include <limits.h>
#include <string.h>
#include <syscall.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#define SECS_IN_HOUR (SECS_IN_MIN*MIN_IN_HOUR)
#define MIN_IN_HOUR 60
#define SECS_IN_MIN 60
int main(int argc, char* argv[])
{   
    struct timespec ts;
    struct tm time;
    if (clock_gettime(CLOCK_REALTIME, &ts) != -1) {
        time_t current_time = ts.tv_sec;
        localtime_r(&current_time, &time);
        char am_or_pm[3];
        if(time.tm_hour / 12 == 0) {
            strcpy(am_or_pm, "AM");
        } else {
            strcpy(am_or_pm, "PM");
        }

        printf("The time is %d:%02d:%02d %s %s\n", time.tm_hour % 12,time.tm_min,time.tm_sec, am_or_pm, tzname[0]);
        int seconds = time.tm_hour*SECS_IN_HOUR + time.tm_min*SECS_IN_MIN + time.tm_sec;
        long long nanoseconds = (long long)(seconds * 1E9 + ts.tv_nsec);
        printf("Time of day in nanoseconds: %lld\n", nanoseconds);
    }
    char hn[HOST_NAME_MAX];
    if (gethostname(hn, HOST_NAME_MAX) != -1) {
        printf("System Network Name: %s\n", hn);
    }
    struct utsname unm;
    if (uname(&unm) != -1) {
        printf("Operating System: %s\n", unm.sysname);        
        printf("Operating System Release: %s\n", unm.release);        
        printf("Operating System Version: %s\n", unm.version);        
        printf("Hardware Type: %s\n", unm.machine);
    }
    int nprocs = get_nprocs();
    printf("Number of processors: %d\n", nprocs);

    long pg_size = sysconf(_SC_PAGESIZE);
    long tot_num_pgs = sysconf(_SC_PHYS_PAGES);
    long av_num_pgs = sysconf(_SC_AVPHYS_PAGES);
    if (tot_num_pgs != -1) {
        printf("Total Memory: %ld bytes\n", pg_size*tot_num_pgs);
    }
    if (av_num_pgs != -1) {
        printf("Available Memory: %ld bytes\n", pg_size*av_num_pgs);
    }

    return 0;
}