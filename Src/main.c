// Nikolai Baschinski
// Nucleo-F767ZI LD1 blinking 1Hz bare metal
// PG0 pin toggle 50 Hz

#include "stm32f767xx.h"
#include "main.h"

int main(void)
{
	init_PLL();
	init_GPIO();
	init_TIM();
	init_NVIC();
	for(;;);
}

void TIM2_IRQHandler(void)
{
	static unsigned int cntr_10ms = 0;

	if(cntr_10ms++ % 100 == 0) {
		GPIO_toggle_green_LED();
	}
	GPIO_toggle_Pin_PG0();

	TIM2->SR &= ~TIM_SR_UIF; // reset UIF bit in the status register of the timer
}

void init_PLL()
{
    RCC->CR |= RCC_CR_HSEBYP; // Clock control register, Enable bypass, the clock comes from the SWD
    RCC->CR |= RCC_CR_HSEON;  // Clock control register, Enable High Speed External Clock
    while(!(RCC->CR & RCC_CR_HSERDY)); // Clock control register, wait until the HSE is ready

    RCC->APB1ENR |= RCC_APB1ENR_PWREN; // Enable the peripheral clock

    FLASH->ACR |= FLASH_ACR_LATENCY_7WS; // Flash latency has to be set

    // SYSCLK prescaler
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // ABH prescaler
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // APB1 prescaler
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // APB2 prescaler

    // PLL Source Mux Prescaler
    #define PLL_M 4
    #define PLL_N 96
    #define PLL_P 0 // means prescaler 2
    RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (PLL_P << 16) | RCC_PLLCFGR_PLLSRC_HSE;

    // Enable the PLL
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY)); // check the ready flag of the PLL

    // System Clock Mux
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while((RCC->CFGR & RCC_CFGR_SWS) != (RCC_CFGR_SWS_PLL));
}

void init_GPIO()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // enable peripheral clock for Port B
	GPIOB->MODER |= (1<<0); // pin 0 port B is output
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; // enable peripheral clock for Port G
	GPIOG->MODER |= (1<<0); // pin 0 port G is output
}

void init_TIM()
{
	RCC->APB1ENR |= (1<<0);      // Enable clock for timer
	TIM2->PSC = (96-1);          // prescaler
	TIM2->ARR = 10000;           // Automatic reset
	TIM2->DIER |= TIM_DIER_UIE;  // Enable interrupt
	TIM2->CR1 |= (1<<0);         // Start timer
	while(!(TIM2->SR & (1<<0))); // Wait until timer is running
}

void init_NVIC()
{
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 5);
}

void GPIO_toggle_green_LED()
{
	GPIOB->ODR & 1 ? (GPIOB->BSRR |= (1<<16)) : (GPIOB->BSRR |= (1<<0));
}

void GPIO_toggle_Pin_PG0()
{
	GPIOG->ODR & 1 ? (GPIOG->BSRR |= (1<<16)) : (GPIOG->BSRR |= (1<<0));
}
