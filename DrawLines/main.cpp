#include "..\CG_MATH_D3D\vector3.h"
#include <iostream>
#include <string>
#include <time.h>
#include <string.h>
#include <tuple>

#include <GL\glut.h>

#include "util.h"
#include "DistantLight.h"
#include "..\CG_MATH_D3D\Matrix4x4.h"
#include "..\CG_MATH_D3D\Quaternion.h"
#include "..\CG_MATH_D3D\MathUtil.h"
#include "..\CG_MATH_D3D\FPScamera.h"

#include "FrameBuffer.h"

#include "ResourceManager\MeshManager.h"
#include "Render.h"

using namespace std;
using namespace CG_MATH;

//全局变量
static bool flag = 1;
FPScamera camera;

const int windowWidth = 600;
const int windowHeight = 600;

Matrix4x3 worldMatrix;
Matrix4x3 viewMatrix;
Matrix4x4 perspectiveMatrix;

FrameBuffer fbo(windowWidth, windowHeight);

DistantLight light;

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

	light.dir = vector3(1.0f, 0.0f, 0.0f);
	light.dir.normalize();
	light.ia = color3f(0.2f, 0.2f, 0.2f);
	light.id = color3f(0.4f, 0.4f, 0.4f);
	light.is = color3f(0.4f, 0.4f, 0.4f);

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
	MeshManager & MM = MeshManager::getInstance();
	MM.addMesh("Resource//ball.obj");

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
	perspectiveMatrix.setupPerspective(50, windowWidth / windowHeight, .1f, 100.0f);
	
	const Render & render = Render::getInstance();
	MeshManager & MM = MeshManager::getInstance();

	RenderOption ro(perspectiveMatrix, viewMatrix, camera.pos, light);

	for (int i = 0; i < MM.m_meshes.size(); ++i)
	{
		render.drawAsLine(MM.m_meshes[0], ro, fbo);
		//render.drawAsFace(MM.m_meshes[0], ro, fbo);
	}

	fbo.display();

	// 画线
	glutSwapBuffers();

}

