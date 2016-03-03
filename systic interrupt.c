#include "PLL.h"
#include "tm4c123gh6pm.h"

void DisableInterrupts(void);// Disable interrupts
void EnableInterrupts(void);// Enable interrupts
void WaitForInterrupt(void);// low power mode
void PortA_Init(void);// start sound output
void SysInit(void);//initialize SysTick timer
void SysLoad(unsigned long period);
//Load reload value

int main(void)
{
 PLL_Init();
 PortA_Init();
 SysInit();
 SysLoad(40000);
 
 while(1){}
}

void PortA_Init(void)
{
 unsigned long volatile delay;
 /*PORT Initialization*/
 SYSCTL_RCGC2_R |= 0x01; // Port A clock
 delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
 GPIO_PORTA_DIR_R |= 0x40; // PA6 output
 GPIO_PORTA_AFSEL_R &= ~0x40; // not alternative
 GPIO_PORTA_AMSEL_R &= ~0x40; // no analog
 GPIO_PORTA_PCTL_R &= ~0x0F000000; // bits for PA6
 GPIO_PORTA_DEN_R |= 0x40; // enable PA6
}

void SysLoad(unsigned long period)
{
 NVIC_ST_RELOAD_R = period -1;
}

void SysInit(void)
{
 NVIC_ST_CTRL_R = 0;
 NVIC_ST_CURRENT_R = 0;// any write to current clears it
 NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF;// priority 0
 NVIC_ST_CTRL_R = 0x00000007;// enable with core clock and interrupts 
}

void SysTick_Handler(void)
{
 GPIO_PORTA_DATA_R^=0x40;
}