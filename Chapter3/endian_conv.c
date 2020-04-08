#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	unsigned short host_port=0x1234;
	unsigned short net_port;
	unsigned long host_addr=0x12345678;
	unsigned long net_addr;
	
	net_port=htons(host_port);
	net_addr=htonl(host_addr);
	
	printf("\n host to network \n");
	printf("Host ordered port: %#x \n", host_port);
	printf("Network ordered port: %#x \n", net_port);
	printf("Host ordered address: %#lx \n", host_addr);
	printf("Network ordered address: %#lx \n", net_addr);
	
	printf("\n network to host \n");

	host_port=ntohs(net_port);
	host_addr=ntohl(net_addr);

	printf("Network ordered port: %#x \n", net_port);
	printf("Host ordered port: %#x \n", host_port);
	printf("Network ordered address: %#lx \n", net_addr);
	printf("Host ordered address: %#lx \n", host_addr);
	
	return 0;
}