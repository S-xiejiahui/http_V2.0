#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <jpeglib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include "lcd.h"

#define IMAGE_W 640
#define IMAGE_H 480
/***************************************************************
 * @brief:
 * @note:
 * @return:
 ***************************************************************/
int convert_yuv_to_rgb_pixel(int y, int u, int v)
{
	 unsigned int pixel32 = 0;
	 unsigned char pixel[3];// = (unsigned char *)&pixel32;
	 int r, g, b;
	 r = y + (1.370705 * (v-128));
	 g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
	 b = y + (1.732446 * (u-128));
	 if(r > 255) r = 255;
	 if(g > 255) g = 255;
	 if(b > 255) b = 255;
	 if(r < 0) r = 0;
	 if(g < 0) g = 0;
	 if(b < 0) b = 0;
	 pixel[0] = r * 220 / 256;
	 pixel[1] = g * 220 / 256;
	 pixel[2] = b * 220 / 256;

	 pixel32 = (pixel[0] << 16 ) | (pixel[1] << 8) | (pixel[2]);
	 return pixel32;
}
/***************************************************************
 * @brief:将 yuyv格式 转化为 argb格式 
 * @note: width 视频图像的宽度、height 视频图像的高度
 * @return:
 ***************************************************************/
void process_yuv_image(unsigned char *yuv, int width, int height)
{
	unsigned int in, out = 0;
	unsigned int pixel_16;
	unsigned char pixel_24[3];
	unsigned int pixel32;
	 int y0, u, y1, v;

	int pixel_num = 0;

 	for(in = 0; in < width * height * 2; in += 4) // 640 *480 *2 就是数组的总大小  每4个一组
	{
		//四字节组合成一个int型数据 
		pixel_16 =
			yuv[in + 3] << 24 |
			yuv[in + 2] << 16 |
			yuv[in + 1] <<  8 |
			yuv[in + 0];
		//将几个数据分量提取出来  y0在最低字节 u在 次低字节 y1在次高字节 V在 高字节 
		y0 = (pixel_16 & 0x000000ff);
		u  = (pixel_16 & 0x0000ff00) >>  8;
		y1 = (pixel_16 & 0x00ff0000) >> 16;
		v  = (pixel_16 & 0xff000000) >> 24;
		
		//通过y0 u v 计算出颜色数据  这里算的是第一个点 
 	 	pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
		pixel_num ++;
		//画点函数 
		lcd_draw_point(pixel_num/width,pixel_num%width,pixel32);

		//这里算的是第二个点 
		pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
		pixel_num ++;
		lcd_draw_point(pixel_num/width,pixel_num%width,pixel32);
 	}
	return ;
}
/***************************************************************
 * @brief: 将JPEG图像数据显示函数
 * @note:
 * @return:
 ***************************************************************/
int show_video_data(char *pjpg_buf, unsigned int jpg_buf_size, int point_x, int point_y, int numerator, int denominator)  
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr 		  jerr;	
	char *pjpg = NULL;
	unsigned int i = 0;
	unsigned int color = 0;
	
	pjpg = pjpg_buf;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	
	jpeg_mem_src(&cinfo,pjpg,jpg_buf_size);
	jpeg_read_header(&cinfo, TRUE);
	//放大、缩小倍数
	cinfo.scale_num   = numerator;//分子
    cinfo.scale_denom = denominator;//分母

	jpeg_start_decompress(&cinfo);	
	
	unsigned char *buffer = malloc(cinfo.output_width * cinfo.output_components);

//６. 读取一行或者多行扫描线数据并处理
	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, &buffer, 1); //dinfo.output_scanline + 1
		//deal with scanlines . RGB/ARGB
		int x; //一行的像素点数量

		char *p = buffer;
		for (x = 0; x < cinfo.output_width; x++)
		{
			unsigned char r, g, b, a = 0;
			int color;
			if (cinfo.output_components == 3)
			{
				r = *p++;
				g = *p++;
				b = *p++;
			} else if (cinfo.output_components == 4)
			{
				a = *p++;
				r = *p++;
				g = *p++;
				b = *p++;
			}
			color = (a << 24) | (r << 16) | (g << 8) |(b) ;

			lcd_draw_point(cinfo.output_scanline - 1 + point_y, x + point_x,  color); 	
		}
	}
