/*
 * fnc.c
 *
 *  Created on: Apr 16, 2018
 *      Author: max
 */


/**
 *
 */
void memcpyr(char *dst, char* src, int size)
{
	char v;
	while (size>0)
	{
		v= *src++;
	    v = ((v >> 1) & 0x55) | ((v & 0x55) << 1);
	    v = ((v >> 2) & 0x33) | ((v & 0x33) << 2);
	    v = ((v >> 4) & 0x0F) | ((v & 0x0F) << 4);
	    *dst++=v;
	    size--;
	}
}

/**
 *
 */
int memcmpr(char *dst, char* src, int size)
{
	char v;
	int  p;
	p = 0;
	while (size>0) {
		v= *src++;
	    v = ((v >> 1) & 0x55) | ((v & 0x55) << 1);
	    v = ((v >> 2) & 0x33) | ((v & 0x33) << 2);
	    v = ((v >> 4) & 0x0F) | ((v & 0x0F) << 4);
	    p++;
	    if (*dst++ != v) {
	    	return p;
	    }
	    size--;
	}
	return 0;
}



