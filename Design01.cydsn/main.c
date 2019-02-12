/* ========================================
 *
 * Copyright Jeferson Rondon - Nicolas Pastran,2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 *
 * ========================================
*/
#include "project.h"
#include<stdio.h>
#include <FS.h>
#include <stdbool.h>


const char ctemp[]= "T,";//Temperatura
const char cvolt[]= "V,";//Voltaje
const char cspeed[]="S,";//Speed Velocidad
const char ctime[]= "t\r\n";//

//Variables de manejo de archivos
FS_FILE *pFile;
char file[15]="DATOS.txt";
char acBuffer[200];
//Varibles de interupcion
volatile char dato_bluetooht='0';
volatile int8 conteo=0;
char muestra[6];

//Variables usadas en el main
char temperatura[8];
volatile int32 conteo_total=0 ,numero_de_muestra=6;
volatile int16 tiempo_muestreo=1, tiempo_total=1;

volatile bool bandera=false;

int16 x=0;

void WriteHead(char sourse){
    char i=0;
    while(pFile!=0){
        sprintf(file,"DATOS%d.txt",i);
        pFile = FS_FOpen(file,"r");//Intenta abrir el archivo, si puede permanece en el while
        i++;
    }
    pFile = FS_FOpen(file,"a");// Crea el nuevo archivo
    pFile = FS_FOpen(file,"w");// Abre para escribir
    if (pFile != 0) {
        if(sourse==0x00){
        FS_Write(pFile, ctemp, strlen(ctemp));// Escribe la cabecera cotrespondiente a Temp
        }
        else if(sourse==0x01){
        FS_Write(pFile, cvolt, strlen(cvolt));// Escribe la cabecera cotrespondiente a Volt
        }
        else{//0x11
        FS_Write(pFile, cspeed, strlen(cspeed));// Escribe la cabecera cotrespondiente a Velo
        }
        FS_Write(pFile, ctime, strlen(ctime));//Simepre escribe la de tiempo
        FS_FClose(pFile);
    }
    else{
        LCD_PrintString("Eror de escritura");
    }
  
}

/*
void WriteData(){
    FS_FOpen(file, "r");
    char aux[15]="Trama";
    if (pFile != 0) {
        FS_Write(pFile, ctemp, strlen(ctemp));// Escribe la cabecera cotrespondiente a Temp
    }else{
        LCD_PrintString("Eror de escritura");
    }
} // Fin de Write Data Modo de llamar int16 dato;WriteData(&dato,1);
*/
void ReadSD(){
    pFile = FS_FOpen(file,"r");
    int i;
    if (pFile != 0) {
        do{
            i = FS_FRead(acBuffer, 1, sizeof(acBuffer) - 1, pFile);//Lee caracter por caracter y los pasa a la ram
        }while (i);
        FS_FClose(pFile);
    }
}

void SendFile(){
    unsigned int i=0;
    char aux='T';
    while(acBuffer[i]!='\r'){
        switch(acBuffer[i]){
        case 'T':
            aux='T';
            break;
        case 'V':
            aux='V';
            break;
        case 'S':
            aux='S';
            break;
        default:
            break;
        }      
    i++;
    }
    i=1;
    while(sizeof(acBuffer)>i){
            sprintf(temperatura,"*%c%d,%d*",aux,acBuffer[4*i],acBuffer[4*i+1]);
            UART_PutString(temperatura);
    }

}

CY_ISR(InterrupRx){
    dato_bluetooht=UART_GetChar();//recibe el dato del bluetooth
    switch (dato_bluetooht){
        case 'm':
        {
            bandera=true;
            break;
        }
        case 'r':{
            WriteHead(0x00);//Crea cabecera de temperatura
            LCD_ClearDisplay();
            LCD_PrintString("Inicio");            
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
        
        case 'd':{
            FS_Remove (file);//Borra el ultimo archivo
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
    //////////////////////////////////////////////////////////////
    UART_Start();
    LCD_Start();
    FS_Init();// Inicia Sistema de archivos
    ADC_Start();
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
    UART_PutString("add_roll_graph(12,7,5,0.0,100.0,100,V,voltaje,X-Axis,Y-Axis,0,0,1,0,0,1,medium,none,1,1,42,97,222)");
    UART_PutString("\r");
    UART_PutString("add_roll_graph(6,7,5,10.0,40.0,100,T,temperatura,X-Axis,Y-Axis,0,0,1,0,0,1,medium,none,1,1,42,255,0)");
    UART_PutString("\r");
    UART_PutString("add_roll_graph(0,7,5,0.0,100.0,100,S,velocidad motor,X-Axis,Y-Axis,1,0,1,0,1,1,medium,none,1,1,255,0,0)");
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
        if(numero_de_muestra!=conteo_total){
            if(dato_bluetooht=='r'){
            CyDelay(tiempo_muestreo*995);
            ADC_StartConvert();
            ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
            temp=100*ADC_GetResult8()/255; // actualiza Roll Graph
            x=x+1;
            sprintf(temperatura,"*T%d,%d*",temp,x);
            UART_PutString(temperatura);
            sprintf(temperatura,"%d,%d\r\n",temp,x);
            FS_FOpen(file, "w");
            if (pFile != 0) {
                FS_Write(pFile, temperatura, strlen(temperatura));// Escribe la cabecera cotrespondiente a Temp
            }else{
                LCD_PrintString("Error de escritura");
            }
            conteo_total++; 
        }else{
            x=0;
            conteo_total=0;
        } 
        }
    }
 }


/* [] END OF FILE */
