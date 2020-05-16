#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <windows.h>
#define BYTE unsigned char 
#define WORD unsigned short
#define DWORD unsigned int
#include	"main.h" 

using namespace std;

/*struct BITMAPFILEHEADER{
	
	WORD bfType;
	DWORD bfsize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
};
struct BITMAPINFOHEADER{

	DWORD bisize;
	int biWidth;
	int biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	int biXPelsPerMeter;
	int biXPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;	
	
};
預設bmp內容 
*/
ImageMatrix::ImageMatrix(){
		height=0;
		width=0;
}

ImageMatrix::ImageMatrix(int height, int width, int d){
	if(d==24){
		this->height = height;
		this->width = width;
		rowsize= (3*width + 3)/4 *4;					//padding
		term = new BYTE[height * rowsize];
	}
	else if(d==32){
		this->height = height;
		this->width = width;
		rowsize= (width *4) + 4;					//padding
		term = new BYTE[height * rowsize];
	}
}

int ImageMatrix::Load(char *filename){
		BITMAPFILEHEADER h;
		BITMAPINFOHEADER hInfo;
		ifstream f;
		f.open(filename, ios::binary);
		f.seekg(0,f.beg);
		f.read((char*)&h, 14);
		f.read((char*)&hInfo, 40);

		width = hInfo.biWidth;
		height = hInfo.biHeight;
		cout<<"reading from "<<filename<<" now ...."<<endl;
		cout<<"wait a minute"<<endl;
		if(hInfo.biBitCount==24)
			*this = ImageMatrix (height,width,24);
		if(hInfo.biBitCount==32)
			*this = ImageMatrix (height,width,32);
		f.read((char*)term , height * rowsize);
		cout<<"width= "<<width<<endl;			//widht and height
		cout<<"height= "<<height<<endl;
		f.close();
		if(hInfo.biBitCount==24)
			return 24;
		else
			return 32;	
}

bool ImageMatrix::Save_24bit(char* filename){
	BITMAPFILEHEADER h={
			0x4d42,			//檔案格式 
			54 + rowsize*height,
			0,
			0,
			54};
	
		BITMAPINFOHEADER hInfo={
			40, 
			width,
			height,
			1,			 //平面維度 
			24,			//pixel
			0,			//無壓縮 
			rowsize*height,		//圖像大小 
			3780,		
			3780,		
			0,			//調色盤	
			0};
		cout<<"writing into "<<filename<<" now ..."<<endl;
		cout<<"wait a minute"<<endl; 
		ofstream f;
		f.open(filename, ios::binary);
		f.write((char*)&h, sizeof(h)); 
		f.write((char*)&hInfo, sizeof(hInfo)); 
		f.write((char*)term , rowsize*height);
		f.close();
		return true;
}
bool ImageMatrix::Save_32bit(char* filename){
	BITMAPFILEHEADER h={
			0x4d42,			//檔案格式 
			122 + rowsize*height,
			0,
			0,
			122};
	
		BITMAPINFOHEADER hInfo={
			40, 
			width,
			height,
			1,			 //平面維度 
			32,			//pixel
			0,			//無壓縮 
			rowsize*height,		//圖像大小 
			3780,		
			3780,		
			0,			//調色盤	
			0};
		cout<<"writing into "<<filename<<" now ..."<<endl;
		cout<<"wait a minute"<<endl; 
		ofstream f;
		f.open(filename, ios::binary);
		f.write((char*)&h, sizeof(h)); 
		f.write((char*)&hInfo, sizeof(hInfo)); 
		f.write((char*)term , rowsize*height);
		f.seekp(0,ios::beg);
		f.close();
		return true;
}

