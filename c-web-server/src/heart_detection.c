// 心跳包检测
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#define  MAX_ONLINE_NUMBER  8
/***************************************************************
 * @brief  
 * @note   
 * @retval 
 **************************************************************/
struct heart_detection
{
    char ipaddr[64];            // 连接的 IP
    int  connection_times;      // 连接的次数
    char status[16];            // 连接转态
    struct timeval start_time;  // 开始连接的时间
    struct timeval end_time;    // 结束连接的时间
    char expand[0];             // 保留位
}connection_client[MAX_ONLINE_NUMBER] = {0};
/***************************************************************
 * @brief  
 * @note   
 * @retval 
 **************************************************************/
int get_the_number_of_online_members()
{
    int i = 0, count = 0;
    for (i = 0; i < MAX_ONLINE_NUMBER; i++)
    {
        if (!strcmp(connection_client[i].status, "on"))
        {
            count++;
        }
    }
    return count;
}
/***************************************************************
 * @brief  
 * @note   
 * @retval 
 **************************************************************/
int get_online_members_info()
{
    int i = 0, count = 0;
    for (i = 0; i < MAX_ONLINE_NUMBER; i++)
    {
        if (!strcmp(connection_client[i].status, "on"))
        {
            printf("ip = %s\n", connection_client[i].ipaddr);
            struct tm cur_tm;
            localtime_r((time_t*)&connection_client[i].start_time, &cur_tm);
            char cur_time[20];
            snprintf(cur_time, 20, "%d-%02d-%02d %02d:%02d:%02d", cur_tm.tm_year+1900, cur_tm.tm_mon+1, cur_tm.tm_mday, cur_tm.tm_hour, cur_tm.tm_min, cur_tm.tm_sec);
            printf("start_time %s\n", cur_time);//打印当前时间
        }
    }
    return count;
}
/***************************************************************
 * @brief  
 * @note   
 * @retval 
 **************************************************************/
void Heartbeat_packet_detection(const char *client_ip)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t sec = tv.tv_sec;
    uint64_t min = tv.tv_sec/60;

    int i = 0;
    for (i = 0; i < MAX_ONLINE_NUMBER; i++)
    {
        if (!strcmp(connection_client[i].status, "") || !strcmp(connection_client[i].status, "off"))
        {
            snprintf(connection_client[i].ipaddr, sizeof(connection_client[i].ipaddr), "%s", client_ip);
            snprintf(connection_client[i].status, sizeof(connection_client[i].status), "%s", "on");
            connection_client[i].start_time.tv_sec  = sec;
            connection_client[i].start_time.tv_usec = min;
            break;
        }
    } 
}