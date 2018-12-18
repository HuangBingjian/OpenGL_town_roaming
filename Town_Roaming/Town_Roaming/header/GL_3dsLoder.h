
#if !defined(AFX_TEXTURE_H__AFD16B9D_0306_4C00_90CF_1AFAC60F2BC4__INCLUDED_)
#define AFX_TEXTURE_H__AFD16B9D_0306_4C00_90CF_1AFAC60F2BC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif // !defined(AFX_TEXTURE_H__AFD16B9D_0306_4C00_90CF_1AFAC60F2BC4__INCLUDED_)

#ifndef _GL3dsLoder_H
#define _GL3dsLoder_H


#include <wchar.h>    
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vector>									
#include <gl/glaux.h>
#include <crtdbg.h>
#include <gl/glut.h>
#include "GL_Base.h"

using namespace std;




#define TGA_RGB		 2
#define TGA_A		 3
#define TGA_RLE		10

struct tImage
{
	int channels;
	int sizeX;
	int sizeY;
	unsigned char *data;
};


#define MAX_TEXTURES 1000								// ����������Ŀ  

/* creates a enum type for mouse buttons */  
enum {  
    BUTTON_LEFT = 0,  
    BUTTON_RIGHT,  
    BUTTON_LEFT_TRANSLATE,  
};  
	

//  ������(Primary Chunk)��λ���ļ��Ŀ�ʼ
#define PRIMARY       0x4D4D

//  ����(Main Chunks)
#define OBJECTINFO    0x3D3D				// �������İ汾��
#define VERSION       0x0002				// .3ds�ļ��İ汾
#define EDITKEYFRAME  0xB000				// ���йؼ�֡��Ϣ��ͷ��

//  ����Ĵμ�����(��������Ĳ��ʺͶ���
#define MATERIAL	  0xAFFF				// ����������Ϣ
#define OBJECT		  0x4000				// ���������桢�������Ϣ

//  ���ʵĴμ�����
#define MATNAME       0xA000				// �����������
#define MATDIFFUSE    0xA020				// ����/���ʵ���ɫ
#define MATMAP        0xA200				// �²��ʵ�ͷ��
#define MATMAPFILE    0xA300				// ����������ļ���



//����Բ��ʵĹ��ս��ж���8888888888888888888888888888888888888888
#define MAT_AMBIENT 0xA010
#define MAT_SPECULAR 0xA030
#define MAT_EMISSIVE 0xA040

#define OBJECT_MESH   0x4100				// �µ��������

//  OBJECT_MESH�Ĵμ�����
#define OBJECT_VERTICES     0x4110			// ���󶥵�
#define OBJECT_FACES		0x4120			// �������
#define OBJECT_MATERIAL		0x4130			// ����Ĳ���
#define OBJECT_UV			0x4140			// �����UV��������

struct tIndices 
{							
	unsigned short a, b, c, bVisible;	
};

// �������Ϣ�Ľṹ
struct tChunk
{
	unsigned short int ID;					// ���ID		
	unsigned int length;					// ��ĳ���
	unsigned int bytesRead;					// ��Ҫ���Ŀ����ݵ��ֽ���
};

// ����3D����࣬���ڱ���ģ���еĶ���
class CVector3 
{
public:
	float x, y, z;
};

// ����2D���࣬���ڱ���ģ�͵�UV��������
class CVector2 
{
public:
	float x, y;
};

//  ��Ľṹ����
struct tFace
{
	int vertIndex[3];			// ��������
	int coordIndex[3];			// ������������
};

//  ������Ϣ�ṹ��
struct tMaterialInfo
{
	char  strName[255];			// ��������
	char  strFile[255];			// �����������ӳ�䣬���ʾ�����ļ�����
	BYTE  color[3];				// �����RGB��ɫ

	
	//���������˲��ʵĻ����⣬����⣬����⣬�������8888888888888888888888888888888888888888
float ambient[4];
float diffuse[4];
float specular[4];
float emissive[4];

int   texureId;				// ����ID
	float uTile;				// u �ظ�
	float vTile;				// v �ظ�
	float uOffset;			    // u ����ƫ��
	float vOffset;				// v ����ƫ��
} ;

