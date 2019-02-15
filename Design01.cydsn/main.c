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
#include <stdio.h>
#include <FS.h>
#include <stdbool.h>


const char ctemp[]= "T,";//Temperatura
const char cvolt[]= "V,";//Voltaje
const char cspeed[]="S,";//Speed Velocidad
const char ctime[]= "t\r\n";//

//Variables de manejo de archivos
FS_FILE *pFile;
char file[15]="DATOS0.txt";
char acBuffer[200];
//Varibles de interupcion
volatile char dato_bluetooht='0';
volatile int8 conteo=0;
char muestra[6];

//Variables usadas en el main
char temperatura[16];
char voltaje[16];
char velocidad[16];
volatile int16 conteo_total=0 ,numero_de_muestra=6;
volatile int16 tiempo_muestreo=1, tiempo_total=1;

volatile bool bandera=false;

volatile char  auxiliar;
volatile uint16 cuenta=0;
volatile uint16 frecuencia=100;


void IniciarArchivos(){
    char i=0;
    while(pFile!=0){
        sprintf(file,"DATOS%d.txt",i);
        pFile = FS_FOpen(file,"r");//Intenta abrir el archivo, si puede permanece en el while
        i++;
    }
        pFile = FS_FOpen(file,"a");// Crea el nuevo archivo

}


void WriteHead(char sourse){
    IniciarArchivos();


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
        LCD_PrintString("E1");
    }
  
}

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

void Termino(){

        LCD_ClearDisplay();
        pFile=FS_FOpen(file, "a");
        
        if (pFile != 0) {
                    FS_Write(pFile,"*", strlen("*"));// Escribe la cabecera cotrespondiente a Temp
                }else{
                    LCD_PrintString("Error");
        }
        
        LCD_PrintString("Termino guardado");
        LCD_Position(1,0);
        LCD_PrintString("en : ");
        char j=0;
        while(file[j]!='.'){
        LCD_PutChar(file[j]);
        j++;
        }
        conteo_total=0;
        dato_bluetooht='1';

}


