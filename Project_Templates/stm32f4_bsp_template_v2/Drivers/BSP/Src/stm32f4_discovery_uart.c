/**
  ******************************************************************************
  * @file    stm32f4_discovery_uart.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file provides set of firmware functions to manage UART as an
  *          alternative to STDIO functions.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery_uart.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_UART
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_UART
  * @brief This file provides set of firmware functions to manage UART as an
  *        alternative to STDIO functions such as printf and scanf.
  * @{
  */


/** @defgroup STM32F4_DISCOVERY_UART_Exported_Handle
 * @{
 */
UART_HandleTypeDef huart4;
DMA_HandleTypeDef hdma_uart4_tx;
DMA_HandleTypeDef hdma_uart4_rx;

UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart6_tx;
DMA_HandleTypeDef hdma_usart6_rx;

uint8_t EscFlag = 0;
uint8_t Uart4Ready = 0;
uint8_t Uart6Ready = 0;

/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_UART_Private_FunctionPrototypes
  * @{
  */


/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_UART_Private_Functions
  * @{
  */

/**
 * @brief This function configures Pins for UART6
 * @param huart Uart Handle
 * @retval  None
 */
static void BSP_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(huart->Instance==USART6)
  {
    /* Peripheral clock enable */
    __USART6_CLK_ENABLE();

    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

#ifdef DMA_MODE_UART6
    /* Peripheral DMA init*/
    hdma_usart6_tx.Instance = DMA2_Stream6;
    hdma_usart6_tx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_tx.Init.Mode = DMA_NORMAL;
    hdma_usart6_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_usart6_tx);

    __HAL_LINKDMA(huart,hdmatx,hdma_usart6_tx);

    hdma_usart6_rx.Instance = DMA2_Stream1;
    hdma_usart6_rx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart6_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_usart6_rx);

    __HAL_LINKDMA(huart,hdmarx,hdma_usart6_rx);
#endif

#ifndef POLLING_MODE_UART6
    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART6_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
#endif
  }
  else if(huart->Instance==UART4)
  {
    /* Peripheral clock enable */
    __UART4_CLK_ENABLE();

    /**
    UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

#ifdef DMA_MODE_UART4
    /* Peripheral DMA init*/
    hdma_usart6_tx.Instance = DMA1_Stream4;
    hdma_usart6_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_tx.Init.Mode = DMA_NORMAL;
    hdma_usart6_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_uart4_tx);

    __HAL_LINKDMA(huart,hdmatx,hdma_uart4_tx);

    hdma_usart6_rx.Instance = DMA1_Stream2;
    hdma_usart6_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart6_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_uart4_rx);

    __HAL_LINKDMA(huart,hdmarx,hdma_uart4_rx);
#endif

#ifndef POLLING_MODE_UART4
    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(UART4_IRQn, 3, 1);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
#endif
  }
}


/**
 * This is General UART Transmit function irrespective of transfer mode
 * @param huart uart handle
 * @param data_buf  Data buffer or string
 * @param size      length of data
 * @param timeout   if polling then specify timeout: MAX_TIMEOUT
 * @return  status of transfer
 */
HAL_StatusTypeDef UART_Transmit(UART_HandleTypeDef* huart,uint8_t *data_buf, uint16_t size, uint32_t timeout)
{
  HAL_StatusTypeDef status=HAL_ERROR;
  uint8_t i;

  if(huart == &huart4)
  {
#ifdef POLLING_MODE_UART4
      status = HAL_UART_Transmit(&huart4, data_buf, size, timeout);
#endif

#ifdef INTERRUPT_MODE_UART4
      status = HAL_UART_Transmit_IT(&huart4, data_buf, size);
      HAL_Delay(1);
#endif

#ifdef DMA_MODE_UART4
      status = HAL_UART_Transmit_DMA(&huart4, data_buf, size);
      HAL_Delay(1);
#endif
  }

  if(huart == &huart6)
  {
#ifdef POLLING_MODE_UART6
      status = HAL_UART_Transmit(&huart6, data_buf, size, timeout);
#endif

#ifdef INTERRUPT_MODE_UART6
      status = HAL_UART_Transmit_IT(&huart6, data_buf, size);
      HAL_Delay(1);
#endif

#ifdef DMA_MODE_UART6
      status = HAL_UART_Transmit_DMA(&huart6, data_buf, size);
//      HAL_Delay(1);
#endif
  }
  return (status);
}


