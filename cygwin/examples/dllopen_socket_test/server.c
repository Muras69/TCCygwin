#define _POSIX_C_SOURCE 200809L

#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <poll.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct server {
    int socket_fd;
    int wake_pipe[2];

    unsigned short port;

    pthread_t thread;
    pthread_mutex_t mutex;

    int running;
};

static const char body[] =
"<!DOCTYPE html>"
"<html lang=\"pl\">"
"<head>"
"<meta charset=\"UTF-8\">"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
"<meta name=\"theme-color\" content=\"#10151e\">"
"<title>Mini Server | System Dashboard</title>"
"<style>"

"* { box-sizing: border-box; }"

"body {"
" margin: 0;"
" min-height: 100vh;"
" background: #10151e;"
" color: #dce5ef;"
" font-family: Arial, Helvetica, sans-serif;"
" display: flex;"
" justify-content: center;"
" align-items: center;"
" padding: 30px 16px;"
"}"

".container {"
" width: 100%;"
" max-width: 850px;"
" background: #171f2b;"
" border: 1px solid #293747;"
" border-radius: 14px;"
" overflow: hidden;"
" box-shadow: 0 20px 60px rgba(0,0,0,.35);"
"}"

".topbar {"
" padding: 16px 24px;"
" background: #1c2735;"
" border-bottom: 1px solid #293747;"
" display: flex;"
" justify-content: space-between;"
" align-items: center;"
" flex-wrap: wrap;"
" gap: 12px;"
"}"

".brand {"
" font-size: 14px;"
" font-weight: bold;"
" letter-spacing: 2px;"
" color: #dce5ef;"
"}"

".brand span { color: #42c8f5; }"

".status {"
" color: #5de0a0;"
" font-size: 12px;"
" letter-spacing: 1px;"
"}"

".dot {"
" display: inline-block;"
" width: 8px;"
" height: 8px;"
" background: #5de0a0;"
" border-radius: 50%;"
" margin-right: 7px;"
"}"

".content { padding: 40px 30px; }"

".eyebrow {"
" color: #42c8f5;"
" font-size: 11px;"
" letter-spacing: 3px;"
" text-transform: uppercase;"
" margin-bottom: 14px;"
"}"

"h1 {"
" font-size: clamp(27px, 5vw, 42px);"
" margin: 0 0 14px;"
" color: #f1f6fc;"
" font-weight: 600;"
"}"

".description {"
" color: #91a1b5;"
" line-height: 1.7;"
" font-size: 15px;"
" max-width: 600px;"
"}"

".line {"
" height: 1px;"
" background: #293747;"
" margin: 30px 0;"
"}"

".grid {"
" display: grid;"
" grid-template-columns: repeat(auto-fit, minmax(190px, 1fr));"
" gap: 14px;"
"}"

".card {"
" background: #1c2735;"
" border: 1px solid #2b3b4d;"
" border-radius: 9px;"
" padding: 20px;"
"}"

".label {"
" color: #8295aa;"
" font-size: 11px;"
" text-transform: uppercase;"
" letter-spacing: 1.5px;"
" margin-bottom: 12px;"
"}"

".value {"
" color: #42c8f5;"
" font-size: 19px;"
" font-weight: bold;"
"}"

".terminal {"
" margin-top: 25px;"
" padding: 18px;"
" background: #0e141c;"
" border: 1px solid #263544;"
" border-radius: 8px;"
" color: #8fa4b9;"
" font-family: monospace;"
" font-size: 12px;"
" line-height: 1.9;"
" overflow-wrap: anywhere;"
"}"

".terminal .prompt { color: #5de0a0; }"
".terminal .command { color: #42c8f5; }"

".footer {"
" padding: 16px 24px;"
" background: #131b26;"
" border-top: 1px solid #293747;"
" display: flex;"
" justify-content: space-between;"
" flex-wrap: wrap;"
" gap: 10px;"
" color: #718399;"
" font-size: 11px;"
" letter-spacing: 1px;"
"}"