ImageMatrix changebrightness_24bit(ImageMatrix , double );
ImageMatrix changebrightness_24bit(ImageMatrix m, double multiply){

	ImageMatrix new_m = ImageMatrix(m.height, m.width,24);
	int height,width;
	
	for(height=0;height<m.height;height++){				//File Offset to PixelArray
		
		for(width=0;width<m.rowsize;width+=3){
			
			try{
				BYTE B = (m.term[height * m.rowsize + width]);
								
				if( B*multiply > 255){
					new_m.term[height * m.rowsize + width]=255; 
					throw(height,width);
				}
				else{
					B *= multiply;
					new_m.term[height * m.rowsize + width]=B;
				}
				
								
						
			}
			catch(int height){
				cout<<"Pixel("<<height<<","<<width<<") B overflow"<<endl;
			}
			
			try{
				BYTE G = (m.term[height * m.rowsize + (width+1)]);
				if( G * multiply > 255){
					new_m.term[height * m.rowsize + (width+1)]=255; 
					throw(height,width);
				}
				else{
					G *= multiply;	
					new_m.term[height * m.rowsize + (width+1)]=G;
				}
			
			}
			catch(int height){
				cout<<"Pixel("<<height<<","<<width<<") G overflow"<<endl;
			}

			

			try{
				BYTE R = (m.term[height * m.rowsize + (width+2)]);
				if( R * multiply > 255){
					new_m.term[height * m.rowsize + (width+2)]=255; 
					throw(height,width);
				}
				else{
					
					R *= multiply;
					new_m.term[height * m.rowsize + (width+2)]=R;
				}
			}
			catch(int height){
				cout<<"Pixel("<<height<<","<<width<<") R overflow"<<endl;
			}
		}
	}

	return new_m;
}
ImageMatrix changebrightness_32bit(ImageMatrix , double );
ImageMatrix changebrightness_32bit(ImageMatrix m, double multiply){

	ImageMatrix new_m = ImageMatrix(m.height, m.width,32);
	int height,width;
	
	for(height=0;height<m.height;height++){
		
		for(width=0;width<m.rowsize;width+=4){		//略過A變數 
			
			try{
				BYTE B = (m.term[height * m.rowsize + width]);
								
				if( B*multiply > 255){
					new_m.term[height * m.rowsize + width]=255; 
					throw(height,width);
				}
				else{
					B *= multiply;
					new_m.term[height * m.rowsize + width]=B;
				}
				
								
						
			}
			catch(int height){
				cout<<"Pixel("<<height<<","<<width<<") B overflow"<<endl;
			}
			
			try{
				BYTE G = (m.term[height * m.rowsize + (width+1)]);
				if( G * multiply > 255){
					new_m.term[height * m.rowsize + (width+1)]=255; 
					throw(height,width);
				}
				else{
					G *= multiply;	
					new_m.term[height * m.rowsize + (width+1)]=G;
				}
			
			}
			catch(int height){
				cout<<"Pixel("<<height<<","<<width<<") G overflow"<<endl;
			}

			

			try{
				BYTE R = (m.term[height * m.rowsize + (width+2)]);
				if( R * multiply > 255){
					new_m.term[height * m.rowsize + (width+2)]=255; 
					throw(height,width);
				}
				else{
					
					R *= multiply;
					new_m.term[height * m.rowsize + (width+2)]=R;
				}
			}
			catch(int height){
				cout<<"Pixel("<<height<<","<<width<<") R overflow"<<endl;
			}
		}
	}

	return new_m;
}


int main(int argc, char *argv[]){
	ImageMatrix m, new_m;
	int bit;
	double multiply;
	multiply = strtod(argv[3],NULL);
	bit = m.Load(argv[1]);
	if(bit==24){
		cout<<"the bmp image is 24bits (RGB) "<<endl;
		new_m = changebrightness_24bit(m,multiply);
		new_m.Save_24bit(argv[2]);
	}
			
	else{
		cout<<"the bmp image is 32bits (ARGB) "<<endl;
		new_m = changebrightness_32bit(m,multiply);
		new_m.Save_32bit(argv[2]);
	}
	cout<<"success!!!"<<endl;
	system("Pause");
	return 0;
}	
