// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 5-0 have the 6-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"


// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long msec);
int main(void){ // Real Lab13 
	       
  Sound_Init(); // initialize SysTick timer and DAC
  Piano_Init();
	DisableInterrupts();
  
    // enable after all initialization are done
  while(1){                

			if((Piano_In()&0x01)==0x01){
			
			EnableInterrupts();
			//Sound_Tone(6378);
			Sound_Tone(2390);
			//Sound_Tone(1000 );
		}
		else if((Piano_In()&0x02)==0x02){
			
			
			EnableInterrupts();
			//Sound_Tone(7584);
			Sound_Tone(2129);
		}
		else if((Piano_In()&0x04)==0x04){
			
			EnableInterrupts();
			Sound_Tone(1896);
			//So
			}
		else if((Piano_In()&0x08)==0x08){
			
			EnableInterrupts();
			Sound_Tone(1596);
			//Sound_Tone(4780);
		}
		else{
			Sound_Off();
		DisableInterrupts();}
		delay(10);
			

  }
	
            
}


            

	

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec){ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


