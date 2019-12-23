// glutEx1.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <iostream>
#include "glew\glew.h"
#include "glew\glext.h"
#include "glut.h"
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glew32s.lib")
#pragma comment(lib,"glext.lib")



//
/*typedef  struct  tagBITMAPFILEHEADER
{
	unsigned short int  bfType;       //位图文件的类型，必须为BM 
	unsigned long       bfSize;       //文件大小，以字节为单位
	unsigned short int  bfReserverd1; //位图文件保留字，必须为0 
	unsigned short int  bfReserverd2; //位图文件保留字，必须为0 
	unsigned long       bfOffBits;  //位图文件头到数据的偏移量，以字节为单位
}BITMAPFILEHEADER;
typedef  struct  tagBITMAPINFOHEADER
{
	long biSize;                        //该结构大小，字节为单位
	long  biWidth;                     //图形宽度以象素为单位
	long  biHeight;                     //图形高度以象素为单位
	short int  biPlanes;               //目标设备的级别，必须为1 
	short int  biBitcount;             //颜色深度，每个象素所需要的位数
	short int  biCompression;        //位图的压缩类型
	long  biSizeImage;              //位图的大小，以字节为单位
	long  biXPelsPermeter;       //位图水平分辨率，每米像素数
	long  biYPelsPermeter;       //位图垂直分辨率，每米像素数
	long  biClrUsed;            //位图实际使用的颜色表中的颜色数
	long  biClrImportant;       //位图显示过程中重要的颜色数
}BITMAPINFOHEADER;*/

#define BITMAP_ID 0x4D42

GLuint texture[5];
#define TEXH 16
#define TEXW 16

GLubyte tex[TEXH][TEXW][3];  // 图像数据

GLint Texture0 = 0;
GLint Texture1 = 1;

bool blight = false;
//

float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

void Draw_Leg();
void multiTexture(char *filename1, char *filename2, float p);
	//
	//读取纹理的函数
	unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
	{
		FILE *filePtr;	// 文件指针
		BITMAPFILEHEADER bitmapFileHeader;	// bitmap文件头
		unsigned char	*bitmapImage;		// bitmap图像数据
		int	imageIdx = 0;		// 图像位置索引
		unsigned char	tempRGB;	// 交换变量

									// 以“二进制+读”模式打开文件filename 
		filePtr = fopen(filename, "rb");
		if (filePtr == NULL) return NULL;
		// 读入bitmap文件图
		fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
		// 验证是否为bitmap文件
		if (bitmapFileHeader.bfType != BITMAP_ID) {
			fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
			return NULL;
		}

		// 读入bitmap信息头
		fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
		// 将文件指针移至bitmap数据
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
		// 为装载图像数据创建足够的内存
		bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
		// 验证内存是否创建成功
		if (!bitmapImage) {
			fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
			return NULL;
		}

		// 读入bitmap图像数据
		fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
		// 确认读入成功
		if (bitmapImage == NULL) {
			fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
			return NULL;
		}

		//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
		for (imageIdx = 0;
			imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
			tempRGB = bitmapImage[imageIdx];
			bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
			bitmapImage[imageIdx + 2] = tempRGB;
		}
		// 关闭bitmap图像文件
		fclose(filePtr);
		return bitmapImage;
	}

	//加载纹理的函数
	void texload(int i, char *filename)
	{
		//GLuint texture[3];
		BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap信息头
		unsigned char*   bitmapData;                                       // 纹理数据

		bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		// 指定当前纹理的放大/缩小过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D,
			0, 	    //mipmap层次(通常为，表示最上层) 
			GL_RGB,	//我们希望该纹理有红、绿、蓝数据
			bitmapInfoHeader.biWidth, //纹理宽带，必须是n，若有边框+2 
			bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2 
			0, //边框(0=无边框, 1=有边框) 
			GL_RGB,	//bitmap数据的格式
			GL_UNSIGNED_BYTE, //每个颜色数据的类型
			bitmapData);	//bitmap数据指针  
	}
	
void generateTex()
{
	//生成红黑相间的图像
	for (int i = 0; i < TEXH; i++) {
		for (int j = 0; j < TEXW; j++) {
			int x = ((i & 4) ^ (j & 4)) * 255;
			tex[i][j][0] = (GLubyte)x;
			tex[i][j][1] = 0;
			tex[i][j][2] = 0;
		}
	}
}	

	//定义纹理的函数
	void init()
	{
		glGenTextures(5, texture);                                         // 第一参数是需要生成标示符的个数, 第二参数是返回标示符的数组
		texload(0, "Monet.bmp");
		texload(1, "Crack.bmp");
		texload(3, "Spot.bmp");
		multiTexture("Crack.bmp", "Spot.bmp", 256);
		//下面生成自定义纹理
		generateTex(); //参考opengl red book，理解后请解释函数的步骤。
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //设置像素存储模式控制所读取的图像数据的行对齐方式.
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TEXW, TEXH, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXW, TEXH, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);

	}

