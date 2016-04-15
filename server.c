#include <stdio.h>
#include <sys/types.h>	/* system type defintions */
#include <sys/socket.h>	/* network system functions */
#include <netinet/in.h>	/* protocol & struct definitions */
#include <stdlib.h>	/* exit() warnings */
#include <string.h>	/* memset warnings */
#include <unistd.h>


#define BUF_SIZE	10240
#define LISTEN_PORT	6000

char buffer[BUF_SIZE];
char filename[50];

void upper_string(char s[]) {
   int c = 0;
   while (s[c] != '\0') {
      if (s[c] >= 'a' && s[c] <= 'z') {
         s[c] = s[c] - 32;
      }
      c++;
   }
}

void sys_cmd(char buf[]){
    char output[BUF_SIZE];
    FILE *fp;
    fp = popen(buf, "r");
    fread(output, BUF_SIZE, 1, fp);
    sprintf(buffer, "%s\n", output);
}

void spreadsheet_func(char cmd[]){
  int i, ln_str = strlen(cmd);
  char state[ln_str];
  char sycmd[ln_str + 100];
  if(cmd[0] == '-'){
    if((cmd[1] == 'o' || cmd[1] == 'O' || cmd[1] == 'c' || cmd[1] == 'C') && cmd[2] == ' '){
      int is_filename = 1;
      for(i = 3; i < ln_str; i++){
        if(cmd[i] == '-'){
          is_filename = 0;
        }else if(is_filename == 1){
          if(cmd[i] == ' '){
            filename[i-3] = '-';
          }else if(cmd[i] != '\n'){
            filename[i-3] = cmd[i];
          }
        }else{
          state[i-3] = cmd[i];
        }
      }
      if(state != '\0'){
        strcpy(state, " ");
      }
      if(cmd[1] == 'o' || cmd[1] == 'O'){
        printf("Openning by %s\n", state);
        sprintf(sycmd, "./spreadsheet.o -t %s %s\n", filename, state);
        printf("executing: %s", sycmd);
      }else{
        printf("Creating %s\n", filename);
        sprintf(sycmd, "./spreadsheet.o -t -c %s %s\n", filename, state);
        printf("executing: %s", sycmd);
      }
      sys_cmd(sycmd);
    }else if((cmd[1] == 'h' || cmd[1] == 'H')){
      printf("Fetching manual\n");
      sprintf(sycmd, "./spreadsheet.o -t");
      printf("executing: %s", sycmd);
      sys_cmd(sycmd);
    }else{
      sprintf(sycmd, "./spreadsheet.o -t %s %s", filename, cmd);
      printf("executing: %s", sycmd);
      sys_cmd(sycmd);
    }
  }else{
    sys_cmd("Invalid argument");
  }
}

int main(int argc, char *argv[]){
    int	sock_listen,sock_recv;
    struct sockaddr_in	my_addr,recv_addr;
    int i,addr_size,bytes_received;
    fd_set readfds;
    struct timeval	timeout={0,0};
    int	incoming_len;
    struct sockaddr	remote_addr;
    int	recv_msg_size;
    char buf[BUF_SIZE];
    char output[600];
    int	select_ret;

    printf("Preparing server\n");

    /* create socket for listening */
    sock_listen=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_listen < 0){
        printf("socket() failed\n");
        exit(0);
    }
        /* make local address structure */
    memset(&my_addr, 0, sizeof (my_addr));	/* zero out structure */
    my_addr.sin_family = AF_INET;	/* address family */
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* current machine IP */
    my_addr.sin_port = htons((unsigned short)LISTEN_PORT);
        /* bind socket to the local address */
    i=bind(sock_listen, (struct sockaddr *) &my_addr, sizeof (my_addr));
    if (i < 0){
        printf("bind() failed\n");
        exit(0);
    }
        /* listen ... */
    i=listen(sock_listen, 5);
    if (i < 0){
        printf("listen() failed\n");
        exit(0);
    }

    if( access("spreadsheet.c", F_OK ) == 0 ) {
      system("gcc -w -o spreadsheet.o spreadsheet.c");
    }else{
      sprintf(buffer, "Error: spreadsheet app could not be found.");
      return;
    }

    printf("Ready to serve\n");
        /* get new socket to receive data on */
    addr_size=sizeof(recv_addr);
    sock_recv=accept(sock_listen, (struct sockaddr *) &recv_addr, &addr_size);

    while (1){

        bytes_received=recv(sock_recv,buf,BUF_SIZE,0);
        buf[bytes_received]=0;

        if (strcmp(buf,"close") == 0)
            break;
        }

        memset(buffer, 0, BUF_SIZE);
        printf("Received: %s\n",buf);
        spreadsheet_func(buf);

        printf("Sending:\n %s\n", buffer);
        write(sock_recv, buffer, BUF_SIZE);

    close(sock_recv);
    close(sock_listen);
}
