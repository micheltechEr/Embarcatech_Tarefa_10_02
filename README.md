# Controle de LEDs RGB, Joystick e Display OLED para Prática de ADC e PWM - Raspberry Pi Pico

## Descrição do Projeto
Este projeto foi desenvolvido para praticar o uso de **ADC (Conversor Analógico-Digital)**, **PWM (Modulação por Largura de Pulso)**, **interrupções** e **comunicação I2C** na placa BitDogLab.Ele combina um **joystick**, dois **botões**, três **LEDs RGB** (vermelho, verde e azul) e um **display OLED SSD1306** para criar uma aplicação interativa.

O sistema utiliza:
- O **joystick** para controlar a posição de um caractere no display OLED e ajustar a intensidade dos LEDs vermelho e azul via PWM.
- **Botões** para alternar funcionalidades, como habilitar/desabilitar o PWM e alterar a espessura de um retângulo no display.
- Um **LED verde** que acende ou apaga ao pressionar o botão do joystick.
- Um **display OLED** para exibir animações e reagir às entradas do usuário.

---

## Componentes Utilizados
- **LED Vermelho (Pino 13):** Controlado via PWM com base na posição do joystick.
- **LED Verde (Pino 11):** Acende ou apaga ao pressionar o botão do joystick.
- **LED Azul (Pino 12):** Controlado via PWM com base na posição do joystick.
- **Joystick:**
  - Eixo X (Pino 27): Lê valores analógicos para controle horizontal.
  - Eixo Y (Pino 26): Lê valores analógicos para controle vertical.
  - Botão (Pino 22): Alterna o estado do LED verde e ajusta a espessura do retângulo no display.
- **Botão A (Pino 5):** Habilita ou desabilita o PWM nos LEDs vermelho e azul.
- **Botão B (Pino 6):** Reservado para futuras funcionalidades.
- **Display OLED SSD1306:** Exibe animações e reage às entradas do usuário.
- **Raspberry Pi Pico:** Placa utilizada para executar o código.
- **Resistores:** Para limitar a corrente nos LEDs (recomenda-se 220Ω ou 330Ω).

---

## Funcionalidades do Projeto
1. **Controle de LEDs via PWM:**
   - O LED vermelho é controlado pelo eixo X do joystick.
   - O LED azul é controlado pelo eixo Y do joystick.
   - O PWM pode ser habilitado ou desabilitado ao pressionar o **Botão A**.

2. **Interação com o Joystick:**
   - Os eixos X e Y do joystick ajustam a posição de um caractere no display OLED.
   - O botão do joystick alterna o estado do LED verde e ajusta a espessura de um retângulo no display.

3. **Exibição no Display OLED:**
   - Um retângulo com bordas ajustáveis é desenhado no display.
   - Um caractere (`~`) move-se dinamicamente com base na posição do joystick.

4. **Tratamento de Interrupções:**
   - As interrupções são usadas para detectar eventos dos botões e implementar debounce.

---

## Guia de Funcionamento na Sua Máquina

Para executar este projeto localmente, siga as instruções abaixo:

### 1. **Clone o repositório:**
   - Abra o **VS Code** e clone este repositório para sua máquina.

### 2. **Importe o projeto:**
   - Certifique-se de ter as extensões do **Raspberry Pi Pico** instaladas no VS Code.
   - Importe o projeto para poder compilá-lo e executá-lo na placa RP2040.

### 3. **Conecte a placa:**
   - Conecte a placa ao computador via USB e coloque-a no modo **BOOTSEL**.

### 4. **Compile o código:**
   - Compile o código diretamente no VS Code.

### 5. **Simulação no Wokwi:**
   - Para simular o projeto, abra o arquivo `diagram.json` disponível nos arquivos do projeto e execute-o no [Wokwi](https://wokwi.com).

### 6. **Execute na placa:**
   - Após a compilação e com a placa no modo **BOOTSEL**, clique em **Executar** ou **Run** para carregar o programa na placa.

---

## Funcionamento do Sistema

1. **Estado Inicial:**
   - Todos os LEDs estão apagados.
   - O display OLED exibe um retângulo com bordas finas.
   - O caractere (`~`) está posicionado no centro do retângulo.

2. **Pressionamento do Botão A:**
   - Alterna o estado do PWM (habilitado/desabilitado) para os LEDs vermelho e azul.

3. **Movimentação do Joystick:**
   - O eixo X controla a intensidade do LED vermelho.
   - O eixo Y controla a intensidade do LED azul.
   - A posição do joystick move o caractere (`~`) no display OLED dentro dos limites do retângulo.

4. **Pressionamento do Botão do Joystick:**
   - Alterna o estado do LED verde (ligado/desligado).
   - Aumenta ou reduz a espessura das bordas do retângulo no display OLED.

---

## Código Fonte

O código fonte está organizado da seguinte forma:
- **Função `init_io`:** Configura os pinos GPIO, ADC e botões.
- **Função `gpio_interrupt_handler`:** Trata interrupções dos botões e implementa debounce.
- **Função `setup_pwm`:** Configura o módulo PWM para os LEDs vermelho e azul.
- **Função `ssd1306_draw_thick_rectangle`:** Desenha um retângulo com bordas ajustáveis no display OLED.
- **Loop Principal (`main`):** Lê os valores do joystick, atualiza o display OLED e controla os LEDs via PWM.

---

## Observações Finais

Este projeto foi desenvolvido com foco em boas práticas de programação, organização e documentação. Ele é ideal para estudantes que desejam praticar o uso de ADC, PWM, interrupções e comunicação I2C no Raspberry Pi Pico.

Caso tenha dúvidas ou sugestões, sinta-se à vontade para abrir uma **issue** ou entrar em contato.

---

### Créditos
- **Autor:** Ângelo Miguel Ribeiro Cerqueira Lima
- **Data:** 17/02/2024

---
