#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <netinet/in.h>


class DNSQuestion
{
	public:
	char qname[1024];
	unsigned short qtype;
	unsigned short qclass;

	public:
	char *Pack( char *data, int buffer_length );
	char *UnPack( char *data, int length );
	void Tell();
	DNSQuestion();
};

DNSQuestion::DNSQuestion()
{
	strcpy(qname, "");
	qtype= 0xffff;
	qclass= 0xffff;
}	

void DNSQuestion::Tell()
{

	printf("DNSQuestion &%08x: qname=\"%s\" qtype=#%d qclass=#%d\n",
		(unsigned int)this, qname, qtype, qclass);
}

// Name string, type word, class word
char *DNSQuestion::UnPack( char *data, int length )
{
	char *cp = data;
	
	assert( data );
	assert( length >= 5 );

	strncpy( qname, data, length );

	cp += strlen( qname );
	
	unsigned short *wp = (unsigned short *)cp;
	qtype = ntohs( *wp++);
	qclass = ntohs( *wp++);

	cp = (char *)wp;
	return cp;
}
	


class DNSRR
{
	public:
	char name[1024]; 
	unsigned short type;
	unsigned short rr_class;
	unsigned int ttl;
	int rd_length;
	char rdata[1024]; 

	public:
	char *Pack( char *data, int buffer_length );
	char *UnPack( char *data, int length );
	void Tell();
	DNSRR();
};

DNSRR::DNSRR()
{
	strcpy(name, "");
	type = 0xffff;
	rr_class = 0xffff;
	ttl=0xffff;
	rd_length = 0;
}	

void DNSRR::Tell()
{
	printf("DNSRR &%08x: name=\"%s\" type=#%d rr_class=#%d, ttl=#%d\n",
		(unsigned int)this, name, type, rr_class,ttl);
	printf("             rd_length=#%d, rdata=&",
		rd_length);
	for( int i = 0 ; i < rd_length ; i++)
	{
		printf("%02x ", rdata[i]);
	}
	printf("\n");
}
	

// Name ASCIIZ, word type, class, ttl, rdlength, rdata
char *DNSRR::UnPack( char *data, int length )
{
	char *cp = data;
	
	assert( data );
	assert( length >= 9 );

	strncpy( name, data, length );

	cp += strlen( name );
	
	unsigned short *wp = (unsigned short *)cp;

	type = ntohs( *wp++);
	rr_class = ntohs( *wp ++);
	ttl = ntohs( *wp++);

	rd_length = ntohs( *wp++);
	memcpy( rdata, (char *)wp, rd_length );
		
	return ((char *)wp)+rd_length;
}

	


class DNSHeader
{
	public:
	unsigned short id;
	bool qr;  // 0=query, 1=response
	char opcode;
	bool aa;  // authorative answer
	bool tc;  // truncation
	bool rd;  // recursion desired
	bool ra;  // recursion available
	char z;  // reserved
	char rcode;   // reponse code
	int qdcount;
	int ancount;
	int nscount;
	int arcount;

	char *UnPack( char *data, int length );
	DNSHeader();
};

DNSHeader::DNSHeader()
{
	id = 0xffff;	
	qr = false;
	opcode = 0xff;
	aa = false;
	tc=false;
	rd=false;
	ra = false;
	rcode = 0xff;
	qdcount = 0;
	ancount = 0;
	nscount = 0;
	arcount = 0;
}	

char *DNSHeader::UnPack( char *data, int length)
{
	assert( length >= 12 );
	unsigned short *wp = (unsigned short *)data;
	id = ntohs(*wp++);

	int flags = ntohs( *wp++);
	qr = flags &1;
	opcode = (flags >> 1 )  & 0xf;	
	aa = (flags >> 5) & 1;
	tc = (flags >> 6) & 1;
	rd = (flags >> 7) & 1;
	ra = (flags >> 8) & 1;
	z = (flags >> 9) & 7;
	rcode = (flags >> 12) & 0xf;
	qdcount = ntohs( *wp++);
	ancount = ntohs( *wp++);
	nscount = ntohs( *wp++);
	arcount = ntohs( *wp++);
	
	return (char *)wp;
}
		
class DNSMessage
{
	public:
	DNSHeader header;
	
	DNSQuestion qds[10];
	DNSRR ans[10];
	DNSRR nss[10];
	DNSRR ars[10];

	public:
	/*
	void add_qd( DNSQuestion *question );
	void add_an( DNSRR *answer );
	void add_ns( DNSRR *nameserver );
	void add_ar( DNSRR *ar);
	*/

	char *Pack( char *data, int buffer_length );
	char *UnPack( char *data, int length );
	void Tell();
};


char *DNSMessage::UnPack( char *data, int length )
{
	char *cp;
	cp = header.UnPack( data, length );
	for(int i = 0 ; i < header.qdcount ; i++)
	{
		cp = qds[i].UnPack( cp, length );		
	}

	return cp;
}	

void DNSMessage::Tell()
{
	printf("DNSMessage:&%08x id=%04x, qr=%d (%s), opcode=%02x\n",
		(unsigned int)this, header.id, 
		header.qr, header.qr?"R":"Q", header.opcode);
	printf("  aa=%d tc=%d rd=%d ra=%d a=%d rcode=#%d\n",
		header.aa, header.tc, header.rd, header.ra, header.z,
		header.opcode);
	printf("  qdcount=#%d:\n", header.qdcount);

	for( int i=0; i < header.qdcount ; i++)
	{	
		qds[i].Tell();
	}	
}



main()
{
}
