#pragma once
#include "cvector.h"

#include <stdio.h>
#include <gl/glut.h>
#include <gl/glaux.h>

#define BITMAP_ID 0x4D42	/**< λͼ�ļ��ı�־ */

struct MyParticle
{
	CVector  position;     // ���ӵ����꣨��������ʾ��
	CVector  velocity;     // ������x,y,z��������ĳ��ٶ�
	CVector  acceleration; // ������x,y,z��������ļ��ٶ�
	float  lifetime;       // ���ӵ������ڣ��������ȣ�
	float  dec;            // ������ʧ���ٶ�
	float  size;           // ���ӵĴ�С
	float  color[3];       // ���ӵ���ɫ
};

class Particle
{
public:
	Particle();
	virtual ~Particle();
	virtual bool Init(int num);      // ���ӵĳ�ʼ������
	virtual void Render() = 0;       // ���ӵ���Ⱦ
	virtual void Update() = 0;       // ���ӵĸ���
	unsigned int ID;                 /**< ���������ID�� */
	int imageWidth;                  /**< ͼ���� */
	int imageHeight;                 /**< ͼ��߶� */
	unsigned char *image;            /**< ָ��ͼ�����ݵ�ָ�� */
	bool        LoadBitmap(const char *file);
	bool        Load(char *filename, GLuint &my_texture);
	void        LoadTreeBitmap(char *filename, GLuint &TreeTexture);
	int m_num;                       // ��������Ŀ
	MyParticle* m_plist;             // �����б�

};

