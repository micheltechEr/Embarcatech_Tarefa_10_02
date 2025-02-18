#include <stdio.h>
#include "pico/stdlib.h"    
#include "pico/time.h"      
#include "hardware/irq.h"   
#include "hardware/pwm.h"   
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/adc.h"     

// Inicio área definitiva e declarativa

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_ADDR 0x3C

#define BUTTON_A 5       
#define BUTTON_B 6       
#define RED_LED 13       
#define GREEN_LED 11     
#define BLUE_LED 12      
#define JOYSTICK_Y_PIN 26  
#define JOYSTICK_X_PIN 27 
#define JOYSTICK_BTN 22  

const uint16_t PWM_WRAP = 25000;
const float PWM_DIV = 100.0;     

static volatile uint a = 1;
static volatile uint32_t last_time = 0; 
volatile bool green_led_state = false;   
volatile bool pwm_enabled = true;         

uint16_t t = 0;
uint16_t R1 = 1;
uint16_t R2 = 60;
uint16_t R3 = 1;
uint16_t R4 = 124;
uint16_t RECT_Y_MIN = 4;
uint16_t RECT_Y_MAX = 52;
uint16_t RECT_X_MIN = 4;
uint16_t RECT_X_MAX = 116;

uint8_t border_width = 0;  
bool is_increasing = true; 

void init_io();
static void gpio_interrupt_handler(uint gpio, uint32_t events);
void setup_pwm();
void ssd1306_draw_thick_rectangle(ssd1306_t *ssd, uint8_t top, uint8_t left, 
                                  uint8_t width, uint8_t height, bool value, uint8_t thickness);

//Fim área definitiva/declarativa

//Função responsável por iniciar leds,adc,botões...etc
void init_io()
{
    gpio_init(RED_LED);
    gpio_init(BLUE_LED);
    gpio_init(GREEN_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_set_dir(BLUE_LED, GPIO_OUT);
    gpio_set_dir(GREEN_LED, GPIO_OUT);
    gpio_put(RED_LED, false);
    gpio_put(BLUE_LED, false);
    gpio_put(GREEN_LED, false);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_init(JOYSTICK_BTN);
    gpio_set_dir(JOYSTICK_BTN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BTN);

    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);  
}

//Função para aplicar tratamento das interrupções dos botões

static void gpio_interrupt_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_time > 200000) // 200 ms de debounce
    {
        last_time = current_time; // Atualiza o tempo do último evento

        if (gpio == JOYSTICK_BTN)
        { 
            green_led_state = !green_led_state;  
            gpio_put(GREEN_LED, green_led_state);
            printf("Botão do Joystick pressionado: LED_VERDE = %d\n", green_led_state);

            // Aumenta ou reduz a espessura das bordas
            if (is_increasing)
            {
                border_width++;
                if (border_width >= 5) 
                {
                    is_increasing = false;
                }
            }
            else
            {
                border_width--;
                if (border_width == 0) 
                {
                    is_increasing = true;
                }
            }
            // Atualiza as dimensões do retângulo
            R1 = border_width;
            R2 = 64 - border_width;
            R3 = border_width;
            R4 = 128 - border_width;
            // Atualiza os limites para o caractere
            RECT_Y_MIN = R1 + 3;
            RECT_Y_MAX = 52 - border_width;
            RECT_X_MIN = R3 + 3;
            RECT_X_MAX = 116 - border_width;
        }

        if (gpio == BUTTON_A)
        { 
            pwm_enabled = !pwm_enabled;   
            printf("Botão A pressionado: PWM = %d\n", pwm_enabled);
        }
    }
}

//Função que configura o módulo PWM para os leds vermelho e azul

void setup_pwm()
{
    gpio_set_function(BLUE_LED, GPIO_FUNC_PWM); 
    gpio_set_function(RED_LED, GPIO_FUNC_PWM);   

    uint slice_num_azul = pwm_gpio_to_slice_num(BLUE_LED);
    uint slice_num_vermelho = pwm_gpio_to_slice_num(RED_LED);

    pwm_set_clkdiv(slice_num_azul, PWM_DIV);      
    pwm_set_clkdiv(slice_num_vermelho, PWM_DIV);

    pwm_set_wrap(slice_num_azul, PWM_WRAP);         
    pwm_set_wrap(slice_num_vermelho, PWM_WRAP);

    pwm_set_gpio_level(BLUE_LED, 0);                
    pwm_set_gpio_level(RED_LED, 0);

    pwm_set_enabled(slice_num_azul, true);         
    pwm_set_enabled(slice_num_vermelho, true);
}

