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
#include <FS.h>

const char acText[]="hello world\r\n";
FS_FILE *pFile;

char dato_bluetooht;
int fila=0;
int columna=0;
char acBuffer[100];



void WriteSD(){
    pFile = FS_FOpen("datos.txt", "a");
    if (pFile != 0) {
        LCD_PrintString("Si escribio");
        FS_Write(pFile, acText, strlen(acText));
        FS_FClose(pFile);
    }else{
        LCD_PrintString("No escribio");
    }
}

void ReadSD(){
    pFile = FS_FOpen("datos.txt", "r");
    int i;
    if (pFile != 0) {
        LCD_Position(0,0);
            do{
            i = FS_FRead(acBuffer, 1, sizeof(acBuffer) - 1, pFile);
            }while (i);
            FS_FClose(pFile);
            i=0;
            while(acBuffer[i]!='\r'){
                LCD_PutChar(acBuffer[i]);
                i++;
            }
    }
}


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
    FS_Init();// Inicia Sistema de archivos
    LCD_Position(0,0);
    LCD_PrintString("CBluetooth");
    
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
