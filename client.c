#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE	10240
#define	SERVER_IP	"127.0.0.1"
#define SERVER_PORT	6000

int main(int argc, char *argv[]){
    int	sock_send;
    struct sockaddr_in	addr_send;
    int	i;
    char text[80], buf[BUF_SIZE];
    int	send_len,bytes_sent;

        /* create socket for sending data */
    sock_send=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_send < 0){
        printf("socket() failed\n");
        exit(0);
    }

        /* create socket address structure to connect to */
    memset(&addr_send, 0, sizeof (addr_send)); /* zero out structure */
    addr_send.sin_family = AF_INET; /* address family */
    addr_send.sin_addr.s_addr = inet_addr(SERVER_IP);
    addr_send.sin_port = htons((unsigned short)SERVER_PORT);

        /* connect to the server */
    i=connect(sock_send, (struct sockaddr *) &addr_send, sizeof (addr_send));
    if (i < 0){
        printf("connect() failed\n");
        exit(0);
    }
    while (1){
        /* send some data */

        memset(&text, 0, 80);
        printf("\nEnter command (-h for help): ");
        // scanf("%s",text);
        fgets (text, 80, stdin);
        if (strcmp(text,"quit") == 0)
            break;

        strcpy(buf,text);
        send_len=strlen(text);
        bytes_sent=send(sock_send,buf,send_len,0);
        memset(&buf, 0, BUF_SIZE);
        i = read(sock_send, buf, BUF_SIZE);
        if(i < 0){
          printf("Error from server");
        }
        printf("%s\n", buf);

    }
    close(sock_send);
}
