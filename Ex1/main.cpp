#include "gl/glut.h"
#include "math.h"

const GLfloat GL_PI = 3.14159265357;

void StarDisplay(GLfloat p1,GLfloat p2,GLfloat v1,GLfloat v2,int flag) {//p1,p2�����ĵ����꣬v1,v2����֪��һ���ⶥ������
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	//���ڴ�Ŷ������������
	GLfloat VTx[12];
	GLfloat VTy[12];

	//����ȷ�����������
	VTx[0] = p1;
	VTy[0] = p2;

	//������Ƿ�Ϊ10����״����С��ͬ�������Σ�����ǵ��ⶥ������߼�Ϊ���������һ����
	//�������ֳ��������Σ���������Ƕ���Ľ�Ϊ18�㣬�������ĵĽ�Ϊ36��
	//����������ĵ�����ľ��룬��������ε���һ�߱߳�

	GLfloat length0 = sqrt((p1 - v1)*(p1 - v1)+(p2 - v2)*(p2 - v2));//������е㵽�ⶥ�����
	GLfloat length1 = length0*sin(0.1*GL_PI)/sin(((float)126/180)*GL_PI);//��length0���������ĵ㵽�ڶ���ľ���

	VTx[1] = v1;
	VTy[1] = v2;

	//�������һ��ֱ������ϵ֮�У�ͨ�������ε���ת��ƴ�������
	double angle = atan((VTy[1] - VTy[0])/(VTx[1] - VTx[0]));//��һ����֪�Ƕ�angle��������һ���㣬angle���ǵ�ǰ������x��ļн�
	if (flag) {
		angle = angle - GL_PI;
	}
	for (int i = 2; i <= 11; i++) {
		angle = angle - 0.2*GL_PI;
		if (i % 2 == 0) {//�ⶥ��
			VTx[i] = VTx[0] + length1 * cos(angle);
			VTy[i] = VTy[0] + length1 * sin(angle);
		}
		else if(i % 2 != 0){//�ڶ���
			VTx[i] = VTx[0] + length0 * cos(angle);
			VTy[i] = VTy[0] + length0 * sin(angle);
		}
	}
	for (int i = 0; i < 12; i++) //���ö���
		glVertex3f(VTx[i], VTy[i], 0.5);
	glEnd();
}

void display()
{
	//��������
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-0.75, 0.5, 0.5);
	glVertex3f(0.75, 0.5, 0.5);
	glVertex3f(0.75, -0.5, 0.5);
	glVertex3f(-0.75, -0.5, 0.5);
	glEnd();

	GLfloat VTx[4];//���4��С����ǵĶ���
	GLfloat VTy[4];
	
	//�ֱ����4��С����ǵĵ�һ������
	VTx[0]= -0.25 - 0.05*cos(atan((0.25 - 0.4) / (-0.5 - -0.25)));
	VTy[0]= 0.4 - 0.05*sin(atan((0.25 - 0.4) / (-0.5 - -0.25)));
	VTx[1] = -0.15 - 0.05*cos(atan((0.25 - 0.3) / (-0.5 - -0.15)));
	VTy[1] = 0.3 - 0.05*sin(atan((0.25 - 0.3) / (-0.5 - -0.15)));
	VTx[2] = -0.15 - 0.05*cos(atan((0.25 - 0.15) / (-0.5 - -0.15)));
	VTy[2] = 0.15 - 0.05*sin(atan((0.25 - 0.15) / (-0.5 - -0.15)));
	VTx[3] = -0.25 - 0.05*cos(atan((0.25 - 0.05) / (-0.5 - -0.25)));
	VTy[3] = 0.05 - 0.05*sin(atan((0.25 - 0.05) / (-0.5 - -0.25)));
	//��StarDisplay�����������
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