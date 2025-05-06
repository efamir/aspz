#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

#define PATH_BUFFER_SIZE 1024

// Display file info similar to ls -l
void display_file_info(const char *filename, const struct stat *statbuf) {
    char perm_string[11];
    struct tm *timeinfo;
    char time_buffer[80];
    struct passwd *pwd;
    struct group *grp;

    // File type
    mode_t mode = statbuf->st_mode;
    if (S_ISREG(mode)) perm_string[0] = '-';
    else if (S_ISDIR(mode)) perm_string[0] = 'd';
    else if (S_ISLNK(mode)) perm_string[0] = 'l';
    else perm_string[0] = '?';

    // Permissions
    perm_string[1] = (mode & S_IRUSR) ? 'r' : '-';
    perm_string[2] = (mode & S_IWUSR) ? 'w' : '-';
    perm_string[3] = (mode & S_IXUSR) ? 'x' : '-';
    perm_string[4] = (mode & S_IRGRP) ? 'r' : '-';
    perm_string[5] = (mode & S_IWGRP) ? 'w' : '-';
    perm_string[6] = (mode & S_IXGRP) ? 'x' : '-';
    perm_string[7] = (mode & S_IROTH) ? 'r' : '-';
    perm_string[8] = (mode & S_IWOTH) ? 'w' : '-';
    perm_string[9] = (mode & S_IXOTH) ? 'x' : '-';
    perm_string[10] = '\0';

    // Links, owner, group, size
    long nlink = statbuf->st_nlink;
    pwd = getpwuid(statbuf->st_uid);
    const char *owner_name = (pwd != NULL) ? pwd->pw_name : "unknown";
    grp = getgrgid(statbuf->st_gid);
    const char *group_name = (grp != NULL) ? grp->gr_name : "unknown";
    off_t size = statbuf->st_size;

    // Modification time
    timeinfo = localtime(&statbuf->st_mtime);
    if (timeinfo == NULL) {
        strcpy(time_buffer, "invalid time");
    } else {
        strftime(time_buffer, sizeof(time_buffer), "%b %e %H:%M", timeinfo);
    }

    // Print formatted info
    printf("%s %ld %s %s %lld %s %s\n",
           perm_string,
           nlink,
           owner_name,
           group_name,
           (long long)size,
           time_buffer,
           filename);
}

int main() {
    DIR *d;
    struct dirent *dir;
    char full_path[PATH_BUFFER_SIZE];
    struct stat statbuf;

    // Open current directory
    d = opendir(".");
    if (d == NULL) {
        perror("Error opening current directory");
        return EXIT_FAILURE;
    }

    // Read directory entries
    while ((dir = readdir(d)) != NULL) {
        // Construct full path
        sprintf(full_path, "./%s", dir->d_name);

        // Get file status (using lstat for symlinks)
        if (lstat(full_path, &statbuf) == -1) {
            perror(dir->d_name); // Print error for this specific file
            continue; // Skip to next entry
        }

        // Display info for the entry
        display_file_info(dir->d_name, &statbuf);
    }

    // Close directory
    closedir(d);

    return EXIT_SUCCESS;
}
