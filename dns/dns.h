


class DNSQuestion
{
	public:
	char *qname;
	unsigned short qtype;
	unsigned short qclass;

	public:
	char *Pack( char *data, int buffer_length );
	char *UnPack( char *data, int length );
	void Tell();
};

class DNSRR
{
	public:
	char *name; 
	unsigned short type;
	unsigned short class;
	unsigned int ttl;
	int rd_length;
	char *rdata;

	public:
	char *Pack( char *data, int buffer_length );
	char *UnPack( char *data, int length );
	void Tell();
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

	DNSQuestion *qd_head;
	DNSRR *an_head;
	DNSRR *ns_head;
	DNSRR *ar_head );

	public:
	void add_qd( DNSQuestion *question );
	void add_an( DNSRR *answer );
	void add_ns( DNSRR *nameserver );
	void add_ar( DNSRR *ar);

	char *Pack( char *data, int buffer_length );
	char *UnPack( char *data, int length );
	void Tell();
};


