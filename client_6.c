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

// AF_INET - Client
main()
{
   int sockfd;
   int len;
   struct sockaddr_in address;
   int result;
   char ip[128];
   char port[128];
   char filename[256];
   char buf[MAXSIZE];
   int filedes, filelen;
   long total;

   printf("MyClient#> IP Address Input : ");
   strcpy(ip, "127.0.0.1");

   printf("MyClient#> Port Number Input : ");
   strcpy(port, "5512");

   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr(ip);
   address.sin_port = htons(atoi(port));
   len = sizeof(address);

   result = connect(sockfd, (struct sockaddr *)&address, len);

   if(result == -1) {
      perror("oops: MyClient");
   }

   printf("MyClient#> Filename -> ");
   fgets(filename, 255, stdin);
   filename[strlen(filename)-1] = 0;


   /* 파일명의 크기를 먼저 보내고 */
   printf(buf, "%d", strlen(filename));
   write(sockfd, buf, strlen(buf));
   /* 파일명을 보내야 서버쪽에서 file이름만 읽어올 수 있다. */
   printf("----%s----\n", filename);
   write(sockfd, filename, strlen(filename));

   filedes = open(filename, O_RDONLY, 0);
   total = lseek(filedes, 0, SEEK_END);

   printf("send filesize %ld \n", total);
   sprintf(buf, "%ld", total);
   write(sockfd, buf, MAXSIZE);

   lseek(filedes, 0, SEEK_SET);
   while((filelen = read(filedes, buf, MAXSIZE)) > 0)
   {
      filelen = write(sockfd, buf, filelen);

      if(filelen == MAXSIZE)
      {
         total -= (long)filelen;
         if(total <= 0)
         {
            printf("file send success\n");
            filelen = 0;
            break;
         }
         else
         {
            printf("success file send %ldpercent  \n", total);
            continue;
         }
      }
      else if(filelen < MAXSIZE)
      {
         total -= (long)filelen;
         if(total <= 0)
         {
            printf("success\n");
            break;
         }
         printf("not success %d sending %ld percent \n", filelen, total);
      }
      else if(filelen == -1)
      {
         printf("file send failed\n");
         exit(0);
      }
   }

   close(sockfd);
   close(filedes);
}
