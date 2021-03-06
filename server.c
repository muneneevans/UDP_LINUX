/*
    Simple udp server
    Silver Moon (m00n.silv3r@gmail.com)
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
 
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data
 
void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(void)
{
	int clients[10], clientcount , found , client;
    struct sockaddr_in si_me, si_other;
     
    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN] , outbuffer[256] , clientport[5];
     
    //initiate client count
    clientcount = 0 ; 
    found = 0 ;
     
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    printf("server is listening");
     
    //keep listening for data
    while(1)
    {
        
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);
        
        client = atoi(buf);
        printf("new client is : %d\n" , client);
         
        
        for( i = 0 ; i <= clientcount ; i++)
		{
			
			if(clients[i] == client )
			{ 
				found = 1 ;
			} 
		}
		if(found == 0)
		{			
			clients[clientcount] = client ;
			clientcount++ ;
		}
        
        
        
        printf("\n\nclients \n");
        bzero(outbuffer , 256);
        bzero(clientport , 5);
        for( i = 0 ; i < clientcount ; i++)
		{
			printf("%d\n" , clients[i] ); 
			sprintf(clientport , "%d" , clients[i]);
			strcat(outbuffer, clientport);
			strcat(outbuffer, ",");
			
		}
		printf("\n%s" , outbuffer);
        
        
        //reply
        if (sendto(s, outbuffer, strlen(outbuffer), 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
        
    }
 
    close(s);
    return 0;
}