//7. 调用　jpeg_finish_decompress()完成解压过程
	jpeg_finish_decompress(&cinfo);

//８. 调用jpeg_destroy_decompress()释放jpeg解压对象dinfo
	jpeg_destroy_decompress(&cinfo);
	free(buffer);
	return 0;
}
/***************************************************************
 * @brief: 显示 jpeg 图片
 * @note:
 * @return:
 ***************************************************************/
void jpeg_display(char *jpeg_pathname, unsigned int start_x, unsigned int start_y, int numerator, int denominator)
{
//1. 分配并初始化一个jpeg解压对象

	struct jpeg_decompress_struct dinfo; //定义了一个jpeg的解压对象
	struct jpeg_error_mgr jerr; //定义一个错误变量
	dinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&dinfo); //初始化这个解压对象
	
	FILE *infile;
	infile = fopen(jpeg_pathname, "r");
	if (infile == NULL)
	{
		fprintf(stderr, "[%d][%s]fopen error", __LINE__, __FUNCTION__);
		return;
	}
	jpeg_stdio_src(&dinfo, infile); //为解压对象dinfo指定要解压的文件
	jpeg_read_header(&dinfo, TRUE);//

    dinfo.scale_num   = numerator;
    dinfo.scale_denom = denominator;

	jpeg_start_decompress(&dinfo); 

	unsigned char *buffer = malloc(dinfo.output_width * dinfo.output_components);

	while (dinfo.output_scanline < dinfo.output_height)
	{
		jpeg_read_scanlines(&dinfo, &buffer, 1); //dinfo.output_scanline + 1

		int x; //一行的像素点数量
		char *p = buffer;
		for (x = 0; x < dinfo.output_width; x++)
		{
			unsigned char r, g, b, a = 0;
			int color;
			if (dinfo.output_components == 3)
			{
				r = *p++;
				g = *p++;
				b = *p++;
			} else if (dinfo.output_components == 4)
			{
				a = *p++;
				r = *p++;
				g = *p++;
				b = *p++;
			}
			color = (a << 24) | (r << 16) | (g << 8) |(b) ;

			lcd_draw_point(dinfo.output_scanline - 1 + start_y, x + start_x,  color); 	
		}
	}
//7. 调用　jpeg_finish_decompress()完成解压过程
	jpeg_finish_decompress(&dinfo);

//８. 调用jpeg_destroy_decompress()释放jpeg解压对象dinfo
	jpeg_destroy_decompress(&dinfo);
	free(buffer);
	fclose(infile);
	return;
}
/***************************************************************
 * @brief:
 * @note:
 * @return:
 ***************************************************************/
