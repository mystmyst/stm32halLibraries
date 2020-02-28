/**
  * Autor: MYST - M. Yudi S. T.
	* e-mal: matheusyudits@gmail.com
	* github: https://github.com/mystmyst/stm32halLibraries
	* language: Portuguese-BR
	* #brief: Biblioteca destinada a leitura dos modulos MAX6675
	  fornecendo valores de leitura do termopar tipo K.
	* todos os direitos livres para quais quer forma de uso
**/

#include "max6675.h"

void MAX6675_Init(MAX6675 data)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = data.pinSck;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(data.gpioSck, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = data.pinData;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(data.gpioData, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = data.pinCS;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(data.gpioCS, &GPIO_InitStruct);

	HAL_GPIO_WritePin(data.gpioCS, data.pinCS, GPIO_PIN_SET); 			//disable
	
	HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(data.gpioData, data.pinSck, GPIO_PIN_RESET);
}
/*
 * #brief: o datasheet mostra como a leitura eh feita. clk em baixa e CS em alta. CS em baixa para 
	 comecar a leitura do sensor. um pulso de lixo e os 12 em seguida sao para adquirir dados na porta
	 data. 3 bits seguintes sao lixo
	 @data: struct com os pinos de clock, chip select e dados
	 % retorna o valor adquirido multiplicado pela valor de fundo de escala
*/
float MAX6675_Value(MAX6675 data)
{
	int buffer;
	
	HAL_GPIO_WritePin(data.gpioCS, data.pinCS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(data.gpioCS, data.pinCS, GPIO_PIN_RESET);		//enable
	
	HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);			
	HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_RESET);  //primeiro clock lixo
	
	for(uint8_t i=0;i<12;i++)		//leitura dos 12 bits
	{
		buffer=buffer<<1;
		HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);			
			if(HAL_GPIO_ReadPin(data.gpioData,data.pinData))
				buffer++;
		HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_RESET);
	}
	
	for(uint8_t i=0;i<3;i++)		//3 bits de lixo
	{
		HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);			
		HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_RESET);  
	}
	HAL_GPIO_WritePin(data.gpioCS, data.pinCS, GPIO_PIN_SET);	//desable
	return (float)buffer*0.25;
}

float MAX6675_Avarege(MAX6675 data,uint8_t times)
{
	float value =times;
	for(uint8_t i=0;i<times;i++)
		value+=MAX6675_Value(data);
	value/=times;
	return value;
}
