#include <errno.h>
#include <stdlib.h>

static int counter = 0;

#include "../../src/http_server.h"

void home(Arena *a, http_req req, http_res *res) {
    (void) req;
    http_res_file(a, res, "index.html");
}

void get_counter(Arena *a, http_req req, http_res *res) {
    (void) req;

    char *body = arena_alloc(a, sizeof(char) * 1000);
    sprintf(body,
            "<div>"
            "Clicked %d times"
            "</div>",
            counter);

    res->body = body;
    res->status = 200;
    res->content_type = "text/html";
}

void increment(Arena *a, http_req req, http_res *res) {
    (void) req;
    counter++;

    char *body = arena_alloc(a, sizeof(char) * 1000);
    sprintf(body,
            "<div>"
            "Clicked %d times"
            "</div>",
            counter);

    res->body = body;
    res->status = 200;
    res->content_type = "text/html";
}

void decrement(Arena *a, http_req req, http_res *res) {
    (void) req;
    counter--;

    char *body = arena_alloc(a, sizeof(char) * 1000);
    sprintf(body,
            "<div>"
            "Clicked %d times"
            "</div>",
            counter);

    res->body = body;
    res->status = 200;
    res->content_type = "text/html";
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "[ERROR] Port not provided\n");
        printf("usage:\n");
        printf("    ./main <port>\n");
        exit(1);
    }

    char *end;
    size_t port = strtod(argv[1], &end);

    int error = 0;
    http_server server = http_server_new(port, &error);
    if (error != 0) {
        fprintf(stderr, "[ERROR] Could not create a server on port %zu\n",
                port);
        exit(1);
    }

    if (http_server_add_route(&server, "/", HTTP_GET, &home) < 0) {
        fprintf(stderr, "[ERROR] Could not add route / to server\n");
        exit(1);
    }

    if (http_server_add_route(&server, "/get-counter", HTTP_GET, &get_counter) <
        0) {
        fprintf(stderr, "[ERROR] Could not add route /get-counter to server\n");
        exit(1);
    }

    if (http_server_add_route(&server, "/increment", HTTP_GET, &increment) <
        0) {
        fprintf(stderr, "[ERROR] Could not add route /increment to server\n");
        exit(1);
    }

    if (http_server_add_route(&server, "/decrement", HTTP_GET, &decrement) <
        0) {
        fprintf(stderr, "[ERROR] Could not add route /decrement to server\n");
        exit(1);
    }

    if (http_server_start(server) < 0) {
        fprintf(stderr, "[ERROR] Server did not terminate properly: %s\n",
                strerror(errno));
        exit(1);
    }

    http_server_free(server);
    return 0;
}
