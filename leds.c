#include "leds.h"

// Definição do número total de pixels na matriz
#define NUM_PIXELS 25

// Definição do pino de saída para o controle da matriz de LEDs
#define OUT_PIN 7

// Inicialização do PIO (Programmable Input/Output)
PIO pio = pio0;
uint sm = 0; // Estado da máquina de estado (SM)


// Função para apagar todos os LEDs na matriz
void apaga_matriz(){
    // Loop para percorrer todos os pixels da matriz
    for (uint8_t i = 0; i < NUM_PIXELS; i++)
    {
        // Obtemos o índice de cada pixel usando a função 'obter_index'
        uint8_t pos = obter_index(i);         

        // Envia um valor zero para apagar o LED na posição correspondente
        // O valor 0x00000000 apaga todos os bits de cor (LED apagado)
        pio_sm_put_blocking(pio0, sm, 0x00000000);
    }
}

// Função para mudar o estado de um pino (ligar/desligar)
void muda_estado(uint8_t pin){
    // Inverte o estado do pino: se estava ligado, desliga; se estava desligado, liga
    gpio_put(pin, !gpio_get(pin));
}

// Função para desenhar um "frame" na matriz de LEDs
void desenha_frame(const uint32_t matriz[10][25], uint8_t frame){
    // Percorre todos os pixels da matriz
    for (uint8_t i = 0; i < NUM_PIXELS; i++)
    {
        // Obtemos o índice de cada pixel
        uint8_t pos = obter_index(i);         

        // Envia o valor do frame selecionado para acender os LEDs de acordo
        pio_sm_put_blocking(pio0, sm, matriz[frame][pos]);
    }
}

// Função para inicializar um LED (definindo o pino como saída)
void init_led(uint8_t pin){
    // Inicializa o pino
    gpio_init(pin);

    // Configura o pino como saída
    gpio_set_dir(pin, GPIO_OUT);
}

// Função para obter o índice de um pixel na matriz de LEDs (mapeamento de coordenadas)
uint8_t obter_index(uint8_t i) {
    uint8_t x = i % 5;  // Calcula a coluna a partir do índice
    uint8_t y = i / 5;  // Calcula a linha a partir do índice

    // A condição 'y % 2 == 0' verifica se a linha é par ou ímpar, invertendo a ordem da coluna em linhas ímpares
    return (y % 2 == 0) ? 24 - (y * 5 + x) : 24 - (y * 5 + (4 - x));
}

// Função para inicializar a matriz de LEDs
void init_led_matrix() {
    bool ok;

    // Configura o clock do sistema para 133 MHz
    ok = set_sys_clock_khz(133000, false);
    // Inicializa a entrada padrão para exibir informações
    stdio_init_all();

    // Configuração do PIO (Programmable I/O)
    uint offset = pio_add_program(pio, &pio_matrix_program);  // Adiciona o programa PIO
    uint sm = pio_claim_unused_sm(pio, true);  // Reivindica uma máquina de estado livre
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);  // Inicializa o programa PIO

    // Apaga a matriz de LEDs no início
    apaga_matriz();
}

// Definição de padrões de LEDs para os números de 0 a 9
// Cada número é representado por um array de 25 elementos (um para cada pixel na matriz)
const uint32_t numeros[10][25] = {
{
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000
},
{
0x00000000, 0x00000000, 0x190000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x190000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x190000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x190000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x190000, 0x00000000, 0x00000000
},
{
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000
},
{
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000
},
{
0x00000000, 0x190000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x190000, 0x00000000
},
{
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000
},
{
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000
},
{
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x190000, 0x00000000
},
{
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000
},
{
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x190000, 0x00000000, 
0x00000000, 0x190000, 0x190000, 0x190000, 0x00000000
}
};