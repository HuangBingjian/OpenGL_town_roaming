#pragma once
#include "GL_Particle.h"
#include <gl/glut.h>

class Snow : public Particle
{
public:
	Snow();
	virtual ~Snow();
	bool		Init(int num);  /** ��ʼ������ */
	void		Render();        /**< ��Ⱦ���� */
	void		Update();        /**< ���¹��� */

	GLuint      m_texture;       /**< ���ӵ����� */

};
