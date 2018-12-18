// *************************************************************
// ��պУ�������գ����������������ʵ����ת
// *************************************************************

#include "..\header\SkyBox.h"

	
CSkyBox::CSkyBox():length(750.0f),width(550.0f),height(400.0f),yRot(0.01f)
{	
}

CSkyBox::~CSkyBox()
{
	/** ɾ�����������ռ�õ��ڴ� */
	for(int i =0 ;i< 25; i++)
	{
		m_texture[i].FreeImage();
		glDeleteTextures(1,&m_texture[i].ID);
	}	
}


/** ��պг�ʼ�� */
bool CSkyBox::init()
{
	char filename[128] ;                                         /**< ���������ļ��� */
	char *bmpName[] = { "back","front","top","left","right" ,
		"night_back","night_front","night_top","night_right","night_left",
		"dask_back","dask_front","dask_top","dask_right","dask_left",
		"snow_right","snow_left","snow_top","snow_back","snow_front",
		"sunday_back","sunday_front","sunday_top","sunday_left","sunday_right"		 };
	for(int i=0; i< 25; i++)
	{
		sprintf(filename,"data/skybox/%s",bmpName[i]);
		strcat(filename,".bmp");
		if(!m_texture[i].LoadBitmap(filename))                     /**< ����λͼ�ļ� */
		{
			MessageBox(NULL,"��պ�װ��λͼ�ļ�ʧ�ܣ�","����",MB_OK);    /**< �������ʧ���򵯳��Ի��� */
			exit(0);
		}
		glGenTextures(1, &m_texture[i].ID);                        /**< ����һ������������� */
		 
		glBindTexture(GL_TEXTURE_2D, m_texture[i].ID);             /**< ����������� */
		/** �����˲� */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		/** �������� */
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_texture[i].imageWidth,
						m_texture[i].imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
					    m_texture[i].image);
	}
	return true;
	
}


