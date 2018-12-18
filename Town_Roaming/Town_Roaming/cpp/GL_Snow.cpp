// *************************************************************
// ��ѩ�����ӵ���������
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
	/** ��ʼ��������ĿΪnum�� */
	if (Particle::Init(num))
	{
		for (int i = 0; i<m_num; i++)
		{
			/** ��ʼ��λ�� */
			float x, y, z, vx, vy, vz;
			x = 750 + rand() % 500;

			y = rand() % 50;

			z = -600 - (rand() % 800);
			m_plist[i].position = CVector(x, y, z);

			/** ��ʼ���ٶ� */
			vx = 0.0001 * (rand() % 100);
			vy = 0.0001 * (rand() % 28);
			vz = 0.0;
			m_plist[i].velocity = CVector(vx, vy, vz);

			/** ��ʼ�����ٶ� */
			m_plist[i].acceleration = CVector(0.0, 0.000005f, 0.0);

			/** ��ʼ����������ʱ�� */
			m_plist[i].lifetime = 100;

			/** ��ʼ�����ӵĳߴ� */
			m_plist[i].size = 0.15f;

			/** ��ʼ�����ӵ���ʧ�ٶ� */
			m_plist[i].dec = (float)(0.005 * (rand() % 50));

			/** ��ʼ�����ӵ���ɫ */
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
	/** ������ */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	/** ��ʼ��Ⱦѩ�� */
	for (int i = 0; i<m_num; ++i)
	{
		float x = m_plist[i].position.x;
		float y = m_plist[i].position.y;
		float z = m_plist[i].position.z;
		float size = m_plist[i].size;

		/** �����������ʧ���������������� */
		if (y <= 0 || m_plist[i].lifetime <= 0)
			break;
		glNormal3f(0.0f, 0.0f, 1.0f);     /**< ���巨�߷��� */
										  /** �������� */
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

/** ѩ���ĸ��� */
void Snow::Update()
{
	for (int i = 0; i<m_num; ++i)
	{
		/** ����λ�� */
		m_plist[i].position.x += 0.1f;
		m_plist[i].position.y -= 0.15f;
		if (m_plist[i].position.x > 1250)
			m_plist[i].position.x = 750;

		/** �����ٶ� */
		m_plist[i].velocity.y += m_plist[i].acceleration.y;

		/** ��������ʱ�� */
		m_plist[i].lifetime -= m_plist[i].dec;

		/** ���������ʧ���������� */
		if (m_plist[i].position.y <= 0 || m_plist[i].lifetime <= 0)
		{
			/** ��ʼ��λ�� */
			float x, y, z, vx, vy, vz;
			x = 700 + rand() % 700;

			y = rand() % 50;

			z = -600 - (rand() % 800);
			m_plist[i].position = CVector(x, y, z);

			/** ��ʼ���ٶ� */
			vx = 0.0001 * (rand() % 100);
			vy = 0.0005 * (rand() % 100);
			vz = 0.0;
			m_plist[i].velocity = CVector(vx, vy, vz);

			/** ��ʼ�����ٶ� */
			m_plist[i].acceleration = CVector(0.0, 0.000005f, 0.0);

			/** ��ʼ������ʱ�� */
			m_plist[i].lifetime = 100;

			/** ��ʼ����ʧ�ٶ� */
			m_plist[i].dec = (float)(0.0005*(rand() % 50));
		}

	}

}

