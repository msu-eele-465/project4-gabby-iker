/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2016, Texas Instruments Incorporated
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
//  MSP430FR235x Demo - Toggle P1.0 using software
//
//  Description: Toggle P1.0 every 0.1s using software.
//  By default, FR235x select XT1 as FLL reference.
//  If XT1 is present, the PxSEL(XIN & XOUT) needs to configure.
//  If XT1 is absent, switch to select REFO as FLL reference automatically.
//  XT1 is considered to be absent in this example.
//  ACLK = default REFO ~32768Hz, MCLK = SMCLK = default DCODIV ~1MHz.
//
//           MSP430FR2355
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |           P1.0|-->LED
//
//   Cash Hao
//   Texas Instruments Inc.
//   November 2016
//   Built with IAR Embedded Workbench v6.50.0 & Code Composer Studio v6.2.0
//******************************************************************************
//#include <msp430.h>

//int main(void)
//{
//    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    //P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    //P1DIR |= BIT0;                          // Set P1.0 to output direction

//    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

//    while(1)
  //  {
        //P1OUT ^= BIT0;                      // Toggle P1.0 using exclusive-OR
    //    __delay_cycles(100000);             // Delay for 100000*(1/MCLK)=0.1s
    //}
//}
#include <msp430.h>

#define RS BIT2  // P1.2 -> RS (Registro de selección)
#define EN BIT3  // P1.3 -> Enable
#define D4 BIT4  // P1.4 -> Data bit 4
#define D5 BIT5  // P1.5 -> Data bit 5
#define D6 BIT6  // P1.6 -> Data bit 6
#define D7 BIT7  // P1.7 -> Data bit 7
#define COL 4
#define ROW 4

void pulseEnable() {
    P1OUT |= EN;             // Establecer Enable en 1
    __delay_cycles(1000);    // Retardo
    P1OUT &= ~EN;            // Establecer Enable en 0
    __delay_cycles(1000);    // Retardo
}

void sendNibble(unsigned char nibble) {
    P1OUT &= ~(D4 | D5 | D6 | D7);  // Limpiar los bits de datos
    P1OUT |= ((nibble & 0x0F) << 4);  // Cargar el nibble en los bits correspondientes (P1.4 a P1.7)
    pulseEnable();  // Pulsar Enable para enviar datos
}

void sendCommand(unsigned char cmd) {
    P1OUT &= ~RS;   // Modo comando
    sendNibble(cmd >> 4);  // Enviar los 4 bits más significativos
    sendNibble(cmd);  // Enviar los 4 bits menos significativos
    __delay_cycles(4000); // Retardo para asegurarse de que el comando se procese
}

void debounce() {
    volatile unsigned int i;
    for (i = 20000; i > 0; i--) {}
}

void InitializePorts_KeyPad() 
{
    // Set rows as inputs (with pull-up)
    P6DIR &= ~0x0F;   // P6.0, P6.1, P6.2, P6.3 as inputs
    P6REN |= 0x0F;    // Activate pull-up
    P6OUT |= 0x0F;    // Activate pull-up in rows
    
    // Set columns as outputs
    P5DIR |= BIT0 | BIT1 | BIT2 | BIT3; // Set P5.0, P5.1, P5.2, P5.3 as outputs
    P5OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3);  // Set low the pins P5.0, P5.1, P5.2, P5.3
}

