#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include "DataFieldDef.h"

BMPImage* loadImage(const char* path)
{
	BMPImage* bmpImg;
	FILE* pFile;
	WORD fileType;
	BITMAPFILEHEADER  bmpFileHeader;
	BITMAPINFOHEADER  bmpInfoHeader;
	int channels = 1;
	int width = 0;
	int height = 0;
	int step = 0;
	int offset = 0;
	BYTE pixVal;
	RgbQuad* quad;
	int i, j, k;

	bmpImg = (BMPImage*)malloc(sizeof(BMPImage));
	pFile = fopen(path, "rb");
	if (!pFile) {
		free(bmpImg);
		return NULL;
	}
	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);
	fileType =  bmpFileHeader.bfType;
	if (fileType == 0x4D42) {
		printf("bmp file! \n");

		bmpImg->bitMapFileHeader =  bmpFileHeader;

		printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
		printf("bmp文件头信息：\n");
		printf("文件大小：%d \n", bmpFileHeader.bfSize);
		printf("保留字：%d \n", bmpFileHeader.bfReserved1);
		printf("保留字：%d \n", bmpFileHeader.bfReserved2);
		printf("位图数据偏移字节数：%d \n", bmpFileHeader.bfOffBits);

		fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

		bmpImg->bitMapInfoHeader = bmpInfoHeader;

		printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
		printf("bmp文件信息头\n");
		printf("结构体长度：%lu \n", bmpInfoHeader.biSize);
		printf("位图宽度：%d \n", bmpInfoHeader.biWidth);
		printf("位图高度：%d \n", bmpInfoHeader.biHeight);
		printf("位图平面数：%d \n", bmpInfoHeader.biPlanes);
		printf("颜色位数BitCount ：%d \n", bmpInfoHeader.biBitCount);
		printf("压缩方式：%lu \n", bmpInfoHeader.biCompression);
		printf("实际位图数据占用的字节数：%lu \n", bmpInfoHeader.biSizeImage);
		printf("X方向分辨率：%ld \n", bmpInfoHeader.biXPelsPerMeter);
		printf("Y方向分辨率：%ld \n", bmpInfoHeader.biYPelsPerMeter);
		printf("使用的颜色数：%lu \n", bmpInfoHeader.biClrUsed);
		printf("重要颜色数：%lu \n", bmpInfoHeader.biClrImportant);
		printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");

		if(bmpInfoHeader.biBitCount == 1) {
			printf("该文件有调色板，即该位图为非真彩色\n\n");
			channels = -1;
			width = bmpInfoHeader.biWidth;
			height = bmpInfoHeader.biHeight;
			//offset = (channels*width)%4;
			int  imageDataSize = (width / 8 + 3 ) / 4 * 4 ;
			offset = imageDataSize * 8 - width ;
			printf("offset = %d\n",offset);
			bmpImg->width = width;
			bmpImg->height = height;
			bmpImg->channels = channels;
			bmpImg->imageData = (BYTE*)malloc(sizeof(BYTE)*imageDataSize*height);
			step = (width + 7) / 8 ;
			//step = channels*width;

			quad = (RgbQuad*)malloc(sizeof(RgbQuad)*2);
			fread(quad, sizeof(RgbQuad), 2, pFile);
			bmpImg->rgbQuad = quad;
			//free(quad);

			for (i=0; i<height; i++) {
				for (j=0; j< (width + 7 ) / 8 ; j++) {
					fread(&pixVal, sizeof(BYTE), 1, pFile);
					bmpImg->imageData[(height-1-i)*step+j] = pixVal;
				}
				if (offset != 0) {
					for(j = 0 ; j < imageDataSize - step ; j ++  )
						fread(&pixVal, sizeof(BYTE), 1, pFile);
					/*for (j=0; j<offset; j++)
					{
					    fread(&pixVal, sizeof(BYTE), 1, pFile);
					}
					*/
				}
			}
		} else if (bmpInfoHeader.biBitCount == 4) {
			printf("\n该文件有调色板，即该位图为非真彩色\n\n");
			channels = 0;
			width = bmpInfoHeader.biWidth;
			height = bmpInfoHeader.biHeight;
			//offset = (channels*width)%4;
			step = (width * 4 + 7 ) / 8 ;
			int  imageDataSize = ((width * 4 + 7) / 8 + 3 ) / 4 * 4 ;
			offset = imageDataSize - step   ;
			//printf("imageDataSize = %d ,step = %d ,  offset = %d\n",imageDataSize , step,offset);
			bmpImg->width = width;
			bmpImg->height = height;
			bmpImg->channels = channels;
			//printf("size = %d\n",imageDataSize*height);
			bmpImg->imageData = (BYTE*)malloc(sizeof(BYTE)*imageDataSize*height);
			// printf("step = %d\n",step );
			//step = channels*width;

			quad = (RgbQuad*)malloc(sizeof(RgbQuad)*16);
			fread(quad, sizeof(RgbQuad), 16, pFile);
			bmpImg->rgbQuad = quad;
			//free(quad);

			for (i=0; i < height; i++) {
				for (j=0; j < step ; j++) {
					fread(&pixVal, sizeof(BYTE), 1, pFile);
					bmpImg->imageData[(height-1-i)*step+j] = pixVal;
				}
				if (offset != 0) {
					for(j = 0 ; j < offset ; j ++  )
						fread(&pixVal, sizeof(BYTE), 1, pFile);
					/*for (j=0; j<offset; j++)
					{
					    fread(&pixVal, sizeof(BYTE), 1, pFile);
					}
					*/
				}
			}
		} else if (bmpInfoHeader.biBitCount == 8) {
			printf("该文件有调色板，即该位图为非真彩色\n\n");
			channels = 1;
			width = bmpInfoHeader.biWidth;
			height = bmpInfoHeader.biHeight;
			offset = (channels*width)%4;
			if (offset != 0) {
				offset = 4 - offset;
			}
			bmpImg->width = width;
			bmpImg->height = height;
			bmpImg->channels = 1;
			bmpImg->imageData = (BYTE*)malloc(sizeof(BYTE)*width*height);
			step = channels*width;

			quad = (RgbQuad*)malloc(sizeof(RgbQuad)*256);
			fread(quad, sizeof(RgbQuad), 256, pFile);
			bmpImg->rgbQuad = quad;
			//free(quad);

			for (i=0; i<height; i++) {
				for (j=0; j<width; j++) {
					fread(&pixVal, sizeof(BYTE), 1, pFile);
					bmpImg->imageData[(height-1-i)*step+j] = pixVal;
				}
				if (offset != 0) {
					for (j=0; j<offset; j++) {
						fread(&pixVal, sizeof(BYTE), 1, pFile);
					}
				}
			}
		} else if (bmpInfoHeader.biBitCount == 24 ) {
			printf("该位图为位真彩色\n\n");
			bmpInfoHeader.biBitCount = 24;
			channels = 3;
			width = bmpInfoHeader.biWidth;
			height = bmpInfoHeader.biHeight;

			bmpImg->width = width;
			bmpImg->height = height;
			bmpImg->channels = 3;
			bmpImg->imageData = (BYTE*)malloc(sizeof(BYTE)*width*3*height);
			step = channels*width;

			offset = (channels*width)%4;
			if (offset != 0) {
				offset = 4 - offset;
			}

			for (i=0; i<height; i++) {
				for (j=0; j<width; j++) {
					for (k=0; k<3; k++) {
						fread(&pixVal, sizeof(BYTE), 1, pFile);
						bmpImg->imageData[(height-1-i)*step+j*3+k] = pixVal;
					}
				}
				if (offset != 0) {
					for (j=0; j<offset; j++) {
						fread(&pixVal, sizeof(BYTE), 1, pFile);
					}
				}
			}
		}
	}

	return bmpImg;
}


