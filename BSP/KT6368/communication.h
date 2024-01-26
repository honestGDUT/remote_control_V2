#pragma once

#include "main.h"

typedef enum {
  mf_althold,
  mf_poshold,
  mf_mission
}mf_mode;

void SendFloatMsg(uint8_t msg_type, float* rc_values);
void KT6368A_Init(void);
void set_rc_connected(void);

uint8_t get_rc_connected(void);
float get_voltage(void);
mf_mode get_mf_mode(void);