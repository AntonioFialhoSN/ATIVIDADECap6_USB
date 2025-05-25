#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "tusb.h"

// Definição dos GPIOs dos LEDs
#define LED_VERDE 11
#define LED_AZUL  12
#define LED_VERMELHO 13

// Inicializa os LEDs como saída
void inicializar_leds() {
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERDE, 0);

    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_put(LED_AZUL, 0);

    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_put(LED_VERMELHO, 0);
}

// Converte uma string para letras minúsculas
void para_minusculas(char *str) {
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

// Acende o LED correspondente à cor
void acender_led_correspondente(const char *comando) {
    if (strcmp(comando, "verde") == 0) {
        gpio_put(LED_VERDE, 1);
        sleep_ms(1000);
        gpio_put(LED_VERDE, 0);
    } else if (strcmp(comando, "azul") == 0) {
        gpio_put(LED_AZUL, 1);
        sleep_ms(1000);
        gpio_put(LED_AZUL, 0);
    } else if (strcmp(comando, "vermelho") == 0) {
        gpio_put(LED_VERMELHO, 1);
        sleep_ms(1000);
        gpio_put(LED_VERMELHO, 0);
    }
}

int main() {
    stdio_init_all();
    inicializar_leds();

    // Aguarda conexão USB
    while (!tud_cdc_connected()) {
        sleep_ms(100);
    }

    printf("Dispositivo CDC conectado via USB.\n");

    while (true) {
        tud_task(); // Necessário para o TinyUSB funcionar corretamente

        if (tud_cdc_available()) {
            char buffer[64];
            uint32_t count = tud_cdc_read(buffer, sizeof(buffer) - 1);
            buffer[count] = '\0'; // Finaliza a string

            // Remove quebras de linha
            char *newline = strchr(buffer, '\n');
            if (newline) *newline = '\0';
            newline = strchr(buffer, '\r');
            if (newline) *newline = '\0';

            // Converte o comando para minúsculas
            para_minusculas(buffer);

            printf("Recebido: %s\n", buffer);

            // ECO de volta ao PC
            tud_cdc_write_str(buffer);
            tud_cdc_write_str("\n");
            tud_cdc_write_flush();

            // Acende LED
            acender_led_correspondente(buffer);
        }
    }

    return 0;
}

