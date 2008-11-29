#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/videodev.h>
#include <linux/videodev2.h>
#include <unistd.h>
#include <string.h>


/*
main()
{
  int dev;
	dev = open("/dev/video0", O_RDONLY );

	assert( dev != -1);
	printf("Openend device\n");

 	int ret;

	for (int i = 0 ; i < 200 ; i ++)
	{
		char buffer[1000000];
		memset( &buffer[0], 0xcc, 100000);
		ret = read( dev, &buffer[0], 1000000);
		printf("read return #%d\n", ret );
		
		for( int j = 0 ; j < 20 ; j++)
		{
			printf("%02x ", (unsigned char)buffer[j]);
		}
		printf("\n");
		
		//sleep(1);
	}
}
*/
