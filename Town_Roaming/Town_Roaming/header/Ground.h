#ifndef __GROUND_H__
#define __GROUND_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "Vector.h"
#include "Camera.h"

#define GL_CLAMP_TO_EDGE	0x812F
#define NUM_TEXTURE	123	//��������
#define width_map 800		//��ͼ�ڿ��
#define length_map 800		//��ͼ�ڳ���

/** ������ */
class CGround
{
public:
	/** ���캯�� */
	CGround();
	~CGround();

	/** ��ʼ�� */
	bool init();


	int	open_light;		//·�ƿ���

	void setlight();
	void openlight();
	void closelight();
	GLuint		m_texture[NUM_TEXTURE];   /**< ��������   */


	/** ��Ⱦ */
	void render();
	/** ������Ⱦ */
	void building_render();
	void building(int tex1, int tex2, int tex3, int tex4, int tex5, float x, float z, float w, float l, float h, float h_add);
	/** ��ľ��Ⱦ */
	void tree_render();
	void tree1(float x, float z, float h, float h_add, float size);
	void tree2(float x, float z, float h, float h_add, float size);
	/** ·����Ⱦ */
	void streetlight_render();
	void streetlight();
	/** �˶�����Ⱦ */
	void playground_render();
	void football();
	void basketball();

private:
	AUX_RGBImageRec *TextureImage[NUM_TEXTURE];						//Ϊ�������洢�ռ�
	float       length;         /**< ���� */
	float       width;          /**< ��� */
//	float       height;         /**< �߶� */
	float		height_base;	//΢���߶�
	float		height_tree;	//��ľ�߶�

};


#endif ///__GROUND_H__