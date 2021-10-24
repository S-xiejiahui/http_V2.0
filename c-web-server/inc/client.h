#ifndef __CLIENT__H__
#define __CLIENT__H__

int start_receive_video_data(int port);
int stop_receive_video_data(void);
int get_a_frame_of_image(char *data, int *length);

#endif