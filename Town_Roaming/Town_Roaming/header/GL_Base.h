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


#define MAX_TEXTURES 1000							// ����������Ŀ
#define MAX_OBSTACLE 10							// ����������Ŀ
#define TEXTURESPATH "data\\textures\\"							// �����ļ���·��
#define MODELSPATH "data\\3ds\\"							// 3DS�ļ���·��

//��lib����뵽�����н��б���
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