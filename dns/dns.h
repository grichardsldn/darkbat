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



