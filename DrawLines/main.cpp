#include "vector3.h"
#include <iostream>
#include <string>
#include <time.h>
#include <string.h>
#include <tuple>

#include <GL\glut.h>

#include "util.h"
#include "Mesh.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "MathUtil.h"
#include "FPScamera.h"

#include "FrameBuffer.h"

using namespace std;
using namespace CG_MATH;

//全局变量
static bool flag = 1;
FPScamera camera;

const int windowWidth = 600;
const int windowHeight = 600;

Matrix4x3 worldMatrix;
Matrix4x3 viewMatrix;
Matrix4x4 persectiveMatrix;

FrameBuffer fbo(windowWidth, windowHeight);


// Meshes;
Mesh cube;

void myDisplay();
void initMesh();

//FPScamera 控制相机。

static int lastX;
static int lastY;
static bool EnableMouseMove = false;

void mouse(int button, int state, int x, int y) {

	if (state == GLUT_DOWN) {
		lastX = x;
		lastY = y;
		EnableMouseMove = true;
	}
	else {
		EnableMouseMove = false;
	}
}

void motion(int x, int y) {

	if (EnableMouseMove) {
		int deltX = x - lastX;
		int deltY = y - lastY;

		lastX = x;
		lastY = y;

		camera.rotate2D(deltX * .2f, deltY * 0.2f);
	}

	myDisplay();
}

// 键盘控制函数

void keyboard(unsigned char key, int x, int y) {

	const float step = 0.05f;

	// 大写转小写
	if ('A' <= key && key <= 'Z')
		key += 'a' - 'A';

	switch (key)
	{
	case 'w':
		camera.move(step, 0, 0);
		break;

	case 's':
		camera.move(-step, 0, 0);
		break;

	case 'a':
		camera.move(0, step, 0);
		break;

	case 'd':
		camera.move(0, -step, 0);
		break;

	case 'q':
		camera.move(0, 0, step);
		break;

	case 'e':
		camera.move(0, 0, -step);
		break;


	default:
		break;
	}

	myDisplay();
}

int main(int argc, char *argv[])
{
	initMesh();

	// init window.

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("DrawLines");
	glutDisplayFunc(&myDisplay);
	//glutIdleFunc(&myDisplay);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	
	glutMainLoop();
	return 0;
}


void initMesh()
{
	Group g;
	g.m_ambient = red;
	g.m_vertices = 
	{
		vector3(-1.0f, 1.0f, -1.0f),
		vector3(1.0f, 1.0f, -1.0f),
		vector3(1.0f, 1.0f, 1.0f),
		vector3(-1.0f, 1.0f, 1.0f),
		vector3(-1.0f, -1.0f, -1.0f),
		vector3(1.0f, -1.0f, -1.0f),
		vector3(1.0f, -1.0f, 1.0f),
		vector3(-1.0f, -1.0f, 1.0f),
	};

	g.m_vertexColors =
	{
		color4f(0.0f, 0.0f, 1.0f, 1.0f),
		color4f(0.0f, 1.0f, 0.0f, 1.0f),
		color4f(0.0f, 1.0f, 1.0f, 1.0f),
		color4f(1.0f, 0.0f, 0.0f, 1.0f),
		color4f(1.0f, 0.0f, 1.0f, 1.0f),
		color4f(1.0f, 1.0f, 0.0f, 1.0f),
		color4f(1.0f, 1.0f, 1.0f, 1.0f),
		color4f(0.0f, 0.0f, 0.0f, 1.0f),
	};

	g.m_indices = 
	{ 
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6, 
	};

	cube.m_groups.push_back(g);
}

void myDisplay(void) {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 背景色 黑色
	fbo.clear(color4f(0.0f, 0.0f, 0.0f, 0.0f), 1.0f);

	// 设置MVP矩阵
	worldMatrix.setupTanslation(vector3(0.0f, 0.0f, 3.0f));
	viewMatrix = camera.getMatrix();
	persectiveMatrix.setupPerspective(50, windowWidth / windowHeight, .1f, 100.0f);
	 
	cube.drawAsLine(worldMatrix*viewMatrix*persectiveMatrix, fbo);

	fbo.display();

	// 画线
	glutSwapBuffers();

}

