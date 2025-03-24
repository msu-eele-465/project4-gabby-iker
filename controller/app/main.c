#include <msp430.h>
#include "C:\Users\gabri\Documents\Spring2025\EELE465\project04\project4-gabby-iker\controller\src\master_i2c.h"
#include "C:\Users\gabri\Documents\Spring2025\EELE465\project04\project4-gabby-iker\controller\src\master_i2c.h"
#include "C:\Users\gabri\Documents\Spring2025\EELE465\project04\project4-gabby-iker\controller\src\rgb_led.h"

int main(void)
{
    master_i2c_init();
    init_keypad();
    return 0;
}