/**
 * This is General UART Receive function irrespective of transfer mode
 * @param huart UART handle
 * @param data_buf  data buffer to receive string or data
 * @param size      length of data to receive
 * @param timeout   if polling then specify timeout: MAX_TIMEOUT
 * @return  status of transfer
 */
HAL_StatusTypeDef UART_Receive(UART_HandleTypeDef* huart,uint8_t *data_buf, uint16_t size, uint32_t timeout)
{
  HAL_StatusTypeDef status=HAL_ERROR;

  EscFlag = 0;                    /* clear EscapeFlag */

  if(huart==&huart4)
  {
#ifdef POLLING_MODE_UART4
    status = HAL_UART_Receive(&huart4, data_buf, size, timeout);
#endif

#ifdef INTERRUPT_MODE_UART4
     HAL_UART_Receive_IT(&huart4, data_buf, size);
     if(Uart4Ready == 1)
     {
         Uart4Ready = 0;
         status = HAL_OK;
     }
#endif

#ifdef DMA_MODE_UART4
     HAL_UART_Receive_DMA(&huart4, data_buf, size);
     if(Uart4Ready == 1)
     {
       Uart4Ready = 0;
       status = HAL_OK;
     }
#endif
  }

  if(huart==&huart6 )
  {
#ifdef POLLING_MODE_UART6
    status = HAL_UART_Receive(&huart6, data_buf, size, timeout);
#endif

#ifdef INTERRUPT_MODE_UART6
    HAL_UART_Receive_IT(&huart6, data_buf, size);
    if(Uart6Ready == 1)
    {
      Uart6Ready = 0;
      status = HAL_OK;
    }
#endif

#ifdef DMA_MODE_UART6
    HAL_UART_Receive_DMA(&huart6, data_buf, size);
    if(Uart6Ready == 1)
    {
      Uart6Ready = 0;
      status = HAL_OK;
    }
#endif
  }

  return (status);
}


/**
 * @brief VT100- code to set cursor to Home
 * @return None
 */
static void uart_reset_cursor (UART_HandleTypeDef* huart)
{
    if(huart == &huart4)
    {
        uprintf(huart,"\x1b[H");   /* escape sequence for vt220 ^[H sets cursor to Home */
    }
    else if (huart == &huart6)
    {
        uprintf(huart,"\x1b[H");   /* escape sequence for vt220 ^[H sets cursor to Home */
    }
}


/**
 * @brief VT100- code to Clear Screen
 * @return  None
 */
static void uart_clear_screen (UART_HandleTypeDef* huart)
{
    if(huart == &huart4)
    {
        uprintf(huart,"\x1b[2J");   /* escape sequence for vt220 ESC[2J clears screen */
    }
    else if (huart == &huart6)
    {
        uprintf(huart,"\x1b[2J");   /* escape sequence for vt220 ESC[2J clears screen */
    }

}

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_UART_Functions
  * @{
  */

/**
 * @brief This function initializes UART with desired baudrate
 * @param huart uart handle
 * @param Baudrate    baudrate value
 * @retval None
 */
void BSP_UART_Init(UART_HandleTypeDef* huart,uint32_t Baudrate)
{
    if(huart == &huart6)
    {
#ifdef DMA_MODE_UART6
      /* DMA controller clock enable */
      __DMA2_CLK_ENABLE();

      /* DMA interrupt init */
      HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 4, 0);
      HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
      HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 4, 0);
      HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
#endif

      __GPIOC_CLK_ENABLE();

      huart6.Instance = USART6;
      huart6.Init.BaudRate = Baudrate;
      huart6.Init.WordLength = UART_WORDLENGTH_8B;
      huart6.Init.StopBits = UART_STOPBITS_1;
      huart6.Init.Parity = UART_PARITY_NONE;
      huart6.Init.Mode = UART_MODE_TX_RX;
      huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
      huart6.Init.OverSampling = UART_OVERSAMPLING_16;

      BSP_UART_MspInit(&huart6);
      HAL_UART_Init(&huart6);
    }

    else if(huart == &huart4)
    {
#ifdef DMA_MODE_UART4
      /* DMA controller clock enable */
      __DMA1_CLK_ENABLE();

      /* DMA interrupt init */
      HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 4, 1);
      HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
      HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 4, 1);
      HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