"@media (max-width: 480px) {"
" .content { padding: 30px 20px; }"
" .topbar { padding: 15px 18px; }"
" .footer { padding: 15px 18px; }"
"}"

"</style>"
"</head>"

"<body>"

"<main class=\"container\">"

"<header class=\"topbar\">"
"<div class=\"brand\">SYSTEM<span> / </span>SERVER</div>"
"<div class=\"status\"><span class=\"dot\"></span>ONLINE</div>"
"</header>"

"<section class=\"content\">"

"<div class=\"eyebrow\">Network service / HTTP</div>"

"<h1>Server is running.</h1>"

"<p class=\"description\">"
"Serwer HTTP zostal uruchomiony poprawnie. "
"Biblioteka systemowa obsluguje polaczenia TCP "
"i dostarcza odpowiedzi HTTP."
"</p>"

"<div class=\"line\"></div>"

"<div class=\"grid\">"

"<div class=\"card\">"
"<div class=\"label\">Service</div>"
"<div class=\"value\">HTTP / TCP</div>"
"</div>"

"<div class=\"card\">"
"<div class=\"label\">Address</div>"
"<div class=\"value\">127.0.0.1</div>"
"</div>"

"<div class=\"card\">"
"<div class=\"label\">Port</div>"
"<div class=\"value\">9090</div>"
"</div>"

"<div class=\"card\">"
"<div class=\"label\">Status</div>"
"<div class=\"value\">Operational</div>"
"</div>"

"</div>"

"<div class=\"terminal\">"
"<span class=\"prompt\">root@server</span>:"
"<span class=\"command\">~#</span> service status<br>"
"[ OK ] TCP socket initialized<br>"
"[ OK ] HTTP response handler ready<br>"
"[ OK ] Network service operational"
"</div>"

"</section>"

"<footer class=\"footer\">"
"<span>MINI HTTP SERVER</span>"
"<span>BUILT WITH C / POSIX</span>"
"</footer>"

"</main>"

"</body>"
"</html>";

static int send_all(int fd, const char *data, size_t length)
{
    size_t total = 0;

    while (total < length) {
#ifdef MSG_NOSIGNAL
        int flags = MSG_NOSIGNAL;
#else
        int flags = 0;
#endif

        ssize_t sent = send(fd, data + total,
                            length - total, flags);

        if (sent < 0) {
            if (errno == EINTR)
                continue;

            return -1;
        }

        if (sent == 0)
            return -1;

        total += (size_t)sent;
    }

    return 0;
}

static void handle_client(int fd)
{
    char request[2048];
    char header[256];

    struct pollfd pfd;
    ssize_t received;
    int length;

    pfd.fd = fd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    /* Maksymalnie 3 sekundy oczekiwania na klienta. */
    if (poll(&pfd, 1, 3000) <= 0)
        return;

    received = recv(fd, request, sizeof(request) - 1, 0);

    if (received <= 0)
        return;

    request[received] = '\0';

    printf("HTTP request:\n%.200s\n", request);

    length = snprintf(
        header,
        sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        sizeof(body) - 1
    );

    if (length < 0 || (size_t)length >= sizeof(header))
        return;

    if (send_all(fd, header, (size_t)length) < 0)
        return;

    (void)send_all(fd, body, sizeof(body) - 1);
}

static void *server_thread(void *arg)
{
    server_t *server = arg;

    struct pollfd fds[2];

    fds[0].fd = server->socket_fd;
    fds[0].events = POLLIN;

    fds[1].fd = server->wake_pipe[0];
    fds[1].events = POLLIN;

    for (;;) {
        int result = poll(fds, 2, -1);

        if (result < 0) {
            if (errno == EINTR)
                continue;

            break;
        }

        if (fds[1].revents & POLLIN) {
            char byte;

            (void)read(server->wake_pipe[0], &byte, 1);
            break;
        }

        if (fds[0].revents & POLLIN) {
            int client_fd = accept(server->socket_fd, NULL, NULL);

            if (client_fd < 0) {
                if (errno == EINTR)
                    continue;

                break;
            }

            handle_client(client_fd);
            close(client_fd);
        }

        if (fds[0].revents & (POLLERR | POLLHUP | POLLNVAL))
            break;
    }

    return NULL;
}

