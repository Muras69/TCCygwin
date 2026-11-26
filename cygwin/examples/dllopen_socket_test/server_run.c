
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <dlfcn.h>

#include "server.h"

int main(void)
{
    void *library;

    int (*create)(unsigned short, server_t **);
    int (*start)(server_t *);
    int (*stop)(server_t *);
    void (*destroy)(server_t *);
    const char *(*version)(void);

    server_t *server = NULL;
    const char *error;

    library = dlopen("./libserver.dll", RTLD_NOW);

    if (library == NULL) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        return 1;
    }

    dlerror();

    *(void **)(&create) = dlsym(library, "server_create");
    *(void **)(&start) = dlsym(library, "server_start");
    *(void **)(&stop) = dlsym(library, "server_stop");
    *(void **)(&destroy) = dlsym(library, "server_destroy");
    *(void **)(&version) = dlsym(library, "server_version");

    error = dlerror();

    if (error != NULL) {
        fprintf(stderr, "dlsym: %s\n", error);
        dlclose(library);
        return 1;
    }

    printf("Server library: %s\n", version());

    if (create(9090, &server) < 0) {
        perror("server_create");
        dlclose(library);
        return 1;
    }

    if (start(server) < 0) {
        perror("server_start");
        destroy(server);
        dlclose(library);
        return 1;
    }

    printf("HTTP server: http://127.0.0.1:9090\n");
    printf("Press ENTER to stop.\n");

    (void)getchar();

    if (stop(server) < 0)
        perror("server_stop");

    destroy(server);

    dlclose(library);

    return 0;
}