#endif

      __GPIOC_CLK_ENABLE();

      huart4.Instance = UART4;
      huart4.Init.BaudRate = Baudrate;
      huart4.Init.WordLength = UART_WORDLENGTH_8B;
      huart4.Init.StopBits = UART_STOPBITS_1;
      huart4.Init.Parity = UART_PARITY_NONE;
      huart4.Init.Mode = UART_MODE_TX_RX;
      huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
      huart4.Init.OverSampling = UART_OVERSAMPLING_16;

      BSP_UART_MspInit(&huart4);
      HAL_UART_Init(&huart4);
    }
}


/**
 * @brief This function is to De-Initilized UART
 * @param huart Uart Handle
 * @retval  None
 */
void BSP_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART6)
  {
    /* Peripheral clock disable */
    __USART6_CLK_DISABLE();

    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

#ifdef DMA_MODE_UART6
    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(huart->hdmatx);
    HAL_DMA_DeInit(huart->hdmarx);
#endif

#ifndef POLLING_MODE_UART6
    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART6_IRQn);
#endif
  }

  else if(huart->Instance==UART4)
  {
    /* Peripheral clock disable */
    __UART4_CLK_DISABLE();

    /**UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11);

#ifdef DMA_MODE_UART4
    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(huart->hdmatx);
    HAL_DMA_DeInit(huart->hdmarx);
#endif

#ifndef POLLING_MODE_UART4
    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(UART4_IRQn);
#endif
  }
}


/**
 * @brief The ugetche() function returns the next character read from the
 *        console and echoes that character to the screen.Characters from
 *        space(20hex) to (7E) are echo to the screen.The ugetche() function
 *        is not define by the ANSI C standard.
 * @param mode  @arg  NONE_BLOCKING
 *              @arg  BLOCKING
 * @return Character or format
 */
uint8_t ugetche(UART_HandleTypeDef* huart,TRANSFER_BLOCK_Type mode)
{
  uint8_t key[1];
  HAL_StatusTypeDef status=HAL_ERROR;

  if(mode == NONE_BLOCKING)
  {
    status = UART_Receive(huart,key, 1, 1);
  }
  else if(mode == BLOCKING)
  {
    while(status != HAL_OK)
    {
      status = UART_Receive(huart,key, 1, 1);
    }
  }
  else if(mode == TIME_BLOCKING)
  {
    status = UART_Receive(huart,key, 1, 100);
  }
  /* Got some data */
  if(status == HAL_OK)
  {
    if (key[0] == In_CR)
    {
      return(key[0]);
    }
    else if (key[0] == In_DELETE || key[0] == In_BACKSPACE)
    {
      return(key[0]);
    }
    else if (key[0] == In_ESC)
    {
      EscFlag = 1;
      return (0);
    }
    else if (key[0] >= ' ')
    {
      return (key[0]);
    }
    else
    {
      UART_Transmit(huart,&key[0], 1, MAX_TIMEOUT);
    }
  }
  return(0);
}


/**
 * @brief One line editor
 * @param s     Pointer to Received buffer
 * @param lim   Length of Received buffer
 * @return  Character count
 */
uint8_t uget_line(UART_HandleTypeDef* huart,int8_t s[], uint8_t lim)
{
  int8_t kb;                 /* input character */

  uint8_t pointer=0;          /* Pointer in buffer */
  uint8_t count=0;            /* character count */
  uint8_t temp;

  while(1)
  {
    kb = ugetche(huart,BLOCKING);

    if (kb == In_ESC)                /* if ESCAPE pressed then exit */
    {
      return (kb);
    }

    else if (kb == In_CR)            /* CARRIAGE return ? */
    {
      s[pointer] = '\0';             /* put null char on last position */
      break;                         /* yes, exit from this loop */
    }

    else if(kb == In_DELETE || kb == In_BACKSPACE)
    {
      if(pointer==0)                 /* any characters entered */
      {
        continue;                    /* no, so get another character */
      }

      temp = Out_BACKSPACE;
      UART_Transmit(huart,&temp, 1, MAX_TIMEOUT);   /* go back one position */
      temp = Out_SPACE;
      UART_Transmit(huart,&temp, 1, MAX_TIMEOUT);   /* erase char */
      temp = Out_BACKSPACE;
      UART_Transmit(huart,&temp, 1, MAX_TIMEOUT);   /* go back one more position */

      pointer--;                     /* decrement pointer */
      count--;                       /* decrement character count */

      continue;
    }

    else if(pointer < lim)
    {
      s[pointer] = kb;               /* save character and increment pointer */
      pointer++;
      count++;                       /* increment count */
      UART_Transmit(huart,&kb, 1, MAX_TIMEOUT);    /* echo character */

      continue;                      /* and get some more */
    }
    else
    {
      temp = In_BELL;
      UART_Transmit(huart,&temp, 1, MAX_TIMEOUT);   /* ring the bell */
    }
  }
  return(count);
}


/**
 * @brief Modified version of Standard Printf statement
 * @paragraph Supports standard formats "%c %s %d %x"
 *            "%d" and "%x" requires non-standard qualifiers,"%dfn, %xfn":-
 *            f supplies a fill character
 *            n supplies a field width
 *
 *        Supports custom formats  "%b  %u %t %y %a"
 *        "%b"  prints a 2 digit BCD value with leading zero
 *        "%u"  prints the 16 bit unsigned integer in hex format
 * @param format  Character format
 * @param ...     <multiple argument>
 * @return  return with valid character or nothing
 */
HAL_StatusTypeDef uprintf(UART_HandleTypeDef* huart,const char *format, ...)
{
  uint8_t hex[]= "0123456789ABCDEF";
  uint32_t width_dec[10] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};
  uint32_t width_hex[10] = { 0x1, 0x10, 0x100, 0x1000, 0x10000, 0x100000, 0x1000000, 0x10000000};
  uint8_t temp;

  int8_t format_flag, fill_char;
  uint32_t u_val, div_val;
  uint16_t base;

  int8_t *ptr;

  va_list ap;
  va_start(ap, format);

  for(;;)
  {
    while((format_flag = *format++) != '%')      /* until full format string read */
    {
      if(!format_flag)
      {                        /* until '%' or '\0' */
        return (HAL_OK);
      }
      UART_Transmit(huart,&format_flag, 1, MAX_TIMEOUT);
    }

    switch(format_flag = *format++)
    {
      case 'c':
        format_flag = va_arg(ap, int32_t);
        UART_Transmit(huart,&format_flag, 1, MAX_TIMEOUT);

        continue;

      default:
        UART_Transmit(huart,&format_flag, 1, MAX_TIMEOUT);

        continue;

      case 'b':
        format_flag = va_arg(ap,int32_t);
        UART_Transmit(huart,&(hex[(uint16_t)format_flag >> 4]), 1, MAX_TIMEOUT);
        UART_Transmit(huart,&(hex[(uint16_t)format_flag & 0x0F]), 1, MAX_TIMEOUT);

        continue;

      case 's':
        ptr = va_arg(ap, int8_t *);
        while(*ptr)
        {
          UART_Transmit(huart,&(*ptr++), 1, MAX_TIMEOUT);
        }

        continue;

      case 'u':
        base = 16;
        div_val = 0x100000;
        u_val = va_arg(ap, uint32_t);
        do
        {
          UART_Transmit(huart,&(hex[u_val/div_val]), 1, MAX_TIMEOUT);
          u_val %= div_val;
          div_val /= base;
        }while(div_val);

        continue;

      case 'd':
        base = 10;
        fill_char = *format++;
        format_flag = ( *format++ ) - '1';
        div_val = width_dec[format_flag];
        u_val = va_arg(ap,int32_t);

        if(((int)u_val) < 0)
        {
          u_val = - u_val;    /* applied to unsigned type, result still unsigned */
          temp = '-';
          UART_Transmit(huart,&temp, 1, MAX_TIMEOUT);
        }

        goto  CONVERSION_LOOP;

      case 'x':
        base = 16;
        fill_char = *format++;
        format_flag = (*format++) - '1';
        div_val = width_hex[format_flag];
        u_val = va_arg(ap, int32_t);

        CONVERSION_LOOP:
        while(div_val > 1 && div_val > u_val)
        {
          div_val /= base;
          UART_Transmit(huart,&fill_char, 1, MAX_TIMEOUT);
        }

        do
        {
          UART_Transmit(huart,&(hex[u_val/div_val]), 1, MAX_TIMEOUT);
          u_val %= div_val;
          div_val /= base;
        }while(div_val);
    }/* end of switch statement */
  }
  return(HAL_OK);
}


