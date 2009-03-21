
#define NUM_SOURCES (10)

class Source
{
	public:

	Source();
	static Source *Record( unsigned short port, unsigned int address, int ref);
	static Source *findRef( int ref );
	unsigned short port;
	unsigned int address;
	int ref;

	static Source sources[NUM_SOURCES];
	static void Tell();
};
