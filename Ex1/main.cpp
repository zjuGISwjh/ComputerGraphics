#include "gl/glut.h"
#include "math.h"

const GLfloat GL_PI = 3.14159265357;

void StarDisplay(GLfloat p1,GLfloat p2,GLfloat v1,GLfloat v2,int flag) {//p1,p2是中心点坐标，v1,v2是已知的一个外顶点坐标
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	//用于存放顶点坐标的数组
	GLfloat VTx[12];
	GLfloat VTy[12];

	//首先确定五角星中心
	VTx[0] = p1;
	VTy[0] = p2;

	//将五角星分为10个形状、大小相同的三角形，五角星到外顶点的连线即为三角形最长的一条边
	//这样划分出的三角形，靠近五角星顶点的角为18°，靠近中心的角为36°
	//由五角星中心到顶点的距离，求出三角形的另一边边长

	GLfloat length0 = sqrt((p1 - v1)*(p1 - v1)+(p2 - v2)*(p2 - v2));//五角星中点到外顶点距离
	GLfloat length1 = length0*sin(0.1*GL_PI)/sin(((float)126/180)*GL_PI);//用length0来计算中心点到内顶点的距离

	VTx[1] = v1;
	VTy[1] = v2;

	//五角星在一个直角坐标系之中，通过三角形的旋转来拼成五角星
	double angle = atan((VTy[1] - VTy[0])/(VTx[1] - VTx[0]));//用一个已知角度angle来计算下一顶点，angle就是当前顶点与x轴的夹角
	if (flag) {
		angle = angle - GL_PI;
	}
	for (int i = 2; i <= 11; i++) {
		angle = angle - 0.2*GL_PI;
		if (i % 2 == 0) {//外顶点
			VTx[i] = VTx[0] + length1 * cos(angle);
			VTy[i] = VTy[0] + length1 * sin(angle);
		}
		else if(i % 2 != 0){//内顶点
			VTx[i] = VTx[0] + length0 * cos(angle);
			VTy[i] = VTy[0] + length0 * sin(angle);
		}
	}
	for (int i = 0; i < 12; i++) //设置顶点
		glVertex3f(VTx[i], VTy[i], 0.5);
	glEnd();
}

void display()
{
	//画出旗面
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-0.75, 0.5, 0.5);
	glVertex3f(0.75, 0.5, 0.5);
	glVertex3f(0.75, -0.5, 0.5);
	glVertex3f(-0.75, -0.5, 0.5);
	glEnd();

	GLfloat VTx[4];//存放4颗小五角星的顶点
	GLfloat VTy[4];
	
	//分别计算4颗小五角星的第一个顶点
	VTx[0]= -0.25 - 0.05*cos(atan((0.25 - 0.4) / (-0.5 - -0.25)));
	VTy[0]= 0.4 - 0.05*sin(atan((0.25 - 0.4) / (-0.5 - -0.25)));
	VTx[1] = -0.15 - 0.05*cos(atan((0.25 - 0.3) / (-0.5 - -0.15)));
	VTy[1] = 0.3 - 0.05*sin(atan((0.25 - 0.3) / (-0.5 - -0.15)));
	VTx[2] = -0.15 - 0.05*cos(atan((0.25 - 0.15) / (-0.5 - -0.15)));
	VTy[2] = 0.15 - 0.05*sin(atan((0.25 - 0.15) / (-0.5 - -0.15)));
	VTx[3] = -0.25 - 0.05*cos(atan((0.25 - 0.05) / (-0.5 - -0.25)));
	VTy[3] = 0.05 - 0.05*sin(atan((0.25 - 0.05) / (-0.5 - -0.25)));
	//用StarDisplay函数画五角星
	StarDisplay(-0.5,0.25,-0.5,0.4,0);
	StarDisplay(-0.25, 0.4, VTx[0], VTy[0], 1);
	StarDisplay(-0.15, 0.3, VTx[1], VTy[1], 1);
	StarDisplay(-0.15, 0.15, VTx[2], VTy[2], 1);
	StarDisplay(-0.25, 0.05, VTx[3], VTy[3], 1);
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Simple GLUT App");

	glClear(GL_COLOR_BUFFER_BIT);//add
	glutDisplayFunc(display);
	//glutDisplayFunc(StarDisplay);
	glutMainLoop();

	return 0;
}