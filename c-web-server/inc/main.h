#ifndef __MAIN_H__
#define __MAIN_H__

#define ERROR 0
#define PORT 8080

typedef struct Cgi_public
{
    char *url;
    void (*callback_function)(int, char*);
}cgi_public;


#endif