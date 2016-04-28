// 0.Documentation Section 
// SoftSound.c
// Runs on LM4F120 or TM4C123
// Use the SysTick timer to request interrupts at 500Hz.
// Jonathan Valvano
// November 3, 2013


#include "PLL.h"
#include "tm4c123gh6pm.h"
// Constant declarations to access port registers using 
// symbolic names instead of addresses
unsigned long ADC0_InSeq3(void);
void ADC0_InitSWTriggerSeq3_Ch1(void);
void SysTick_Wait10ms(unsigned long delay);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

void PortF_Init(void);
void SysFun(void);
void SysLoad(double period);
unsigned long Led;
unsigned int ADCvalue;
	
// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
  PLL_Init();   
	
PortF_Init();
	ADC0_InitSWTriggerSeq3_Ch1();
	SysFun();
	
	 
	
  EnableInterrupts();
	
	/*Initialize ports , ADC and timers*/
  while(1){   
		/*Your code goes here*/
		 ADCvalue=ADC0_InSeq3();
	//GPIO_PORTF_DATA_R =0x00; //clear all bits
	if (ADCvalue == 0 && ADCvalue <=1024){
		 Led = GPIO_PORTF_DATA_R;   // read previous
    Led = Led^0x08;            // toggle green led
		 GPIO_PORTF_DATA_R = Led;   // output
		//GPIO_PORTF_DATA_R =0x08; //green
		SysLoad(8000000); //100ms
		
  }
else if (ADCvalue > 1024 && ADCvalue <= 3072){
    Led = GPIO_PORTF_DATA_R;   // read previous
    Led = Led^0x0A;            // toggle yellow led 
	 GPIO_PORTF_DATA_R = Led;   // output
	//GPIO_PORTF_DATA_R =0x0A; //yellow led
		SysLoad(16000000); //200ms
}
else if(ADCvalue >3072) {
	
		 Led = GPIO_PORTF_DATA_R;   // read previous
    Led = Led^0x02;            // toggle red LED
	 GPIO_PORTF_DATA_R = Led;   // output
	//GPIO_PORTF_DATA_R =0x02;  //red led
		SysLoad(4000000);} //50ms
}
	}
// **************Port_Init*********************
// Initialize SysTick periodic interrupts
// Input: none
// Output: none

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}
void SysLoad(double period){

	
	NVIC_ST_RELOAD_R = period-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
	
}

void SysFun(void){
	
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;
          
} 

//ADC initialisation on PE2

void ADC0_InitSWTriggerSeq3_Ch1(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;   // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;         //    allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0x04;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog function on PE2
  SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0 
  delay = SYSCTL_RCGC2_R;         
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger  (continuously ON)
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1; // 11) channel Ain1 (PE2)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0 (FLAG AFT COMPLETION)
  ADC0_ACTSS_R |= 0x0008;         // 13) enable sample sequencer 3 
}






//------------ADC0_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC0_InSeq3(void){  
	unsigned long result;
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}




