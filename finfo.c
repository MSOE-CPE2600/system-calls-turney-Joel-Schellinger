/**
 * @file info.c
 * @brief Code for displaying file information
 * @author Joel Schellinger
 * Compile instructions: make
 * Assignement: Lab 9
 * Course: CPE-2600
 * Section: 111
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
int main(int argc, char* argv[])
{
    // Get the file info
    struct stat statbuf;
    if (stat(argv[1], &statbuf) == -1) {
        perror("Error getting file info:");
        exit(EXIT_FAILURE);
    }

    // Determine file type
    mode_t mt = statbuf.st_mode;
    char* file_type;
    switch (mt & S_IFMT) {
        case S_IFSOCK:
        file_type = "Socket";
        break;
        case S_IFLNK:
        file_type = "Symbolic Link";
        break;
        case S_IFREG:
        file_type = "Regular File";
        break;
        case S_IFBLK:
        file_type = "Block Device";
        break;
        case S_IFDIR:
        file_type = "Directory";
        break;
        case S_IFCHR:
        file_type = "Character Device";
        break;
        case S_IFIFO:
        file_type = "FIFO";
        break;
    }
    printf("File Type: %s\n", file_type);
    
    // Determine permissions
    char perms[4];
    char* cpy = perms;
    if (statbuf.st_mode & S_IRUSR) {
        *cpy++ = 'r';
    }
    if (statbuf.st_mode & S_IWUSR) {
        *cpy++ = 'w';
    }
    if (statbuf.st_mode & S_IXUSR) {
        *cpy++ = 'x';
    }
    *cpy = 0;
    printf("Permissions: %s\n", perms);

    // Get username as a string
    struct passwd* pw;
    pw = getpwuid(statbuf.st_uid);
    if (pw == NULL) {
        perror("Error getting username");
        exit(EXIT_FAILURE);
    }
    printf("Owner: %s\n", pw->pw_name);

    // Print the file size
    printf("File Size: %ld bytes\n", statbuf.st_size);

    // Print modification time in user friendly way
    time_t last_mod_time = statbuf.st_mtim.tv_sec;
    struct tm last_mod_time_tm;
    localtime_r(&last_mod_time, &last_mod_time_tm);
    printf("Last modified: %02d", 1 + last_mod_time_tm.tm_mon);
    printf("/%02d/%d", last_mod_time_tm.tm_mday, 1900 + last_mod_time_tm.tm_year);
    printf("%2d:%02d", last_mod_time_tm.tm_hour % 12, last_mod_time_tm.tm_min);
    char* am_or_pm;
    if (last_mod_time_tm.tm_hour / 12== 1) {
        am_or_pm = "PM";
    } else {
        am_or_pm = "AM";
    }
    printf(" %s\n", am_or_pm);
    
    return 0;
}