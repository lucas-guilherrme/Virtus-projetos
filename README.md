---

![](/img/virtus-cc.png)

---

# 📌 Utilização da biblioteca - comentários 

# Libraries for Raspberry Pi Pico W

This repository contains libraries for use on the Raspberry Pi Pico W. The code present in this library was written in C with the SDK of Raspberry Pi Pico W and the working environment used was Visual Studio Code, using the Raspberry Pi Pico extension present in the development tool.The entire modeling and verification phase of the libraries was done using the [BitDogLab](https://github.com/BitDogLab/BitDogLab) board.

This library provides an easy-to-use interface for:

- **Joystick** (Reading analog values ​​and applying filters)  
- **LED Matrix** (Control of a 5x5 RGB matrix via PIO)  
- **Buttons** (Single, double and long press click detection)  
- **Examples** (In examples.h, demonstrating the built-in usage)

---

## Structure

```txt
.
├── inc
│   ├── button.h         # Button definitions (structures, events, etc.)
│   ├── joystick.h       # Joystick settings (filters, dead zone, etc.)
│   ├── ledMatrix.h      # LED Matrix Settings
│   └── ws2818b.pio.h    # Header automatically generated from PIO code
|
├── src
│   ├── button.c         # Function implementations for buttons
│   ├── joystick.c       # Joystick function implementations
│   ├── ledMatrix.c      # Function implementations for the LED matrix
│   └── main.c           # Main entry point for testing libraries
|
├── examples.h           # Example functions for joystick and LED
├── ws2818b.pio          # PIO code to generate WS2812/WS2818 signals
└── CMakeLists.txt       # Build configuration via CMake
```

### Joystick
The joystick library allows you to read analog values ​​and apply various filters:

#### Main Variables:
- `x_raw`, `y_raw`: Store raw ADC readings
- `x_filtered`, `y_filtered`: Filtered readings
- `x_buffer`, `y_buffer`: Buffers for moving average
- `prev_x`, `prev_y`: Last filtered value

#### Structure:
- `JoystickState`: Stores all the above fields to represent the joystick state

#### Main Functions:
- `Joystick_Init(JoystickState *js)`: Initializes ADC and sets initial state
- `Joystick_Read(JoystickState *js)`: Reads raw ADC channels
- `Joystick_ApplyFilters(JoystickState *js)`: Applies dead zone, moving average and low-pass
- `Joystick_ApplyDeadZone(int16_t value)`: Centers values ​​close to the midpoint
- `Joystick_MovingAverageFilter(...)`: Smooths noise with moving average
- `Joystick_LowPassFilter(...)`: Smooths out sudden changes
- `Joystick_MedianFilter(...)`: Removes outliers (not used directly in ApplyFilters)

### LED Matrix
The LED Matrix library handles a grid of 5x5 RGB LEDs:

#### Main Variables:
- `led_matrix[25]`: 25 LED buffer (store colors)
- `pio_ws` e `sm_ws`: PIO instance and state machine for WS2812
- `ws2812_config`: Contains the output pin

#### Primary Functions:
- `LedMatrix_Init(ws2812_config_t *ws2812_config)`: Initialize PIO/program and clear the array
- `LedMatrix_Fill(red, green, blue)`: Fills all LEDs with one color
- `LedMatrix_FillRectangle(x1, y1, x2, y2, red, green, blue)`: Colors a specific rectangle within the matrix
- `LedMatrix_SetPixel(x, y, red, green, blue)`: Sets the color of an individual LED
- `LedMatrix_Update()`: Sends the data buffer to the physical LEDs
- `LedMatrix_Clear()`: Resets all LEDs and updates

#### Internal (static) functions:
- `clamp(int val, int min, int max)`: Limits value in a range
- `build_color(...)`: Convert intensities to format `0xRGB`

### Buttons
The button library handles debounce, double click and long press:

#### Main Variables (global in button.c):
- `button_a_state`, `button_b_state`: Structures for each button
- `last_press_time`, `last_release_time`, `first_release_time`: Time control
- `event`: Enum representing the current event
- `click_counter`: Click counter

#### Structures:
- `ButtonState`: Stores information such as GPIO, click times and previous state
- `ButtonEvent` (enum): `IDLE`, `SINGLE_CLICK`, `DOUBLE_CLICK`, `LONG_PRESS`

#### Main Functions:
- `get_button_state(char* button_name)`: Returns the `ButtonState` for A or B
- `init_button(char* button_name)`: Configure GPIO and pull-up for a button
- `debounce_button(ButtonState* b)`: Performs reading with debounce
- `long_press_button(ButtonState* b, duration_ms)`: Detects if it has been pressed for a long time
- `double_click_button(ButtonState* b, interval_ms)`: Check double clicks
- `init_button_IT(uint gpio)`: Enables interrupts on the indicated GPIO
- `handle_button_IT(...)`: Edge treatment routine (descent/ascent)
- `process_button_state(void (*event_handler)(ButtonEvent))`: Validates the event type and calls the handler

### Examples
In `examples.h`, there are functions for demonstration:
- **Joystick tests**: como `Test_joystick_Complete`, que exibe leituras filtradas.
- **LED Matrix**: por exemplo, `Test_LedMatrix_Spiral` anima a matriz de LEDs percorrendo colunas.

### main.c
The main file initializes the joystick, LED matrix, and can call test functions or custom examples. You can edit this file to add your own logic or integrate the use of the libraries.

## Notes
- O arquivo `ws2818b.pio` contém código PIO assembly para LEDs WS281x.
- A biblioteca de botões gerencia interrupções internas e, via `process_button_state`, chama um handler que pode ser customizado.

## Installation
- First, you must clone this repository to your personal local workspace.

```sh
$ git clone https://github.com/gabrielmdrss/Virtus_Libraries.git
```

- Now, you will have this entire repository downloaded, depending on which specific library of the three you will use, you will only need to include the .h and .c files from the inc and src directories, respectively, to your personal project for that library individually. So, after cloning the repository, paste these two folders into your personal project and add these lines to your CMakeLists.txt, ensuring the addition of the .c files for the proper compilation of the files present in the library.
  
```c
add_executable(libraries 
        src/joystick.c
        src/ledMatrix.c
        src/button.c
        src/main.c
        )
```
- Depending on which library files you have in the inc and src folders, you will need to include the same .c files in the `add_executable` field.

&nbsp;&nbsp;&nbsp;&nbsp;

- After that, simply include the .h files to use the functions and resources present in the libraries. For example:
  
```c
#include "inc/joystick.h"
#include "inc/ledMatrix.h"
#include "inc/button.h" 
```

## Conclusion
By following the instructions and examples in this repository, you will be able to harness the Joystick, LED Matrix, and Button libraries for your Raspberry Pi Pico W projects.


For further development, remember to integrate the corresponding header and source files into your own project, update your CMake configuration accordingly, and explore the provided demos in main.c or examples.h. 


---

1. Dependências do Código com Outros Arquivos
O código apresentado possui as seguintes dependências externas:

joystick.h: O arquivo de cabeçalho joystick.h é necessário para definir a estrutura JoystickState, constantes, e possivelmente as assinaturas das funções implementadas no código. Sem ele, o código não compilará corretamente.
hardware/adc.h: Esta biblioteca é usada para interagir com o ADC (Conversor Analógico-Digital) do hardware. Ela fornece funções como adc_init, adc_gpio_init, adc_select_input e adc_read, que são essenciais para a leitura dos valores analógicos do joystick.
Constantes e macros: O código depende de várias constantes e macros, como ADC_PIN_X, ADC_PIN_Y, ADC_CHANEL_1, ADC_CHANEL_0, AVERAGE_VALUE, DEADZONE_THRESHOLD, MOVING_AVG_WINDOW e LOWPASS_ALPHA. Essas constantes precisam ser definidas em joystick.h ou em outro arquivo de configuração.
2. Capacidade de Usar a Biblioteca Isolada
A biblioteca pode ser usada de forma isolada, mas algumas condições precisam ser atendidas:

Definição de constantes: Todas as constantes mencionadas acima devem ser definidas corretamente no arquivo de cabeçalho ou em um arquivo de configuração separado.
Mock de funções de hardware: Para testes em ambientes sem acesso ao hardware real (como em simulações ou testes unitários), as funções relacionadas ao ADC (adc_init, adc_gpio_init, etc.) precisam ser mockadas ou substituídas por implementações simuladas.
Independência do hardware: O código depende diretamente do ADC do hardware. Para torná-lo mais portátil, seria necessário abstrair as interações com o ADC em uma camada de hardware separada, permitindo que a biblioteca seja usada em diferentes plataformas.
3. Possíveis Melhorias a Serem Feitas
A seguir estão algumas melhorias que podem ser implementadas para tornar o código mais robusto, reutilizável e fácil de manter:

Abstração do Hardware:

Criar uma camada de abstração para o ADC, encapsulando as funções de hardware (adc_init, adc_gpio_init, etc.) em uma interface genérica. Isso tornaria o código mais portátil e independente do hardware específico.
Validação de Parâmetros:

Adicionar verificações para garantir que o ponteiro JoystickState *js não seja nulo antes de usá-lo nas funções. Isso evitaria falhas em caso de chamadas incorretas.
Documentação:

Melhorar os comentários no código, explicando o propósito de cada função e os parâmetros esperados. Isso facilita a compreensão e manutenção do código.
Configuração Dinâmica:

Permitir que os pinos do ADC e outras configurações (como o tamanho da janela de média móvel e o valor do dead zone) sejam configurados dinamicamente em vez de depender de constantes fixas.
Testes Unitários:

Garantir que todas as funções tenham testes unitários abrangentes para validar seu comportamento em diferentes cenários, incluindo casos de borda.
Otimização do Filtro Mediano:

O algoritmo de ordenação usado no filtro mediano é ineficiente (O(n²)). Substituí-lo por um algoritmo mais eficiente, como Quickselect, pode melhorar o desempenho, especialmente para buffers maiores.
Gerenciamento de Memória:

Verificar se os buffers usados nos filtros (como no filtro de média móvel e mediano) são alocados e gerenciados corretamente. Caso contrário, pode haver problemas de memória em sistemas com recursos limitados.
Modularização:

Dividir o código em módulos menores, como um módulo para filtros (filters.c) e outro para a interface do joystick (joystick.c). Isso melhora a organização e facilita a reutilização de partes específicas do código.
Suporte a Mais Eixos:

Expandir a biblioteca para suportar joysticks com mais de dois eixos (por exemplo, Z ou rotações), tornando-a mais versátil.
Tratamento de Erros:

Implementar mecanismos de tratamento de erros para lidar com falhas no ADC ou leituras inválidas, garantindo maior robustez.
Conclusão
O código apresentado é funcional e bem estruturado para o propósito de leitura e processamento de dados de um joystick. No entanto, melhorias como abstração de hardware, otimização de algoritmos e modularização podem aumentar sua portabilidade, desempenho e facilidade de manutenção. Além disso, a inclusão de testes unitários e documentação mais detalhada tornará a biblioteca mais confiável e fácil de usar em diferentes projetos.