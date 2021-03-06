#include "a4950.h"

/*  tim3 for left motor
    forward moving when tim3 ch1 > tim3 ch2
    tim4 for right motor
    forward moving when tim4 ch1 > tim4 ch2    */

#define SPEED_MIN   45
#define SPEED_MAX   55
#define MAX_ARR_A     899
#define EXEC_MAX    6
extern u8 RUNNING;

void set_speed(int speed_L,int speed_R)
{
    if(RUNNING == 0)
        return ;
    if(speed_L>100 || speed_L<-100 || speed_R>100 || speed_R<-100){
        printf("set speed error!\r\n");
        return ;
    }
    if(speed_L >= 0){
        TIM_SetCompare3(TIM4,(int)(MAX_ARR_A*(1-speed_L/100.0)));
        TIM_SetCompare4(TIM4,MAX_ARR_A);
    }
    else if(speed_L < 0){
        TIM_SetCompare3(TIM4,MAX_ARR_A);
        TIM_SetCompare4(TIM4,(int)(MAX_ARR_A*(1+speed_L/100.0)));
    }
    if(speed_R >= 0){
        TIM_SetCompare1(TIM3,(int)(MAX_ARR_A*(1-speed_R/100.0)));
        TIM_SetCompare2(TIM3,MAX_ARR_A);
    }
    else if(speed_R < 0){
        TIM_SetCompare1(TIM3,MAX_ARR_A);
        TIM_SetCompare2(TIM3,(int)(MAX_ARR_A*(1+speed_R/100.0)));
    }
}

void add_speed(uint8_t L_add,uint8_t R_add)
{
    int L_speed,R_speed;
    if(L_add>100-SPEED_MIN){
        L_add = 100-SPEED_MIN;
    }
    if(R_add>100-SPEED_MIN){
        R_add = 100-SPEED_MIN;
    }
    L_speed = SPEED_MIN+L_add;
    R_speed = SPEED_MIN+R_add;
    set_speed(L_speed,R_speed);
}

void start_forward(void)
{
    set_speed(SPEED_MIN,SPEED_MIN);
}

void stop_forward(void)
{
    set_speed(0,0);
}

void turn_left_A(void)
{
    set_speed(0,SPEED_MIN);
}

void turn_right_A(void)
{
    set_speed(SPEED_MIN,0);
}

void left_back_only(void)
{
    set_speed(-SPEED_MIN,0);
}

void right_back_only(void)
{
    set_speed(0,-SPEED_MIN);
}

void left_speed_add(uint8_t add)
{
    uint8_t max_add = SPEED_MAX-SPEED_MIN;
    if(add > max_add*EXEC_MAX){
        set_speed(SPEED_MIN+3,0);
    }
    else{
        if(add > max_add){
            add = max_add;
        }
        set_speed(SPEED_MIN+add,SPEED_MIN);
    }
}

void right_speed_add(uint8_t add)
{
    uint8_t max_add = SPEED_MAX-SPEED_MIN;
    if(add > max_add*EXEC_MAX){
        set_speed(0,SPEED_MIN+3);
    }
    else{
        if(add > max_add){
            add = max_add;
        }
        set_speed(SPEED_MIN,SPEED_MIN+add);
    }
}

void MotorA_start(void)
{
	motorA_PWM_Init(MAX_ARR_A,0);
}

void motorA_test(void)
{
    start_forward();
    delay_lms(2000);
    stop_forward();
    delay_ms(100);

    left_back_only();
    delay_ms(1000);
    stop_forward();
    delay_ms(100);
    
    right_back_only();
    delay_ms(1000);
    stop_forward();
    delay_ms(100);

    return ;
}

// /* https://blog.csdn.net/qq_36958104/article/details/83661117  */
// void monitor_PWM_Init(u16 arr,u16 psc)
// {
//     GPIO_InitTypeDef     GPIO_InitStrue;
//     TIM_OCInitTypeDef     TIM_OCInitStrue;
//     TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;

//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);        //??????TIM3?????????GPIO??????
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);// ??????GPIOB??????(LED???BP5??????),??????AFIO??????(?????????3??????2??????????????????BP5??????)

// #if defined(TIM3_PartialRemap) && TIM3_PartialRemap
//     GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_5;     // TIM3_CH2
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // ????????????
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //????????????????????????
//     GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO?????????????????????

//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_4;     // TIM3_CH1
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // ????????????
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //????????????????????????
//     GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO?????????????????????
	
// // ??????PB0??????????????????lcd????????????????????????LCD_Init()????????????PB0?????????TIM3_CH2????????????
//     // GPIO_InitStrue.GPIO_Pin=GPIO_Pin_0;     // TIM3_CH2
//     // GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // ????????????
//     // GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //????????????????????????
//     // GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO?????????????????????
// #endif

// #if defined(TIM3_FullRemap) && TIM3_FullRemap    
//     GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_6;     // TIM3_CH1
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // ????????????
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //????????????????????????
//     GPIO_Init(GPIOC,&GPIO_InitStrue);                //GPIO?????????????????????
	
//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_7;     // TIM3_CH2
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // ????????????
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //????????????????????????
//     GPIO_Init(GPIOC,&GPIO_InitStrue);                //GPIO?????????????????????
// #endif

// #if defined(TIM4_Remap) && TIM4_Remap    
//     GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);

//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_12;     // TIM4_CH1
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // ????????????
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //????????????????????????
//     GPIO_Init(GPIOD,&GPIO_InitStrue);                //GPIO?????????????????????
	
//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_13;     // TIM4_CH2
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // ????????????
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //????????????????????????
//     GPIO_Init(GPIOD,&GPIO_InitStrue);                //GPIO?????????????????????
// #endif
    
//     TIM_TimeBaseInitStrue.TIM_Period=arr;    //????????????????????????
//     TIM_TimeBaseInitStrue.TIM_Prescaler=psc;        //???????????????
//     TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;    //?????????????????????
//     TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;        //????????????????????????????????????????????????????????????????????????TIM_CKD_DIV1
//     TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);        //TIM3???????????????(??????PWM?????????)
    
//     TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM2;        // PWM??????2:CNT>CCR???????????????
//     TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// ????????????-??????????????????
//     TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// ????????????
// 	TIM_OC1Init(TIM3,&TIM_OCInitStrue);			// TIM3_CH1    PB4
//     TIM_OC2Init(TIM3,&TIM_OCInitStrue);        // TIM3_CH2    PB5
// 	//TIM3?????????2PWM ????????????	PB5	 CH2	the right monitor
// 	// TIM_OC3Init(TIM3,&TIM_OCInitStrue);        //TIM3?????????3PWM ????????????   PB0  CH3 the left monitor
 
//     TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);        //????????????????????????
//     TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);        //????????????????????????
// 	// TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);        //????????????????????????
    
//     TIM_Cmd(TIM3,ENABLE);        //??????TIM3
// }
