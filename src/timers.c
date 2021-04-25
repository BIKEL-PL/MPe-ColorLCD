/*
 * Bafang / APT 8X0C / 560C display firmware
 *
 * Based on OpenSourceEBike firmware with Copyrights (C) to Casainho, 2018, 2019, 2020
 * https://github.com/OpenSourceEBike/Color_LCD
 *
 * Adaptation for MPe (ebikecomputer.com)  Copyright (C) BIKEL.PL Marek Przybylak, Maciej Gibowicz 2020, 2021
 *
 * Released under the GPL License, Version 3
 */

#include "stm32f10x.h"
#include "timers.h"
#include "main.h"
#include "pins.h"

static volatile uint32_t _ms;
volatile uint32_t time_base_counter_1ms = 0;

void delay_ms (uint32_t ms)
{
  _ms = 1;
  while (ms >= _ms) ;
}

void SysTick_Handler(void) // runs every 1ms
{
  _ms++; // for delay_ms ()

  time_base_counter_1ms++;
}

void systick_init (void)
{
  /* Setup SysTick Timer for 1 millisecond interrupts, also enables Systick and Systick-Interrupt */
  if (SysTick_Config(SystemCoreClock / 1000))
  {
    /* Capture error */
    while (1);
  }
}

volatile uint32_t get_time_base_counter_1ms (void)
{
  return time_base_counter_1ms;
}

// used for LCD backlight
#ifdef USE_WITH_8X0C
void timer_LCD_BL_init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  // reset TIM3
  TIM_DeInit(TIM3);

  /* Time Base configuration */
  // Our target is PWM with 100Hz frequency: 128MHz clock (PCLK1), 128MHz/1280000 = 100Hz
  // Since Timer3 is 16 bits, we need to use prescaller with 32, as: 40000 * 32 = 1280000
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = (32 - 1);
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = (40000 - 1);
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* TIM3 counter enable */
  TIM_Cmd(TIM3, ENABLE);
}
#endif
#ifdef USE_WITH_560C
void timer_LCD_BL_init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  // reset TIM3
  TIM_DeInit(TIM1);

  // Time Base configuration
  // Our target is PWM with 100Hz frequency: 128MHz clock (PCLK1), 128MHz/1280000 = 100Hz  //FIXME it is now 104MHz, not 128Mhz
  // Since Timer3 is 16 bits, we need to use prescaller with 32, as: 40000 * 32 = 1280000
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = (32 - 1);
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = (40000 - 1);
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  // PWM1 Mode configuration: Channel2
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_CtrlPWMOutputs(TIM1, ENABLE); //konieczne dla TIM1

  // TIM3 counter enable
  TIM_Cmd(TIM1, ENABLE);
}
#endif

// every 100ms
void TIM4_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    /* Clear TIMx TIM_IT_Update pending interrupt bit */
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

  }
}

void timer4_init(void)
{
  // enable TIMx clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  // reset TIMx
  TIM_DeInit(TIM4);

  /* Time base configuration */
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = (10000 - 1);
  TIM_TimeBaseStructure.TIM_Prescaler = (1280 - 1); // 128MHz clock (PCLK1), 128MHz/12800000 = 10Hz --> 100ms each increment of the counter/timer
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit (TIM4, &TIM_TimeBaseStructure);

  /* Enable the TIMx global Interrupt */
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM4_INTERRUPT_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init (&NVIC_InitStructure);

  /* TIMx TIM_IT_Update enable */
  TIM_ITConfig (TIM4, TIM_IT_Update, ENABLE);

  /* TIM4 counter enable */
  TIM_Cmd (TIM4, ENABLE);
}
