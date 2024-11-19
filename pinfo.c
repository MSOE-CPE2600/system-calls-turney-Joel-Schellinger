/**
 * @file info.c
 * @brief Code for process information system calls
 * @author Joel Schellinger
 * Compile instructions: make
 * Assignement: Lab 9
 * Course: CPE-2600
 * Section: 111
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <sched.h>
#include <time.h>
#include <sys/sysinfo.h>

#define BASE_10 10

bool get_int(const char* str, int* i);

int main(int argc, char* argv[])
{
    // Ensure only one parameter
    if (argc > 2) {
        errno = E2BIG;
        perror("Error! pinfo takes one and only one parameter");
        exit(EXIT_FAILURE);
    }
    pid_t pid;

    // Use getpid if the user does not specify a pid
    if (argc == 1) {
        pid = getpid();
    } else  { 
        // Get the int if failed print out the error
        if (!get_int(argv[1], &pid)) {
            errno = EINVAL;
            perror("Invalid PID! Please enter a number!");
            exit(EXIT_FAILURE);
        }
    }
    errno = 0; // Reset errno

    // Get the nice value
    int prio = getpriority(PRIO_PROCESS, pid);
    if (errno == ESRCH) {
        perror("Error! Couldn't find pid!");
        exit(EXIT_FAILURE);
    }
    printf("Priority: %d\n", prio);
    
    // Get scheduling policy
    int policy = sched_getscheduler(pid);
    char* policy_str;

    // Get the policy in user-friendly format
    switch(policy) {
        case SCHED_OTHER:
        policy_str = "OTHER";
        break;

        case SCHED_BATCH:
        policy_str = "BATCH";
        break;

        case SCHED_IDLE:
        policy_str = "IDLE";
        break;

        case SCHED_FIFO:
        policy_str = "FIFO";
        break;

        case SCHED_RR:
        policy_str = "ROUND_ROBIN";
        break;

        default:
        perror("Error! Couldn't find scheduling policy");
        exit(EXIT_FAILURE); // Exit to avoid printing an unknown policy
        break;
    }

    // Print the scheduling policy
    printf("Scheduling Policy: %s\n", policy_str); 
    return 0;
}
/**
 * @brief Gets an int from a string
 * @param str The string to get the int from
 * @param i A pointer to the int to store the int in
 * @return true if successful
 */
bool get_int(const char* str, int* i) {
    char* end_ptr;
    *i = strtol(str, &end_ptr, BASE_10);
    

    // Check if conversion succeeded and consumed all characters in str
    return end_ptr != str && *end_ptr == '\0';
}