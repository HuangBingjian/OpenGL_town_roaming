// *************************************************************
// 粒子效果：烟花下雪就是基于粒子效果实现的
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
	m_plist = new MyParticle[m_num]; // 分配内存空间
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
	fseek(fp, 54, SEEK_SET);   // 读取24位真彩色位图  
	image = (unsigned char*)malloc(width*height * 3);
	int rc;
	rc = fread(image, sizeof(unsigned   char), width*height * 3, fp);
	fclose(fp);
	BYTE texture[256][256][4]; // 注意：每个像素占用4个字节，不是原来的3个

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			// 把颜色值写入    
			texture[i][j][2] = (GLubyte)*(image + i*width * 3 + j * 3);
			texture[i][j][1] = (GLubyte)*(image + i*width * 3 + j * 3 + 1);
			texture[i][j][0] = (GLubyte)*(image + i*width * 3 + j * 3 + 2);
			// 设置alpha值,假设黑色为透明色    
			if (texture[i][j][0] == 0.0 && texture[i][j][1] == 0.0 && texture[i][j][2] == 0.0)
				texture[i][j][3] = 0;  // 透明，设为0    
			else
				texture[i][j][3] = 255;// 不透明，设为255    
		}
	}

	// 映射纹理    
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
		MessageBox(NULL, "载入位图文件失败!", "错误", MB_OK);
		exit(0);
	}

	/** 生成纹理对象名称 */
	glGenTextures(1, &my_texture);

	/** 创建纹理对象 */
	glBindTexture(GL_TEXTURE_2D, my_texture);

	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth,
		imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		image);
	return true;
}
bool Particle::LoadBitmap(const char *file)
{
	FILE *pFile = 0; /**< 文件指针 */

					 /** 创建位图文件信息和位图文件头结构 */
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;

	unsigned char textureColors = 0;/**< 用于将图像颜色从BGR变换到RGB */

									/** 打开文件,并检查错误 */
	pFile = fopen(file, "rb");
	if (pFile == 0) return false;

	/** 读入位图文件头信息 */
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);

	/** 检查该文件是否为位图文件 */
	if (header.bfType != BITMAP_ID)
	{
		fclose(pFile);             /**< 若不是位图文件,则关闭文件并返回 */
		return false;
	}

	/** 读入位图文件信息 */
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

	/** 保存图像的宽度和高度 */
	imageWidth = bitmapInfoHeader.biWidth;
	imageHeight = bitmapInfoHeader.biHeight;

	/** 确保读取数据的大小 */
	if (bitmapInfoHeader.biSizeImage == 0)
		bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth *
		bitmapInfoHeader.biHeight * 3;

	/** 将指针移到数据开始位置 */
	fseek(pFile, header.bfOffBits, SEEK_SET);

	/** 分配内存 */
	image = new unsigned char[bitmapInfoHeader.biSizeImage];

	/** 检查内存分配是否成功 */
	if (!image)                        /**< 若分配内存失败则返回 */
	{
		delete[] image;
		fclose(pFile);
		return false;
	}

	/** 读取图像数据 */
	fread(image, 1, bitmapInfoHeader.biSizeImage, pFile);

	/** 将图像颜色数据格式进行交换,由BGR转换为RGB */
	for (int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index += 3)
	{
		textureColors = image[index];
		image[index] = image[index + 2];
		image[index + 2] = textureColors;
	}

	fclose(pFile);       /**< 关闭文件 */
	return true;         /**< 成功返回 */
}