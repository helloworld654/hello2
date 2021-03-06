#include "l298n.h"
#include "AllHead.h"
#include "grey.h"

// u16	speed_min = 380;   // the speed_min will change to 0 unexcepted because of the u16 type,when change to int,fix it
int	speed_min = 380;  //min:420 for mg310 using l298n sanlun 
u16 max_add = 230;   //200 can finish the wandao in pid method
extern u8 RUNNING;
#if defined(MONITORL_ERROR) && MONITORL_ERROR
u8 monitor_error_add = 50;
#endif

void Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	    //使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;				 //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	// GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化PA4/5/6/7
	GPIO_Init(GPIOG, &GPIO_InitStructure);					 //根据设定参数初始化PA4/5/6/7
}

void Motor_Stop(void)
{
	Mr_P = 0;
	Mr_N = 0;
	Ml_P = 0;
	Ml_N = 0;
}

void Motor_Forward(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 1;
	Mr_N = 0;
	Ml_P = 1;
	Ml_N = 0;
}

void Motor_Backward(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 0;
	Mr_N = 1;
	Ml_P = 0;
	Ml_N = 1;
}

void Motor_Turnleft(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 1;
	Mr_N = 0;
	Ml_P = 0;
	Ml_N = 0;
}

void Motor_Turnright(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 0;
	Mr_N = 0;
	Ml_P = 1;
	Ml_N = 0;
}

void Motor_Leftback(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 0;
	Mr_N = 0;
	Ml_P = 0;
	Ml_N = 1;
}

void Motor_Rightback(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 0;
	Mr_N = 1;
	Ml_P = 0;
	Ml_N = 0;
}

void left_add(int add)
{
	if(RUNNING == 0)
		return ;
	if(add < 0){
		printf("error in left_add() add < 0\r\n");
		return ;
	}
	if(add > max_add){
		printf("in left_add() add:%d\tbeyond the max_add:%d\r\n",add,max_add);
		add = max_add;
		printStopMess(3);
		// RUNNING = 0;
	}
	TIM_SetCompare2(TIM3,speed_min-add);
}

void right_add(int add)
{
	if(RUNNING == 0)
		return ;
	if(add < 0){
		printf("error in right_add() add < 0\r\n");
		return ;
	}
	if(add > max_add){
		printf("in right_add() add:%d\tbeyond the max_add:%d\r\n",add,max_add);
		add = max_add;
		printStopMess(4);
		// RUNNING = 0;
	}
#if defined(MONITORL_ERROR) && MONITORL_ERROR
	TIM_SetCompare1(TIM3,speed_min-add-monitor_error_add);
#else
	TIM_SetCompare1(TIM3,speed_min-add);
#endif
}

void Motor_startL(void)
{
	int sp_start=350;
	Motor_Init();
	monitor_PWM_Init(899,0);
	TIM_SetCompare1(TIM3,sp_start);   //the right monitor	PB4
	TIM_SetCompare2(TIM3,sp_start); 	// the left monitor		PB5
	Motor_Forward();
	delay_ms(10);
	right_add(0);
	left_add(0);
}

void motor_test(void)
{
	delay_ms(1500);
	delay_ms(1500);
	delay_ms(1500);
	Motor_Stop();
	return ;

	// Motor_Turnleft();
	// delay_ms(1000);
	// Motor_Turnright();
	// delay_ms(1000);
	// Motor_Forward();
	// delay_ms(1000);
	// Motor_Stop();
	// return ;

	// delay_ms(500);
	// left_add(200);
	// right_add(0);
	// delay_ms(1500);
	// Motor_Stop();
	// return ;

	// delay_ms(500);
	// left_add(0);
	// right_add(200);
	// delay_ms(1500);
	// Motor_Stop();
	// return ;
}

void turnA(void)
{
	if(RUNNING == 0)
		return ;
	Motor_Forward();
	delay_ms(400);
	Motor_Turnright();
	delay_ms(800);
	Motor_Stop();
	// delay_ms(1500);
	// delay_ms(1500);
	// RUNNING = 0;
}

// /* https://blog.csdn.net/qq_36958104/article/details/83661117  */
// void monitor_PWM_Init(u16 arr,u16 psc)
// {
//     GPIO_InitTypeDef     GPIO_InitStrue;
//     TIM_OCInitTypeDef     TIM_OCInitStrue;
//     TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;

//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);        //使能TIM3和相关GPIO时钟
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);// 使能GPIOB时钟(LED在BP5引脚),使能AFIO时钟(定时器3通道2需要重映射到BP5引脚)

// #if defined(TIM3_PartialRemap) && TIM3_PartialRemap
//     GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_5;     // TIM3_CH2
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
//     GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO端口初始化设置

//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_4;     // TIM3_CH1
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
//     GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO端口初始化设置
	
// // 如果PB0被初始化，则lcd不能正常显示，在LCD_Init()中有用到PB0，所以TIM3_CH2无法使用
//     // GPIO_InitStrue.GPIO_Pin=GPIO_Pin_0;     // TIM3_CH2
//     // GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
//     // GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
//     // GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO端口初始化设置
// #endif

// #if defined(TIM3_FullRemap) && TIM3_FullRemap    
//     GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_6;     // TIM3_CH1
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
//     GPIO_Init(GPIOC,&GPIO_InitStrue);                //GPIO端口初始化设置
	
//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_7;     // TIM3_CH2
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
//     GPIO_Init(GPIOC,&GPIO_InitStrue);                //GPIO端口初始化设置
// #endif

// #if defined(TIM4_Remap) && TIM4_Remap    
//     GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);

//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_12;     // TIM4_CH1
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
//     GPIO_Init(GPIOD,&GPIO_InitStrue);                //GPIO端口初始化设置
	
//     GPIO_InitStrue.GPIO_Pin=GPIO_Pin_13;     // TIM4_CH2
//     GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
//     GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
//     GPIO_Init(GPIOD,&GPIO_InitStrue);                //GPIO端口初始化设置
// #endif
    
//     TIM_TimeBaseInitStrue.TIM_Period=arr;    //设置自动重装载值
//     TIM_TimeBaseInitStrue.TIM_Prescaler=psc;        //预分频系数
//     TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;    //计数器向上溢出
//     TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;        //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
//     TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);        //TIM3初始化设置(设置PWM的周期)
    
//     TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM2;        // PWM模式2:CNT>CCR时输出有效
//     TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// 设置极性-有效为高电平
//     TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// 输出使能
// 	TIM_OC1Init(TIM3,&TIM_OCInitStrue);			// TIM3_CH1    PB4
//     TIM_OC2Init(TIM3,&TIM_OCInitStrue);        // TIM3_CH2    PB5
// 	//TIM3的通道2PWM 模式设置	PB5	 CH2	the right monitor
// 	// TIM_OC3Init(TIM3,&TIM_OCInitStrue);        //TIM3的通道3PWM 模式设置   PB0  CH3 the left monitor
 
//     TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);        //使能预装载寄存器
//     TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);        //使能预装载寄存器
// 	// TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);        //使能预装载寄存器
    
//     TIM_Cmd(TIM3,ENABLE);        //使能TIM3
// }