//Função responsável por desenhar o retângulo

void ssd1306_draw_thick_rectangle(ssd1306_t *ssd, uint8_t top, uint8_t left, 
                                  uint8_t width, uint8_t height, bool value, uint8_t thickness)
{
    for (uint8_t t = 0; t < thickness; ++t)
    {
        // Desenha o retângulo aumentando a espessura a cada iteração
        ssd1306_rect(ssd, top - t, left - t, width + 2 * t, height + 2 * t, value, false);
    } 
}

//Função main

int main()
{
    // Inicializa comunicação USB CDC para monitor serial
    stdio_init_all(); 
    
    // Inicializa entradas/saídas, PWM e configura interrupções
    init_io();
    setup_pwm();
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_interrupt_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_interrupt_handler);
    gpio_set_irq_enabled_with_callback(JOYSTICK_BTN, GPIO_IRQ_EDGE_FALL, true, &gpio_interrupt_handler);
  
    // Inicializa a comunicação I2C (400 kHz)
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    // Inicializa e configura o display OLED
    ssd1306_t display;
    ssd1306_init(&display, WIDTH, HEIGHT, false, I2C_ADDR, I2C_PORT);
    ssd1306_config(&display);
    ssd1306_send_data(&display);
    ssd1306_fill(&display, false);
    ssd1306_send_data(&display);
   
    // Variáveis para controle do joystick e da animação do display
    bool cor = true;
    char letra = '\0';
    uint32_t last_print_time = 0; 
    uint16_t previous_y = 40;
    uint16_t previous_x = 40;
    uint16_t new_y = previous_y;
    uint16_t new_x = previous_x;
    bool border_control = green_led_state;

    // Variáveis para controle do PWM
    int pwm_x = 0;
    int pwm_y = 0;

    while (true)
    {
        cor = !cor;
        // Leitura dos valores do ADC para os eixos Y e X
        adc_select_input(0); // Eixo Y (pino 26)
        uint16_t adc_y_value = 4095 - adc_read();
        adc_select_input(1); // Eixo X (pino 27)
        uint16_t adc_x_value = adc_read();

        // Calcula as novas coordenadas para desenhar o caractere
        int new_y = adc_y_value / 64;  // Ajuste de escala conforme necessário
        int new_x = adc_x_value / 32;  // Ajuste de escala conforme necessário

        // Aplica os limites para que o caractere não ultrapasse o retângulo
        new_y = (new_y < RECT_Y_MIN) ? RECT_Y_MIN : (new_y > RECT_Y_MAX) ? RECT_Y_MAX : new_y;
        new_x = (new_x < RECT_X_MIN) ? RECT_X_MIN : (new_x > RECT_X_MAX) ? RECT_X_MAX : new_x;

        // Apaga o caractere anterior e desenha o novo caractere
        ssd1306_draw_char(&display, ' ', previous_x, previous_y);
        ssd1306_draw_char(&display, '~', new_x, new_y);

        previous_y = new_y;
        previous_x = new_x;

        ssd1306_send_data(&display);

        // Se houver alteração no estado do LED verde, redesenha o retângulo
        if (border_control != green_led_state)
        {
            ssd1306_fill(&display, false); // Limpa o display
            ssd1306_draw_thick_rectangle(&display, R1, R3, R4 - R3, R2 - R1, true, border_width);
            ssd1306_send_data(&display);
            border_control = green_led_state;
        }

        // Controle do PWM baseado nos valores lidos do ADC
        pwm_x = abs((int16_t)(adc_x_value - 2090));
        pwm_y = abs((int16_t)(adc_y_value - 2090));

        if (pwm_enabled == true)
        {
            if (pwm_x > 20 && pwm_y > 20)
            {
                pwm_set_gpio_level(RED_LED, ((pwm_x * 25000) / 2090));
                pwm_set_gpio_level(BLUE_LED, ((pwm_y * 25000) / 2090));
            }
            else
            {
                pwm_set_gpio_level(RED_LED, 0);
                pwm_set_gpio_level(BLUE_LED, 0);
            }
        }
        else
        {
            pwm_set_gpio_level(RED_LED, 0);
            pwm_set_gpio_level(BLUE_LED, 0);
        }
    }
}
