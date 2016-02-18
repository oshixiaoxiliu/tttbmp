#ifndef DATAFIELDDEF_H_INCLUDED
#define DATAFIELDDEF_H_INCLUDED


#define WORD    unsigned short
#define DWORD   unsigned int
#define LONG    int
#define BYTE    unsigned char

#pragma pack(1)
typedef struct {
	WORD    bfType ;
	DWORD   bfSize ;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
}  BITMAPFILEHEADER , *PBITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER {
	DWORD 	biSize;
	LONG 	biWidth;
	LONG 	biHeight;
	WORD 	biPlanes;
	WORD 	biBitCount;
	DWORD 	biCompression;
	DWORD 	biSizeImage;
	LONG 	biXPelsPerMeter;
	LONG 	biYPelsPerMeter;
	DWORD 	biClrUsed;
	DWORD 	biClrImportant;
} BITMAPINFOHEADER , *PBITMAPINFOHEADER;
#pragma pack()

typedef struct tagRGBQUAD {
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbReserved;
} RgbQuad , *PRgbQuad;

typedef struct {
	BITMAPFILEHEADER    bitMapFileHeader;
	BITMAPINFOHEADER    bitMapInfoHeader;
	RgbQuad*            rgbQuad;
	int                 width;
	int                 height;
	int                 channels;
	BYTE*               imageData;
} BMPImage;

#endif // DATAFIELDDEF_H_INCLUDED
