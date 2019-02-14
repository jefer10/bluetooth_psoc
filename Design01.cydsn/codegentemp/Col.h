/*******************************************************************************
* File Name: Col.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Col_H) /* Pins Col_H */
#define CY_PINS_Col_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Col_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Col__PORT == 15 && ((Col__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Col_Write(uint8 value);
void    Col_SetDriveMode(uint8 mode);
uint8   Col_ReadDataReg(void);
uint8   Col_Read(void);
void    Col_SetInterruptMode(uint16 position, uint16 mode);
uint8   Col_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Col_SetDriveMode() function.
     *  @{
     */
        #define Col_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Col_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Col_DM_RES_UP          PIN_DM_RES_UP
        #define Col_DM_RES_DWN         PIN_DM_RES_DWN
        #define Col_DM_OD_LO           PIN_DM_OD_LO
        #define Col_DM_OD_HI           PIN_DM_OD_HI
        #define Col_DM_STRONG          PIN_DM_STRONG
        #define Col_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Col_MASK               Col__MASK
#define Col_SHIFT              Col__SHIFT
#define Col_WIDTH              1u

/* Interrupt constants */
#if defined(Col__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Col_SetInterruptMode() function.
     *  @{
     */
        #define Col_INTR_NONE      (uint16)(0x0000u)
        #define Col_INTR_RISING    (uint16)(0x0001u)
        #define Col_INTR_FALLING   (uint16)(0x0002u)
        #define Col_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Col_INTR_MASK      (0x01u) 
#endif /* (Col__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Col_PS                     (* (reg8 *) Col__PS)
/* Data Register */
#define Col_DR                     (* (reg8 *) Col__DR)
/* Port Number */
#define Col_PRT_NUM                (* (reg8 *) Col__PRT) 
/* Connect to Analog Globals */                                                  
#define Col_AG                     (* (reg8 *) Col__AG)                       
/* Analog MUX bux enable */
#define Col_AMUX                   (* (reg8 *) Col__AMUX) 
/* Bidirectional Enable */                                                        
#define Col_BIE                    (* (reg8 *) Col__BIE)
/* Bit-mask for Aliased Register Access */
#define Col_BIT_MASK               (* (reg8 *) Col__BIT_MASK)
/* Bypass Enable */
#define Col_BYP                    (* (reg8 *) Col__BYP)
/* Port wide control signals */                                                   
#define Col_CTL                    (* (reg8 *) Col__CTL)
/* Drive Modes */
#define Col_DM0                    (* (reg8 *) Col__DM0) 
#define Col_DM1                    (* (reg8 *) Col__DM1)
#define Col_DM2                    (* (reg8 *) Col__DM2) 
/* Input Buffer Disable Override */
#define Col_INP_DIS                (* (reg8 *) Col__INP_DIS)
/* LCD Common or Segment Drive */
#define Col_LCD_COM_SEG            (* (reg8 *) Col__LCD_COM_SEG)
/* Enable Segment LCD */
#define Col_LCD_EN                 (* (reg8 *) Col__LCD_EN)
/* Slew Rate Control */
#define Col_SLW                    (* (reg8 *) Col__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Col_PRTDSI__CAPS_SEL       (* (reg8 *) Col__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Col_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Col__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Col_PRTDSI__OE_SEL0        (* (reg8 *) Col__PRTDSI__OE_SEL0) 
#define Col_PRTDSI__OE_SEL1        (* (reg8 *) Col__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Col_PRTDSI__OUT_SEL0       (* (reg8 *) Col__PRTDSI__OUT_SEL0) 
#define Col_PRTDSI__OUT_SEL1       (* (reg8 *) Col__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Col_PRTDSI__SYNC_OUT       (* (reg8 *) Col__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Col__SIO_CFG)
    #define Col_SIO_HYST_EN        (* (reg8 *) Col__SIO_HYST_EN)
    #define Col_SIO_REG_HIFREQ     (* (reg8 *) Col__SIO_REG_HIFREQ)
    #define Col_SIO_CFG            (* (reg8 *) Col__SIO_CFG)
    #define Col_SIO_DIFF           (* (reg8 *) Col__SIO_DIFF)
#endif /* (Col__SIO_CFG) */

/* Interrupt Registers */
#if defined(Col__INTSTAT)
    #define Col_INTSTAT            (* (reg8 *) Col__INTSTAT)
    #define Col_SNAP               (* (reg8 *) Col__SNAP)
    
	#define Col_0_INTTYPE_REG 		(* (reg8 *) Col__0__INTTYPE)
#endif /* (Col__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Col_H */


/* [] END OF FILE */
