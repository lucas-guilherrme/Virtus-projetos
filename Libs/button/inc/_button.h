#ifndef BOTAO_H
#define BOTAO_H
/************************************|includ|**************************/
#include "pico/stdlib.h"
#include "hardware/gpio.h"

/************************************|define|**************************/
#define BUTTON_A 5 
#define BUTTON_B 6 

#define DEBOUNCING 100 // Tempo de debouncing em milissegundos

#define BUTTON_MODE_IT  0
/************************************|function|***********************/
void button_init(void);

bool _read_button_A(void);
bool _read_button_B(void);

void button_A_callback(uint gpio, uint32_t events);
void button_B_callback(uint gpio, uint32_t events);

#endif //BOTAO_H