#include "stm32f4xx.h"                  // Device header
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char  str[50];
uint32_t i=0,a=0;
char gelenVeri[21] = {0};
const char *karsilastir = "yesil";
const char *yeni = "mavi";
const char *karsilastir2 = "sari";
void USART_Puts(USART_TypeDef* USARTx,volatile char *s) // char karakter sayisi kadar döndürüyor
{
 while(*s)
 {
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	 USART_SendData(USARTx,*s);
	 *s++;
 }	 
}	

void USART2_IRQHandler(void)
	{
 // Okunan data registeri bos ise
 // yani herhangi veri gelmediyse bekle
 // RXNE: RX data register not empty
 		
	 while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
  	char tmp = USART_ReceiveData(USART2); // Data registeri dolu ise okunan veriyi gelenVeri'ye kaydet
    USART_SendData(USART2, tmp);
		gelenVeri[i]=tmp;
		i++;
		  	if(tmp=='1')// Kirmizi
			{
			    GPIO_ResetBits (GPIOD, GPIO_Pin_12);
		      GPIO_SetBits (GPIOD, GPIO_Pin_13 | GPIO_Pin_14);
			
			}	
       if(tmp=='2') // Yesil
			{
			    GPIO_ResetBits (GPIOD, GPIO_Pin_13);
		      GPIO_SetBits (GPIOD, GPIO_Pin_12 | GPIO_Pin_14);
			
			}				
       if(tmp=='3') // Mavi
			{
			    GPIO_ResetBits (GPIOD, GPIO_Pin_14);
	      	GPIO_SetBits (GPIOD, GPIO_Pin_12 | GPIO_Pin_13);
			
			}		
       if(tmp=='4') // Sari
			{
			    GPIO_ResetBits (GPIOD, GPIO_Pin_12 | GPIO_Pin_13);
		      GPIO_SetBits (GPIOD, GPIO_Pin_14);
			
			}					
			  if(tmp=='5') // // Beyaz
			{
			    GPIO_ResetBits (GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
			
			}		
		
			
		
				
		
	  /*if(strcmp(gelenVeri, "metin") == 0)
		{
				GPIO_SetBits(GPIOD,GPIO_Pin_12);
		}*/
    //str[i]=USART_ReceiveData(USART2);
		//i++;
		/*char ch = USART2-> DR;
		i++;
		if(ch=='k' && i==1)
			{
			GPIO_SetBits(GPIOD,GPIO_Pin_12);
			
		  
			}
		else
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_12);
		}*/
 // Gelen veriyi ekrana basmak icin
 // Yollanan data registerin bos olup olmamasini denetle
// while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
 //USART_SendData(USART2, gelenVeri);
	
}
	/*char USART2_IRQHandler(void)
	{
 // Okunan data registeri bos ise
 // yani herhangi veri gelmediyse bekle
 // RXNE: RX data register not empty
 while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
  
 // Data registeri dolu ise okunan veriyi gelenVeri'ye kaydet
 char gelenVeri = USART_ReceiveData(USART2);

 // Gelen veriyi ekrana basmak icin
 // Yollanan data registerin bos olup olmamasini denetle
// while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
 USART_SendData(USART2, gelenVeri);
	return gelenVeri;
}*/
 void delay(uint32_t time)
{
  while(time)
	time--;	
}	



int main()
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); // transmitter tx A2 GPIOya bagli usartta
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //GPIODya clock verdik,AHB1 hangi clock hattina bagli oldugu
	
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14  ;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); // D portu Out olarak tanimlanmis
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; //Alternatif fonksiyonlar(input,output,adc disinda baska sey oldugu)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);//USARTtan bilgiyi GPIOya atacaz,CPUya tanitmamiz lazim
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	USART_InitStructure.USART_BaudRate=115200; // Saniye içinde hat üzerinden kaç tane bit gönderilmesi gerektigi
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	
	USART_Cmd(USART2,ENABLE);
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
 
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 
  NVIC_Init(&NVIC_InitStructure);


	while(1)
	{
		 
		 USART2_IRQHandler();
	// sprintf(str, "Metin \n");
	//USART_Puts(USART2,str);
		//char* a=USART2_IRQHandler();
		
		
			
		/*i=600000;
		while(i)
			i--;*/
	
	}
	
	
}	
	

	
 
