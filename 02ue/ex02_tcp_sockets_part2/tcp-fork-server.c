#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define MAXPENDING 5

int main(int argc, char **argv)
{
        unsigned short listen_port;     // Server port
        int listen_sock;        // Socket descriptor for server
        int client_sock;        // Socket descriptor for client
        struct sockaddr_in listen_addr; // Local address
        struct sockaddr_in client_addr; // Client address

        // Check command line arguments
        if (argc != 2) {
                fprintf(stderr, "Missing parameters. Usage: %s <server-port>\n",
                        argv[0]);
                return 1;
        }
        // Create socket for incoming connections
        if ((listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
                perror("socket() failed");
                return 1;
        }
        // Construct local address structure 
        listen_port = atoi(argv[1]);    // First arg: listening port number

        memset(&listen_addr, 0, sizeof(listen_addr));   // Zero out structure
        listen_addr.sin_family = AF_INET;       // Internet address family
        listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);        // Any incoming interface
        listen_addr.sin_port = htons(listen_port);      // Local port

        // Bind to the local address
        if (bind
            (listen_sock, (struct sockaddr *)&listen_addr,
             sizeof(listen_addr)) < 0) {
                perror("bind() failed");
                return 1;
        }
        // Mark the socket so it will listen for incoming connections
        if (listen(listen_sock, MAXPENDING) < 0) {
                perror("listen() failed");
                return 1;
        }

        for (;;) {              /* Run forever */
                socklen_t addr_len = sizeof(client_addr);

                // Wait for a client to connect */
                if ((client_sock =
                     accept(listen_sock, (struct sockaddr *)&client_addr,
                            &addr_len)) < 0) {
                        perror("accept() failed");
                        return 1;
                }

                if (fork() == 0) {
                    // I'm the new child process...
                    close(listen_sock);   // Close listening socket -- not our business any more

                    printf("New child running...\n");
                    while (1) {
                        char buf[100];
                        int bytes;
                        bytes = read(client_sock, buf, 100);
                        printf("read returned %d\n", bytes);
                        if (bytes == 0) 
                            break;
                    } 
                    // Fill in code which handles the connection
                
                    printf("terminating child\n");
                    exit(0);  // Child process finally kills itself
                } else {
                    // client_sock is connected to a client
                    printf("Forked a new process for a connection from %s\n",
                           inet_ntoa(client_addr.sin_addr));
                }
        }
        /* NOT REACHED */
        return 1;
}
