
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BUFFERSIZE 1024
#define PORT 5678


int main() {

    int server_fd;
    int client_fd;

    struct sockaddr_in client; // Socketadresse eines Clients
    socklen_t client_len; // Länge der Client-Daten
    char input[BUFFERSIZE]; // Daten vom Client an den Server
    int bytes_read; // Anzahl der Bytes, die der Client geschickt hat

    // Socket erstellen
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0 ){
        fprintf(stderr, "socket konnte nicht erstellt werden\n");
        exit(-1);
    }

    int option = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int));    // Socket Optionen setzen für schnelles wiederholtes Binden der Adresse


    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    int bind_status = bind(server_fd, (struct sockaddr *) &server, sizeof(server));   // Socket binden
    if (bind_status < 0 ){
        fprintf(stderr, "socket konnte nicht gebunden werden\n");
        exit(-1);
    }


    // Socket lauschen lassen
    int listen_status = listen(server_fd, 5);
    if (listen_status < 0 ){
        fprintf(stderr, "socket konnte nicht listen gesetzt werden\n");
        exit(-1);
    }

    while (1) {

        client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len); // Verbindung eines Clients wird entgegengenommen
        if(client_fd < 0) {
            fprintf(stderr, "Client konnte nicht verbunden werden\n");
        }

        bytes_read = read(client_fd, input, BUFFERSIZE); // Lesen von Daten, die der Client schickt

        while (bytes_read > 0) { // Zurückschicken der Daten, solange der Client welche schickt (und kein Fehler passiert)
            printf("sending back the %d bytes I received...\n", bytes_read);

            write(client_fd, input, bytes_read);
            bytes_read = read(client_fd, input, BUFFERSIZE);

        }
        close(client_fd);
    }

    close(server_fd);
}