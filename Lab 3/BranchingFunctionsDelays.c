#include "TExaS.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

unsigned long Out; // output to PF2 (blue LED)
unsigned long In;  //input to PF4
void PortF_Init() { 
		volatile unsigned long delay;
		SYSCTL_RCGC2_R = 0x000000020;
		delay = SYSCTL_RCGC2_R;
		GPIO_PORTF_LOCK_R = 0x4C4F434B;
		GPIO_PORTF_CR_R = 0x1F;
		GPIO_PORTF_AMSEL_R = 0x00;
		GPIO_PORTF_PCTL_R = 0x00000000;
		GPIO_PORTF_DIR_R = 0x0E;
		GPIO_PORTF_AFSEL_R = 0x00;
		GPIO_PORTF_PUR_R = 0x11;
		GPIO_PORTF_DEN_R = 0x1F;
		GPIO_PORTF_DATA_R = 0x04;		
	}

// delay 100ms for the loop
void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}

int main(void){ 
	unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz

  // initialization goes here	
  PortF_Init();

	EnableInterrupts();           // enable interrupts for the grader

	while(1){
		// body goes here
		Delay100ms(1);
		In = GPIO_PORTF_DATA_R&0x10; // read pf4 into sw1
		In = In >> 2;

		if (In == 0x00) {
			// toggle blue LED
			GPIO_PORTF_DATA_R ^= 0x04;
		} else {
			GPIO_PORTF_DATA_R = 0x04;
		}
  }
}