bool save24BITImage(const char* path, BMPImage* bmpImg)
{
	FILE *pFile;
	unsigned short fileType;
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	int step;
	int offset;
	BYTE pixVal = '\0';
	int i, j;
	RgbQuad* quad;

	pFile = fopen(path, "wb");
	if (!pFile) {
		return false;
	}

	fileType = 0x4D42;
	// fwrite(&fileType, sizeof(unsigned short), 1, pFile);
	bmpFileHeader.bfType = fileType;

	if (bmpImg->channels == 3) { //24位，通道，彩图
		step = bmpImg->channels*bmpImg->width;
		offset = step%4;
		if (offset != 4) {
			step += 4-offset;
		}


		bmpFileHeader.bfSize = bmpImg->height*step + 54;
		bmpFileHeader.bfReserved1 = 0;
		bmpFileHeader.bfReserved2 = 0;
		bmpFileHeader.bfOffBits = 54;
		//fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);

		bmpInfoHeader.biSize = 40;
		bmpInfoHeader.biWidth = bmpImg->width;
		bmpInfoHeader.biHeight = bmpImg->height;
		bmpInfoHeader.biPlanes = 1;
		bmpInfoHeader.biBitCount = 24;
		bmpInfoHeader.biCompression = 0;
		bmpInfoHeader.biSizeImage = bmpImg->height*step;
		bmpInfoHeader.biXPelsPerMeter = 0;
		bmpInfoHeader.biYPelsPerMeter = 0;
		bmpInfoHeader.biClrUsed = 0;
		bmpInfoHeader.biClrImportant = 0;
		//fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);


		fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);
		fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

		for (i=bmpImg->height-1; i>-1; i--) {
			for (j=0; j<bmpImg->width; j++) {
				pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3];
				fwrite(&pixVal, sizeof(BYTE), 1, pFile);
				pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+1];
				fwrite(&pixVal, sizeof(BYTE), 1, pFile);
				pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+2];
				fwrite(&pixVal, sizeof(BYTE), 1, pFile);
			}
			if (offset!=0) {
				for (j=0; j<4-offset; j++) {
					pixVal = 0;
					fwrite(&pixVal, sizeof(BYTE), 1, pFile);
				}
			}
		}
	} else if (bmpImg->channels == 1) { //8位，单通道，灰度图
		step = bmpImg->width;
		offset = step%4;
		if (offset != 4) {
			step += 4 - offset;
		}

		//  bmp文件四个部分的大小只和，可以错误
		bmpFileHeader.bfSize = 54 + 256*4 + (bmpImg->width + 3 ) / 4 * 4 * bmpImg->height ;

		printf("bmpFileHeader.bfSize = %d\n" , bmpFileHeader.bfSize);
		bmpFileHeader.bfReserved1 = 0;
		bmpFileHeader.bfReserved2 = 0;
		bmpFileHeader.bfOffBits = 54 + 256*4;
		// fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);

		bmpInfoHeader.biSize = 40;
		bmpInfoHeader.biWidth = bmpImg->width;
		bmpInfoHeader.biHeight = bmpImg->height;
		bmpInfoHeader.biPlanes = 1;
		bmpInfoHeader.biBitCount = 8;
		bmpInfoHeader.biCompression = 0;
		bmpInfoHeader.biSizeImage = bmpImg->height*step;
		bmpInfoHeader.biXPelsPerMeter = 0;
		bmpInfoHeader.biYPelsPerMeter = 0;
		bmpInfoHeader.biClrUsed = 0;
		bmpInfoHeader.biClrImportant = 0;
		///fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

		fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);

		fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);


		quad = (RgbQuad*)malloc(sizeof(RgbQuad)*256);
		for (i=0; i<256; i++) {
			quad[i].rgbBlue = i;
			quad[i].rgbGreen = i;
			quad[i].rgbRed = i;
			quad[i].rgbReserved = 0;
		}
		fwrite(quad, sizeof(RgbQuad), 256, pFile);
		free(quad);

		// fwrite(bmpImg->rgbQuad,sizeof(RgbQuad),256,pFile);


		for (i=bmpImg->height-1; i>-1; i--) {
			for (j=0; j<bmpImg->width; j++) {
				pixVal = bmpImg->imageData[i*bmpImg->width+j];
				fwrite(&pixVal, sizeof(BYTE), 1, pFile);
			}
			if (offset!=0) {
				for (j=0; j<4-offset; j++) {
					pixVal = 0;
					fwrite(&pixVal, sizeof(BYTE), 1, pFile);
				}
			}
		}
	}

	fclose(pFile);




	return true;
}





