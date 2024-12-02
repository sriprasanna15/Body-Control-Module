
#include <LPC21XX.H>
#include "can_driver.h"
#include "uart0_header.h"
#define M1 (1 << 12)
#define M2 (1 << 13)

void motor_clock(void) {
    IOSET0 = M1;
    IOCLR0 = M2;
}

void motor_anticlock(void) {
    IOCLR0 = M1;
    IOSET0 = M2;
}

void motor_off(void) {
    IOCLR0 = M1;
    IOCLR0 = M2;
}

void wiper(void) {
int i;
	for(i=0;i<3;i++)
	{
    motor_clock();
    delay_ms(500);
    motor_anticlock();
    delay_ms(500);
	}
}

int main() {
    CAN2_MSG m1;
    int wiper_on = 0;

    IODIR0 |= M1 | M2;
    can2_init();
    uart0_config(115200);
    uart0_tx_str("nodeB TESTING CAN\r\n");

    while (1) {
        can2_rx(&m1);
        if (m1.rtr == 0) {
            if (m1.id == 0x03) {
                if (wiper_on) {
                    motor_off();
                    wiper_on = 0;
                } else {
                    wiper_on = 1;
                }
            }
        } else {
            uart0_tx_str("nodeB: REMOTE-FRAME is received\r\n");
        }

        if (wiper_on) {
            wiper();
        }
    }
}
