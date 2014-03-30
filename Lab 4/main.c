#include "TExaS.h"

// Constant declarations to access port registers using 
// symbolic names instead of addresses
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
// 2. Declarations Section
//   Global Variables
unsigned long SW1; // input from PF4
unsigned long SW2; // input from PF0
unsigned long Out1; // output PF1
unsigned long Out2; // output PF2
unsigned long Out4; // output PF4

//   Function Prototypes
void PortF_Init(void);
void FlashSOS(void);
void delay(unsigned long halfsecs);
void EnableInterrupts(void);  // Enable interrupts

// 3. Subroutines Section
int main(void){
  TExaS_Init(SW_PIN_PF40, LED_PIN_PF321);  // activate grader and set system clock to 80 MHz
  PortF_Init(); // Init port PF4 PF2 PF0    
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    do{
      SW1 = GPIO_PORTF_DATA_R&0x10; // PF4 into SW1
			SW2 = GPIO_PORTF_DATA_R&0x01; // PF0 into SW2
    } while(SW1 == 0x10 || SW2 == 0x01);
		FlashSOS();
		delay(1); // delay between signals
  }
}

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R |= 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R &= 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R &= 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R &= ~0x11;          // 5.1) PF4,PF0 input, 
  GPIO_PORTF_DIR_R |= 0x0E;          // 5.2) PF3 PF2 PF1 output  
  GPIO_PORTF_AFSEL_R &= 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R |= 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R |= 0x1F;          // 7) enable digital pins PF4-PF0     
}

void FlashSOS(void){
  //S
  GPIO_PORTF_DATA_R |= 0x0A;  delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A; delay(1);
  GPIO_PORTF_DATA_R |= 0x0A;  delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A; delay(1);
  GPIO_PORTF_DATA_R |= 0x0A;  delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A; delay(1);
  //O
  GPIO_PORTF_DATA_R |= 0x0A; delay(4);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(4);
  GPIO_PORTF_DATA_R |= 0x0A; delay(4);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(4);
  GPIO_PORTF_DATA_R |= 0x0A; delay(4);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(4);
  //S
  GPIO_PORTF_DATA_R |= 0x0A; delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(1);
  GPIO_PORTF_DATA_R |= 0x0A; delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(1);
  GPIO_PORTF_DATA_R |= 0x0A; delay(1);
  GPIO_PORTF_DATA_R &= ~0x0A;delay(1);
  delay(8); // Delay for 5 secs in between flashes 
}

void delay(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 6666664;
    while(i > 0){
      i = i - 1;
    }
    time = time - 1;
  }
}
