// community_net.h

#ifndef GLANG_COMMUNITY_NET_H
#define GLANG_COMMUNITY_NET_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Configuration de l'agent Gopu
#define GOPU_UA "gopu/v1.0 (Advanced Industry Agent; G-Lang Runtime)"

typedef enum {
    NET_HTTP_GET,
    NET_HTTP_POST,
    NET_TCP_STREAM,
    NET_UDP_DATAGRAM
} NetProtocol;

/**
 * 1. MOTEUR HTTP (Via libcurl natif de build-base)
 */
typedef struct {
    char* url;
    char* payload;
    long  response_code;
    char* raw_response;
} GHttpRequest;

// Exécute une requête avec le User-Agent gopu/v1.0
void native_net_http_exec(GHttpRequest* req, NetProtocol method);


/**
 * 2. SOCKETS BAS NIVEAU (TCP/UDP natif Linux)
 * Pour créer des serveurs ou des clients bruts.
 */
typedef struct {
    int fd;             // File descriptor du socket
    uint16_t port;
    char* address;
    bool is_listening;
} GSocket;

// Création d'un serveur complet
int  native_net_socket_create(uint16_t port, NetProtocol proto);
void native_net_socket_listen(int server_fd, void (*on_connect)(int client_fd));
void native_net_socket_send(int fd, const char* data, size_t len);
char* native_net_socket_recv(int fd, size_t max_len);


/**
 * 3. WEB SERVER TOOLS (Abstractions pour .gg)
 */
typedef struct {
    char* route;
    void (*handler)(GHttpRequest* req);
} GRoute;

// Lance un serveur web industriel sur le port spécifié
void native_web_server_start(uint16_t port, GRoute* routes, int route_count);

#endif

