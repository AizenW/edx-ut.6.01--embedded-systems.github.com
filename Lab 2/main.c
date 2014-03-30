
#include <stdio.h>  
#include "uart.h"   
#include "TExaS.h"   

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
unsigned long Calc_Area(unsigned long l, unsigned long w); 
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****
int main (void) {
  unsigned long length, width, area;
	TExaS_Init(UART_PIN_PA0,UART_PIN_PA1); // this initializes the TExaS grader lab 5
  UART_Init();    // initialize UART for printing
  printf("\nThis program calculates areas of rectangular rooms.\n");
  EnableInterrupts();  // the grader needs interrupts
  while(1) {
    printf("\nGive length: "); scanf("%ld", &length);  // Get input
    printf("\nGive width: ");  scanf("%ld", &width);   // Get input
    area = Calc_Area(length, width);
    printf("\nArea of the room = %ld\n",  area);
  }
}

unsigned long Calc_Area(unsigned long l, unsigned long w) {
  unsigned long result;


	result = 0;
	if((l >= 3 & w >= 3) & (l <= 20 & w <= 20)) {
		result = l * w;
	}
  
  return(result);
}