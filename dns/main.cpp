#include <stdio.h>
#include "dns.h"
#include "UDPSocket.h"

main()
{
	UDPSocket sock;

	sock.Bind( 53 );

	char *buf = new char[2000];

	for( ;;)
	{
		DNSMessage *dns= new DNSMessage();
		printf("In Recvfrom\n");
		int red;
		red = sock.ReceiveFrom( buf, 2000 );
		printf("Received Packet\n");
		dns->UnPack( buf, red);
		dns->Tell();
	}
	
}
