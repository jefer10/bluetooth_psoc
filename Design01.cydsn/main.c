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
#include<stdio.h>
#include <FS.h>


const char acText[]="hello world\r\n";
FS_FILE *pFile;

char dato_bluetooht;
char temperatura[8];
int16 x=0;
int8 dato_proceso;
int fila=0;
int columna=0;
char acBuffer[100];
int8 conteo=0;
int8 conteo1=0;
char muestra[4];
int16 tiempo_caja=500;
int16 muestra_caja;
volatile char  auxiliar;

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
    
    /*if((dato_bluetooht=='m')||(conteo>=1))
    {
        auxiliar=UART_GetChar(); 
        muestra[conteo]=auxiliar -0x30;
        conteo=conteo+1;
        if (conteo>=3)
        {
            conteo=0;
            tiempo_caja=(muestra[0]*1000)+(muestra[1]*100)+(muestra[2]*10)+(muestra[3]);
            
        }
        
    }
    if((dato_bluetooht='T')||(conteo1>=1))
    {
        auxiliar=UART_GetChar();
        muestra[conteo]=auxiliar-0x30;
        conteo1=conteo1+1;
        if(conteo1>3)
        {
            conteo1=0;
            muestra_caja=(muestra[0]*1000)+(muestra[1]*100)+(muestra[2]*10)+(muestra[3]);
        }
    }
    */
    /*
    fila=fila + 1;
    if(fila==15){
        fila=0;
        columna=columna+1;
    }
    if(columna>1){
        columna=0;
    }
    LCD_Position(columna,fila);
    LCD_PutChar(dato_bluetooht);*/
   //UART_PutChar(dato_bluetooht);///envio el dato recibido por bluetooth
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isrRX_StartEx(InterrupRx);
    //////////////////////////////////////////////////////////////
    UART_Start();
    LCD_Start();
    FS_Init();// Inicia Sistema de archivos
    ADC_Start();
    LCD_Position(0,0);
    LCD_PrintString("CBluetooth");
    //////////////////////////////////////////////////////////////////////////////////////
                        /////construccion del panel en la app
    UART_PutString("*.kwl");
    UART_PutString("\r");
    UART_PutString("clear_panel()");
    UART_PutString("\r");
    UART_PutString("set_grid_size(17,10)");
    UART_PutString("\r");
    UART_PutString("add_text(0,2,medium,L,muestras,245,240,245,)");
    UART_PutString("\r");
    UART_PutString("add_text(0,1,medium,L,tiempo,245,240,245,)");
    UART_PutString("\r");
    UART_PutString("add_text(0,3,medium,L,start,255,255,255,)");
    UART_PutString("\r");
    UART_PutString("add_text(8,0,xlarge,L,proyecto Bluetooth,0,255,0,)");
    UART_PutString("\r");
    UART_PutString("add_button(16,1,21,1,4)");
    UART_PutString("\r");
    UART_PutString("add_button(16,2,22,2,5)");
    UART_PutString("\r");
    UART_PutString("add_button(16,3,23,3,6)");
    UART_PutString("\r");
    UART_PutString("add_button(16,4,7,O,o)");
    UART_PutString("\r");
    UART_PutString("add_button(8,6,14,r,)");
    UART_PutString("\r");
    UART_PutString("add_button(14,6,15,Y,)");
    UART_PutString("\r");
    UART_PutString("add_button(2,6,16,G,)");
    UART_PutString("\r");
    UART_PutString("add_button(2,3,17,B,)");
    UART_PutString("\r");
    UART_PutString("add_roll_graph(12,7,5,0.0,100.0,100,t,voltaje,X-Axis,Y-Axis,0,0,1,0,0,1,medium,none,1,1,42,97,222)");
    UART_PutString("\r");
    UART_PutString("add_roll_graph(6,7,5,10.0,40.0,100,G,temperatura,X-Axis,Y-Axis,0,0,1,0,0,1,medium,none,1,1,42,255,0)");
    UART_PutString("\r");
    UART_PutString("add_roll_graph(0,7,5,0.0,100.0,100,v,velocidad motor,X-Axis,Y-Axis,1,0,1,0,1,1,medium,none,1,1,255,0,0)");
    UART_PutString("\r");
    UART_PutString("add_monitor(7,1,8,,1)");
    UART_PutString("\r");
    UART_PutString("add_send_box(2,1,3,,m,)");
    UART_PutString("\r");
    UART_PutString("add_send_box(2,2,3,,T,)");
    UART_PutString("\r");
    UART_PutString("set_panel_notes(,,,)");
    UART_PutString("\r");
    UART_PutString("run()");
    UART_PutString("\r");
    UART_PutString("*");
    UART_PutString("\r");
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        /////////variables del programa de paneles
    //tiempo_caja=500; // time interval in ms for updating panel indicators 
    unsigned long last_time=0; // time of last update
    int16 temp; // Roll Graph trace value
    unsigned long t=0;
     

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
                /* Place your application code here. */
        
        if(dato_bluetooht=='S'){
            
        }      
        
        if(dato_bluetooht=='O'){ //Button Pressed
      //<--- Insert button pressed code here 
        }
        
        if(dato_bluetooht=='r'){
            ADC_StartConvert();
            ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
            temp=100*ADC_GetResult8()/255;
            
            // actualiza Roll Graph
            x=x+1;
            sprintf(temperatura,"*G%d,%d*",temp,x);
            UART_PutString(temperatura);
        }
        if(dato_bluetooht=='Y'){ //Button Pressed
      //<--- Insert button pressed code here 
        }
        
        if(dato_bluetooht=='G'){ //Button Pressed
      //<--- Insert button pressed code here 
        }
        if(dato_bluetooht=='B'){ //Button Pressed
      //<--- Insert button pressed code here 
        }
        CyDelay(tiempo_caja);
        LCD_Position(0,0);
        LCD_PutChar(dato_bluetooht);
            
        }

    }


/* [] END OF FILE */
