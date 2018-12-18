#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "Vector.h"
#include "Camera.h"

#define GL_CLAMP_TO_EDGE	0x812F

/** 天空盒类 */
class CSkyBox
{
public:
	/** 构造函数 */
	CSkyBox();
	~CSkyBox();

	/** 初始化 */
	bool init();
	
	/** 渲染 */
	void render(int is_night);

private:
	
	CBMPLoader  m_texture[25];   /**< 天空盒纹理   */
	Vector3     m_CameraPos;    /**< 当前摄像机位置 */
	float       length;         /**< 长度 */
	float       width;          /**< 宽度 */
	float       height;         /**< 高度 */
	float       yRot;           /**< 绕Y轴旋转 */
		
};


#endif ///__SKYBOX_H__