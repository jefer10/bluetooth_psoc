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
#include <stdbool.h>


const char acText[]="hello world\r\n";
FS_FILE *pFile;

volatile char dato_bluetooht;
char temperatura[8];
volatile int32 conteo_total=0 ,numero_de_muestra=6;
volatile int16 tiempo_muestreo=1, tiempo_total=1;
int16 x=0;
int8 dato_proceso;
int fila=0;
int columna=0;
char acBuffer[100];
volatile int8 conteo=0;
char muestra[4];
volatile bool bandera=false;
volatile char  auxiliar;
volatile uint32 contador;
volatile uint32 rpm;
volatile uint32 frecuencia;



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
CY_ISR(frecu){
    conteo=Counter_ReadCounter();
    frecuencia=conteo;
    rpm=60*frecuencia;
    Counter_WriteCounter(0);     
}

CY_ISR(InterrupRx){
    dato_bluetooht=UART_GetChar();//recibe el dato del bluetooth
    switch (dato_bluetooht){
        case 'm':
        {
            bandera=true;
            break;
        }
        case 'f':
        {
            unsigned char i=1;
            tiempo_muestreo=muestra[0];
            while(conteo>i){
                tiempo_muestreo=muestra[i]+10*tiempo_muestreo;        
                i++;
            }
            LCD_Position(1,11);
            LCD_PrintNumber(tiempo_muestreo);
            conteo=0;
            conteo_total=0;
            numero_de_muestra=(60*tiempo_total)/tiempo_muestreo;//60 para manejarlo en minutos por
            bandera=false;
            break;
        }
        
        case 'j':
        {
            unsigned char i=1;
            tiempo_total=muestra[0];
            while(conteo>i){
                tiempo_total=muestra[i]+10*tiempo_total;        
                i++;
            }
            LCD_Position(0,11);
            LCD_PrintNumber(tiempo_total);
            conteo=0;       
            conteo_total=0;
            numero_de_muestra=(60*tiempo_total)/tiempo_muestreo;//60 para manejarlo en minutos por
            bandera=false;
            break;
        }
        default:
        {
            if (bandera==true)
            {
                muestra[conteo]=dato_bluetooht-0x30;
                conteo=conteo+1;  
            }
            break;
        }
    }
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isrRX_StartEx(InterrupRx);
     isr_1_StartEx(frecu);
    //////////////////////////////////////////////////////////////
    UART_Start();
    LCD_Start();
    FS_Init();// Inicia Sistema de archivos
    ADC_Start();
    PWM_Start();
    Counter_Start();
    /////////////////////////////////////////////////////////////////////
    LCD_Position(0,0);
    LCD_PrintString("t total");
    LCD_Position(1,0);
    LCD_PrintString("t muestra");
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
    UART_PutString("add_button(8,6,14,r,r)");
    UART_PutString("\r");
    UART_PutString("add_button(14,6,15,Y,Y)");
    UART_PutString("\r");
    UART_PutString("add_button(2,6,16,G,G)");
    UART_PutString("\r");
    UART_PutString("add_button(2,3,17,B,B)");
    UART_PutString("\r");
    UART_PutString("add_roll_graph(12,7,5,0.0,100.0,100,t,voltaje,X-Axis,Y-Axis,0,0,1,0,0,1,medium,none,1,1,42,97,222)");
    UART_PutString("\r");
    UART_PutString("add_roll_graph(6,7,5,10.0,40.0,100,G,temperatura,X-Axis,Y-Axis,0,0,1,0,0,1,medium,none,1,1,42,255,0)");
    UART_PutString("\r");
    UART_PutString("add_roll_graph(0,7,5,0.0,100.0,100,v,velocidad motor,X-Axis,Y-Axis,1,0,1,0,1,1,medium,none,1,1,255,0,0)");
    UART_PutString("\r");
    UART_PutString("add_monitor(7,1,8,,1)");
    UART_PutString("\r");
    UART_PutString("add_send_box(2,1,3,,m,j)");
    UART_PutString("\r");
    UART_PutString("add_send_box(2,2,3,,m,f)");
    UART_PutString("\r");
    UART_PutString("set_panel_notes(,,,)");
    UART_PutString("\r");
    UART_PutString("run()");
    UART_PutString("\r");
    UART_PutString("*");
    UART_PutString("\r");
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        /////////variables del programa de paneles
    //tiempo_muestreo=500; // time interval in ms for updating panel indicators 
    int16 temp; // Roll Graph trace value
     

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        
        if(numero_de_muestra>conteo_total){    
        
            if(dato_bluetooht=='r'){
            CyDelay(tiempo_muestreo*995);
            ADC_StartConvert();
            ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
            temp=100*ADC_GetResult8()/255; // actualiza Roll Graph
            x=x+1;
            sprintf(temperatura,"*G%d,%d*",temp,x);
            UART_PutString(temperatura);
            
            conteo_total++; 
        }
        if(dato_bluetooht=='Y'){ //Button Pressed
      //<--- Insert button pressed code here 
        }
        if(dato_bluetooht=='G'){ //Button Pressed
      //<--- Insert button pressed code here 
        } 
        }
    }
 }


/* [] END OF FILE */
