// community_net.c
    
#include "../../../include/native/community_net.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Simulation de l'appel Curl avec User-Agent Gopu
void native_net_http_exec(GHttpRequest* req, NetProtocol method) {
    printf("[Gopu-Net] Requesting %s with Agent: %s\n", req->url, GOPU_UA);
    // Ici on linkerait avec libcurl : curl_easy_setopt(curl, CURLOPT_USERAGENT, GOPU_UA);
    req->response_code = 200;
    req->raw_response = strdup("{\"status\": \"ok\", \"engine\": \"gopu/v1.0\"}");
}

int native_net_socket_create(uint16_t port, NetProtocol proto) {
    int fd = socket(AF_INET, (proto == NET_TCP_STREAM) ? SOCK_STREAM : SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[Gopu-Error] Binding failed");
        return -1;
    }
    return fd;
}
double native_get_nanos() {
    return 0.0; // À remplacer par le vrai calcul de temps plus tard
}

