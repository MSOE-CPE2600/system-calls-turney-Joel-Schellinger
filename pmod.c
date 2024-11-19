/**
 * @file info.c
 * @brief Code for modifying the priority of a process
 * @author Joel Schellinger
 * Compile instructions: make
 * Assignement: Lab 9
 * Course: CPE-2600
 * Section: 111
 */

#include <time.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char* argv[])
{
    // Increment nice value by 10
    // The higher the nice value the lower the priority
    int suc = nice(10);
    if (suc == -1) { // check for success
        perror("Failed");
    }

    // Wait 1,837,272,638 ns
    struct timespec ts;
    ts.tv_nsec = 837272638;
    ts.tv_sec = 1;
    nanosleep(&ts, NULL);
    return 0;
}