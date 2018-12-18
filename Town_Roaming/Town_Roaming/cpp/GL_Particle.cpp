// *************************************************************
// ����Ч�����̻���ѩ���ǻ�������Ч��ʵ�ֵ�
// *************************************************************

#define _CRT_SECURE_NO_WARNINGS
#include "..\header\GL_Particle.h"

Particle::Particle()
{

}

Particle::~Particle()
{
	if (m_plist != NULL)
	{
		delete[]m_plist;
		m_plist = NULL;
	}

}
bool Particle::Init(int num)
{
	m_num = num;
	m_plist = new MyParticle[m_num]; // �����ڴ�ռ�
	if (m_plist == NULL)
		return false;
	return true;
}
void Particle::LoadTreeBitmap(char *filename, GLuint &TreeTexture)
{

	glGenTextures(1, &TreeTexture);
	glBindTexture(GL_TEXTURE_2D, TreeTexture);

	AUX_RGBImageRec* myimage = NULL;
	unsigned char *image;
	myimage = auxDIBImageLoad(filename);
	int width, height;
	width = myimage->sizeX;
	height = myimage->sizeY;
	FILE *fp;
	fp = fopen(filename, "rb");
	if (!fp)
		return;
	fseek(fp, 54, SEEK_SET);   // ��ȡ24λ���ɫλͼ  
	image = (unsigned char*)malloc(width*height * 3);
	int rc;
	rc = fread(image, sizeof(unsigned   char), width*height * 3, fp);
	fclose(fp);
	BYTE texture[256][256][4]; // ע�⣺ÿ������ռ��4���ֽڣ�����ԭ����3��

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			// ����ɫֵд��    
			texture[i][j][2] = (GLubyte)*(image + i*width * 3 + j * 3);
			texture[i][j][1] = (GLubyte)*(image + i*width * 3 + j * 3 + 1);
			texture[i][j][0] = (GLubyte)*(image + i*width * 3 + j * 3 + 2);
			// ����alphaֵ,�����ɫΪ͸��ɫ    
			if (texture[i][j][0] == 0.0 && texture[i][j][1] == 0.0 && texture[i][j][2] == 0.0)
				texture[i][j][3] = 0;  // ͸������Ϊ0    
			else
				texture[i][j][3] = 255;// ��͸������Ϊ255    
		}
	}

	// ӳ������    
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	free(myimage->data);
	free(myimage);
	free(image);
}
bool  Particle::Load(char *filename, GLuint &my_texture)
{
	if (!LoadBitmap(filename))
	{
		MessageBox(NULL, "����λͼ�ļ�ʧ��!", "����", MB_OK);
		exit(0);
	}

	/** ��������������� */
	glGenTextures(1, &my_texture);

	/** ����������� */
	glBindTexture(GL_TEXTURE_2D, my_texture);

	/** �����˲� */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/** �������� */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth,
		imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		image);
	return true;
}
bool Particle::LoadBitmap(const char *file)
{
	FILE *pFile = 0; /**< �ļ�ָ�� */

					 /** ����λͼ�ļ���Ϣ��λͼ�ļ�ͷ�ṹ */
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;

	unsigned char textureColors = 0;/**< ���ڽ�ͼ����ɫ��BGR�任��RGB */

									/** ���ļ�,�������� */
	pFile = fopen(file, "rb");
	if (pFile == 0) return false;

	/** ����λͼ�ļ�ͷ��Ϣ */
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);

	/** �����ļ��Ƿ�Ϊλͼ�ļ� */
	if (header.bfType != BITMAP_ID)
	{
		fclose(pFile);             /**< ������λͼ�ļ�,��ر��ļ������� */
		return false;
	}

	/** ����λͼ�ļ���Ϣ */
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

	/** ����ͼ��Ŀ�Ⱥ͸߶� */
	imageWidth = bitmapInfoHeader.biWidth;
	imageHeight = bitmapInfoHeader.biHeight;

	/** ȷ����ȡ���ݵĴ�С */
	if (bitmapInfoHeader.biSizeImage == 0)
		bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth *
		bitmapInfoHeader.biHeight * 3;

	/** ��ָ���Ƶ����ݿ�ʼλ�� */
	fseek(pFile, header.bfOffBits, SEEK_SET);

	/** �����ڴ� */
	image = new unsigned char[bitmapInfoHeader.biSizeImage];

	/** ����ڴ�����Ƿ�ɹ� */
	if (!image)                        /**< �������ڴ�ʧ���򷵻� */
	{
		delete[] image;
		fclose(pFile);
		return false;
	}

	/** ��ȡͼ������ */
	fread(image, 1, bitmapInfoHeader.biSizeImage, pFile);

	/** ��ͼ����ɫ���ݸ�ʽ���н���,��BGRת��ΪRGB */
	for (int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index += 3)
	{
		textureColors = image[index];
		image[index] = image[index + 2];
		image[index + 2] = textureColors;
	}

	fclose(pFile);       /**< �ر��ļ� */
	return true;         /**< �ɹ����� */
}