/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430FR231x Demo - Toggle P1.0 using software
//
//  Description: Toggle P1.0 every 0.1s using software.
//  By default, FR231x select XT1 as FLL reference.
//  If XT1 is present, the PxSEL(XIN & XOUT) needs to configure.
//  If XT1 is absent, switch to select REFO as FLL reference automatically.
//  XT1 is considered to be absent in this example.
//  ACLK = default REFO ~32768Hz, MCLK = SMCLK = default DCODIV ~1MHz.
//
//           MSP430FR231x
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |           P1.0|-->LED
//
//   Darren Lu
//   Texas Instruments Inc.
//   July 2015
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1 
//******************************************************************************

#include <msp430.h>


// Puerto 2
#define RS BIT0     // P2.0
#define EN BIT6     // P2.6

// Puerto 1
#define D4 BIT4     // P1.4
#define D5 BIT5     // P1.5
#define D6 BIT6     // P1.6
#define D7 BIT7     // P1.7

void sendData(unsigned char data) {
    P2OUT |= RS;    // Modo datos
    sendNibble(data >> 4);  // Enviar los 4 bits más significativos
    sendNibble(data & 0x0F);  // Enviar los 4 bits menos significativos (corregido)
    __delay_cycles(4000); // Retardo para procesar los datos
}

void sendNibble(unsigned char nibble) {
    P1OUT &= ~(D4 | D5 | D6 | D7);  // Limpiar los bits de datos
    P1OUT |= ((nibble & 0x0F) << 4);  // Cargar el nibble en los bits correspondientes (P1.4 a P1.7)
    pulseEnable();  // Pulsar Enable para enviar datos
}

void pulseEnable() {
    P2OUT |= EN;             // Establecer Enable en 1
    __delay_cycles(1000);    // Retardo
    P2OUT &= ~EN;            // Establecer Enable en 0
    __delay_cycles(1000);    // Retardo
}

void sendCommand(unsigned char cmd) {
    P2OUT &= ~RS;   // Modo comando
    sendNibble(cmd >> 4);  // Enviar los 4 bits más significativos
    sendNibble(cmd);  // Enviar los 4 bits menos significativos
    __delay_cycles(4000); // Retardo para asegurarse de que el comando se procese
}

unsigned char cursorState = 0;  // 0 = OFF, 1 = ON
void toggleCursor() {
    
    cursorState ^= 1;  // Alternar entre 0 y 1 usando XOR

    if (cursorState) {
        sendCommand(0x0E);  // Display ON, Cursor ON
    } else {
        sendCommand(0x0C);  // Display ON, Cursor OFF
    }
}

unsigned char cursorBlinkState = 0;
void toggleBlinkCursor() {
    cursorBlinkState ^= 1;  // Alternar entre 0 y 1 usando XOR

    if (cursorBlinkState) {
        sendCommand(0x0F);  // Cursor ON con parpadeo
    } else {
        sendCommand(0x0E);  // Cursor ON sin parpadeo
    }
}

void lcdInit() {
    // Configurar pines como salida
    P1DIR |= D4 | D5 | D6 | D7;
    P2DIR |= RS | EN;

    // Limpiar salidas
    P1OUT &= ~(D4 | D5 | D6 | D7);
    P2OUT &= ~(RS | EN);
    __delay_cycles(50000);  // Retardo de inicio
    sendNibble(0x03);  // Inicialización del LCD
    __delay_cycles(5000);  // Retardo
    sendNibble(0x03);  // Repetir la inicialización
    __delay_cycles(200);  // Retardo
    sendNibble(0x03);  // Repetir la inicialización
    sendNibble(0x02);  // Establecer modo de 4 bits

    sendCommand(0x28);  // Configurar LCD: 4 bits, 2 líneas, 5x8
    sendCommand(0x0C);  // Encender display, apagar cursor
    sendCommand(0x06);  // Modo de escritura automática
    sendCommand(0x01);  // Limpiar la pantalla
    __delay_cycles(2000); // Esperar para limpiar la pantalla
}

void lcdSetCursor(unsigned char position) {
    sendCommand(0x80 | position);  // Establecer la dirección del cursor en la DDRAM
}

void lcdPrint(const char* str, unsigned char startPos) {
    lcdSetCursor(startPos);
    while (*str) {
        sendData(*str++);
        startPos++;
        if (startPos == 0x10) startPos = 0x40;  // Salto automático a segunda línea
    }
}

int main() {
    char key_unlocked;
    WDTCTL = WDTPW | WDTHOLD;  // Detener el watchdog
    PM5CTL0 &= ~LOCKLPM5;

    lcdInit();  // Inicializar el LCD
        
    switch (key_unlocked) 
    { 
        case 'C':
        toggleCursor();
        break;

        case '9':
        toggleBlinkCursor();
        break;

        case '0':
        sendCommand(0x01);
        __delay_cycles(2000);
        lcdPrint("STATIC", 0x00);
        break;

        case '1':
        sendCommand(0x01);
        __delay_cycles(2000);
        lcdPrint("TOGGLE", 0x00);
        lcdPrint("PERIOD=0.25", 0x40);
        break;

        case '2':
        sendCommand(0x01);
        __delay_cycles(2000);
        lcdPrint("UP", 0x00);
        lcdPrint("COUNTER", 0x03);
        break;

        case '3':
        sendCommand(0x01);
        __delay_cycles(2000);
        lcdPrint("IN", 0x00);
        lcdPrint("ANDOUT", 0x03);
        break;

        case '4':
        sendCommand(0x01);
        __delay_cycles(2000);
        lcdPrint("DOWN", 0x00);
        lcdPrint("COUNTER", 0x05);
        break;

        case '5':
        sendCommand(0x01);
        __delay_cycles(2000);
        lcdPrint("ROTATE 1 LEFT", 0x00);
        break;

        case '6':
        sendCommand(0x01);
        __delay_cycles(2000);
        lcdPrint("ROTATE 7 RIGHT", 0x00);
        break;

        case '7':
        sendCommand(0x01);
        __delay_cycles(2000);
        lcdPrint("FILL", 0x00);
        lcdPrint("LEFT", 0x05);
        break;
    }
}
