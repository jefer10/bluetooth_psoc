/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
char dato_bluetooht;
int fila=0;
int columna=0;

CY_ISR(InterrupRx){
    dato_bluetooht=UART_GetChar();//recibe el dato del bluetooth
    fila=fila + 1;
    if(fila==15){
        fila=0;
        columna=columna+1;
    }
    if(columna>1){
        columna=0;
    }
    LCD_Position(columna,fila);
    LCD_PutChar(dato_bluetooht);
    UART_PutChar(dato_bluetooht);///envio el dato recibido por bluetooth
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isrRX_StartEx(InterrupRx);
    //////////////////////////////////////////////////////////////
    UART_Start();
    LCD_Start();
    LCD_Position(0,0);
    LCD_PrintString("CBluetooth");
    
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
