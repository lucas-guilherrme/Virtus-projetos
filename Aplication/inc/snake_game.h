#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "config.h"

// Níveis de dificuldade
#define SNAKE_SPEED_EASY 300   // Velocidade em ms (fácil)
#define SNAKE_SPEED_MEDIUM 225 // Velocidade em ms (médio, 25% mais rápido)
#define SNAKE_SPEED_HARD 90    // Velocidade em ms (difícil, 70% mais rápido)

// Função principal do jogo da cobrinha
void play_snake_game(JoystickState *js, int difficulty);

#endif