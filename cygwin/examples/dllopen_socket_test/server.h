#ifndef SERVER_H
#define SERVER_H

typedef struct server server_t;

int server_create(unsigned short port, server_t **result);
int server_start(server_t *server);
int server_stop(server_t *server);
void server_destroy(server_t *server);

const char *server_version(void);

#endif
