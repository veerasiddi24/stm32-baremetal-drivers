#include "stm32l476xx.h"
#include "stm32l476xx_gpio_Driver.h"

void ADC_Init(void)
{
    /* 1. Enable ADC clock */
    ADC_PCLK_EN();

    /* 2. Exit deep power-down */
    ADC1->CR &= ~(1 << 29);

    /* 3. Enable voltage regulator */
    ADC1->CR |= (1 << 28);

    /* 4. Wait long enough */
    for(volatile int i = 0; i < 100000; i++);

    /* 5. Ensure ADC disabled */
    if(ADC1->CR & (1 << 0))
    {
        ADC1->CR |= (1 << 1);
        while(ADC1->CR & (1 << 0));
    }

    /* 6. Start calibration */
    ADC1->CR |= (1 << 31);

    /* Debug check */
    while(ADC1->CR & (1 << 31));

    /* 7. Clear ADRDY */
    ADC1->ISR |= (1 << 0);

    /* 8. Enable ADC */
    ADC1->CR |= (1 << 0);

    while(!(ADC1->ISR & (1 << 0)));
}

uint16_t ADC_Read(void)
{
    ADC1->CR |= (1 << 2);

    while(!(ADC1->ISR & (1 << 2)));

    return (uint16_t)ADC1->DR;
}

int main(void)
{
    GPIO_Handle_t GPIO;
    uint16_t adc_value;

    GPIO_peripherial_Clk_Cntrl(GPIOA, SET);
    ADC_PCLK_EN();

    GPIO.pGPIOx = GPIOA;
    GPIO.GPIO_PinConfig.GPIO_PinNumber = 0;   // PA0
    GPIO.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ANALOG;
    GPIO.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_Init(&GPIO);

    ADC_Init();

    while(1)
    {
        adc_value = ADC_Read();
    }
}