char keypad[ROW][COL] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char System_Unlocking(void)
{
    int row, col;

    // Go through 4 columns
    for (col = 0; col < 4; col++) {
        // Put column down (active)
        P5OUT &= ~(1 << col);   // P5.0, P5.1, P5.2, P5.3
        __delay_cycles(1000);  // Little stop to stabilize the signal

        // Go through 4 rows
        for (row = 0; row < 4; row++) {
            if ((P6IN & (1 << row)) == 0) {  // We detect that the row is low
                debounce();  // Wait to filter the bouncing
                if ((P6IN & (1 << row)) == 0) {  // Confirm that the key is pressed
                    char key = keypad[row][col];
                    // Wait until the key is released to avoid multiple readings 
                    while ((P6IN & (1 << row)) == 0);
                    // Deactivate the column before returning
                    P5OUT |= (1 << col);                    
                    return key;
                }
            }
        }
        // Put the column high (deactivated)
        P5OUT |= (1 << col);
    }

    return 0; // No key pressed
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



void sendData(unsigned char data) {
    P1OUT |= RS;    // Modo datos
    sendNibble(data >> 4);  // Enviar los 4 bits más significativos
    sendNibble(data & 0x0F);  // Enviar los 4 bits menos significativos (corregido)
    __delay_cycles(4000); // Retardo para procesar los datos
}

void lcdInit() {
    P1DIR |= RS | EN | D4 | D5 | D6 | D7; // Configurar los pines como salida
    P1OUT &= ~(RS | EN | D4 | D5 | D6 | D7); // Limpiar los pines

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

int main() {
    char key_unlocked;
    int unlocked;
    WDTCTL = WDTPW | WDTHOLD;  // Detener el watchdog
    PM5CTL0 &= ~LOCKLPM5;

    //----------------------------------------WRITE THINGS ON THE LCD SCREEN------------------------------------------------------------
    lcdInit();  // Inicializar el LCD
          
    //lcdSetCursor(0x00);  // Mover el cursor a la posición 0x00
    //sendData('A');       // Mostrar la letra 'A'

    //lcdSetCursor(0x01);  // Mover el cursor a la posición 0x01
    //sendData('B');       // Mostrar la letra 'b'


    //-----------------------------------------------LCD + KEYPAD-------------------------------------------------------------------------
    InitializePorts_KeyPad();  //Initialize KeyPad
    
    unlocked=1;
    while (unlocked==1) 
    {
        key_unlocked=System_Unlocking();
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
                lcdSetCursor(0x00);  // Mover el cursor a la posición 0x00
                sendData('S');       // Mostrar la letra 'S'
                lcdSetCursor(0x01);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'T'
                lcdSetCursor(0x02);  // Mover el cursor a la posición 0x00
                sendData('A');       // Mostrar la letra 'A'
                lcdSetCursor(0x03);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'T'
                lcdSetCursor(0x04);  // Mover el cursor a la posición 0x00
                sendData('I');       // Mostrar la letra 'T'
                lcdSetCursor(0x05);  // Mover el cursor a la posición 0x00
                sendData('C');       // Mostrar la letra 'T'
            break;

            case '1':
                sendCommand(0x01);
                 __delay_cycles(2000);
                lcdSetCursor(0x00);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'S'
                lcdSetCursor(0x01);  // Mover el cursor a la posición 0x00
                sendData('O');       // Mostrar la letra 'T'
                lcdSetCursor(0x02);  // Mover el cursor a la posición 0x00
                sendData('G');       // Mostrar la letra 'A'
                lcdSetCursor(0x03);  // Mover el cursor a la posición 0x00
                sendData('G');       // Mostrar la letra 'T'
                lcdSetCursor(0x04);  // Mover el cursor a la posición 0x00
                sendData('L');       // Mostrar la letra 'T'
                lcdSetCursor(0x05);  // Mover el cursor a la posición 0x00
                sendData('E');       // Mostrar la letra 'T'
                //SHOW PERIOD
                lcdSetCursor(0x40);  // Mover el cursor a la posición 0x00
                sendData('P');       // Mostrar la letra 'S'
                lcdSetCursor(0x41);  // Mover el cursor a la posición 0x00
                sendData('E');       // Mostrar la letra 'T'
                lcdSetCursor(0x42);  // Mover el cursor a la posición 0x00
                sendData('R');       // Mostrar la letra 'T'
                lcdSetCursor(0x43);  // Mover el cursor a la posición 0x00
                sendData('I');       // Mostrar la letra 'T'
                lcdSetCursor(0x44);  // Mover el cursor a la posición 0x00
                sendData('O');       // Mostrar la letra 'T'
                lcdSetCursor(0x45);  // Mover el cursor a la posición 0x00
                sendData('D');       // Mostrar la letra 'T'
               
                lcdSetCursor(0x47);  // Mover el cursor a la posición 0x00
                sendData('=');       // Mostrar la letra 'T'
                
                lcdSetCursor(0x49);  // Mover el cursor a la posición 0x00
                sendData('0');       // Mostrar la letra 'T' 
                lcdSetCursor(0x4A);  // Mover el cursor a la posición 0x00
                sendData('.');       // Mostrar la letra 'T'
                lcdSetCursor(0x4B);  // Mover el cursor a la posición 0x00
                sendData('2');       // Mostrar la letra 'T' 
                lcdSetCursor(0x4C);  // Mover el cursor a la posición 0x00
                sendData('5');       // Mostrar la letra 'T' 
            break;        

            case '2':
                sendCommand(0x01);
                 __delay_cycles(2000);
                lcdSetCursor(0x00);  // Mover el cursor a la posición 0x00
                sendData('U');       // Mostrar la letra 'S'
                lcdSetCursor(0x01);  // Mover el cursor a la posición 0x00
                sendData('P');       // Mostrar la letra 'T'
                
                lcdSetCursor(0x03);  // Mover el cursor a la posición 0x00
                sendData('C');       // Mostrar la letra 'T'
                lcdSetCursor(0x04);  // Mover el cursor a la posición 0x00
                sendData('O');       // Mostrar la letra 'T'
                lcdSetCursor(0x05);  // Mover el cursor a la posición 0x00
                sendData('U');       // Mostrar la letra 'T'
                lcdSetCursor(0x06);  // Mover el cursor a la posición 0x00
                sendData('N');       // Mostrar la letra 'S'
                lcdSetCursor(0x07);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'T'
                lcdSetCursor(0x08);  // Mover el cursor a la posición 0x00
                sendData('E');       // Mostrar la letra 'A'
                lcdSetCursor(0x09);  // Mover el cursor a la posición 0x00
                sendData('R');       // Mostrar la letra 'T'  
            break;

            case '3':
                sendCommand(0x01);
                 __delay_cycles(2000);
                lcdSetCursor(0x00);  // Mover el cursor a la posición 0x00
                sendData('I');       // Mostrar la letra 'S'
                lcdSetCursor(0x01);  // Mover el cursor a la posición 0x00
                sendData('N');       // Mostrar la letra 'T'
                
                lcdSetCursor(0x03);  // Mover el cursor a la posición 0x00
                sendData('A');       // Mostrar la letra 'T'
                lcdSetCursor(0x04);  // Mover el cursor a la posición 0x00
                sendData('N');       // Mostrar la letra 'T'
                lcdSetCursor(0x05);  // Mover el cursor a la posición 0x00
                sendData('D');       // Mostrar la letra 'T'
                
                lcdSetCursor(0x07);  // Mover el cursor a la posición 0x00
                sendData('O');       // Mostrar la letra 'T'
                lcdSetCursor(0x08);  // Mover el cursor a la posición 0x00
                sendData('U');       // Mostrar la letra 'A'
                lcdSetCursor(0x09);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'T'  
            break;

            case '4':
                sendCommand(0x01);
                 __delay_cycles(2000);
                lcdSetCursor(0x00);  // Mover el cursor a la posición 0x00
                sendData('D');       // Mostrar la letra 'S'
                lcdSetCursor(0x01);  // Mover el cursor a la posición 0x00
                sendData('O');       // Mostrar la letra 'T'
                lcdSetCursor(0x02);  // Mover el cursor a la posición 0x00
                sendData('W');       // Mostrar la letra 'T'
                lcdSetCursor(0x03);  // Mover el cursor a la posición 0x00
                sendData('N');       // Mostrar la letra 'T'
                
                lcdSetCursor(0x05);  // Mover el cursor a la posición 0x00
                sendData('C');       // Mostrar la letra 'T'
                lcdSetCursor(0x06);  // Mover el cursor a la posición 0x00
                sendData('O');       // Mostrar la letra 'T'
                lcdSetCursor(0x07);  // Mover el cursor a la posición 0x00
                sendData('U');       // Mostrar la letra 'T'
                lcdSetCursor(0x08);  // Mover el cursor a la posición 0x00
                sendData('N');       // Mostrar la letra 'A'
                lcdSetCursor(0x09);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'T' 
                lcdSetCursor(0x0A);  // Mover el cursor a la posición 0x00
                sendData('E');       // Mostrar la letra 'T'
                lcdSetCursor(0x0B);  // Mover el cursor a la posición 0x00
                sendData('R');       // Mostrar la letra 'T' 
            break;

            case '5':
                sendCommand(0x01);
                 __delay_cycles(2000);
                lcdSetCursor(0x00);  // Mover el cursor a la posición 0x00
                sendData('R');       // Mostrar la letra 'S'
                lcdSetCursor(0x01);  // Mover el cursor a la posición 0x00
                sendData('O');       // Mostrar la letra 'T'
                lcdSetCursor(0x02);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'T'
                lcdSetCursor(0x03);  // Mover el cursor a la posición 0x00
                sendData('A');       // Mostrar la letra 'T'
                lcdSetCursor(0x04);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'T'
                lcdSetCursor(0x05);  // Mover el cursor a la posición 0x00
                sendData('E');       // Mostrar la letra 'T'
               
                lcdSetCursor(0x07);  // Mover el cursor a la posición 0x00
                sendData('1');       // Mostrar la letra 'T'
                
                lcdSetCursor(0x09);  // Mover el cursor a la posición 0x00
                sendData('L');       // Mostrar la letra 'T' 
                lcdSetCursor(0x0A);  // Mover el cursor a la posición 0x00
                sendData('E');       // Mostrar la letra 'T'
                lcdSetCursor(0x0B);  // Mover el cursor a la posición 0x00
                sendData('F');       // Mostrar la letra 'T' 
                lcdSetCursor(0x0C);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'T' 
            break;

            case '6':
                sendCommand(0x01);
                 __delay_cycles(2000);
                lcdSetCursor(0x00);  // Mover el cursor a la posición 0x00
                sendData('R');       // Mostrar la letra 'S'
                lcdSetCursor(0x01);  // Mover el cursor a la posición 0x00
                sendData('O');       // Mostrar la letra 'T'
                lcdSetCursor(0x02);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'T'
                lcdSetCursor(0x03);  // Mover el cursor a la posición 0x00
                sendData('A');       // Mostrar la letra 'T'
                lcdSetCursor(0x04);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'T'
                lcdSetCursor(0x05);  // Mover el cursor a la posición 0x00
                sendData('E');       // Mostrar la letra 'T'
               
                lcdSetCursor(0x07);  // Mover el cursor a la posición 0x00
                sendData('7');       // Mostrar la letra 'T'
                
                lcdSetCursor(0x09);  // Mover el cursor a la posición 0x00
                sendData('R');       // Mostrar la letra 'T' 
                lcdSetCursor(0x0A);  // Mover el cursor a la posición 0x00
                sendData('I');       // Mostrar la letra 'T'
                lcdSetCursor(0x0B);  // Mover el cursor a la posición 0x00
                sendData('G');       // Mostrar la letra 'T' 
                lcdSetCursor(0x0C);  // Mover el cursor a la posición 0x00
                sendData('H');       // Mostrar la letra 'T' 
                lcdSetCursor(0x0D);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'T' 
            break;

            case '7':
                sendCommand(0x01);
                 __delay_cycles(2000);
                lcdSetCursor(0x00);  // Mover el cursor a la posición 0x00
                sendData('F');       // Mostrar la letra 'S'
                lcdSetCursor(0x01);  // Mover el cursor a la posición 0x00
                sendData('I');       // Mostrar la letra 'T'
                lcdSetCursor(0x02);  // Mover el cursor a la posición 0x00
                sendData('L');       // Mostrar la letra 'T'
                lcdSetCursor(0x03);  // Mover el cursor a la posición 0x00
                sendData('L');       // Mostrar la letra 'T'
                
                lcdSetCursor(0x05);  // Mover el cursor a la posición 0x00
                sendData('L');       // Mostrar la letra 'T'
                lcdSetCursor(0x06);  // Mover el cursor a la posición 0x00
                sendData('E');       // Mostrar la letra 'T'
                lcdSetCursor(0x07);  // Mover el cursor a la posición 0x00
                sendData('F');       // Mostrar la letra 'T'
                lcdSetCursor(0x08);  // Mover el cursor a la posición 0x00
                sendData('T');       // Mostrar la letra 'A'
            break;
        }
        // SHOW LAST PRESSED KEY - RIGHT BOTTOM CORNER 
    }
    while (1);  // Bucle infinito para mantener el programa corriendo
}


