#ifndef BOTAO_H
#define BOTAO_H
/************************************|includ|**************************/
#include "config.h"
/************************************|function|***********************/
void button_init(void);
bool _read_button_A(void);
bool _read_button_B(void);
void button_A_callback(uint gpio, uint32_t events);
void button_B_callback(uint gpio, uint32_t events);

#endif //BOTAO_H