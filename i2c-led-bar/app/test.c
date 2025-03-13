#include <msp430.h>

#define SLAVE_ADDR  0x68   // Slave I2C Address

void I2C_Slave_Init(void);

volatile unsigned char receivedData = 0;

int i;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  // Stop Watchdog Timer

    __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, enable interrupts
}

void I2C_Slave_Init(void)
{
    // Configure P1.2 and P1.3 as I2C
    P1SEL0 |= BIT2 | BIT3;
    P1SEL1 &= ~(BIT2 | BIT3);

    // Configure USCI_B0 as I2C Slave
    UCB0CTLW0 = UCSWRST;           // Enable reset state
    UCB0CTLW0 |= UCMODE_3 | UCSYNC; // I2C Slave, Synchronous mode
    UCB0I2COA0 = SLAVE_ADDR | UCOAEN; // Set own address and enable
    UCB0CTLW0 &= ~UCSWRST;         // Clear reset to start operation
    UCB0IE |= UCRXIE;              // Enable Receive Interrupt
    init_led_bar();
}

// I2C ISR
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
    switch (__even_in_range(UCB0IV, USCI_I2C_UCTXIFG0))
    {
        case USCI_I2C_UCRXIFG0:     // Receive Interrupt
            receivedData = UCB0RXBUF; // Read received data
            break;
        default: 
            break;
    }
}

