#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define PROC_FILE "/proc/mytaskinfo"
#define BUFFER_SIZE 1024

int main() {
    int fd;
    char *buffer;
    ssize_t bytes_read;

    
    const char *states[] = {"R", "S", "D", "T", "t", "X", "Z", "P", "I"};


    buffer = malloc(sizeof(char) * BUFFER_SIZE);

    fd = open(PROC_FILE, O_RDWR);
    if (fd == -1) {
        perror("Unable to open " PROC_FILE);
        return 1;
    }

    for (int i = 0; i < sizeof(states) / sizeof(states[0]); i++) {
        write(fd, states[i], strlen(states[i]));

        while ((bytes_read = read(fd, buffer, BUFFER_SIZE - 1)) > 0) {
            buffer[bytes_read] = '\0';
            printf("%s", buffer);
        }

        printf("\n");
    }
    free(buffer);

    close(fd);

    return 0;
}
