#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include "DataFieldDef.h"
#include "BMPReadAndWrite.h"

#define MAXCHAR 1000000
#define MXALINE 1000

bool save24BITImage(const char* path, BMPImage* bmpImg);
int totlen ;
char * buf ;
void getData(char * path)
{
	char * bufp = buf ;
	int len ;
	FILE * txt = fopen(path,"rb");
	if(txt == NULL) {
		printf("error ,can't find file ");
		return ;
	}
	totlen = 0 ;
	while(fgets(bufp,MXALINE,txt)!=NULL) {
		len = strlen(bufp) ;
		totlen += (len) ;
		bufp = (buf +  totlen ) ;
	}
	printf("totlen = %d\n",totlen);
}

int findSquare(int x )
{
	int xr = (int)sqrt((double)x);
	if(xr * xr == x ) {
		return xr ;
	} else {
		return xr + 1 ;
	}
}
int main()
{
	buf = (char*)malloc(MAXCHAR);
	memset(buf,0,MAXCHAR) ;
	getData("main.txt");
	int channel = 3 ;
	int xr = findSquare((totlen + channel - 1 ) /channel );
	int size = xr * xr * channel ;
	printf("xr = %d  totlen = %d\n",xr,totlen );
	BYTE* bytes = (BYTE *)malloc(sizeof(BYTE) * size);
	memset(bytes,0, size ) ;
	for(int i = 0; i < totlen ; i ++) bytes[i] = buf[i];

	BMPImage* bmp = (BMPImage*)malloc(sizeof(BMPImage));
	bmp->channels =  channel ;
	bmp->height = xr ;
	bmp->width = xr ;
	bmp->imageData = bytes ;

	save24BITImage("aaa.bmp",bmp);
	BMPImage*img = loadImage("aaa.bmp");
	printf("width = %d\n",img->width);
	printf("height = %d\n",img->height);
	int len = img->channels * img->height * img->width ;
	for(int i = 0 ; i < len ; i ++ ) {
		printf("%c",img->imageData[i]);
	}
	free(buf);
	
	return 0;
}
