#ifndef BMPREADANDWRITE_H
#define BMPREADANDWRITE_H

#include "DataFieldDef.h"

BMPImage* loadImage(const char* path);
bool save24BITImage(const char* path, BMPImage* bmpImg);

#endif // BMPREADANDWRITE_H