__attribute__((dllexport)) int server_create(unsigned short port, server_t **result)
{
    server_t *server;

    if (result == NULL || port == 0) {
        errno = EINVAL;
        return -1;
    }

    *result = NULL;

    server = calloc(1, sizeof(*server));

    if (server == NULL)
        return -1;

    server->socket_fd = -1;
    server->wake_pipe[0] = -1;
    server->wake_pipe[1] = -1;
    server->port = port;

    int error = pthread_mutex_init(&server->mutex, NULL);

    if (error != 0) {
        free(server);
        errno = error;
        return -1;
    }

    if (pipe(server->wake_pipe) < 0) {
        int saved_errno = errno;

        pthread_mutex_destroy(&server->mutex);
        free(server);

        errno = saved_errno;
        return -1;
    }

    *result = server;

    return 0;
}

__attribute__((dllexport)) int server_start(server_t *server)
{
    struct sockaddr_in address;
    int opt = 1;
    int error;

    if (server == NULL) {
        errno = EINVAL;
        return -1;
    }

    pthread_mutex_lock(&server->mutex);

    if (server->running) {
        pthread_mutex_unlock(&server->mutex);
        errno = EALREADY;
        return -1;
    }

    server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server->socket_fd < 0)
        goto error;

    if (setsockopt(server->socket_fd,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &opt,
                   sizeof(opt)) < 0)
        goto error;

    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(server->port);

    if (bind(server->socket_fd,
             (struct sockaddr *)&address,
             sizeof(address)) < 0)
        goto error;

    if (listen(server->socket_fd, 5) < 0)
        goto error;

    error = pthread_create(&server->thread,
                           NULL,
                           server_thread,
                           server);

    if (error != 0) {
        errno = error;
        goto error;
    }

    server->running = 1;

    pthread_mutex_unlock(&server->mutex);

    return 0;

error:
    {
        int saved_errno = errno;

        if (server->socket_fd >= 0) {
            close(server->socket_fd);
            server->socket_fd = -1;
        }

        pthread_mutex_unlock(&server->mutex);

        errno = saved_errno;
        return -1;
    }
}

__attribute__((dllexport)) int server_stop(server_t *server)
{
    int error;

    if (server == NULL) {
        errno = EINVAL;
        return -1;
    }

    pthread_mutex_lock(&server->mutex);

    if (!server->running) {
        pthread_mutex_unlock(&server->mutex);
        return 0;
    }

    {
        char byte = 1;
        ssize_t written;

        do {
            written = write(server->wake_pipe[1], &byte, 1);
        } while (written < 0 && errno == EINTR);

        if (written != 1) {
            int saved_errno = written < 0 ? errno : EIO;

            pthread_mutex_unlock(&server->mutex);

            errno = saved_errno;
            return -1;
        }
    }

    pthread_mutex_unlock(&server->mutex);

    error = pthread_join(server->thread, NULL);

    if (error != 0) {
        errno = error;
        return -1;
    }

    pthread_mutex_lock(&server->mutex);

    close(server->socket_fd);
    server->socket_fd = -1;
    server->running = 0;

    pthread_mutex_unlock(&server->mutex);

    return 0;
}

__attribute__((dllexport)) void server_destroy(server_t *server)
{
    if (server == NULL)
        return;

    (void)server_stop(server);

    close(server->wake_pipe[0]);
    close(server->wake_pipe[1]);

    pthread_mutex_destroy(&server->mutex);

    free(server);
}

__attribute__((dllexport)) const char *server_version(void)
{
    return "1.0.0";
}