CY_ISR(ISR_SW){
    cuenta++;
    Col_ClearInterrupt();
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
    i=3;
    while(acBuffer[i+1]!='*'){
        if(acBuffer[i]=='\n'){
        sprintf(temperatura,"*%c",aux);
        UART_PutString(temperatura);
        
        }else if(acBuffer[i]=='\r'){
        UART_PutString("*");
        }else{
        sprintf(temperatura,"%c",acBuffer[i]);
        UART_PutString(temperatura);
        }
        i++;
            
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
        case 't':{
            LCD_ClearDisplay();
            LCD_PrintString("Inicio Tempe");            
            WriteHead(0x00);//Crea cabecera de temperatura
            break;
        }
        case 's':{
            LCD_ClearDisplay();
            LCD_PrintString("Inicio Voltaje");            
            WriteHead(0x01);//Crea cabecera de temperatura
            break;
        }
        case 'v':{
            LCD_ClearDisplay();
            LCD_PrintString("Inicio Velocidad");
            WriteHead(0x02);//Crea cabecera de temperatura
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
            LCD_ClearDisplay();
            LCD_PrintString("Borro");
            LCD_Position(1,0);
            char j=0;
            while(file[j]!='.'){
            LCD_PutChar(file[j]);
            j++;
            }
            FS_Remove (file);//Borra el ultimo archivo
            break;
        }
        
        case 'e':{
            LCD_ClearDisplay();
            LCD_PrintString("Leyendo");
            LCD_Position(1,0);
            char j=0;
            while(file[j]!='.'){
            LCD_PutChar(file[j]);
            j++;
            }
            ReadSD();
            SendFile();
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
    isr_pin_StartEx(ISR_SW);
    //////////////////////////////////////////////////////////////
    UART_Start();
    LCD_Start();
    FS_Init();// Inicia Sistema de archivos
    ADC_Start();
    ADC2_Start();
    IniciarArchivos();
    LCD_Position(0,0);
    LCD_PrintString("t total    1min");
    LCD_Position(1,0);
    LCD_PrintString("t muestra 10seg");
    //////////////////////////////////////////////////////////////////////////////////////

///////////// Build panel in app

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
    UART_PutString("add_text(8,0,xlarge,L,proyecto Bluetooth,0,255,0,)");
    UART_PutString("\r");
    UART_PutString("add_button(16,3,2,e,)");//Carga el archivo
    UART_PutString("\r");
    UART_PutString("add_button(16,4,6,d,)");//Borra ultimo archivo
    UART_PutString("\r");
    UART_PutString("add_button(8,6,14,t,)");
    UART_PutString("\r");
    UART_PutString("add_button(14,6,15,s,)");
    UART_PutString("\r");
    UART_PutString("add_button(2,6,17,v,)");
    UART_PutString("\r");
    UART_PutString("add_roll_graph(12,7,5,0,5000,100,V,voltaje,X-Axis,Y-Axis,0,0,1,0,0,1,medium,none,1,1,42,97,222)");
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
    //UART_PutString("set_panel_notes(,,,)");
    //UART_PutString("\r");
    UART_PutString("run()");
    UART_PutString("\r");
    UART_PutString("*");
    UART_PutString("\r");




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        /////////variables del programa de paneles
    int16 temp; // Roll Graph trace value
    

    for(;;)
    {
        if(dato_bluetooht=='t'){
            if(numero_de_muestra!=conteo_total){
                CyDelay(tiempo_muestreo*995);
                ADC_StartConvert();
                ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
                temp=80*ADC_GetResult8()/255; // actualiza Roll Graph
                sprintf(temperatura,"*T%d,%d*",temp,conteo_total);
                UART_PutString(temperatura);
                sprintf(temperatura,"%d,%d\r\n",temp,conteo_total);
                pFile=FS_FOpen(file, "a");
                if (pFile != 0) {
                    FS_Write(pFile, temperatura, strlen(temperatura));// Escribe la cabecera cotrespondiente a Temp
                }else{
                    LCD_PrintString("Error");
                }
                FS_FClose(pFile);
                conteo_total++; 
            }else{
                    Termino();
            }  
        }
        if(dato_bluetooht=='s')
        {
            if(numero_de_muestra!=conteo_total){
                CyDelay(tiempo_muestreo*995);
                ADC2_StartConvert();
                ADC2_IsEndConversion(ADC2_WAIT_FOR_RESULT);
                temp=5000*ADC2_GetResult16()/255; // actualiza Roll Graph
                sprintf(voltaje,"*V%d,%d*",temp,conteo_total);
                UART_PutString(voltaje);
                sprintf(voltaje,"%d,%d\r\n",temp,conteo_total);
                pFile=FS_FOpen(file, "a");
                if (pFile != 0) {
                    FS_Write(pFile, voltaje, strlen(voltaje));// Escribe la cabecera cotrespondiente a Temp
                }else{
                    LCD_PrintString("Error");
                }
                FS_FClose(pFile);
                conteo_total++; 
            }else{
                Termino();
            }
        }
        if(dato_bluetooht=='v')
        {
            if(numero_de_muestra!=conteo_total){
                cuenta=0;
                CyDelay(tiempo_muestreo*995);
                frecuencia=cuenta*60*tiempo_muestreo;
                sprintf(velocidad,"*S%i,%d*",frecuencia,conteo_total);
                UART_PutString(velocidad);
                sprintf(velocidad,"%d,%i\r\n",frecuencia,conteo_total);
                pFile=FS_FOpen(file, "a");
                if (pFile != 0) {
                    FS_Write(pFile, velocidad, strlen(velocidad));// Escribe la cabecera cotrespondiente a Temp
                }else{
                    LCD_PrintString("Error");
                }
                FS_FClose(pFile);
                conteo_total++; 
            }else{
                Termino();
            }
        
        }

    }
 }


/* [] END OF FILE */
