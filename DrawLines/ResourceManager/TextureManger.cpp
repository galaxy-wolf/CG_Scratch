#include "TextureManger.h"
#include <cstdio>


using namespace std;

// 释放纹理信息
TextureManager::~TextureManager()
{
	reset();
}

void TextureManager::reset()
{
	m_nameToImage2D.clear();

	// 纹理存储路径置为空
	m_baseDir = string();
}

IplImage* TextureManager::getImage(const char * texPath)
{

	if (m_nameToImage2D.count(texPath))
		return m_nameToImage2D[texPath];

	// load image;

	string fullPath = m_baseDir + "//" + texPath;
	IplImage *Iface = cvLoadImage(fullPath.c_str());
	if (!Iface)
	{
		fprintf(stderr, "%s %d: opencv load file error: %s\n", __FILE__, __LINE__, fullPath.c_str());
		exit(-1);
	}



	// 创建OpengGL 纹理
	m_nameToImage2D[texPath] = Iface; //?
	return Iface;
}
