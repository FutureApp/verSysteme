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
        unsigned short listen_port;     // Server port */
        int listen_sock;        // Socket descriptor for server
        int client_sock;        // Socket descriptor for client
        struct sockaddr_in listen_addr; // Local address */
        struct sockaddr_in client_addr; // Client address */

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
                // client_sock is connected to a client
                printf("New connection from %s\n",
                       inet_ntoa(client_addr.sin_addr));

              for (;;) {
               int ret;
               // Maximum size of incoming message
               int msglen = 100;

               // Buffer for message
               char buf[msglen + 1];   // One more to ensure that there is a trailing NULL char.
               memset(buf, 0, msglen + 1);
               ret = read(client_sock, buf, msglen);// Return value is amount of bytes read, -1 in case of error
               char *empty = "";
               if(strcmp(buf, empty) != 0){printf("Data read: '%s'\n", buf);};
               char *myGet = "GET\n";
               if(strcmp(buf, myGet) == 0){
                 printf("GET request received. Sending timestamp\n");

                 // Create message to send
                 time_t t = time(NULL);
                 char *msg = ctime(&t);
                 msglen = strlen(msg) + 1;
                 // Write the whole message in one go, fail if this does not work
                 ret = write(client_sock, msg, msglen);
                 // Return value is amount of bytes written, -1 in case of error
                 if (ret != msglen) {
                         perror("Error during write");
                         return 1;
                 }
               }
             }
             /* NOT REACHED */
             close(client_sock);
        }
        /* NOT REACHED */
        return 1;
}
