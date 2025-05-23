// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#define PORT 1234
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

int make_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address = {0};
    int epoll_fd;
    struct epoll_event ev, events[MAX_EVENTS];

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);
    make_non_blocking(server_fd);

    epoll_fd = epoll_create1(0);
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);

    printf("Server running on port %d...\n", PORT);

    while (1) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == server_fd) {
                int client_fd = accept(server_fd, NULL, NULL);
                make_non_blocking(client_fd);
                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                printf("New client connected.\n");
            } else {
                char buffer[BUFFER_SIZE] = {0};
                int client_fd = events[i].data.fd;
                int bytes = read(client_fd, buffer, sizeof(buffer));
                if (bytes <= 0) {
                    close(client_fd);
                    printf("Client disconnected.\n");
                } else {
                    printf("Client said: %s", buffer);
                    write(client_fd, "Hello Client!\n", 14);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
