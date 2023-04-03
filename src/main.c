/* Copyright 2022, Laboratorio de Microprocesadores
 * Facultad de Ciencias Exactas y Tecnología
 * Universidad Nacional de Tucuman
 * http://www.microprocesadores.unt.edu.ar/
 * Copyright 2022, Esteban Volentini <evolentini@herrera.unt.edu.ar>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief Simple sample of use LPC HAL gpio functions
 **
 ** \addtogroup samples Sample projects
 ** \brief Sample projects to use as a starting point
 ** @{ */

/* === Headers files inclusions =============================================================== */
#include "bsp.h"
#include <stdbool.h>
#include "pantalla.h"
#include <stddef.h>
#include "reloj.h"

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

typedef enum {
    HORA_SIN_AJUSTAR,
    MOSTRANDO_HORA,
    AJUSTANDO_MINUTOS_ACTUAL,
    AJUSTANDO_HORAS_ACTUAL,
    AJUSTANDO_MINUTOS_ALARMA,
    AJUSTANDO_HORAS_ALARMA,

} modo_t;

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */
static board_t board;

static modo_t modo;

static reloj_t reloj;

static const uint8_t LIMITE_MINUTOS [] = {6, 0};
static const uint8_t LIMITE_HORAS [] = {2, 4};

/* === Private function implementation ========================================================= */

void CambiarModo(modo_t valor){
    modo = valor;
    switch (modo)
    {
    case HORA_SIN_AJUSTAR:
        MostrarDigitosParpadeando(board->display, 0, 3, 250);
        break;
    case MOSTRANDO_HORA:
        MostrarDigitosParpadeando(board->display, 0, 0, 0);
        break;
    case AJUSTANDO_MINUTOS_ACTUAL:
        MostrarDigitosParpadeando(board->display, 2, 3, 250);
        break;
    case AJUSTANDO_HORAS_ACTUAL:
        MostrarDigitosParpadeando(board->display, 0, 1, 250);
        break;
    case AJUSTANDO_MINUTOS_ALARMA:
        MostrarDigitosParpadeando(board->display, 2, 3, 250);
        break;
    case AJUSTANDO_HORAS_ALARMA:
        MostrarDigitosParpadeando(board->display, 0, 1, 250); 
        break;
    
    default:
        break;
    }
}

void SonarAlarma(reloj_t reloj){
}

void IncrementarBCD(uint8_t numero [2], const uint8_t limite[2]){
    numero[1]++;
    if (numero[1] > 9){
        numero[1] = 0;
        numero[0]++;
    }
    if ((numero[0] == limite[0]) && (numero[1] == limite[1])){
        numero[0] = 0;
        numero[1] = 0;
    }
}

void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]){
    numero[1]--;
    if (numero[1] > 9){
        numero[1] = 0;
        numero[0]--;
    }
    if ((numero[0] > limite[0]) && (numero[1] > limite[1])){
        numero[1] = 0;
        numero[0] = 0;
    }
}

/* === Public function implementation ========================================================= */

int main(void) { 
    uint8_t entrada[4];
    board = BoardCreate();
    reloj = CrearReloj(10, SonarAlarma);
    
    SisTick_Init(1000);
    CambiarModo(HORA_SIN_AJUSTAR);

   
    while (true) {
        //RefrescarPantalla(board->display);
        //genero interrupcion cada X segundos y evito refrescar la pantalla aqui

        if (ActivaEntradaDigital(board->aceptar)){
            // EscribirPantallaBCD(board->display, (uint8_t[]){1, 2, 3, 4}, 4);
            // MostrarCambiosPuntos(board->display, 1, 2);
            if(modo == AJUSTANDO_MINUTOS_ACTUAL){
                CambiarModo(AJUSTANDO_HORAS_ACTUAL);
            } else if (modo == AJUSTANDO_HORAS_ACTUAL){
                ConfigurarReloj(reloj, entrada, sizeof(entrada));
                CambiarModo(MOSTRANDO_HORA);
            }
        }

        if (ActivaEntradaDigital(board->cancelar)){
            //EscribirPantallaBCD(board->display, NULL, 0);
            //TraerHoraReloj(reloj, entrada, sizeof(entrada));consulto la hora
            if (TraerHoraReloj(reloj, entrada, sizeof(entrada))){
                CambiarModo(MOSTRANDO_HORA);
            } else {
                CambiarModo(HORA_SIN_AJUSTAR);
            };
        }

        if (ActivaEntradaDigital(board->establecer_tiempo)){
            CambiarModo(AJUSTANDO_MINUTOS_ACTUAL);
            TraerHoraReloj(reloj, entrada, sizeof(entrada));
            EscribirPantallaBCD(board->display, entrada, sizeof(entrada));
        }

        if (ActivaEntradaDigital(board->establecer_alarma)){    
            CambiarModo(AJUSTANDO_MINUTOS_ALARMA);
            ConfigurarAlarmaReloj(reloj, entrada, sizeof(entrada));
            EscribirPantallaBCD(board->display, entrada, sizeof(entrada));
            MostrarCambiosPuntos(board->display, 0, 3);
        }

        if (ActivaEntradaDigital(board->decrementar)){
            if((modo == AJUSTANDO_MINUTOS_ACTUAL) || (modo == AJUSTANDO_MINUTOS_ALARMA)){
                DecrementarBCD(&entrada[2], LIMITE_MINUTOS);//entro con uno que no es el primero
                EscribirPantallaBCD(board->display, entrada, sizeof(entrada));
            }else if ((modo == AJUSTANDO_HORAS_ACTUAL) || (modo == AJUSTANDO_HORAS_ALARMA)){
                DecrementarBCD(entrada, LIMITE_HORAS);//entro con el primero
                EscribirPantallaBCD(board->display, entrada, sizeof(entrada));
            }
        }

        if (ActivaEntradaDigital(board->incrementar)){   
            if((modo == AJUSTANDO_MINUTOS_ACTUAL) || (modo == AJUSTANDO_MINUTOS_ALARMA)){
                IncrementarBCD(&entrada[2], LIMITE_MINUTOS);//entro con uno que no es el primero
                EscribirPantallaBCD(board->display, entrada, sizeof(entrada));
            }else if ((modo == AJUSTANDO_HORAS_ACTUAL) || (modo == AJUSTANDO_HORAS_ALARMA)){
                IncrementarBCD(entrada, LIMITE_HORAS);//entro con el primero
                EscribirPantallaBCD(board->display, entrada, sizeof(entrada));
            }       
        }

        for (int index = 0; index < 20; index++) {
            for (int delay = 0; delay < 25000; delay++) {
                __asm("NOP");
            }
        }
    }
}

void SysTick_Handler(void){
    static uint16_t contador =0;
    uint8_t hora[4];

    RefrescarPantalla(board->display);
    NuevoTickReloj(reloj);

    contador = (contador +1) % 1000;
    if (modo<= MOSTRANDO_HORA){
        TraerHoraReloj(reloj, hora, sizeof(hora));
        EscribirPantallaBCD(board->display, hora, sizeof(hora));
        if (contador > 500){
         MostrarCambiosPuntos(board -> display, 1, 1);
        }
    }
}
/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
