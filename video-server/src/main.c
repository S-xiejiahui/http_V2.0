#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>

#include "lcd.h"
#include "queue.h"
#include "yuyv_to_jpeg.h"
#include "yuyv_show.h"
#include "v4l2_device_config.h"
#include "main.h"
#include <arpa/inet.h>

extern pthread_mutex_t mutex;
Queue *queue_display = NULL;
/***************************************************************************************************
					函数作用：摄像头捕捉画面显示线程
***************************************************************************************************/
#define MEMORY_600KB  600 * 1024
#define MEMORY_39KB   39  * 1024
void *display(void *arr)
{
	int n = 0;
	char data[MEMORY_600KB + 32] = {0}, jpeg[MEMORY_39KB] = {0};
	pthread_detach(pthread_self());
	while(1)
	{
		while(!queue_is_empty(queue_display))
		{
			pthread_mutex_lock(&mutex);

			pop_queue(queue_display, data);
			int w = compress_yuyv_to_jpeg(data, jpeg, sizeof(jpeg), QUILITY);
			show_video_data(jpeg, sizeof(jpeg), 80, 0, 1, 1);
			printf("write = %d\n", w);
			// n++;
			// if(n == 5)
			// {
			// 	FILE *fp = fopen("4.mp4", "a+");
			// 	if(fp)
			// 	{
			// 		fwrite(jpeg, sizeof(jpeg), 1, fp);
			// 		pthread_mutex_unlock(&mutex);
			// 		return;
			// 	}
			// }
				pthread_mutex_unlock(&mutex);
		}
	}	
}
/***************************************************************************************************
					函数作用：主函数
					作用：
						ps1：初始化线程
						ps2：访问摄像头驱动，将数据入队
***************************************************************************************************/
int main(int argc, char*argv[])
{
	lcd_init(); //初始化屏幕 并映射 
	pthread_mutex_init(&mutex, NULL);

	pthread_t pthread_video;
	pthread_create(&pthread_video, NULL, display, (void *)NULL);

	if(NULL == (queue_display = Ini_queue()))
	{
		printf("malloc queue error");
		return -1;
	}

	v4l2_turn_on_the_camera("/dev/video7");		/*step 1: 打开摄像头设备*/
	v4l2_query_device_function();				/*step 2: 查询设备功能(类型)*/
	v4l2_show_Supported_video_formats();		/*step 3: 枚举设备支持的视频格式*/
	v4l2_set_video_format();					/*step 4: 设置设备的视频格式。(属性)*/
	v4l2_request_video_buffer();				/*step 5: 申请视频缓冲，并映射，再把它加入到采集队列中去。*/
	v4l2_start_collecting(queue_display);		/*step 6: 查询缓冲区结束后才可以开始采集*/
	v4l2_end_of_acquisition(); 					/*step 7：停止采集*/
	v4l2_resource_recovery();					/*step 8：资源回收*/

	pthread_mutex_destroy(&mutex);
	lcd_uninit();
	return 0;
}

