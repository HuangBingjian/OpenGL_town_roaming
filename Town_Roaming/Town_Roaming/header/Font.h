#ifndef	__GLFONT_H__
#define	__GLFONT_H__

#include "stdafx.h"

/** ���������� */
class GLFont															
{
public:
    /** ���캯������������ */
	GLFont();
	~GLFont();
    ///��Ա����
	bool InitFont();  /**< ��ʼ������ */
	void PrintText(char *string, float x, float y); /**< ��(x,y)�����string���� */
	
protected:
	HFONT m_hFont;  /**< ������ */
		
};

#endif	// __GLFONT_H__