#include "main.h"
#include "stdio.h"

//delay
void delay1(void);

char datas[10] = "SIJAN";
char uartbuffer[10] = { 0 };
char trasmitcomplete = 0;

/*
 * Inizialize the GPIO pins
 * SCK_PIN : PA5 (SPI clock)
 * MOSI_PIN : PA7 (SPI dataout)
 * CS_PIN : PB5
 */
void gpioinit(void) {

    RCC_AHB1_CLKEN(RCC_AHB1Peri_GPIOA);
    RCC_AHB1_CLKEN(RCC_AHB1Peri_GPIOB);

    GPIO_Config(GPIOA, SCK_PIN, Alternate_Mode, No, AF_SPI);   //SCK PIN
    GPIO_Config(GPIOA, MOSI_PIN, Alternate_Mode, No, AF_SPI);  //MOSI PIN
    GPIO_Config(GPIOB, CS_PIN, Output_Mode, Pull_Down, AF0);   //CS   PIN
}

/*
 * Initialize the SPI1
 */
void spiinit(void) {

    RCC_APB2_CLKEN(RCC_APB2Peri_SPI1);

    SPI_Config(SPI1, SPI_BaudRate_sysclkby16, SPI_BIDIMode_2Line, SPI_First_MSB,
            SPI_DataFrame_16Bit, SPI_Master, SPI_Trasnmit);
    SPI_Enable(SPI1);
}

/*
 * Initialize UART2
 * Send Text From Serial Monitor
 */
void uartinit(void) {

    RCC_APB1_CLKEN(RCC_APB1Peri_USART2);
    GPIO_Config(GPIOA, 3, Alternate_Mode, No, AF_SPI_USART);
    GPIO_Config(GPIOA, 2, Alternate_Mode, No, AF_SPI_USART);

    USART_ConfigTypedef huart;

    huart.Mode = USART_TX_RX_ENABLE;
    huart.Oversampling_Mode = Oversampling_16;
    huart.Parity_Control = Parity_Disable;
    huart.Parity_Selection = Parity_Odd;
    huart.Baudrate = 0x683;  //1200 0x3415 //0x683--9600

    USART_Config(USART2, huart);
    UART_IT_EN(USART2, UART_RX_INTEN);
    NVIC_EnableIRQ(USART2_IRQn);

    __enable_irq();

    USART_Enable(USART2);
}

/*
 * USART2 Interrupt Handler
 * Receive text and stores in Buffer
 */
void USART2_IRQHandler(void) {

    uint8_t tempdata;
    static char length = 0;

    //If receive Interrupt flag
    if (CHECK_UART_ITFLAG(USART2, UART_RXIT_FLAG)) {
        tempdata = Receive_UART(USART2); //USART2->DR;

        if (tempdata == 0x0A || length > 9) {
            uartbuffer[length - 1] = '\0';
            length = 0;
            trasmitcomplete = 1;
        } else {
            uartbuffer[length] = tempdata;
            length = length + 1;
        }
    }
}

/*
 * Main functions begins
 */
int main(void) {

    gpioinit();
    spiinit();
    uartinit();

    while (1) {

        if (trasmitcomplete) {
            for (int l = 0; l < 10; l++) {
                datas[l] = uartbuffer[l];
            }
            trasmitcomplete = 0;
        }
        Scroll_Text(datas);
        delay1();
    }

    return 0;
}


void delay1(void) {
    unsigned static volatile long i = 0;
    unsigned static volatile long j = 0;
    for (i = 0; i < 200; i++) {
        for (j = 0; j < 400;) {
            j++;
        }
    }
}


