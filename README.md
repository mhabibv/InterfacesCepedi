# Projeto de Controle de Display OLED com Botões e LEDs

Este projeto implementa um sistema utilizando um display OLED SSD1306, botões e LEDs, controlados por um Raspberry Pi Pico. O programa permite interagir com o sistema através de dois botões, que alternam os estados de dois LEDs e exibem informações no display OLED.

## Requisitos

- Raspberry Pi Pico (ou outra placa com suporte ao microcontrolador RP2040)
- Display OLED SSD1306 (conectado via I2C)
- Dois botões (A e B)
- Dois LEDs (verde e azul)
- Fios de conexão e protoboard para montagem do circuito

## Conexões

### I2C (para o display OLED):
- **SDA**: Pino 14 (conecte ao pino SDA do display)
- **SCL**: Pino 15 (conecte ao pino SCL do display)
- **VCC**: 3.3V (ou 5V, dependendo do seu display)
- **GND**: GND (terra)

### Botões:
- **Botão A**: Pino 5
- **Botão B**: Pino 6
  - Conectar cada botão entre o pino do Raspberry Pi Pico e o GND com resistor de pull-up interno.

### LEDs:
- **LED Verde**: Pino 11
- **LED Azul**: Pino 12
  - Conectar os LEDs aos pinos correspondentes com resistores limitadores de corrente.

## Como compilar e carregar o programa

1. **Configuração do ambiente**:
   - Certifique-se de que você tenha o [SDK do Raspberry Pi Pico](https://github.com/raspberrypi/pico-sdk) configurado no seu ambiente de desenvolvimento.
   - Instale as dependências necessárias para compilar o código, como o CMake.

2. **Compilação**:
   - Navegue até o diretório do projeto e crie um diretório de build:
     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ```
   
3. **Carregar no Raspberry Pi Pico**:
   - Após a compilação, o arquivo gerado será `program.uf2`. Copie este arquivo para o Raspberry Pi Pico em modo de bootloader (pressionando o botão BOOTSEL e conectando o Pico ao computador).
   - O programa será carregado automaticamente e o Raspberry Pi Pico será reiniciado.

## Como usar o programa

### Interação com o display e LEDs:

1. **Botão A**:
   - Ao pressionar o **Botão A**, o LED verde alterna entre aceso e apagado.
   - O estado atual do LED verde (aceso ou apagado) será exibido no display OLED.

2. **Botão B**:
   - Ao pressionar o **Botão B**, o LED azul alterna entre aceso e apagado.
   - O estado atual do LED azul (aceso ou apagado) será exibido no display OLED.

3. **Entrada de caracteres**:
   - O programa espera por uma entrada de caractere do usuário através do console (usando `scanf`).
   - Se o caractere for um número (`0-9`), ele será exibido na matriz de LEDs.
   - Caso contrário, a matriz de LEDs será apagada.

4. **Display OLED**:
   - O display OLED exibirá o texto "Sinal" e o caractere inserido no console.

5. **Debounce**:
   - O sistema possui um mecanismo de **debounce** de 250ms para evitar múltiplas leituras de um único pressionamento de botão devido ao ruído elétrico.

## Funcionalidades

- **Controle de LEDs**: Permite alternar entre os estados aceso/desligado dos LEDs verde e azul.
- **Exibição de caracteres**: Exibe números na matriz de LEDs e caracteres no display OLED.
- **Debounce de botões**: Evita múltiplas leituras para um único pressionamento de botão.

## Link do vídeo que mostra a execução
- https://www.youtube.com/watch?v=B2YUkdsH8EI

