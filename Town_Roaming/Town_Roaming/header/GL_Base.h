#ifndef __GLBase_H__
#define __GLBase_H__

#include <wchar.h>    
//#include "GL3dsLoder.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <gl/gl.h>										
#include <gl/glu.h>		
#include <GL/glut.h>  								
#include <gl/glaux.h>
#include <crtdbg.h>
#include <GL/glext.h>

using namespace std;


#define MAX_TEXTURES 1000							// 最大的纹理数目
#define MAX_OBSTACLE 10							// 最大的纹理数目
#define TEXTURESPATH "data\\textures\\"							// 纹理文件的路径
#define MODELSPATH "data\\3ds\\"							// 3DS文件的路径

//将lib库加入到工程中进行编译
#pragma   comment(lib,"glut.lib")
#pragma   comment(lib,"glut32.lib")
#pragma   comment(lib,"glaux.lib")
#pragma   comment(lib,"glu.lib")
#pragma   comment(lib,"glu32.lib")
#pragma comment(lib, "opengl.lib")
#pragma   comment(lib,"opengl32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "User32.lib") 
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib") 

#endif	// __GLBase_H__