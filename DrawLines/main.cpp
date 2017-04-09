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
#include "ResourceManager\TextureManger.h"
#include "Render.h"

using namespace std;
using namespace CG_MATH;

//ȫ�ֱ���
static bool flag = 1;
enum DrawMode{DRAW_LINE=0, DRAW_TRIANGLE, DRAW_MODE_SIZE};
static DrawMode drawMode = DRAW_TRIANGLE; // 0: ���ߣ� 1������
FPScamera camera;


vector<string> models = {
	"Resource//box.obj",
	"Resource//ball.obj",
	"Resource//dunpai.obj",
};

static int curModelID = 2;

const int windowWidth = 600;
const int windowHeight = 600;

Matrix4x3 worldMatrix;
Matrix4x3 viewMatrix;
Matrix4x4 perspectiveMatrix;

FrameBuffer fbo(windowWidth, windowHeight);

DistantLight light;

void myDisplay();
void initMesh();

//FPScamera ���������

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

// ���̿��ƺ���

void keyboard(unsigned char key, int x, int y) {

	const float step = 0.05f;

	// ��дתСд
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
	case 'f':
		drawMode = (DrawMode)((drawMode + 1) % DRAW_MODE_SIZE);
		break;

	case 'm':
		curModelID = (curModelID + 1) % models.size();
		MeshManager::getInstance().reset();
		MeshManager::getInstance().addMesh(models[curModelID]);
		break;
	default:
		break;
	}

	myDisplay();
}

void printHelp()
{
	std::cout << "ʹ�÷�����" << std::endl;
	std::cout << "�����������������ӽ��ƶ���" << std::endl;
	std::cout << "[w, s, a, d]: �����ӵ�ǰ�������ƶ���" << std::endl;
	std::cout << "[q, e] :�����ӵ������ƶ���" << std::endl;
	std::cout <<std::endl;
	std::cout << "[f]: �ı����ģʽ��" << std::endl;
	std::cout << "[m]: �л�ģ�͡�" << std::endl;
}

int main(int argc, char *argv[])
{
	initMesh();

	light.dir = vector3(0.0f, 0.0f, 1.0f);
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
	
	printHelp();

	glutMainLoop();
	return 0;
}



void initMesh()
{
	MeshManager & MM = MeshManager::getInstance();
	MM.addMesh(models[curModelID]);

	TextureManager::getInstance().setBaseDirPath("Resource//");
	
}

void myDisplay(void) {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ����ɫ ��ɫ
	fbo.clear(color4f(0.0f, 0.0f, 0.0f, 0.0f), 1.0f);

	// ����MVP����
	worldMatrix.setupTanslation(vector3(0.0f, 0.0f, 3.0f));
	viewMatrix = camera.getMatrix();
	perspectiveMatrix.setupPerspective(50, windowWidth / windowHeight, .1f, 100.0f);
	
	const Render & render = Render::getInstance();
	MeshManager & MM = MeshManager::getInstance();

	RenderOption ro(perspectiveMatrix, viewMatrix, camera.pos, light);

	for (int i = 0; i < MM.m_meshes.size(); ++i)
	{
		switch (drawMode)
		{
		case DRAW_LINE: // ����
			render.drawAsLine(MM.m_meshes[0], ro, fbo);
			break;
		case DRAW_TRIANGLE:  // ����
			render.drawAsFace(MM.m_meshes[0], ro, fbo);
			break;
		default:
			break;
		}			
	}

	fbo.display();

	// ����
	glutSwapBuffers();

}

