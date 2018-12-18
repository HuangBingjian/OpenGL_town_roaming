#pragma once
#include <gl/glut.h>
#include <math.h>

#define FRAND   (((float)rand()-(float)rand())/RAND_MAX)
#define Clamp(x, min, max)  x = (x<min  ? min : x<max ? x : max);
#define SQUARE(x)  (x)*(x)

struct vector3_t
{
	vector3_t(float x, float y, float z) : x(x), y(y), z(z) {}
	vector3_t(const vector3_t &v) : x(v.x), y(v.y), z(v.z) {}
	vector3_t() : x(0.0f), y(0.0f), z(0.0f) {}

	vector3_t& operator=(const vector3_t &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	vector3_t operator+(const vector3_t &rhs) const
	{
		return vector3_t(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	vector3_t operator-(const vector3_t &rhs) const
	{
		return vector3_t(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	vector3_t operator*(const float scalar) const
	{
		return vector3_t(x * scalar, y * scalar, z * scalar);
	}

	float operator*(const vector3_t &rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	vector3_t operator^(const vector3_t &rhs) const
	{
		return vector3_t(y * rhs.z - rhs.y * z, rhs.x * z - x * rhs.z, x * rhs.y - rhs.x * y);
	}

	float& operator[](int index)
	{
		return v[index];
	}

	float Length()
	{
		float length = (float)sqrt(SQUARE(x) + SQUARE(y) + SQUARE(z));
		return (length != 0.0f) ? length : 1.0f;
	}

	vector3_t Normalize()
	{
		*this = *this * (1.0f / Length());
		return *this;
	}

	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float v[3];
	};
};

// 焰火粒子的数据结构
struct firepoint                          
{
	float x, y, z;             // 位置
	float r, g, b;             // 颜色
	float speed, l;            // 速度
	int   angle1, angle2;      // 角度
};

const int   fireN = 300;                   // 焰火数目
const float dspeed = 0.06;                 // 焰火减速
firepoint   firePoint[30][fireN];          // 爆炸后的焰火粒子
bool        fireDraw[30];                  // 标识是否绘制焰火
firepoint   unexplosion[30];               // 未爆炸的焰火粒子
AUX_RGBImageRec *Tex_fire;						//为纹理创建存储空间
GLuint      m_firepoint;


AUX_RGBImageRec *Load_fire(char *Filename)				// 读取位图图象
{
	FILE *File = NULL;									// 文件句柄

	if (!Filename)										// 确定文件名已给出
	{
		return NULL;									// 如果文件名未给出则返回NULL
	}

	File = fopen(Filename, "r");							// 检测文件是否存在

	if (File)											// 文件是否存在?
	{
		fclose(File);									// 关闭文件
		return auxDIBImageLoad(Filename);				// 读取位图并返回一个指针
	}

	return NULL;										//如果调用文件失败则返回NULL
}

// 绘制焰火
void FireWorks()                            
{
	if (Tex_fire = Load_fire("data/textures/explosion.bmp"))        /**< 载入位图文件 */
	{
		glGenTextures(1, &m_firepoint);                        /**< 生成一个纹理对象名称 */
		glBindTexture(GL_TEXTURE_2D, m_firepoint);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, Tex_fire->sizeX, Tex_fire->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Tex_fire->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		MessageBox(NULL, "装载位图文件失败！", "错误", MB_OK);    /**< 如果载入失败则弹出对话框 */
		exit(0);
	}

	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glAlphaFunc(GL_GREATER, 0);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glDepthMask(GL_FALSE);

	float mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	vector3_t X(mat[0], mat[4], mat[8]);
	vector3_t Z(mat[1], mat[5], mat[9]);
	vector3_t pos(0, 0.0, 0);
	pos.y = 0;
	glColor4f(1, 1, 1, 1);
	for (int i = 0; i<30; i++)
	{
		if (fireDraw[i] == true && unexplosion[i].speed>0)
		{
			glPushMatrix();
			glTranslatef(unexplosion[i].x, unexplosion[i].y, unexplosion[i].z);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3fv((pos + (X + Z)*-0.8).v); // 左下点
			glTexCoord2f(1.0, 0.0); glVertex3fv((pos + (X - Z)* 0.8).v); // 右下点
			glTexCoord2f(1.0, 1.0); glVertex3fv((pos + (X + Z)* 0.8).v); // 右上点
			glTexCoord2f(0.0, 1.0); glVertex3fv((pos + (Z - X)* 0.8).v); // 左上点
			glEnd();
			glPopMatrix();

			unexplosion[i].y += unexplosion[i].speed;
			unexplosion[i].speed -= dspeed;
			if (unexplosion[i].speed <= 0)
				unexplosion[i].speed = 0;
		}
	}

	for (int i1 = 0; i1<30; i1++)
		for (int i2 = 0; i2<fireN; i2++)
		{
			if (fireDraw[i1] == true && unexplosion[i1].speed == 0 && (firePoint[i1][i2].r>0 || firePoint[i1][i2].g>0 || firePoint[i1][i2].b>0))
			{
				{
					glPushMatrix();
					glColor3f(firePoint[i1][i2].r, firePoint[i1][i2].g, firePoint[i1][i2].b);
					glTranslatef(unexplosion[i1].x, unexplosion[i1].y, unexplosion[i1].z);
					glRotatef(firePoint[i1][i2].angle1, 1, 0, 0);
					glGetFloatv(GL_MODELVIEW_MATRIX, mat);
					vector3_t X(mat[0], mat[4], mat[8]);
					vector3_t Z(mat[1], mat[5], mat[9]);
					vector3_t pos(0, 0.0, 0);
					pos.y = 0;
					glTranslatef(firePoint[i1][i2].x, 0, firePoint[i1][i2].z);
					glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0); glVertex3fv((pos + (X + Z)*-2).v);  // 左下点
					glTexCoord2f(1.0, 0.0); glVertex3fv((pos + (X - Z) * 2).v); // 右下点
					glTexCoord2f(1.0, 1.0); glVertex3fv((pos + (X + Z) * 2).v); // 右上点
					glTexCoord2f(0.0, 1.0); glVertex3fv((pos + (Z - X) * 2).v); // 左上点
					glEnd();
					glPopMatrix();
				}
				firePoint[i1][i2].x += cos(firePoint[i1][i2].angle2)*firePoint[i1][i2].speed;
				firePoint[i1][i2].z -= sin(firePoint[i1][i2].angle2)*firePoint[i1][i2].speed;

				if (firePoint[i1][i2].r>0)
					firePoint[i1][i2].r -= 0.02;
				if (firePoint[i1][i2].g>0)
					firePoint[i1][i2].g -= 0.02;
				if (firePoint[i1][i2].b>0)
					firePoint[i1][i2].b -= 0.02;
				if (firePoint[i1][i2].speed >= 0.2)
					firePoint[i1][i2].speed -= 0.2;
			}

		}
	glDepthMask(GL_TRUE);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}

// 重置焰火状态
void FireInit()                             
{
	for (int i = 0; i<30; i += 3)
	{
		if (rand() % 2 == 0)
			unexplosion[i].x = 900 + rand() % 300;
		else
			unexplosion[i].x = 1100 - rand() % 300;

		unexplosion[i + 1].x = unexplosion[i].x + float(rand() % 100) / 10.0;
		unexplosion[i + 2].x = unexplosion[i].x - float(rand() % 100) / 10.0;

		if (rand() % 2 == 0)
			unexplosion[i].z = -1400  + rand() % 400;
		else
			unexplosion[i].z = -1000 - rand() % 400;

		unexplosion[i + 1].z = unexplosion[i].z + float(rand() % 100) / 10.0;
		unexplosion[i + 2].z = unexplosion[i].z - float(rand() % 100) / 10.0;

		unexplosion[i].speed = 3 + float(rand() % 150) / 50.0;
		unexplosion[i + 1].speed = 3 + float(rand() % 150) / 50.0;
		unexplosion[i + 2].speed = 3 + float(rand() % 150) / 50.0;

		unexplosion[i].y = 50;
		unexplosion[i + 1].y = 50;
		unexplosion[i + 2].y = 50;
	}

	for (int i1 = 0; i1<30; i1++)
	{
		if (rand() % 2 == 0)
			fireDraw[i1] = true;
		else
			fireDraw[i1] = false;
	}

	for (int i2 = 0; i2<30; i2++)
	{
		int c = rand() % 5;
		for (int i3 = 0; i3<fireN; i3++)
		{
			firePoint[i2][i3].x = 0;
			firePoint[i2][i3].z = 0;
			firePoint[i2][i3].angle1 = rand() % 360;
			firePoint[i2][i3].angle2 = rand() % 360;
			firePoint[i2][i3].speed = (float)(rand() % 100) / 17.0;

			switch (c)
			{
			case  0:                                   // 红色焰火
			{
				firePoint[i2][i3].b = 0;
				firePoint[i2][i3].g = 0;
				firePoint[i2][i3].r = 1;
				break;
			}

			case  1:                                   // 绿色焰火
			{
				firePoint[i2][i3].b = 0;
				firePoint[i2][i3].g = 1;
				firePoint[i2][i3].r = 0;
				break;
			}

			case  2:                                   // 黄色焰火
			{
				firePoint[i2][i3].b = 0;
				firePoint[i2][i3].g = 1;
				firePoint[i2][i3].r = 1;
				break;
			}

			case  3:                                   // 白色焰火
			{
				firePoint[i2][i3].b = 1;
				firePoint[i2][i3].g = 1;
				firePoint[i2][i3].r = 1;
				break;
			}

			case  4:                                   // 彩色焰火
			{
				if (rand() % 3 == 0)
					firePoint[i2][i3].b = 0;
				else
					firePoint[i2][i3].b = 1;

				if (rand() % 3 == 0)
					firePoint[i2][i3].g = 0;
				else
					firePoint[i2][i3].g = 1;

				if (rand() % 3 == 0)
					firePoint[i2][i3].r = 0;
				else
					firePoint[i2][i3].r = 1;
				break;
			}
			}
		}
	}
}

// 因为环境光已经设置材质颜色即光照颜色了,如果需要多彩烟花的话,可以启动颜色材料模型:glEnable( GL_COLOR_MATERIAL );
// 然后还需要设置材料属性:glLightModeli(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 
// 这个表示模型的正面接受环境光和散射光，你可以修改这两个参数