#include <iostream>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#define BYTE unsigned char 
#define WORD unsigned short
#define DWORD unsigned int
 

using namespace std;





class ImageMatrix{
public:
	int height;
	int width;
	int rowsize;

public:
	BYTE* term;
	ImageMatrix();
	ImageMatrix(int height, int width,int d);
	
	
	int Load(char* filename);
	bool Save_24bit(char* filename);
	bool Save_32bit(char* filename);
};
