#pragma once
#include <map>
#include <string>
#include "../util.h"

struct Image2D {
	unsigned int width;
	unsigned int height;
	color4f *data;
};


// TextureManger �࣬�����������е�������Դ
// ��������ֻ����һ��TextureManger������ʹ�õ���ģʽ
class TextureManager
{
	
private:
	TextureManager()=default;

	~TextureManager();

public:

	// ��ֹ��������͸�ֵ

	TextureManager(const TextureManager& t) = delete;
	TextureManager& operator=(const TextureManager& t) = delete;

	// ���ʵ��

	static TextureManager& getInstance() {
		
		static TextureManager Instance;
		
		return Instance;
	}

	// ���������ļ���·��
	void setBaseDirPath(const std::string& path) { m_baseDir = path; }

	// ����
	void reset();

	// ���TexID
	Image2D& getImage(const char* texPath);

	
private:
	// ˽�г�Ա����
	std::map<std::string, Image2D> m_nameToImage2D;
	std::string m_baseDir;

};



