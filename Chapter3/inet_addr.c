#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	char *addr1="1.2.3.4";
	char *addr2="127.212.124.256";
	char *addr3="127.212.124.255";

	unsigned long conv_addr=inet_addr(addr1);
	if(conv_addr==INADDR_NONE)
		printf("Error occured! \n");
	else
		printf("Network ordered integer addr: %s -> %#lx \n", addr1, conv_addr);
	
	conv_addr=inet_addr(addr2);
	if(conv_addr==INADDR_NONE)
		printf("Error occureded \n");
	else
		printf("Network ordered integer addr: %s -> %#lx \n\n", addr2, conv_addr);

	conv_addr=inet_addr(addr3);
	if(conv_addr==INADDR_NONE)
		printf("Error occureded \n");
	else
		printf("Network ordered integer addr: %s -> %#lx \n\n", addr3, conv_addr);
	return 0;
}