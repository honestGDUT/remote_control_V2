#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"


typedef enum {
    none_flag =0,
    short_flag ,
    double_flag,
    long_flag,
}key_click;

typedef enum {
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_L,
    KEY_R,
}KEY_ID;

typedef struct {
    uint8_t key_sta;
    uint8_t judge_sta;
    key_click key_event;
    key_click last_key_event;
    uint8_t double_click_timeEN;
    unsigned int    double_click_time;
    unsigned int    key_time;
    unsigned int    h_time;
}keys;

#define KEY_NUM 6
extern keys key[KEY_NUM];
void key_init(void);


#endif
