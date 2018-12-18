#ifndef __CBMPLOADER_H__
#define __CBMPLOADER_H__

#include "stdafx.h"

#define BITMAP_ID 0x4D42	/**< 位图文件的标志 */

/** 位图载入类 */
class CBMPLoader
{
   public:
      CBMPLoader();
      ~CBMPLoader();

      bool LoadBitmap(const char *filename); /**< 装载一个bmp文件 */
      void FreeImage();                /**< 释放图像数据 */

      unsigned int ID;                 /**< 生成纹理的ID号 */
      int imageWidth;                  /**< 图像宽度 */
      int imageHeight;                 /**< 图像高度 */
      unsigned char *image;            /**< 指向图像数据的指针 */
};

#endif //__CBMPLOADER_H__

