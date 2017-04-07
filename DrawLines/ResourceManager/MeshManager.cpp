#include "MeshManager.h"

#include "glm.h"


using namespace std;
using namespace CG_MATH;


MeshManager::MeshManager()
	:m_triangleNum(0)
{
}


MeshManager::~MeshManager()
{
}

void MeshManager::addMesh(const string &path)
{
	
	m_meshes.emplace_back(path);

	m_triangleNum += m_meshes[m_meshes.size() - 1].m_triangleNum;

}



