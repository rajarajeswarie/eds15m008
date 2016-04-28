#include "PLL.h"
#include "tm4c123gh6pm.h"

void DisableInterrupts(void);// Disable interrupts
void EnableInterrupts(void);// Enable interrupts
void WaitForInterrupt(void);// low power mode
void PortE_Init(void);// start sound output
void PortB_Init(void);
void PortD_Init(void);
void SysFun(void);
void SysInit(void);//initialize SysTick timer
void SysLoad(unsigned long period);//Load reload value
unsigned long RSW,LSW;
int main(void)
{
	PLL_Init();
	SysFun();
	PortE_Init();
	PortB_Init();
        PortD_Init();
	SysInit();
	SysLoad(1600000);

	while(1)
	{
		GPIO_PORTE_DATA_R = 0x01;
GPIO_PORTB_DATA_R = 0x01; 
		SysLoad(1600000);
    RSW = GPIO_PORTE_DATA_R&0x01;     // read PE0 into SW1
    LSW = GPIO_PORTE_DATA_R&0x02;     // read PE1 into SW2
	if(!RSW && !LSW){    //both not pressed
GPIO_PORTD_DATA_R = 0x01;
GPIO_PORTB_DATA_R = 0x01;    
} else
if(!RSW && LSW){    //left sw pressed
GPIO_PORTD_DATA_R = 0x01;  //right forward
GPIO_PORTB_DATA_R = 0x02;   //left backward 
} else
if(RSW && !LSW){  //right sw pressed
GPIO_PORTD_DATA_R = 0x02;  // right backward
GPIO_PORTB_DATA_R = 0x01;    // left forward
} else
{
GPIO_PORTD_DATA_R = 0x02;  // right backward
GPIO_PORTB_DATA_R = 0x02;    // left backward
}
	
	}
}

void PortE_Init(void)
{
	unsigned long volatile delay;
	/*PORT Initialization*/
	SYSCTL_RCGC2_R |= 0x10; // Port a clock
	delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
	GPIO_PORTE_DIR_R |= 0x03; // PE0.PE1 output
	GPIO_PORTE_AFSEL_R &= ~0x03; // not alternative
	GPIO_PORTE_AMSEL_R &= ~0x03; // no analog
	GPIO_PORTE_PCTL_R &= ~0x000000FF; // bits for PE1,PE0
	GPIO_PORTE_DEN_R |= 0x03; // enable PE0,PE1
}

void PortB_Init(void)
{
	unsigned long volatile delay;
	/*PORT Initialization*/
	SYSCTL_RCGC2_R |= 0x02; // Port a clock
	delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
	GPIO_PORTB_DIR_R |= 0x03; // PB0.PB1 output
	GPIO_PORTB_AFSEL_R &= ~0x03; // not alternative
	GPIO_PORTB_AMSEL_R &= ~0x03; // no analog
	GPIO_PORTB_PCTL_R &= ~0x000000FF; // bits for PB0,PB1
	GPIO_PORTB_DEN_R |= 0x03; // enable PB0,PB1
}
void PortD_Init(void)
{
	unsigned long volatile delay;
	/*PORT Initialization*/
	SYSCTL_RCGC2_R |= 0x08; // Port a clock
	delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
	GPIO_PORTD_DIR_R |=  ~0x03; // PD0.PD1 input
	GPIO_PORTD_AFSEL_R &= ~0x03; // not alternative
	GPIO_PORTD_AMSEL_R &= ~0x03; // no analog
	GPIO_PORTD_PCTL_R &= ~0x000000FF; // bits for PD0,PD1
	GPIO_PORTD_DEN_R |= 0x03; // enable PD0,PD1
}


void SysLoad(unsigned long period)
{
	NVIC_ST_RELOAD_R = period -1;
	NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears  
	while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag 
		}
}
void SysInit(void)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;// any write to current clears it
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF;// priority 0
	NVIC_ST_CTRL_R = 0x00000005;// enable with core clock and interrupts
}
void SysFun(void)
{
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup  
	NVIC_ST_CTRL_R = 0x00000005; 
} 