class tMatREF
{
public:
	int nMaterialID;
	USHORT *pFaceIndexs;
	int nFaceNum;
	bool bHasTexture;
public:
	tMatREF()
	{
	   nMaterialID=-1;
	   nFaceNum=0;
	   pFaceIndexs=NULL;
	   bHasTexture=false;
	}
};


//  ������Ϣ�ṹ��
struct t3DObject 
{
	int  numOfVerts;			// ģ���ж������Ŀ
	int  numOfFaces;			// ģ���������Ŀ
	int  numTexVertex;			// ģ���������������Ŀ
	int  materialID;			// ����ID
	bool bHasTexture;			// �Ƿ��������ӳ��
	char strName[255];			// ���������
	CVector3  *pVerts;			// ����Ķ���
	CVector3  *pNormals;		// ����ķ�����
	CVector2  *pTexVerts;		// ����UV����
	tFace *pFaces;				// ���������Ϣ
//888888888888888888888888888888888888888888888888888888888888888888
	tMatREF *pMaterialREFS;
double m_minX;
double m_maxX;
double m_minY;
double m_maxY;
double m_minZ;
double m_maxZ;
};

//  ģ����Ϣ�ṹ��

struct t3DModel 
{
	int numOfObjects;					// ģ���ж������Ŀ
	int numOfMaterials;					// ģ���в��ʵ���Ŀ
	vector<tMaterialInfo> vctMaterials;	// ����������Ϣ
	vector<t3DObject> vctObject;			// ģ���ж���������Ϣ
	UINT g_Texture[MAX_TEXTURES];
	GLuint showList ;
};


// CLoad3DS�ദ�����е�װ�����
class CLoad3DS
{
public:
public:
	void Bytes2Floats(BYTE* pbs,float* pfs,int num,float fsk);
/*	CLoad3DS();								// ��ʼ�����ݳ�Ա
	~CLoad3DS();*/
	// װ��3ds�ļ���ģ�ͽṹ��
	bool Import3DS(t3DModel *pModel, char *strFileName);
	bool init(t3DModel *g_3DModel,char *filename);
	void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID);
	void Load_Texture(UINT textureArray[], LPSTR strFileName, int textureID);
	tImage * LoadTGA(const char *strFileName);
	tImage * LoadBMP(const char *strFileName);

	GLuint Show3DSModel(t3DModel *g_3DModel);
	char * StringCat(char *s1,char *s2);
		
private:
	// ��һ���ַ���
	int GetString(char *);
	// ����һ����
	void ReadChunk(tChunk *);
	// ����һ����
	void ProcessNextChunk(t3DModel *pModel, tChunk *);
	// ����һ�������
	void ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *);
	// ����һ�����ʿ�
	void ProcessNextMaterialChunk(t3DModel *pModel, tChunk *);
	// ��������ɫ��RGBֵ
	void ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk,USHORT typeFlag=0);
	// ������Ķ���
	void ReadVertices(t3DObject *pObject, tChunk *);
	// �����������Ϣ
	void ReadVertexIndices(t3DObject *pObject, tChunk *);
	// ���������������
	void ReadUVCoordinates(t3DObject *pObject, tChunk *);
	// ���������Ĳ�������
	void ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk,vector<tMatREF*> *pvmatids);
	// ������󶥵�ķ�����
	void ComputeNormals(t3DModel *pModel);
	// �ر��ļ����ͷ��ڴ�ռ�
	void CleanUp();
	// �ļ�ָ��
	FILE *m_FilePointer;
//	tChunk *m_CurrentChunk;//��ǰ��
//	tChunk *m_TempChunk;//��һ����
};


#endif 
