#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "Vector.h"
#include "Camera.h"

#define GL_CLAMP_TO_EDGE	0x812F

/** ��պ��� */
class CSkyBox
{
public:
	/** ���캯�� */
	CSkyBox();
	~CSkyBox();

	/** ��ʼ�� */
	bool init();
	
	/** ��Ⱦ */
	void render(int is_night);

private:
	
	CBMPLoader  m_texture[25];   /**< ��պ�����   */
	Vector3     m_CameraPos;    /**< ��ǰ�����λ�� */
	float       length;         /**< ���� */
	float       width;          /**< ��� */
	float       height;         /**< �߶� */
	float       yRot;           /**< ��Y����ת */
		
};


#endif ///__SKYBOX_H__