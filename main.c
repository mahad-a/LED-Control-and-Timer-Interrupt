#include "msp.h"

#define PORT1_BIT1 (P1IN & (1<<0 ) )
#define DELAY_VALUE 100000
#define RGBMASK 0x07

int redPin = 1;
int button1Pressed, button2Pressed = 0;
int i = 10000; // debouncer delay
volatile int ticks = 0;
volatile int behaviour = 0;
volatile static int d = 1;
int t = 2500000;
void cycleColours();
int main(void){

	//P1SEL0 and P1SEL1 must be 0 (GPIO)
	P1SEL0 &=(uint8_t)(~((1<<4) | (1<<1) | (1<<0)));
	P1SEL1 &=(uint8_t)(~((1<<4) | (1<<1) | (1<<0)));
	P2SEL0 &=(uint8_t)(~((1<<2) | (1<<1) | (1<<0)));
	P2SEL1 &=(uint8_t)(~((1<<2) | (1<<1) | (1<<0)));

	//Disable the Watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	//Configure the switches (P1.1 and P1.4) as inputs.
	P1DIR &=(uint8_t)(~((1<<4) | (1<<1)));

	// Pull-Up Internal Resistors
	P1REN |=(uint8_t)((1<<4) | (1<<1));
	P1OUT |=(uint8_t)((1<<4) | (1<<1));

	//disable interrupts
	P1->IE &= ~((1<<0));
	P1->IE &= ~((1<<0) | (1<<1) | (1<<2));
	P1->IE &= ~((1<<1) | (1<<4));


	//configure interupts
	P1IES |= (uint8_t)(1 << 1);
	P1IFG &= (uint8_t)~(1 << 1);
	P1IE |= (uint8_t)(1 << 1);   

	P1IES |= (uint8_t)(1 << 4);
	P1IFG &= (uint8_t)~(1 << 4);
	P1IE |= (uint8_t)(1 << 4);   

	//Timer configuration
	//At 100Hz, need to count 100 cycles for 1 second
	TA0CTL = TIMER_A_CTL_TASSEL_1;
	TA0CTL &= (uint16_t)(~((1<<5) | (1<<4))); //stop timer
	TA0CTL &= (uint16_t)(~(1<<0)); //clear interrupt flag
	TIMER_A0->CCR[0] = (uint16_t)(32768); //100-1
	TA0CTL |= (uint16_t)((1<<1)); //interrupt enable

	//Configure NVIC
	NVIC_SetPriority(TA0_N_IRQn,2);
	NVIC_ClearPendingIRQ(TA0_N_IRQn);
	NVIC_EnableIRQ(TA0_N_IRQn);

	//Configure NVIC
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC_ClearPendingIRQ(PORT1_IRQn);
	NVIC_EnableIRQ(PORT1_IRQn);

	TA0CTL |= TIMER_A_CTL_MC_1; //up mode (count to CCR0)


	//Configure NVIC
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC_ClearPendingIRQ(PORT1_IRQn);
	NVIC_EnableIRQ(PORT1_IRQn);

	// Globally enable interrupts in CPU
	__ASM("CPSIE I");


	//Configure the LEDs (P1.0 and P2.0, P2.1, P2.2) as outputs
	P1DIR |=(uint8_t)(1<<0);
	P2DIR |=(uint8_t)(((1<<2) | (1<<1) | (1<<0)));

	// Initialize LEDs states (all turned off).
	P1OUT &=~(uint8_t)(1<<0);
	P2OUT &=~(uint8_t)(((1<<2) | (1<<1) | (1<<0)));

	
	// Create an infinite loop
	while (1) {
		// change to regluar mode
		if(behaviour == 0) {
			d = 1; 
		} else if (behaviour == 1) {
			d = 2;
		} else {
			P1->OUT &= (uint8_t)(~(1<<0));
			P2->OUT &=(uint8_t)~(((1<<2) | (1<<1) | (1<<0)));
			d = 0;
			ticks = 0;
		}
		
		if (redPin) {
			if (ticks > d) {
				cycleColours();
				ticks = 0;
			}
		} else {
			if (ticks > d) {
				P1->OUT ^= (uint8_t)(1 << 0);
				ticks = 0;
			}
		}

	}

	return 0;
}

void PORT1_IRQHandler(void) {
	int i = 10;
	while(i > 0){
		i--;
	}
	if ((P1IFG & (uint8_t)(1<<1)) != 0){
		P1IFG &= ~(uint8_t)(1<<1);
		
		if (redPin ==1){
			redPin = 0;
		}else{
			redPin = 1;
		}
	}
	
	// handle ISR
	 if ((P1IFG & (uint8_t)(1<<4)) != 0){
		P1IFG &= ~(uint8_t)(1<<4);
		behaviour += (behaviour + 1) % 3;
		
	
	}
}

void TA0_N_IRQHandler(void) {
	// clear interupt flag
	TA0CTL &= (uint16_t)(~(1<<0));
	ticks++;
}

void cycleColours(void) {
	uint8_t led_state = ((P2->OUT & RGBMASK) >> 0);
	led_state++;
	P2->OUT &= ~(RGBMASK);
	P2->OUT |= (led_state & RGBMASK) << 0;
}