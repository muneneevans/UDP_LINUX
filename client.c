/*
    Simple udp client
    Silver Moon (m00n.silv3r@gmail.com)
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
 
#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data
 
void die(char *s)
{
    perror(s);
    exit(1);
}
 
void Connect(int port)
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    int peers[10], finalpeers[10] , peercount , found ;
    char buf[BUFLEN];
    char message[BUFLEN];
	char* token; 
 
	
	
	peercount = 0 ;
	found = 0 ;
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
     
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
 
	
    
		printf("connecting");
        printf("Enter message : ");
        sprintf(message , "%d" , port);
        //gets(message);
         
        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }
         
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            die("recvfrom()");
        }
		
		printf("response\n");
        puts(buf);
        
        
        token = strtok(buf, ",");
		while(token != NULL)
		{
			peers[peercount] = atoi(token);		
			token = strtok(NULL, ",");
			peercount++ ; 
		}
		
		printf("\n peers");
		for( i = 0 ; i < peercount ; i ++)
		{			

			
			finalpeers[i] = peers[i] ;
			printf("\n %d" , finalpeers[i]);
			}
			
		
    
 
    close(s);
    
}

int Listen(int port)
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
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);
        
        sprintf(outbuffer , "acknowledged");      
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



int main()
{
	int myport , option; 
	myport = 0 ;
	
	printf("please enter your port\n");
		scanf("%d", &myport);
	
	while(1){
		
		
		printf("\n please enter an option");
		printf("\n 1: Get clients");
		printf("\n 2: Talk to peer");
		scanf("%d" , &option);
		
		switch(option)
		{
			case 1 :
				Connect(myport);
			break ;
			
			case 2 :
			break ;
			
			case 3:
			break ;
			}
				
		
		
		
		
		
		
	
	}
	
	return 0 ;
	}