bool saveImage(const char* path, BMPImage* bmpImg)
{
	FILE *pFile;
	unsigned short fileType;
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	int step;
	int offset;
	BYTE pixVal = '\0';
	int i, j;
	RgbQuad* quad;

	pFile = fopen(path, "wb");
	if (!pFile) {
		return false;
	}

	fileType = 0x4D42;
	//fwrite(&fileType, sizeof(unsigned short), 1, pFile);
	bmpFileHeader.bfType = fileType;


	if(bmpImg->channels == -1) {
		int  imageDataSize = (bmpImg->width / 8 + 3 ) / 4 * 4 ;
		step = ((bmpImg->width + 7 ) / 8 )  ;
		offset = imageDataSize - step ;
		/*
		        bmpFileHeader.bfSize = 54 + 4*2 + imageDataSize *(bmpImg->height);
		        bmpFileHeader.bfReserved1 = 0;
		        bmpFileHeader.bfReserved2 = 0;
		        bmpFileHeader.bfOffBits = 54 + 4*2;
		        fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);


		        bmpInfoHeader.biSize = 40;
		        bmpInfoHeader.biWidth = bmpImg->width;
		        bmpInfoHeader.biHeight = bmpImg->height;
		        bmpInfoHeader.biPlanes = 1;
		        bmpInfoHeader.biBitCount = 1;
		        bmpInfoHeader.biCompression = 0;
		        bmpInfoHeader.biSizeImage = bmpImg->height*step;
		        bmpInfoHeader.biXPelsPerMeter = 0;
		        bmpInfoHeader.biYPelsPerMeter = 0;
		        bmpInfoHeader.biClrUsed = 0;
		        bmpInfoHeader.biClrImportant = 0;
		        fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
		*/
		fwrite(&bmpImg->bitMapFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);
		fwrite(&bmpImg->bitMapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

		/*
		                quad = (RgbQuad*)malloc(sizeof(RgbQuad)*2);
		                for (i=0; i<2; i++)
		                {
		                    quad[i].rgbBlue = i;
		                    quad[i].rgbGreen = i;
		                    quad[i].rgbRed = i;
		                    quad[i].rgbReserved = 0;
		                }
		                fwrite(quad, sizeof(RgbQuad), 2, pFile);
		                free(quad);
		*/
		fwrite(bmpImg->rgbQuad,sizeof(RgbQuad),2,pFile);


		for (i=bmpImg->height-1; i>-1; i--) {
			for (j=0; j< (bmpImg->width +7 )/ 8; j++) {
				pixVal = bmpImg->imageData[i*step+j];
				fwrite(&pixVal, sizeof(BYTE), 1, pFile);
			}
			if (offset!=0) {
				for (j=0; j<offset; j++) {
					pixVal = 0;
					fwrite(&pixVal, sizeof(BYTE), 1, pFile);
				}
			}
		}
	} else if(bmpImg->channels == 0 ) {

		int  imageDataSize = ((bmpImg->width * 4 + 7 ) / 8 + 3 ) / 4 * 4 ;
		step = ((bmpImg->width * 4) + 7 ) / 8  ;
		offset = imageDataSize - step ;
		//printf("imageDataSize = %d ,step = %d ,  offset = %d\n",imageDataSize , step,offset);
		// fflush(stdout);
		/*
		        bmpFileHeader.bfSize = 54 + 4*16 + imageDataSize * bmpImg->height;
		        bmpFileHeader.bfReserved1 = 0;
		        bmpFileHeader.bfReserved2 = 0;
		        bmpFileHeader.bfOffBits = 54 + 4*16;
		        fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);

		        bmpInfoHeader.biSize = 40;
		        bmpInfoHeader.biWidth = bmpImg->width;
		        bmpInfoHeader.biHeight = bmpImg->height;
		        bmpInfoHeader.biPlanes = 1;
		        bmpInfoHeader.biBitCount = 4;
		        bmpInfoHeader.biCompression = 0;
		        bmpInfoHeader.biSizeImage = bmpImg->height*step;
		        bmpInfoHeader.biXPelsPerMeter = 0;
		        bmpInfoHeader.biYPelsPerMeter = 0;
		        bmpInfoHeader.biClrUsed = 0;
		        bmpInfoHeader.biClrImportant = 0;
		        fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
		*/
		fwrite(&bmpImg->bitMapFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);
		fwrite(&bmpImg->bitMapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);


		fwrite(bmpImg->rgbQuad,sizeof(RgbQuad),16,pFile);


		for (i=bmpImg->height-1; i>-1; i--) {
			for (j=0; j< (bmpImg->width * 4 + 7 )/ 8; j++) {
				pixVal = bmpImg->imageData[i*step+j];
				fwrite(&pixVal, sizeof(BYTE), 1, pFile);
			}
			if (offset!=0) {
				for (j=0; j<offset; j++) {
					pixVal = 0;
					fwrite(&pixVal, sizeof(BYTE), 1, pFile);
				}
			}
		}
	}
	if (bmpImg->channels == 3) { //24位，通道，彩图
		step = bmpImg->channels*bmpImg->width;
		offset = step%4;
		if (offset != 4) {
			step += 4-offset;
		}

		/*
		        bmpFileHeader.bfSize = bmpImg->height*step + 54;
		        bmpFileHeader.bfReserved1 = 0;
		        bmpFileHeader.bfReserved2 = 0;
		        bmpFileHeader.bfOffBits = 54;
		        fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);

		        bmpInfoHeader.biSize = 40;
		        bmpInfoHeader.biWidth = bmpImg->width;
		        bmpInfoHeader.biHeight = bmpImg->height;
		        bmpInfoHeader.biPlanes = 1;
		        bmpInfoHeader.biBitCount = 24;
		        bmpInfoHeader.biCompression = 0;
		        bmpInfoHeader.biSizeImage = bmpImg->height*step;
		        bmpInfoHeader.biXPelsPerMeter = 0;
		        bmpInfoHeader.biYPelsPerMeter = 0;
		        bmpInfoHeader.biClrUsed = 0;
		        bmpInfoHeader.biClrImportant = 0;
		        fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
		*/

		fwrite(&bmpImg->bitMapFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);
		fwrite(&bmpImg->bitMapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

		for (i=bmpImg->height-1; i>-1; i--) {
			for (j=0; j<bmpImg->width; j++) {
				pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3];
				fwrite(&pixVal, sizeof(BYTE), 1, pFile);
				pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+1];
				fwrite(&pixVal, sizeof(BYTE), 1, pFile);
				pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+2];
				fwrite(&pixVal, sizeof(BYTE), 1, pFile);
			}
			if (offset!=0) {
				for (j=0; j<4-offset; j++) {
					pixVal = 0;
					fwrite(&pixVal, sizeof(BYTE), 1, pFile);
				}
			}
		}
	} else if (bmpImg->channels == 1) { //8位，单通道，灰度图
		step = bmpImg->width;
		offset = step%4;
		if (offset != 4) {
			step += 4 - offset;
		}

		//  bmp文件四个部分的大小只和，可以错误
		bmpFileHeader.bfSize = 54 + 256*4 + (bmpImg->width + 3 ) / 4 * 4 * bmpImg->height ;

		printf("bmpFileHeader.bfSize = %d\n" , bmpFileHeader.bfSize);
		bmpFileHeader.bfReserved1 = 0;
		bmpFileHeader.bfReserved2 = 0;
		bmpFileHeader.bfOffBits = 54 + 256*4;
		fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);

		bmpInfoHeader.biSize = 40;
		bmpInfoHeader.biWidth = bmpImg->width;
		bmpInfoHeader.biHeight = bmpImg->height;
		bmpInfoHeader.biPlanes = 1;
		bmpInfoHeader.biBitCount = 8;
		bmpInfoHeader.biCompression = 0;
		bmpInfoHeader.biSizeImage = bmpImg->height*step;
		bmpInfoHeader.biXPelsPerMeter = 0;
		bmpInfoHeader.biYPelsPerMeter = 0;
		bmpInfoHeader.biClrUsed = 0;
		bmpInfoHeader.biClrImportant = 0;
		fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

		//  fwrite(bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);

		//    fwrite(bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);


		quad = (RgbQuad*)malloc(sizeof(RgbQuad)*256);
		for (i=0; i<256; i++) {
			quad[i].rgbBlue = i;
			quad[i].rgbGreen = i;
			quad[i].rgbRed = i;
			quad[i].rgbReserved = 0;
		}
		fwrite(quad, sizeof(RgbQuad), 256, pFile);
		free(quad);

		// fwrite(bmpImg->rgbQuad,sizeof(RgbQuad),256,pFile);


		for (i=bmpImg->height-1; i>-1; i--) {
			for (j=0; j<bmpImg->width; j++) {
				pixVal = bmpImg->imageData[i*bmpImg->width+j];
				fwrite(&pixVal, sizeof(BYTE), 1, pFile);
			}
			if (offset!=0) {
				for (j=0; j<4-offset; j++) {
					pixVal = 0;
					fwrite(&pixVal, sizeof(BYTE), 1, pFile);
				}
			}
		}
	}
	fclose(pFile);

	return true;
}
