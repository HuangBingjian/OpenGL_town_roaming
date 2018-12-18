#pragma once
#include "cvector.h"

#include <stdio.h>
#include <gl/glut.h>
#include <gl/glaux.h>

#define BITMAP_ID 0x4D42	/**< 位图文件的标志 */

struct MyParticle
{
	CVector  position;     // 粒子的坐标（用向量表示）
	CVector  velocity;     // 粒子在x,y,z三个方向的初速度
	CVector  acceleration; // 粒子在x,y,z三个方向的加速度
	float  lifetime;       // 粒子的生命期（生命长度）
	float  dec;            // 粒子消失的速度
	float  size;           // 粒子的大小
	float  color[3];       // 粒子的颜色
};

class Particle
{
public:
	Particle();
	virtual ~Particle();
	virtual bool Init(int num);      // 粒子的初始化函数
	virtual void Render() = 0;       // 粒子的渲染
	virtual void Update() = 0;       // 粒子的更新
	unsigned int ID;                 /**< 生成纹理的ID号 */
	int imageWidth;                  /**< 图像宽度 */
	int imageHeight;                 /**< 图像高度 */
	unsigned char *image;            /**< 指向图像数据的指针 */
	bool        LoadBitmap(const char *file);
	bool        Load(char *filename, GLuint &my_texture);
	void        LoadTreeBitmap(char *filename, GLuint &TreeTexture);
	int m_num;                       // 粒子总数目
	MyParticle* m_plist;             // 粒子列表

};

