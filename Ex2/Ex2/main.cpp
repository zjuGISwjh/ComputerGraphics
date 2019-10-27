// main.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "glut.h"

float fTranslate;//ƽ������
float fRotate;//��ת����
float fScale = 1.0f;//��������
/*
void Draw_Triangle() // This function draws a triangle with RGB colors
{
	glBegin(GL_TRIANGLES);
		 glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		 glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f,-1.0f, 0.0f);
		 glColor3f(0.0f, 0.0f,1.0f);
		glVertex3f( 1.0f,-1.0f, 0.0f);			
	glEnd();
}
*/

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void idle()
{
	glutPostRedisplay();
}

/*
void redraw()
{
// If want display in wireframe mode
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix
	
	glPushMatrix();
		glTranslatef(-2.0f, 0.0f,-6.0f);		// Place the triangle Left
		glTranslatef(0.0f, fTranslate, 0.0f);	// Translate in Y direction
		Draw_Triangle();						// Draw triangle					
	glPopMatrix();

    glPushMatrix();
		glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
		glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
		Draw_Triangle();						// Draw triangle
	glPopMatrix();

	fTranslate += 0.005f;
	fRotate    += 0.5f;

	if(fTranslate > 0.5f) fTranslate = 0.0f;
	glutSwapBuffers();
}
*/

//��һ�����ӿ�����һ�������壬����һ����������ĺ���
void DrawCube(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat z1, GLfloat z2){
	GLfloat vertex[8][3] = {
		//�˸����㣬��v1-v8
		x1,y1,z2,
		x2,y1,z2,
		x1,y2,z2,
		x2,y2,z2,
		x1,y1,z1,
		x2,y1,z1,
		x1,y2,z1,
		x2,y2,z1
	};
	//��ʾ6��ƽ�棬���ﲻ����float
	GLint surface[6][4] = {
		0,1,3,2,
		4,5,7,6,
		2,3,7,6,
		0,1,5,4,
		0,2,6,4,
		1,3,7,5
	};
	glBegin(GL_QUADS);//ָ�����Ʒ�ʽ
					  //��ÿ��������ֳ����������
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 4; j++)
			glVertex3fv(vertex[surface[i][j]]);//����������Ҫʹ��glVertex3fv
	glEnd();
}

//�����Ӳ�ֳ�5����������ƣ���1�������4������
void DrawTable()
{
	DrawCube(0.0, 1.0, 0.0, 0.8, 0.6, 0.8);//����
	DrawCube(0.1, 0.3, 0.1, 0.3, 0.0, 0.6);
	DrawCube(0.7, 0.9, 0.1, 0.3, 0.0, 0.6);
	DrawCube(0.1, 0.3, 0.5, 0.7, 0.0, 0.6);
	DrawCube(0.7, 0.9, 0.5, 0.7, 0.0, 0.6);
}


void redraw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClear(GL_COLOR_BUFFER_BIT);//�������
	glLoadIdentity();
	//ƽ��
	glPushMatrix();//glpushmatrix��glpopmatrix���ʹ�ÿ���ʹ����ԭ��ص�ԭ����λ��
	glTranslatef(-2.5f, 0.0f, -6.0f);
	glTranslatef(0.0f, fTranslate, 0.0f);
	DrawTable();
	glPopMatrix();
	//��ת
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(fRotate, 0, 1.0f, 0);	
	DrawTable();
	glPopMatrix();
	//����
	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -6.0f);
	glScalef(fScale, fScale, fScale);
	DrawTable();
	glPopMatrix();
	//���Ӹ���
	fTranslate += 0.1f;
	fRotate += 0.5f;
	fScale -= 0.1f;

	if (fTranslate > 0.5f) fTranslate = 0.0f;
	if (fScale < 0.5f)//��С��һ���̶�֮����ԭ����С
		fScale = 1.0f;
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640,480);                                                  
	glutCreateWindow("Exercise2");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);		
	glutIdleFunc(idle);					

	glutMainLoop();

	return 0;
}


