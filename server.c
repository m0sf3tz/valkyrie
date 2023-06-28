#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
   
#define PORT       1024
#define MAGIC_LEN  4
#define SIZE_LEN   4
#define IMG_LEN    153600
#define MAGIC_NUM  0xDEADBEEF 
// Driver code
int main() {
    int sockfd;
    char buffer[IMG_LEN];
    struct sockaddr_in servaddr, cliaddr;
       
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
       
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
       
    // Filling server information
    servaddr.sin_family      = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port        = htons(PORT);
       
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
       
    socklen_t len;
    int n;
   
    len = sizeof(cliaddr);  //len is value/result
 
    uint32_t rxed = 0; 
    while(1) {
      uint32_t magic, size, count;
      char name[100];
      memset(name, 0 , sizeof(name));
      sprintf(name, "img_%d.grayscale", rxed++);

      n = recvfrom(sockfd, (char *)&(magic), MAGIC_LEN, 
                  MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                  &len);
      printf("magic: %u\n", magic);

      if(magic != MAGIC_NUM) {
        continue;
      }

      n = recvfrom(sockfd, (char *)&(size), SIZE_LEN, 
            MSG_WAITALL, ( struct sockaddr *) &cliaddr,
            &len);
      printf("size %u\n", size);
      
      
      FILE * file = fopen(name, "w");
      count = 0;

      while(count < size){
         n = recvfrom(sockfd, buffer, 65507, 
              0, ( struct sockaddr *) &cliaddr,
              &len);

         printf("read chunk %d\n", n);
         if(0 > n) { break; }
         count += n;
         fwrite(buffer, 1, n, file);
      }
      fclose(file);
    }
    return 0;
}
