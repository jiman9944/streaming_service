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

    int  clnt_sock;

    struct sockaddr_in clnt_addr;



    char buf[BUF_SIZE];

    socklen_t clnt_addr_sz;

    int recv_len, status;



    char PORT[5];

    char IP[16];

    pid_t pid;

    

    printf("Input Server IP Address : ");

    scanf("%s", IP);



    printf("Input Server Port Number : ");

    scanf("%s", PORT);



    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);

    if(clnt_sock == -1)

        error("socket() error");



    memset(&clnt_addr, 0, sizeof(clnt_addr));

    clnt_addr.sin_family = AF_INET;

    clnt_addr.sin_addr.s_addr = inet_addr(IP);

    clnt_addr.sin_port = htons(atoi(PORT));



    printf("open!! client ");



    clnt_addr_sz = sizeof(clnt_addr);

    if(connect(clnt_sock, (struct sockaddr*)&clnt_addr, clnt_addr_sz) == -1)

        error("connect() error");

    

    printf("connect to server! ");



    recv_len = read(clnt_sock, buf, BUF_SIZE);

    buf[recv_len] = 0;

    printf("server Message: %s ", buf);

    


    pid = fork(); // make child process

    

    while(1)

    {

        if(pid    == 0)

        {

            if(recv_len>1)

            {

                recv_len = read(clnt_sock, buf, BUF_SIZE);

                if( !strcmp(buf, "q ") || !strcmp(buf, "Q ") )

                {

                    printf("exit chatting program..wait..3sec ");

                    sleep(3);

                    break;

                }

                buf[recv_len] = 0;

                printf("server Message:%s", buf);

                
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

        return 2;

    }



    else

    {

        while(!waitpid(-1, &status, WNOHANG))

        {

            sleep(5);

            printf("waitting.......");

        }

        printf(" ");



        printf("send child returned value(%d) from kernnel! ", WEXITSTATUS(status));

        printf("exit parent process ");

        close(clnt_sock);

    }



    return 0;

}
