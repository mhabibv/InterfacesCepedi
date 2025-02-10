#include <stdio.h>                 // Inclusão da biblioteca padrão de entrada e saída (para uso de printf, scanf, etc.)
#include "pico/stdlib.h"            // Biblioteca principal do Raspberry Pi Pico (para controle de GPIO, temporizadores, etc.)
#include "hardware/i2c.h"           // Biblioteca para controle de comunicação I2C
#include "ssd1306.h"                // Biblioteca para controle do display OLED SSD1306
#include "font.h"                   // Biblioteca de fontes para o display OLED
#include "leds.h"                   // Biblioteca para controle de LEDs (assumido que é uma interface para LEDs conectados ao sistema)

#define I2C_PORT i2c1              // Define o porto I2C como i2c1
#define I2C_SDA 14                 // Define o pino 14 como o pino SDA (dados do I2C)
#define I2C_SCL 15                 // Define o pino 15 como o pino SCL (clock do I2C)
#define ENDERECO 0x3C              // Define o endereço do display OLED como 0x3C (endereço padrão para SSD1306)

#define GREEN 11                   // Define o pino 11 como o pino do LED verde
#define BLUE 12                    // Define o pino 12 como o pino do LED azul

#define BUTTON_A 5                 // Define o pino 5 como o pino do botão A
#define BUTTON_B 6                 // Define o pino 6 como o pino do botão B

ssd1306_t ssd;                    // Instância do display SSD1306

void init_button(uint8_t pin);    // Função para inicializar o botão
void button_callback(uint gpio, uint32_t events);   // Função para a callback do botão (ao ser pressionado)

absolute_time_t button_debounce;  // Variável para armazenar o tempo de debounce do botão

int main() {
  stdio_init_all();               // Inicializa a entrada e saída padrão (necessário para printf e scanf)

  i2c_init(I2C_PORT, 400*1000);   // Inicializa o I2C com uma taxa de 400kHz
  init_i2c_pins(I2C_SDA, I2C_SCL); // Inicializa os pinos SDA e SCL para comunicação I2C

  // Inicializa o display SSD1306 com largura (WIDTH) e altura (HEIGHT) definidas no código da biblioteca
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT);
  ssd1306_config(&ssd);           // Configura o display (como inverter cores, etc.)
  ssd1306_send_data(&ssd);        // Envia dados para o display

  // Preenche o display com cor preta (apagado)
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  init_led_matrix();              // Inicializa a matriz de LEDs
  init_led(GREEN);                // Inicializa o LED verde
  init_led(BLUE);                 // Inicializa o LED azul
  init_button(BUTTON_A);          // Inicializa o botão A
  init_button(BUTTON_B);          // Inicializa o botão B

  // Configura interrupções para os botões A e B
  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
  gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);

  // Configura o tempo de debounce para os botões
  button_debounce = delayed_by_ms(get_absolute_time(), 250);

  bool cor = true;                // Flag para alternar entre cores no display
  bool matriz = false;            // Flag para controlar se a matriz de LEDs está desenhada ou não
  char caracter;                  // Variável para armazenar o caractere digitado

  while (1) {
    cor = !cor;                   // Alterna entre verdadeiro e falso para mudar a cor

    // Espera por um caractere digitado
    scanf("%c", &caracter);
    printf("%c\n", caracter);     // Exibe o caractere digitado no console

    // Verifica se o caractere é um número (de '0' a '9')
    if (caracter >= '0' && caracter <= '9') {
      // Chama a função para desenhar o número na matriz de LEDs
      desenha_frame(numeros, caracter - 48);  
      matriz = true;              // Marca que a matriz de LEDs deve ser desenhada
    } else if (matriz) {
      // Se a matriz já foi desenhada e o caractere não for um número, apaga a matriz
      apaga_matriz();
      matriz = false;             // Marca que a matriz de LEDs foi apagada
    }

    // Alterna a cor de fundo do display OLED
    ssd1306_fill(&ssd, !cor);
    // Desenha um retângulo no display (pode ser usado como indicador de estado)
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
    // Desenha uma string no display
    ssd1306_draw_string(&ssd, "Sinal ", 8, 10);
    // Desenha o caractere no display
    ssd1306_draw_char(&ssd, caracter, 90, 10);  
    ssd1306_send_data(&ssd);        // Envia os dados para o display
    sleep_ms(1000);                 // Pausa de 1 segundo
  }
}

// Função para inicializar o botão (configura o pino como entrada com pull-up)
void init_button(uint8_t pin) {
  gpio_init(pin);                 // Inicializa o pino
  gpio_set_dir(pin, GPIO_IN);     // Define a direção do pino como entrada
  gpio_pull_up(pin);              // Ativa o resistor de pull-up interno
}

// Função callback chamada quando o botão é pressionado (interrupção)
void button_callback(uint gpio, uint32_t events) {
  // Verifica se o tempo de debounce passou
  if (time_reached(button_debounce)) {
    if (gpio == BUTTON_A) {
      // Alterna o estado do LED verde quando o botão A é pressionado
      muda_estado(GREEN);
      
      // Se o LED azul estiver aceso, apaga ele
      if (gpio_get(BLUE)) muda_estado(BLUE);

      // Exibe no display o estado do LED verde
      ssd1306_draw_string(&ssd, gpio_get(GREEN) ? "Verde Aceso " : "Verde Desligado ", 8, 30);
    } else if (gpio == BUTTON_B) {
      // Alterna o estado do LED azul quando o botão B é pressionado
      muda_estado(BLUE);
      
      // Se o LED verde estiver aceso, apaga ele
      if (gpio_get(GREEN)) muda_estado(GREEN);
      
      // Exibe no display o estado do LED azul
      ssd1306_draw_string(&ssd, gpio_get(BLUE) ? "Azul Aceso" : "Azul Desligado ", 8, 30);
    }
    
    ssd1306_send_data(&ssd);       // Envia os dados atualizados para o display
    button_debounce = delayed_by_ms(get_absolute_time(), 200);  // Atualiza o tempo de debounce
  }
}