/********************************************************************//**
* @brief UART Utility functions
**********************************************************************/

/**
 * @brief VT100- code to Clear Screen and Reset Cursor
 * @return  None
 */
void uart_clr_scr_rst_cur (UART_HandleTypeDef* huart)
{
  uart_clear_screen(huart);
  uart_reset_cursor(huart);
}


/**
 * @brief Erase Character
 * @return  None
 */
void uErase_Char (UART_HandleTypeDef* huart)
{
  uprintf(huart,"%c", Out_BACKSPACE);
  uprintf(huart,"%c", Out_SPACE);
  uprintf(huart,"%c", Out_BACKSPACE);
}


/**
 * @brief Erase Character with Underscore '_'
 * @return  None
 */
void uErase_Char_With_UnderScore (UART_HandleTypeDef* huart)
{
  uprintf(huart,"%c", Out_BACKSPACE);       /* erase character on the screen */
  uprintf(huart,"_");                       /* and write '_' on the screen */
  uprintf(huart,"%c", Out_BACKSPACE);       /* erase character on the screen */
}


/**
 * @brief Erase Backslash
 * @return None
 */
void uErase_Backslash (UART_HandleTypeDef* huart)
{
  uprintf(huart,"%c", Out_BACKSPACE);       /* erase character on the screen */
  uprintf(huart,"/");                       /* and write '/' on the screen */
  uprintf(huart,"%c", Out_BACKSPACE);       /* erase character on the screen */
  uprintf(huart,"%c", Out_BACKSPACE);       /* erase character on the screen */
  uprintf(huart,"_");                       /* and write '_' on the screen */
  uprintf(huart,"%c", Out_BACKSPACE);       /* erase character on the screen */
}


/**
 * @brief Erase Semicolon
 * @return  None
 */
void uErase_SemiColon (UART_HandleTypeDef* huart)
{
  uprintf(huart,"%c", Out_BACKSPACE);       /* erase character on the screen */
  uprintf(huart,":");                       /* and write ':' on the screen */
  uprintf(huart,"%c", Out_BACKSPACE);       /* erase character on the screen */
  uprintf(huart,"%c", Out_BACKSPACE);       /* erase character on the screen */
  uprintf(huart,"_");                       /* and write '_' on the screen */
  uprintf(huart,"%c", Out_BACKSPACE);       /* erase character on the screen */
}


/**
 * @brief Erase and Ring the Bell
 * @return  None
 */
void uErase_And_RingTheBell (UART_HandleTypeDef* huart)
{
  uprintf(huart,"%c", Out_BACKSPACE);         /* back space */
  uprintf(huart,"_");                         /* erase 1st char on the screen */
  uprintf(huart,"%c", Out_BACKSPACE);         /* back space */
  uprintf(huart,"_");                         /* erase 2nd char on the screen */
  uprintf(huart,"%c", Out_BACKSPACE);         /* back space */
  uprintf(huart,"\7");                        /* ring the bell */
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/*****************************************END OF FILE******************************************/
