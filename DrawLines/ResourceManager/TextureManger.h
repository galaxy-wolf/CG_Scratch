#pragma once
#include <map>
#include <string>
#include "../util.h"
#include <opencv2/opencv.hpp>


struct Image2D {
	unsigned int width;
	unsigned int height;
	color3f *data;
};


// TextureManger 类，用来管理场景中的纹理资源
// 整个程序只能有一个TextureManger，所以使用单例模式
class TextureManager
{
	
private:
	TextureManager()=default;

	~TextureManager();

public:

	// 禁止拷贝构造和赋值

	TextureManager(const TextureManager& t) = delete;
	TextureManager& operator=(const TextureManager& t) = delete;

	// 获得实例

	static TextureManager& getInstance() {
		
		static TextureManager Instance;
		
		return Instance;
	}

	// 设置纹理文件夹路径
	void setBaseDirPath(const std::string& path) { m_baseDir = path; }

	// 重置
	void reset();

	// 获得TexID
	IplImage* getImage(const char* texPath);

	
private:
	// 私有成员函数
	std::map<std::string, IplImage*> m_nameToImage2D;
	std::string m_baseDir;

};



