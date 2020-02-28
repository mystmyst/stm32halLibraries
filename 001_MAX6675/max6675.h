/**
  * Autor: MYST - M. Yudi S. T.
	* e-mal: matheusyudits@gmail.com
	* github: https://github.com/mystmyst/stm32halLibraries
	* language: Portuguese-BR
	* #brief: Biblioteca destinada a leitura dos modulos MAX6675
	  fornecendo valores de leitura do termopar tipo K.
	* todos os direitos livres para quais quer forma de uso
**/
#ifndef MAX6675_H_
#define MAX6675_H_

#include "stm32f1xx_hal.h" //mude para a familia de controladores do seu uso

typedef struct _max6675
{
	GPIO_TypeDef* gpioSck;
	GPIO_TypeDef* gpioCS;
	GPIO_TypeDef* gpioData;
	uint16_t pinSck;
	uint16_t pinCS;
	uint16_t pinData;
} MAX6675;

void MAX6675_Init(MAX6675);
float MAX6675_Value(MAX6675);
float MAX6675_Avarege(MAX6675,uint8_t);

#endif /*MAX6675_H_*/
