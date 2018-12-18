#ifndef __C3DSModel_H__
#define __C3DSModel_H__

#include "stdafx.h"
#include "Vector.h"                 /**< ����������ͷ�ļ� */
#include "GL_3dsLoder.h"

/** 3DSģ���� */
class C3DSModel
{
public:
	/** ���캯������������ */

	C3DSModel();
	~C3DSModel();
	bool init(float x,float y,float z,float xScale,float yScale,float zScale,char *filename);
	bool render(float degree_x, float degree_y, float degree_z);



public:
	static C3DSModel  *m_pC3DSModel;    
	t3DModel model3DS;
	float x,y,z;			//ģ�ͳ��ֵ�λ��
	float xScale,yScale,zScale;		//ģ�����ŵı���
	char *filename;

};

#endif //__Static3DS_H__