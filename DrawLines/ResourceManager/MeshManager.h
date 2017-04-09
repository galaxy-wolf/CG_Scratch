#pragma once
#include <string>
#include <vector>
#include "../../CG_MATH_D3D/vector3.h"
#include "../../CG_MATH_D3D/Matrix4x3.h"
#include "Mesh.h"

class Mesh;

// �������е�mesh�� 
// ������mesh �ϲ���һ����mesh�� ����cuda����
//
// ʹ�õ���ģʽ
class MeshManager
{

public:

	// ��������
	static MeshManager &getInstance() {

		static MeshManager Instance;
		return Instance;
	}

	// ��ֹ�����͹��캯��
	MeshManager(const MeshManager &m) = delete;
	MeshManager & operator=(const MeshManager &m) = delete;

	void addMesh(const std::string& path);
	void reset() { m_meshes.clear(); m_triangleNum = 0; }

	// ��������
	// position, normal, texture coordinate
	const int m_pSize = 3;
	const int m_nSize = 3;
	const int m_tcSize = 2;

	const int m_pOffset = 0;
	const int m_nOffset = m_pOffset + m_pSize;
	const int m_tcOffset = m_nOffset + m_nSize;

	const int m_vtxSize = m_pSize + m_nSize + m_tcSize;

	std::vector<Mesh> m_meshes;

	unsigned int m_triangleNum;

	

private:
	
	
	// ˽�в���

	MeshManager();
	~MeshManager();
	
	// ˽�г�Ա


};