#if 0
void yuyv_to_rgb(unsigned char* yuv,unsigned char* rgb)
{
    unsigned int i;
    unsigned char* y0 = yuv + 0;   
    unsigned char* u0 = yuv + 1;
    unsigned char* y1 = yuv + 2;
    unsigned char* v0 = yuv + 3;
 
    unsigned  char* r0 = rgb + 0;
    unsigned  char* g0 = rgb + 1;
    unsigned  char* b0 = rgb + 2;
    unsigned  char* r1 = rgb + 3;
    unsigned  char* g1 = rgb + 4;
    unsigned  char* b1 = rgb + 5;
   
    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;
 
    for(i = 0; i <= (WIDTH * HEIGHT) / 2 ;i++)
    {
        bt0 = 1.164 * (*y0 - 16) + 2.018 * (*u0 - 128); 
        gt0 = 1.164 * (*y0 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128); 
        rt0 = 1.164 * (*y0 - 16) + 1.596 * (*v0 - 128); 
   
        bt1 = 1.164 * (*y1 - 16) + 2.018 * (*u0 - 128); 
        gt1 = 1.164 * (*y1 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128); 
        rt1 = 1.164 * (*y1 - 16) + 1.596 * (*v0 - 128); 
    
      
                   if(rt0 > 250)      rt0 = 255;
        if(rt0< 0)        rt0 = 0;    
 
        if(gt0 > 250)     gt0 = 255;
        if(gt0 < 0)    gt0 = 0;    
 
        if(bt0 > 250)    bt0 = 255;
        if(bt0 < 0)    bt0 = 0;    
 
        if(rt1 > 250)    rt1 = 255;
        if(rt1 < 0)    rt1 = 0;    
 
        if(gt1 > 250)    gt1 = 255;
        if(gt1 < 0)    gt1 = 0;    
 
        if(bt1 > 250)    bt1 = 255;
        if(bt1 < 0)    bt1 = 0;    
                    
        *r0 = (unsigned char)rt0;
        *g0 = (unsigned char)gt0;
        *b0 = (unsigned char)bt0;
    
        *r1 = (unsigned char)rt1;
        *g1 = (unsigned char)gt1;
        *b1 = (unsigned char)bt1;
 
        yuv = yuv + 4;
        rgb = rgb + 6;
        if(yuv == NULL)
          break;
 
        y0 = yuv;
        u0 = yuv + 1;
        y1 = yuv + 2;
        v0 = yuv + 3;
  
        r0 = rgb + 0;
        g0 = rgb + 1;
        b0 = rgb + 2;
        r1 = rgb + 3;
        g1 = rgb + 4;
        b1 = rgb + 5;
    }   
}
/***************************************************************
 * @brief:
 * @note:
 * @return:
 ***************************************************************/
void rgb_to_bmp(unsigned char* pdata, FILE* bmp_fd)     
{
    //分别为rgb数据，要保存的bmp文件名 
    int size = WIDTH * HEIGHT * 3 * sizeof(char); // 每个像素点3个字节  
    // 位图第一部分，文件信息  
    BMPFILEHEADER_T bfh;  
    bfh.bfType = (unsigned short)0x4d42;  //bm  
    bfh.bfSize = size  // data size  
        + sizeof( BMPFILEHEADER_T ) // first section size  
        + sizeof( BMPINFOHEADER_T ) // second section size  
        ;  
    bfh.bfReserved1 = 0; // reserved  
    bfh.bfReserved2 = 0; // reserved  
    bfh.bfOffBits = sizeof( BMPFILEHEADER_T )+ sizeof( BMPINFOHEADER_T );//真正的数据的位置 
//    printf("bmp_head== %ld\n", bfh.bfOffBits); 
    // 位图第二部分，数据信息  
    BMPINFOHEADER_T bih;  
    bih.biSize = sizeof(BMPINFOHEADER_T);  
    bih.biWidth = WIDTH;  
    bih.biHeight = -HEIGHT;//BMP图片从最后一个点开始扫描，显示时图片是倒着的，所以用-height，这样图片就正了  
    bih.biPlanes = 1;//为1，不用改  
    bih.biBitCount = 24;  
    bih.biCompression = 0;//不压缩  
    bih.biSizeImage = size;  
 
    bih.biXPelsPerMeter = 0;//像素每米  
  
    bih.biYPelsPerMeter = 0;  
    bih.biClrUsed = 0;//已用过的颜色，为0,与bitcount相同  
    bih.biClrImportant = 0;//每个像素都重要   
    
    fwrite( &bfh, 8, 1, bmp_fd); 
    fwrite(&bfh.bfReserved2, sizeof(bfh.bfReserved2), 1, bmp_fd);  
    fwrite(&bfh.bfOffBits, sizeof(bfh.bfOffBits), 1, bmp_fd);  
    fwrite(&bih, sizeof(BMPINFOHEADER_T), 1, bmp_fd);  
    fwrite(pdata, size, 1, bmp_fd);   
} 
#endif