// *************************************************************
// 下雪：粒子的生命周期
// *************************************************************

#include "..\header\GL_Snow.h"
#include "..\header\cvector.h"

Snow::Snow()
{

}

Snow::~Snow()
{

}
bool Snow::Init(int num)
{
	/** 初始化粒子数目为num个 */
	if (Particle::Init(num))
	{
		for (int i = 0; i<m_num; i++)
		{
			/** 初始化位置 */
			float x, y, z, vx, vy, vz;
			x = 750 + rand() % 500;

			y = rand() % 50;

			z = -600 - (rand() % 800);
			m_plist[i].position = CVector(x, y, z);

			/** 初始化速度 */
			vx = 0.0001 * (rand() % 100);
			vy = 0.0001 * (rand() % 28);
			vz = 0.0;
			m_plist[i].velocity = CVector(vx, vy, vz);

			/** 初始化加速度 */
			m_plist[i].acceleration = CVector(0.0, 0.000005f, 0.0);

			/** 初始化粒子生命时间 */
			m_plist[i].lifetime = 100;

			/** 初始化粒子的尺寸 */
			m_plist[i].size = 0.15f;

			/** 初始化粒子的消失速度 */
			m_plist[i].dec = (float)(0.005 * (rand() % 50));

			/** 初始化粒子的颜色 */
			m_plist[i].color[0] = 1.0;
			m_plist[i].color[1] = 1.0;
			m_plist[i].color[2] = 1.0;

		}
		return true;
	}
	else
		return false;
}

void Snow::Render()
{
	/** 绑定纹理 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	/** 开始渲染雪花 */
	for (int i = 0; i<m_num; ++i)
	{
		float x = m_plist[i].position.x;
		float y = m_plist[i].position.y;
		float z = m_plist[i].position.z;
		float size = m_plist[i].size;

		/** 如果该粒子消失或生命结束则跳出 */
		if (y <= 0 || m_plist[i].lifetime <= 0)
			break;
		glNormal3f(0.0f, 0.0f, 1.0f);     /**< 定义法线方向 */
										  /** 画出粒子 */
		glPushMatrix();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size, y - size, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size, y + size, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size, y + size, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size, y - size, z);
		glEnd(); 
		glPopMatrix();


	}

	Update();
}

/** 雪花的更新 */
void Snow::Update()
{
	for (int i = 0; i<m_num; ++i)
	{
		/** 更新位置 */
		m_plist[i].position.x += 0.1f;
		m_plist[i].position.y -= 0.15f;
		if (m_plist[i].position.x > 1250)
			m_plist[i].position.x = 750;

		/** 更新速度 */
		m_plist[i].velocity.y += m_plist[i].acceleration.y;

		/** 更新生存时间 */
		m_plist[i].lifetime -= m_plist[i].dec;

		/** 如果粒子消失或生命结束 */
		if (m_plist[i].position.y <= 0 || m_plist[i].lifetime <= 0)
		{
			/** 初始化位置 */
			float x, y, z, vx, vy, vz;
			x = 700 + rand() % 700;

			y = rand() % 50;

			z = -600 - (rand() % 800);
			m_plist[i].position = CVector(x, y, z);

			/** 初始化速度 */
			vx = 0.0001 * (rand() % 100);
			vy = 0.0005 * (rand() % 100);
			vz = 0.0;
			m_plist[i].velocity = CVector(vx, vy, vz);

			/** 初始化加速度 */
			m_plist[i].acceleration = CVector(0.0, 0.000005f, 0.0);

			/** 初始化生命时间 */
			m_plist[i].lifetime = 100;

			/** 初始化消失速度 */
			m_plist[i].dec = (float)(0.0005*(rand() % 50));
		}

	}

}

