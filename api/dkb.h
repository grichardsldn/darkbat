
class dkbElement;

// shapes are relative to 0-0-0.
class dkbShape
{
	public:	
	void addLine( int x1, int y1, int z1, int x2, int y2, int z2, int col);
	void addPoint( int x1, int x2, int y2, int col);
	void addFlatRect( int x, int y, int z, int x2, int y2, int z2, int col );

	dkbShape();

	dkbElement *head;
	private:
	void addElement( dkbElement *element );
};

class dkbAngle
{
};
	   

class dkbPos
{
	public:
	int x; 
	int y;
	int z;

	char * write( char *buf );
};

class dkbBlock
{
	public:
	unsigned short block_x;
	unsigned short block_y;
	unsigned char block_z;
};

class dkbShapeEntry;
class UDPSocket;

class dkbObj
{
	public:
	void addShape( dkbShape *shape, dkbAngle angle, dkbPos
		traslation, int ref);
	void removeShape( int ref);

	bool connect( dkbBlock block);
	void project( dkbBlock block, dkbPos position  );
	void moveRel( dkbPos pos );

	dkbObj();

	static void *start(void *ptr);
	void Start();
	
	private:
	void Xmit();
	dkbShapeEntry *shapes[10];
	dkbPos position;
	pthread_t run_thread;
	UDPSocket *socket;
	int ref;
};