void CSkyBox::render(int is_night)
{
	/** ��ó����й���״̬ */
	GLboolean lp;
	glGetBooleanv(GL_LIGHTING,&lp);

	m_CameraPos = Camera::GetCamera()->getPosition();

	glDisable(GL_LIGHTING);            /**< �رչ��� */
	glEnable(GL_TEXTURE_2D);
	if (is_night == 0  )
	{
		/** ��ʼ���� */
		glPushMatrix();
		glTranslatef(m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		/** ���Ʊ��� */
		glBindTexture(GL_TEXTURE_2D, m_texture[0].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height*1.1, -length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height*1.1, -length);
		glEnd();
		/** ����ǰ�� */
		glBindTexture(GL_TEXTURE_2D, m_texture[1].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height*1.1, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height*1.1, length);
		glEnd();
		/** ���ƶ��� */
		glBindTexture(GL_TEXTURE_2D, m_texture[2].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, height, length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, height, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, -length);
		glEnd();
		/** �������� */
		glBindTexture(GL_TEXTURE_2D, m_texture[3].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 1.0f);  glVertex3f(-width, height, -length);
		glTexCoord2f(0.0f, 1.0f);  glVertex3f(-width, height, length);
		glTexCoord2f(0.0f, 0.0f);  glVertex3f(-width, -height*1.1, length);
		glTexCoord2f(1.0f, 0.0f);  glVertex3f(-width, -height*1.1, -length);
		glEnd();
		/** �������� */
		glBindTexture(GL_TEXTURE_2D, m_texture[4].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height*1.1, -length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height*1.1, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
		glEnd();
		glPopMatrix();                 /** ���ƽ��� */
	}
	else if(is_night == 1)
	{
		/** ��ʼ���� */
		glPushMatrix();
		glTranslatef(m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		/** ���Ʊ��� */
		glBindTexture(GL_TEXTURE_2D, m_texture[5].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height*1.1, -length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height*1.1, -length);
		glEnd();
		/** ����ǰ�� */
		glBindTexture(GL_TEXTURE_2D, m_texture[6].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height*1.1, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height*1.1, length);
		glEnd();
		/** ���ƶ��� */
		glBindTexture(GL_TEXTURE_2D, m_texture[7].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, height, length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, height, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, -length);
		glEnd();
		/** �������� */
		glBindTexture(GL_TEXTURE_2D, m_texture[8].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 1.0f);  glVertex3f(-width, height, -length);
		glTexCoord2f(0.0f, 1.0f);  glVertex3f(-width, height, length);
		glTexCoord2f(0.0f, 0.0f);  glVertex3f(-width, -height*1.1, length);
		glTexCoord2f(1.0f, 0.0f);  glVertex3f(-width, -height*1.1, -length);
		glEnd();
		/** �������� */
		glBindTexture(GL_TEXTURE_2D, m_texture[9].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height*1.1, -length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height*1.1, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
		glEnd();
		glPopMatrix();                 /** ���ƽ��� */
	}
	else if (is_night == 2)
	{
		/** ��ʼ���� */
		glPushMatrix();
		glTranslatef(m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		/** ���Ʊ��� */
		glBindTexture(GL_TEXTURE_2D, m_texture[10].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height*1.1, -length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height*1.1, -length);
		glEnd();
		/** ����ǰ�� */
		glBindTexture(GL_TEXTURE_2D, m_texture[11].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height*1.1, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height*1.1, length);
		glEnd();
		/** ���ƶ��� */
		glBindTexture(GL_TEXTURE_2D, m_texture[12].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, height, length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, height, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, -length);
		glEnd();
		/** �������� */
		glBindTexture(GL_TEXTURE_2D, m_texture[13].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 1.0f);  glVertex3f(-width, height, -length);
		glTexCoord2f(0.0f, 1.0f);  glVertex3f(-width, height, length);
		glTexCoord2f(0.0f, 0.0f);  glVertex3f(-width, -height*1.1, length);
		glTexCoord2f(1.0f, 0.0f);  glVertex3f(-width, -height*1.1, -length);
		glEnd();
		/** �������� */
		glBindTexture(GL_TEXTURE_2D, m_texture[14].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height*1.1, -length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height*1.1, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
		glEnd();
		glPopMatrix();                 /** ���ƽ��� */
	}
	else if (is_night == 5 || is_night == 4)
	{
		/** ��ʼ���� */
		glPushMatrix();
		glTranslatef(m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		/** ���Ʊ��� */
		glBindTexture(GL_TEXTURE_2D, m_texture[15].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height*1.1, -length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height*1.1, -length);
		glEnd();
		/** ����ǰ�� */
		glBindTexture(GL_TEXTURE_2D, m_texture[16].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height*1.1, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height*1.1, length);
		glEnd();
		/** ���ƶ��� */
		glBindTexture(GL_TEXTURE_2D, m_texture[17].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, height, length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, height, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, -length);
		glEnd();
		/** �������� */
		glBindTexture(GL_TEXTURE_2D, m_texture[18].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 1.0f);  glVertex3f(-width, height, -length);
		glTexCoord2f(0.0f, 1.0f);  glVertex3f(-width, height, length);
		glTexCoord2f(0.0f, 0.0f);  glVertex3f(-width, -height*1.1, length);
		glTexCoord2f(1.0f, 0.0f);  glVertex3f(-width, -height*1.1, -length);
		glEnd();
		/** �������� */
		glBindTexture(GL_TEXTURE_2D, m_texture[19].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height*1.1, -length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height*1.1, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
		glEnd();
		glPopMatrix();                 /** ���ƽ��� */
	}
	else if ( is_night == 3)
	{
		/** ��ʼ���� */
		glPushMatrix();
		glTranslatef(m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		/** ���Ʊ��� */
		glBindTexture(GL_TEXTURE_2D, m_texture[20].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height*1.1, -length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height*1.1, -length);
		glEnd();
		/** ����ǰ�� */
		glBindTexture(GL_TEXTURE_2D, m_texture[21].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height*1.1, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height*1.1, length);
		glEnd();
		/** ���ƶ��� */
		glBindTexture(GL_TEXTURE_2D, m_texture[22].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, height, length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, height, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, -length);
		glEnd();
		/** �������� */
		glBindTexture(GL_TEXTURE_2D, m_texture[23].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(1.0f, 1.0f);  glVertex3f(-width, height, -length);
		glTexCoord2f(0.0f, 1.0f);  glVertex3f(-width, height, length);
		glTexCoord2f(0.0f, 0.0f);  glVertex3f(-width, -height*1.1, length);
		glTexCoord2f(1.0f, 0.0f);  glVertex3f(-width, -height*1.1, -length);
		glEnd();
		/** �������� */
		glBindTexture(GL_TEXTURE_2D, m_texture[24].ID);
		glBegin(GL_QUADS);
		/** ָ����������Ͷ������� */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height*1.1, -length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height*1.1, length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
		glEnd();
		glPopMatrix();                 /** ���ƽ��� */
	}


	if(lp)                         /** �ָ�����״̬ */  
		glEnable(GL_LIGHTING);

//	glDisable(GL_TEXTURE_2D);

	 yRot += 0.01f;
	 if(yRot > 360.0f)
		 yRot = 0.0f;

	

}
