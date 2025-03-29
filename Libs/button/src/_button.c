#include "../inc/_button.h"

#if BUTTON_MODE_IT
    volatile bool button_A_pressed = false;
    volatile bool button_B_pressed = false;
#endif

void button_init(void){
    // Configurando o botao A
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Configurando o botao B
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    #if BUTTON_MODE_IT
        gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_A_callback);
        gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_B_callback);
    #endif
}

#if BUTTON_MODE_IT

void button_A_callback(uint gpio, uint32_t events) {    
    static absolute_time_t last_intrrup_time_A= {0};
    absolute_time_t current_time = get_absolute_time();

    if(absolute_time_diff_us(last_intrrup_time_A, current_time) > DEBOUNCING*1000){
        button_A_pressed = true;
        last_intrrup_time_A = current_time;
    }
}

void button_B_callback(uint gpio, uint32_t events) {
    static absolute_time_t last_intrrup_time_B = {0};
    absolute_time_t current_time = get_absolute_time();

    if(absolute_time_diff_us(last_intrrup_time_B, current_time) > DEBOUNCING*1000){
        button_B_pressed = true;
        last_intrrup_time_B = current_time;
    }
}

#endif

// Função para ler o estado do botão A e retornar true se pressionado
// e false caso contrário. Inclui um debouncing simples.
bool _read_button_A(void){
    #if BUTTON_MODE_IT
        if(button_A_pressed){
            button_A_pressed = false; // Reset the flag after reading
            return true;
        }
        return false;
    #else
    if(!gpio_get(BUTTON_A)){
        sleep_ms(DEBOUNCING);
        if(!gpio_get(BUTTON_A))
            return true;
    }
    return false;
    #endif
}


bool _read_button_B(void){
    #if BUTTON_MODE_IT
        if(button_B_pressed){
            button_B_pressed = false; // Reset the flag after reading
            return true;
        }
        return false;
    #else      
    if(!gpio_get(BUTTON_B)){
        sleep_ms(DEBOUNCING);
        if(!gpio_get(BUTTON_B))
            return true;
    }
    return false;
    #endif
}