//纹理混合函数
	void multiTexture(char *filename1, char *filename2,float p) {
		BITMAPINFOHEADER bitmapInfoHeader1;                                 // bitmap信息头
		BITMAPINFOHEADER bitmapInfoHeader2;
		unsigned char*   bitmapData1;                                       // 纹理数据
		unsigned char*   bitmapData2;

		unsigned char	tempRGB;	// 交换变量

		int	imageIdx = 0;		// 图像位置索引

		bitmapData1 = LoadBitmapFile(filename1, &bitmapInfoHeader1);
		bitmapData2 = LoadBitmapFile(filename2, &bitmapInfoHeader1);

		unsigned char	*bitmapData3;		// bitmap图像数据
		BITMAPINFOHEADER *bitmapInfoHeader3;
		bitmapInfoHeader3 = &bitmapInfoHeader1;
		bitmapData3 = new unsigned char[bitmapInfoHeader3->biSizeImage];

		

		for (int i = 0; i < bitmapInfoHeader3->biSizeImage; i++)
			bitmapData3[i] = (bitmapData1[i] * bitmapData2[i]) / p;

		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		

		glTexImage2D(GL_TEXTURE_2D,
			0, 	    //mipmap层次(通常为，表示最上层) 
			GL_RGB,	//我们希望该纹理有红、绿、蓝数据
			bitmapInfoHeader1.biWidth, //纹理宽带，必须是n，若有边框+2 
			bitmapInfoHeader1.biHeight, //纹理高度，必须是n，若有边框+2 
			0, //边框(0=无边框, 1=有边框) 
			GL_RGB,	//bitmap数据的格式
			GL_UNSIGNED_BYTE, //每个颜色数据的类型
			bitmapData3);	//bitmap数据指针 
	}
	


//

void drawCube()
{
	int i, j;
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat z1 = -0.5, z2 = 0.5;

	//指定六个面的四个顶点，每个顶点用3个坐标值表示  
	GLfloat point[6][4][3] = {
		{ { x1,y1,z1 },{ x2,y1,z1 },{ x2,y2,z1 },{ x1,y2,z1 } },
	{ { x1,y1,z1 },{ x2,y1,z1 },{ x2,y1,z2 },{ x1,y1,z2 } },
	{ { x2,y1,z1 },{ x2,y2,z1 },{ x2,y2,z2 },{ x2,y1,z2 } },
	{ { x1,y1,z1 },{ x1,y2,z1 },{ x1,y2,z2 },{ x1,y1,z2 } },
	{ { x1,y2,z1 },{ x2,y2,z1 },{ x2,y2,z2 },{ x1,y2,z2 } },
	{ { x1,y1,z2 },{ x2,y1,z2 },{ x2,y2,z2 },{ x1,y2,z2 } }
	};
	int dir[4][2] = { { 1,1 },{ 1,0 },{ 0,0 },{ 0,1 } };
	//设置正方形绘制模式  

	glBegin(GL_QUADS);
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 4; j++) {
			glTexCoord2iv(dir[j]);
			glVertex3fv(point[i][j]);
		}
	}
	glEnd();

}
//

void Draw_Triangle() // This function draws a triangle with RGB colors
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[Texture0]);
	glPushMatrix();
	glTranslatef(0, 0, 4+1);
	glRotatef(90, 1, 0, 0);
	//
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//设置纹理受光照影响
	//
	glutSolidTeapot(1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texture[Texture1]);
	glBindTexture(GL_TEXTURE_2D, texture[Texture1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//设置纹理受光照影响
	
	//
	//multi();
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	//glutSolidCube(1.0);
	drawCube();
	//DrawCubeMulti();
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[Texture1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//设置纹理受光照影响
	

	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void Draw_Leg()
{
	glScalef(1, 1, 3);
	//glutSolidCube(1.0);
	drawCube();
	//DrawCubeMulti();
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	if (bPersp) {
		gluPerspective(45.0f, whRatio,0.1f,100.0f);
		//glFrustum(-3, 3, -3, 3, 3,100);
	} else {
		glOrtho(-3 ,3, -3, 3,-100,100);
	}

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; break; }

	case ' ': {bAnim = !bAnim; break;}
	case 'o': {bWire = !bWire; break;}

	case 'a': {
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
			  }
	case 'd': {
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
			  }
	case 'w': {
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
			  }
	case 's': {
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
			  }
	case 'z': {
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
			  }
	case 'c': {
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
			  }
	case 't': {
		if (Texture0 == 0) {
			Texture0 = 2;
		}
		else {
			Texture0 = 0;
		}
		break;
			  }
	case 'y': {
		if (Texture1 == 1) {
			Texture1 = 4;
		}
		else {
			Texture1 = 1;
		}
		break;
	}
	case 'b':{
		blight = !blight;
		break;
	}
	}

	updateView(wHeight, wWidth);
}


void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = {5,5,5,1};

	GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	if (blight) {
		glLightfv(GL_LIGHT0, GL_AMBIENT, red);
	}
	else {
		glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	}
	//glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);

	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Triangle();						// Draw triangle

	if (bAnim) fRotate    += 0.5f;
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	//
	init();
	//
	glutMainLoop();
	return 0;
}


