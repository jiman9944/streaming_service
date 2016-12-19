#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>

#define MAXSIZE 2048


main()
{
   int server_sockfd, client_sockfd;
   int server_len, client_len;
   struct sockaddr_in server_address;
   struct sockaddr_in client_address;
   char ip[128];
   char port[128];
   int filedes, filelen;
   long total;

   int readSize;


   printf("MyServer#> IP_Address Input : ");
   strcpy(ip, "127.0.0.1");

   printf("MyServer#> Port Number Input : ");
   strcpy(port, "5512");


   server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = inet_addr(ip);
   server_address.sin_port = htons(atoi(port));
   server_len = sizeof(server_address);
   bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

   listen(server_sockfd, 5);
   client_len = sizeof(client_address);
   client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

   printf("add client\n");

   char filename[256];
   char buf[MAXSIZE];

   
   /* 파일명의 길이를 먼저 읽어오고 */
   readSize = read(client_sockfd, buf, MAXSIZE);
   buf[readSize] = '\0';
   printf("0------%s-------\n", buf);


   /* 파일명의 길이만큼 일어와야한다. 그렇게 하지 않으면 뒤에 파일 사이즈까지 같이 읽어 에러난다. */
   readSize = read(client_sockfd, filename, atoi(buf));
   filename[readSize] = '\0';
   printf("1------%s-------\n", filename);


   filedes = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

   readSize = read(client_sockfd, buf, MAXSIZE);
   buf[readSize] = '\0';
   printf("2------%s-------\n", buf);


   total = atoi(buf);
   printf("receive file size %ld\n", total);

   while((filelen=read(client_sockfd ,buf, MAXSIZE)) > 0)
   {
      filelen = write(filedes, buf, filelen);

      if(filelen == MAXSIZE)
      {
         total -= (long)filelen;
         continue;
         printf("receive success %ldpercent\n", total);
      }
      else if(filelen < MAXSIZE)
      {
         total -= (long)filelen;
         if(total <= 0)
         {
            printf("receive success\n");
            break;
         }
         printf("not success receive %ld percent\n", total);
      }
      else if(filelen == -1)
      {
         printf("receive failed\n");
         exit(0);
      }
   }

   close(client_sockfd);
   close(filedes);
}
