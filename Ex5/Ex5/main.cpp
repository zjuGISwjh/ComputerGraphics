// glutEx1.cpp : 定义控制台应用程序的入口点。
//


#include <stdlib.h>
#include "glut.h"

float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

void Draw_Leg();

//
bool turnOnFlash = false;
bool lightColor = false;
//

void Draw_Triangle() // This function draws a triangle with RGB colors
{
	//
	float mat_diffuse[] = { 0.85f, 0.65f, 0.2f, 1.0f };
	float mat_specular[] = { 0.6f, 0.6f, 0.6f, 1.0f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);//漫反射颜色
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);//镜面反射颜色
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 50);
	//
	glPushMatrix();
	glTranslatef(0, 0, 4+1);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(1);
	glPopMatrix();

	//
	float zhuoMian_diffuse[] = {1.0f,0.0f,0.0f,1.0f};
	float zhuoMian_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, zhuoMian_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zhuoMian_specular);
	//
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	//
	float firstLeg_diffuse[] = { 0.0f,1.0f,0.0f,1.0f };
	float firstLeg_specular[] = { 0.0f, 1.0f, 0.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, firstLeg_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, firstLeg_specular);
	//
	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	//
	float secondLeg_diffuse[] = { 1.0f,1.0f,0.0f,1.0f };
	float secondLeg_specular[] = { 1.0f, 1.0f, 0.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, secondLeg_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, secondLeg_specular);
	//
	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	//
	float thirdLeg_diffuse[] = { 0.0f,1.0f,1.0f,1.0f };
	float thirdLeg_specular[] = { 0.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, thirdLeg_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, thirdLeg_specular);
	//
	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	//
	float forthLeg_diffuse[] = { 0.0f,0.0f,1.0f,1.0f };
	float forthLeg_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, forthLeg_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, forthLeg_specular);
	//
	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

}

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
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
//
GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };//默认白光
GLfloat secondColor[] = { 0.0,1.0,0.0,0.0 };//绿光
GLfloat light_pos[] = { 5,5,5,1 };

GLfloat flashColor[] = { 1.0, 0.0, 0.0, 1.0 };//聚光灯颜色，默认红光
GLfloat flashAmbient[] = { 1.0, 1.0, 1.0, 1.0 };//聚光灯环境光，默认白光
GLfloat flashPos[] = { 0,5,0,1 };
GLfloat spotangle = 5.0;
GLfloat lightDir[] = { 0,-1.0,0 };
//

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
	case 'j': {
		light_pos[0] -= 0.4f;
		break;
			  }
	case 'l': {
		light_pos[0] += 0.4f;
		break;
			  }
	case 'i': {
		light_pos[1] += 0.4f;
		break;
			  }
	case 'k': {
		light_pos[1] -= 0.4f;
		break;
			  }
	case '.': {
		lightColor = !lightColor;//切换光源颜色
		break;
			  }
	case 'b': {
		turnOnFlash=!turnOnFlash;//开启聚光灯
		break;
			  }
	case 'm': {
		spotangle += 0.5;//裁剪角度增大
		break;
			  }
	case 'n': {
		spotangle -= 0.5;//裁剪角度减小
		break;
			  }
	case '4': {
		lightDir[0] -= 0.05;//光照方向左移
		break;
			  }
	case '6': {
		lightDir[0] += 0.05;
		break;
			  }
	case '8': {
		lightDir[1] += 0.05;//光照方向右移
		break;
			  }
	case '2':
		lightDir[1] -= 0.05;
		break;
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
	//GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light_pos[] = {5,5,5,1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	//
	if (lightColor) {
		glLightfv(GL_LIGHT0, GL_AMBIENT, secondColor);
	}
	else { 
		glLightfv(GL_LIGHT0, GL_AMBIENT, color); 
	}
	//
	glEnable(GL_LIGHT0);

	//选择是否开启聚光灯
	//聚光灯
	if (turnOnFlash) {
		glLightfv(GL_LIGHT1, GL_AMBIENT, flashAmbient);             //设置环境光成分
		glLightfv(GL_LIGHT1, GL_SPECULAR, flashColor);                    //设置镜面光成分
		glLightfv(GL_LIGHT1, GL_DIFFUSE, flashColor);                     //设置漫射光成分

		glLightfv(GL_LIGHT1, GL_POSITION, flashPos);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotangle);             //裁减角度
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDir);          //光源方向
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 4.);                    //聚集度
		glEnable(GL_LIGHT1);
		glDisable(GL_LIGHT0);
	}
	//

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

	glutMainLoop();
	return 0;
}


