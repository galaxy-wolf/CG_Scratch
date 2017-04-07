#include "TextureManger.h"
#include <cstdio>
//#include <opencv2/opencv.hpp>

using namespace std;

// �ͷ�������Ϣ
TextureManager::~TextureManager()
{
	reset();
}

void TextureManager::reset()
{
	m_nameToImage2D.clear();

	// �����洢·����Ϊ��
	m_baseDir = string();
}

Image2D& TextureManager::getImage(const char * texPath)
{

	if (m_nameToImage2D.count(texPath))
		return m_nameToImage2D[texPath];

	// load image;

	string fullPath = m_baseDir + "//" + texPath;
	//IplImage *Iface = cvLoadImage(fullPath.c_str());
	//if (!Iface)
	//{
	//	fprintf(stderr, "%s %d: opencv load file error: %s\n", __FILE__, __LINE__, fullPath.c_str());
	//	exit(-1);
	//}

	// ����OpengGL ����
	m_nameToImage2D[texPath] = Image2D(); //?
	return m_nameToImage2D[texPath];
}