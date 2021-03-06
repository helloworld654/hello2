#include "AllHead.h"

#if defined(BIKING) && BIKING
#include "l298n.h"
#if defined(PID_METHOD) && PID_METHOD
pid_struct	line_pid;
pid_struct	oK_pid;
#endif
#endif
void JTAG_Set(u8 mode);
u8 RUNNING = 1;

int main(void)
{	 
	uint8_t res = 0;
	uart_init(115200);	 	//串口初始化为 115200	
	JTAG_Set(0x01);
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	TIM6_Int_Init(10000,7199);	//10Khz计数频率,1秒钟中断  printf N fps message in timer.c  N frames every second  result:15fps		
#if defined(BIKING) && BIKING
	// Motor_startL();  // for l298n
	MotorA_start();		//for A4950 in motor.c
	// MotorR_start();
	RUNNING = 1;
#if defined(PID_METHOD) && PID_METHOD
/*   finish it
	PID_init(&line_pid,3,0.1,2);
	PID_init(&oK_pid,40,3,30);
*/
	PID_init(&line_pid,3,0.1,2);
	PID_init(&oK_pid,40,3,30);
#endif
#if defined(START_TEST) && START_TEST
	// motor_test();      // for l298n
	motorA_test();		//for A4950
	// motorR_test();
	RUNNING = 0;
	printf("hello\r\n");
#endif
#endif
#if defined(LCD_ON_OFF) && LCD_ON_OFF
	LCD_Init();			   		//初始化LCD  
	POINT_COLOR=RED;			//设置字体为红色 
	LCD_ShowString(30,230,200,16,16,"OV7670 Init...");	  
#endif
	res = 1;
	while(res)//初始化OV7670
	{
		res = OV7670_Init();
		printf("call OV7670_Init(),res:%d\r\n",res);
#if defined(LCD_ON_OFF) && LCD_ON_OFF
		LCD_ShowString(30,230,200,16,16,"OV7670 Error!!");
		delay_ms(200);
		LCD_Fill(30,230,239,246,WHITE);
#endif
		delay_ms(200);
	}
#if defined(LCD_ON_OFF) && LCD_ON_OFF
	LCD_ShowString(30,230,200,16,16,"OV7670 Init OK");
	delay_ms(500);	 	   
	LCD_Clear(BLACK); 
#endif
	EXTI11_Init();		// PB15 for ov7670 VSYNC interput pin in exti.c
	OV7670_Window_Set(12,176,240,320); 
	OV7670_CS=0;			
	printf("before while 1\r\n");
	while(1)
	{
#if defined(BIKING) && BIKING
		if(RUNNING){
#endif
			cameraOperation();//摄像头更新显示以及后续的图像处理操作接口
			delay_ms(1);
#if defined(BIKING) && BIKING
		}
		else{
			stop_forward();
			printf("monitor stop\r\n");
			delay_ms(1000);
		} 
#endif
	}
}

/*针对调试和使用冲突问题*/
void JTAG_Set(u8 mode)
{
	u32 temp;
	temp = mode;
	temp <<= 25;
	RCC->APB2ENR |= 1<<0;
	AFIO->MAPR &= 0xF8FFFFFF;
	AFIO->MAPR |= temp;
}
