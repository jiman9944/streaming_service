#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <sys/wait.h>

#include <sys/socket.h>



#define BUF_SIZE 1024



void error(char* message)

{

    fputs(message, stderr);

    fputc(' ', stderr);

    exit(1);

}



int main(int argc, char** argv)

{

    int serv_sock, clnt_sock;

    struct sockaddr_in serv_addr;

    struct sockaddr_in clnt_addr;



    char serv_message[] = "server START";

    char buf[BUF_SIZE];

    char PORT[5];



    socklen_t clnt_addr_sz;

    int recv_len, status;

    

    pid_t pid;

    

    printf("Server Port : ");

    fflush(stdin);

    scanf("%s", PORT);    



    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if(serv_sock == -1)

        error("socket() error");



    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    serv_addr.sin_port = htons(atoi(PORT));



    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)

        error("bind() error");



    if(listen(serv_sock, 5) == -1)

        error("listen() error");

    

    printf("open!! server ");



    clnt_addr_sz = sizeof(clnt_addr);

    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_sock, &clnt_addr_sz);

    if(clnt_sock == -1)

        error("connect() error!");



    printf("Chatting on ");

    printf("conneted to Client ");

    write(clnt_sock, serv_message, sizeof(serv_message));

    pid = fork(); // make child process

    

    while(1)

    {        



        if(pid==0)

        {    

            recv_len = read(clnt_sock, buf, BUF_SIZE);

            if(recv_len > 1)

            {

                if( !strcmp(buf, "q ") || !strcmp(buf, "Q ") )

                {

                    printf("exit chatting program..wait..3sec ");

                    sleep(3);

                    break;

                }



                buf[recv_len] = 0;

                printf("client Message : %s", buf);

               

            }

        }



        else

        {

            fgets(buf, BUF_SIZE, stdin);

            

            if( !strcmp(buf, "Q ") || !strcmp(buf, "q "))

            {

                write(clnt_sock, buf, BUF_SIZE);

                shutdown(clnt_sock, SHUT_WR);

                break;

            }

            write(clnt_sock, buf, BUF_SIZE);

        }

        

    }

    

    

    if(pid == 0)

    {

        printf("exit child_process ");

        
        close(clnt_sock);

        close(serv_sock);

        return 1;

    }



    else

    {

        while(!waitpid(-1, &status, WNOHANG))

        {

            sleep(5);

            printf("waitting......");

        }

        printf(" ");



        printf("send child returned value(%d) from kernnel! ", WEXITSTATUS(status));

        printf("exit server thanks.... ");

        printf("exit parent process ");

        close(clnt_sock);

        close(serv_sock);

    }



    return 0;

}
