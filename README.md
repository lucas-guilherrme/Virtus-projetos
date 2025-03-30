---

![](/img/virtus-cc.png)

---

## 📜 Índice

- [Introdução](#-introdução)
- [Fluxograma do programa](#-fluxograma-do-programa)
- [Diagrama Lógico](#-diagrama-lógico)
- [Descrição do Fluxo](#-descrição-do-fluxo)
  - [Inicialização dos Periféricos](#inicialização-dos-periféricos)
  - [Teste Inicial no Display](#teste-inicial-no-display)
  - [Exibição do Menu](#exibição-do-menu)
- [Detalhes do Menu](#-detalhes-do-menu)
- [Resumo](#-resumo)
- [Análise das bibliotecas](#-análise-das-bibliotecas)
  - [Joystick](#joystick)
  - [Botões](#botões)
  - [Matriz de LEDs](#matriz-de-leds)
- [Contribuintes](#-contribuintes)

# 📌 Introdução

Este projeto foi desenvolvido para o **Raspberry Pi Pico W** e contém bibliotecas escritas em **C** utilizando o SDK oficial da Raspberry Pi. O objetivo principal é fornecer uma interface fácil de usar para componentes como **joystick**, **matriz de LEDs RGB** e **botões**, permitindo a criação de aplicações interativas e dinâmicas. O ambiente de desenvolvimento utilizado foi o **Visual Studio Code**, com suporte à extensão do Raspberry Pi Pico.

As bibliotecas foram projetadas para serem modulares e reutilizáveis, com exemplos práticos que demonstram como integrá-las em projetos personalizados. Além disso, todo o processo de modelagem e verificação foi realizado utilizando a placa **BitDogLab**, garantindo confiabilidade e compatibilidade com o hardware.

O projeto inclui funcionalidades como:
- **Leitura de Joystick**: Aplicação de filtros (média móvel, passa-baixa, zona morta) para leituras analógicas estáveis.
- **Controle de Matriz de LEDs**: Manipulação de uma matriz 5x5 RGB utilizando PIO.
- **Gerenciamento de Botões**: Detecção de cliques simples, duplos e longos, com suporte a debounce e interrupções.

Com uma estrutura bem organizada e documentação clara, este repositório é ideal para quem deseja explorar o potencial do Raspberry Pi Pico W em projetos de hardware interativo.

---

## ✨ Structure

```txt
c:\Users\Lucas Guilherme\Documents\Raspberry Pi Pico\MODULO03\projeto_revisao_lib
├── Aplication
│   ├── inc
│   │   ├── button.h
│   │   ├── joystick.h
│   │   ├── ledMatrix.h
│   │   ├── examples.h
│   │   └── ws2818b.pio.h
│   ├── src
│   │   ├── button
│   │   │   └── button.c
│   │   ├── joystick
│   │   │   └── joystick.c
│   │   ├── ledMatrix
│   │   │   └── ledMatrix.c
│   │   ├── games
│   │   │   └── jogo_da_velha.c
│   │   └── main.c
│   └── ws2818b.pio
├── img
│   └── virtus-cc.png
├── README.md
└── CMakeLists.txt
```

# 📋 Fluxograma do programa

```txt
+-----------------------------+
|         Início              |
+-----------------------------+
            |
            v
+-----------------------------+
| Inicializa Periféricos      |
| - stdio_init_all()          |
| - ssd_1306_init()           |
| - button_init()             |
| - Joystick_Init(&js)        |
+-----------------------------+
            |
            v
+-----------------------------+
| Limpa Tela e Exibe Teste    |
| - ssd_1306_fill(black)      |
| - ssd_1306_draw_circle()    |
| - ssd_1306_up_date_screen() |
+-----------------------------+
            |
            v
+-----------------------------+
| Exibe Menu                  |
| - show_menu(&js, game_names)|
+-----------------------------+
            |
            v
+-----------------------------+
| Seleciona Jogo              |
| switch(game)                |
| - Caso 0: Jogo da Velha     |
| - Caso 1: Snake (não usado) |
| - Caso 2: Sequência Correta |
+-----------------------------+
            |
            v
+-----------------------------+
|         Fim                 |
+-----------------------------+

```

Aqui está um diagrama lógico para o programa descrito no código. Ele representa o fluxo principal do programa, desde a inicialização até a seleção de um jogo no menu.

---

# 📋 Diagrama Lógico

```plaintext
+-----------------------------+
|         Início             |
+-----------------------------+
            |
            v
+-----------------------------+
| Inicializa Periféricos      |
| - stdio_init_all()          |
| - ssd_1306_init()           |
| - button_init()             |
| - Joystick_Init(&js)        |
+-----------------------------+
            |
            v
+-----------------------------+
| Limpa Tela e Exibe Teste    |
| - ssd_1306_fill(black)      |
| - ssd_1306_draw_circle()    |
| - ssd_1306_up_date_screen() |
+-----------------------------+
            |
            v
+-----------------------------+
| Exibe Menu                  |
| - show_menu(&js, game_names)|
+-----------------------------+
            |
            v
+-----------------------------+
| Seleciona Jogo              |
| switch(game)                |
| - Caso 0: Jogo da Velha     |
| - Caso 1: Snake (não usado) |
| - Caso 2: Sequência Correta |
+-----------------------------+
            |
            v
+-----------------------------+
|         Fim                 |
+-----------------------------+
```

---

# 📋 Descrição do Fluxo

1. **Início**:
   - O programa começa com a inicialização dos periféricos necessários, como o display OLED, os botões e o joystick.

2. **Inicialização dos Periféricos**:
   - Funções como `ssd_1306_init()` e `Joystick_Init()` configuram o hardware para uso.
   - O display é preparado para exibir informações.

3. **Teste Inicial no Display**:
   - Um círculo é desenhado na tela como teste visual para verificar se o display está funcionando corretamente.

4. **Exibição do Menu**:
   - A função `show_menu` exibe as opções disponíveis no menu, permitindo que o usuário navegue usando o joystick e selecione uma opção pressionando o botão.

5. **Seleção do Jogo**:
   - Com base na opção selecionada no menu, o programa executa o jogo correspondente:
     - **Caso 0**: Inicia o "Jogo da Velha" chamando a função `play_tic_tac_toe()`.
     - **Caso 1**: Placeholder para o jogo "Snake" (não implementado).
     - **Caso 2**: Placeholder para o jogo "Sequência Correta" (não implementado).

6. **Fim**:
   - Após a execução do jogo selecionado, o programa termina.

---

# 📋 Detalhes do Menu

Dentro da função `show_menu`, o fluxo segue esta lógica:

```plaintext
+-----------------------------+
| Exibe Opções do Menu        |
| - Limpa a tela              |
| - Escreve as opções         |
| - Destaca a opção atual     |
+-----------------------------+
            |
            v
+-----------------------------+
| Lê Entrada do Joystick      |
| - Joystick_Read()           |
| - Joystick_ApplyFilters()   |
+-----------------------------+
            |
            v
+-----------------------------+
| Navega pelo Menu            |
| - Move para cima ou baixo   |
| - Atualiza opção selecionada|
+-----------------------------+
            |
            v
+-----------------------------+
| Verifica Botão de Seleção   |
| - _joystick_read_button()   |
| - Retorna opção selecionada |
+-----------------------------+
```

---

# 📚 Resumo 
Este diagrama lógico descreve o fluxo principal do programa e a interação com o menu. Ele é projetado para ser modular, permitindo a adição de novos jogos ou funcionalidades no futuro. O joystick é usado para navegação, enquanto o botão confirma a seleção, proporcionando uma interface simples e intuitiva.

---

# 💡 Análise das bibliotecas

## 💡🔧 **joystick**

1. __Dependências do Código com Outros Arquivos__

joystick.h: O arquivo de cabeçalho joystick.h é necessário para definir a estrutura JoystickState, constantes, e possivelmente as assinaturas das funções implementadas no código. Sem ele, o código não compilará corretamente.

hardware/adc.h: Esta biblioteca é usada para interagir com o ADC (Conversor Analógico-Digital) do hardware. Ela fornece funções como adc_init, adc_gpio_init, adc_select_input e adc_read, que são essenciais para a leitura dos valores analógicos do joystick.

Constantes e macros: O código depende de várias constantes e macros, como ADC_PIN_X, ADC_PIN_Y, ADC_CHANEL_1, ADC_CHANEL_0, AVERAGE_VALUE, DEADZONE_THRESHOLD, MOVING_AVG_WINDOW e LOWPASS_ALPHA. Essas constantes precisam ser definidas em joystick.h ou em outro arquivo de configuração.

2. __Capacidade de Usar a Biblioteca Isolada__

A biblioteca pode ser usada de forma isolada, mas algumas condições precisam ser atendidas:

_Definição de constantes:_ Todas as constantes mencionadas acima devem ser definidas corretamente no arquivo de cabeçalho.

_Independência do hardware:_ O código depende diretamente do ADC do hardware. Para torná-lo mais portátil, seria necessário abstrair as interações com o ADC em uma camada de hardware separada, permitindo que a biblioteca seja usada em diferentes plataformas.

3. __Possíveis Melhorias a Serem Feitas__

 - Abstração do Hardware
 - Validação de Parâmetros
 - Documentação
 - Tratamento de Erros

---

## 💡🔧 **Biblioteca do Botão**

O código implementa uma solução robusta para gerenciar botões em um sistema embarcado, incluindo funcionalidades como debounce (eliminação de ruídos), detecção de cliques simples, cliques duplos e pressionamentos longos. Ele também utiliza interrupções para lidar com eventos de botão de forma eficiente e fornece feedback visual por meio de LEDs.

1. __Estados Globais dos Botões__

As variáveis globais button_a_state, button_b_state e button_sw_state armazenam o estado de cada botão, incluindo informações como o GPIO associado, o último estado, o tempo do último evento e flags para detecção de eventos.

2. __Inicialização dos Botões__

A função init_button configura o GPIO de um botão como entrada com resistor pull-up e inicializa seu estado.
A função init_button_IT configura interrupções para detectar bordas de subida e descida no GPIO do botão.

3. __Feedback Visual__

A função blink_led controla LEDs para fornecer feedback visual com base nos eventos detectados (acredito que só foi um teste para ver o funcionamento e não faz parte da biblioteca).
A função event_function é um exemplo de manipulador de eventos que pisca LEDs diferentes para cliques simples, cliques duplos e pressionamentos longos.

4. __Pontos a Serem Melhorados__

_Uso de Strings para Identificar Botões:_ O uso de strings (char* button_name) para identificar botões em funções como get_button_state e init_button é ineficiente e propenso a erros. Uma abordagem melhor seria usar um enum ou constantes inteiras para representar os botões, reduzindo o custo de comparação de strings.

_Sugestão:_
 - Modularidade no Manipulador de Interrupções
 - Dependência de Variáveis Globais
 - Falta de Documentação para Algumas Funções

## 💡🔧 **Matriz de led**

_Uso de Constantes Fixas:_ Constantes como 5 (dimensão da matriz) e 4 (índice máximo) estão espalhadas pelo código, dificultando alterações futuras.

_Melhoria_: Definir macros/definições para a dimensão da matriz e índices máximos.

---

OBS: Lógica das aplicações em desenvolvimento

---

# 👥 Contribuintes

### *Lucas Guilherme*
### *Pedro Wilson*