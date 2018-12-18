#pragma once
#include "GL_Particle.h"
#include <gl/glut.h>

class Snow : public Particle
{
public:
	Snow();
	virtual ~Snow();
	bool		Init(int num);  /** 初始化过程 */
	void		Render();        /**< 渲染过程 */
	void		Update();        /**< 更新过程 */

	GLuint      m_texture;       /**< 粒子的纹理 */

};
