#include <sp_led_matrix.h>
#include "characters.h"

unsigned char length = 0;
static char MatrixBuffer[12] = { 0 };

/*
 * Set the data in corresponding Register
 * MSB 8 bit Register Address and LSB 8 bit Data
 *
 */
void Set_Register(int addr, char data) {
    SPI_TxBufferEmpty(SPI1);
    GPIO_Write(GPIOB, CS_PIN, LOW);
    SPI_Senddata(SPI1, addr << (8) | data);
    SPI_NotBusy(SPI1);
    GPIO_Write(GPIOB, CS_PIN, HIGH);
}

/*
 * Draws the Music Spectrum in 8x8 Matrix
 */
void Draw_Spectrum(char *data1) {

    Set_Register(MAX7219_SHUTDOWN_REG, 0x0);

    //Display all rows (Digits)
    Set_Register(MAX7219_SCANLIMIT_REG, 7);

    //Set corresponding level for corresponding Row (Digits)
    for (int i = 7; i >= 0; i--) {
        Set_Register(MAX7219_DIGIT_REG(i), *data1);
        data1++;
    }

    Set_Register(MAX7219_SHUTDOWN_REG, 0x1);
}

/*
 * Write One Character in Matrix
 *
 */
void Write_Char(char data) {

    Set_Register(MAX7219_SHUTDOWN_REG, 0x0);
    Set_Register(MAX7219_SCANLIMIT_REG, 7);
    char value;

    for (int i = 7; i >= 0; i--) {
        value = characters[(data - 0x41) * 8 + (7 - i)];
        Set_Register(MAX7219_DIGIT_REG(i), value);
    }
    Set_Register(MAX7219_SHUTDOWN_REG, 0x1);
}

/*
 * Scroll Characters
 * *data is pointer to array of characters
 *
 */
void Scroll_Text(char *data) {

    Set_Register(MAX7219_SHUTDOWN_REG, 0x00);
    Set_Register(MAX7219_SCANLIMIT_REG, 7);

    char copyarr[8] = { 0 };

    Copy_Array(*data, characters, copyarr);
    Buffer_Array(copyarr);

    while (*data) {

        for (int d = 0; d < 8; d++) {
            for (int i = 0; i < 8; i++) {
                Set_Register(MAX7219_DIGIT_REG(i), MatrixBuffer[i]);
            }
            Shift_Array(copyarr, copyarr);

            if (d > 6) {
                Copy_Array(*(data + 1), characters, copyarr);
                Buffer_Array(copyarr);
            }
            delay1();
            Set_Register(MAX7219_SHUTDOWN_REG, 0x1);
        }
        data++;
    }
}


//Copy the Array from font (size 4) table to a local array
void Copy_Array(char index, char *orig, char *copy) {

    /*
    if (index == 0) {
        for (int p = 0; p < 8; p++) {
            *copy = *(orig + p);
            copy++;
        }
    } else {
    */
        for (int p = 0; p < 4; p++) {
            *copy = characters[(index - 0x41) * 4 + p];
            copy++;
        }
}

/*
 * Copy the array from local array to MatrixBuffer[]
 * Only the content in MatrixBuffer is displayed
 *
 */
void Buffer_Array(char *orig) {
    for (int p = 0; p < 8; p++) {
        MatrixBuffer[p + 8] = orig[p];
    }
}


/*
 * Shift the buffer array in to Left
 *
 */
void Shift_Array(char *orig, char *copy) {

    for (int s = 0; s < 12; s++) {
        MatrixBuffer[s] = MatrixBuffer[s + 1];
    }
}

