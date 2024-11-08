/*******************************************************************************
**                                                                            **
** Copyright (C) Infineon Technologies (2013)                                 **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to Infineon       **
** Technologies. Passing on and copying of this document, and communication   **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME  : FlsLoader_Cfg.h                                               **
**                                                                            **
**  $CC VERSION : \main\51 $                                                 **
**                                                                            **
**  DATE, TIME: 2021-08-05, 17:01:17                                      **
**                                                                            **
**  GENERATOR : Build b141014-0350                                          **
**                                                                            **
**  AUTHOR    : DL-AUTOSAR-Engineering                                        **
**                                                                            **
**  VENDOR    : Infineon Technologies                                         **
**                                                                            **
**  DESCRIPTION : This file contains                                          **
**                Code template for FlsLoader_Cfg.h file                      **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: No                                       **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
TRACEABILITY : [cover parentID=SAS_NAS_FLSLOADER_PR914,SAS_NAS_FLSLOADER_PR915,
               SAS_AS4XX_FLSLOADER_PR678,SAS_NAS_FLSLOADER_PR916] [/cover]
*******************************************************************************/
#ifndef FLSLOADER_CFG_H
#define FLSLOADER_CFG_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
/* Offset of PFlashX  */
#define FLSLOADER_PF_S0   (0x000000U)
#define FLSLOADER_PF_S1   (0x004000U)
#define FLSLOADER_PF_S2   (0x008000U)
#define FLSLOADER_PF_S3   (0x00C000U)
#define FLSLOADER_PF_S4   (0x010000U)
#define FLSLOADER_PF_S5   (0x014000U)
#define FLSLOADER_PF_S6   (0x018000U)
#define FLSLOADER_PF_S7   (0x01C000U)
#define FLSLOADER_PF_S8   (0x020000U)
#define FLSLOADER_PF_S9   (0x028000U)
#define FLSLOADER_PF_S10  (0x030000U)
#define FLSLOADER_PF_S11  (0x038000U)
#define FLSLOADER_PF_S12  (0x040000U)
#define FLSLOADER_PF_S13  (0x048000U)
#define FLSLOADER_PF_S14  (0x050000U)
#define FLSLOADER_PF_S15  (0x058000U)
#define FLSLOADER_PF_S16  (0x060000U)
#define FLSLOADER_PF_S17  (0x070000U)
#define FLSLOADER_PF_S18  (0x080000U)
#define FLSLOADER_PF_S19  (0x090000U)
#define FLSLOADER_PF_S20  (0x0A0000U)
#define FLSLOADER_PF_S21  (0x0C0000U)
#define FLSLOADER_PF_S22  (0x0E0000U)
#define FLSLOADER_PF_S23  (0x100000U)
#define FLSLOADER_PF_S24  (0x140000U)
#define FLSLOADER_PF_S25  (0x180000U)
#define FLSLOADER_PF_S26  (0x1C0000U)

/* Higher 5 nibbles of Data Flash */
#define FLSLOADER_DF_S0   (0xAF000)
#define FLSLOADER_DF_S1   (0xAF010)
#define FLSLOADER_DF_S2   (0xAF020)
#define FLSLOADER_DF_S3   (0xAF030)
#define FLSLOADER_DF_S4   (0xAF040)
#define FLSLOADER_DF_S5   (0xAF050)
#define FLSLOADER_TC26X   (1U)
#define FLSLOADER_TC27X   (2U)
#define FLSLOADER_TC29X   (3U)
#define FLSLOADER_TC23X   (4U)
#define FLSLOADER_TC22X   (5U)
#define FLSLOADER_TC21X   (6U)

#define NO_PROTECTION     (0U)
#define READ_PROTECTION   (1U)
#define WRITE_PROTECTION  (2U)
#define WOP_PROTECTION    (3U)
#define OTP_PROTECTION    (4U)


#define FLSLOADER_DERIVATIVE             (FLSLOADER_TC23X)
#define FLSLOADER_PROCON_CHECK          ((uint32)(0x7FFFFFFU))








/* FLASH LOADER module instance ID */
#define FLSLOADER_INSTANCE_ID            ((uint8)1U)

/* Software version numbers of the module.*/

/* Vendor specific implementation version information */
#define FLSLOADER_SW_MAJOR_VERSION       (2U)
#define FLSLOADER_SW_MINOR_VERSION       (2U)
#define FLSLOADER_SW_PATCH_VERSION       (9U)


 
#define FLSLOADER_PFLASH0_START_ADDRESS  ((FlsLoader_AddressType)(0xa0000000U))

 

#define FLSLOADER_DFLASH0_START_ADDRESS  ((FlsLoader_AddressType)(0xaf000000U))

#define FLSLOADER_DFLASH0_END            ((FlsLoader_AddressType)(0xaf01ffffU))
#define FLSLOADER_DFLASH_BANK_SIZE       (0x20000U)

#define FLSLOADER_PFLASH0_END            ((FlsLoader_AddressType)(0xa01fffffU))
#define FLSLOADER_PFLASH0_SIZE           (0x200000U)




/* UCB PFLASH address */
#define FLSLOADER_UCB_PFLASH             ((FlsLoader_AddressType)(0xaf100000U))
/* UCB DFLASH address */
#define FLSLOADER_UCB_DFLASH             ((FlsLoader_AddressType)(0xaf100400U))
/* UCB DFLASH address */
#define FLSLOADER_UCB_OTP                ((FlsLoader_AddressType)(0xaf100c00U))

/* UCB Start address */
#define FLSLOADER_UCB_START_ADDRESS      ((FlsLoader_AddressType)(0xaf100000U))

/* UCB End address */
#define FLSLOADER_UCB_END_ADDRESS        ((FlsLoader_AddressType)(0xaf103fffU))


#define FLSLOADER_NUM_OF_PF0_SECTORS     (27U)

#define FLSLOADER_NUM_OF_DF_SECTORS      (16U) 

#define FLSLOADER_PF_BANKS                (1U)

#define FLSLOADER_FLASH_BUSY_MASK       (0x0000000AU)

/*
Configuration: FLSLOADER_DEV_ERROR_DETECT
Preprocessor switch to enable/disable the development error detection and
reporting.
-If ON, Enable development error detecion
-If OFF,Disable development error detecion
*/
#define FLSLOADER_DEV_ERROR_DETECT       (STD_OFF)
/*
Configuration: FLSLOADER_ENABLE_LOCKCHECK
Preprocessor switch to enable/disable the Lockcheck
The erase and write API's has Lock Check functionality, substantially
this is a big section, user could remove the same, if locking is not
used.
-If ON, Enable the FlsLoader Lockcheck in Write / Erase API's
-If OFF,Disable the FlsLoader Lockcheck in Write / Erase API's
*/
#define FLSLOADER_ENABLE_LOCKCHECK       (STD_OFF)
/*
Configuration: FLSLOADER_DEINIT_API
Preprocessor switch to enable/disable the deinitilize FlsLoader API
-If ON, Enable FlsLoader Deinitilization API
-If OFF,Disable FlsLoader Deinitilization API
*/
#define FLSLOADER_DEINIT_API             (STD_OFF)

/*
Configuration: FLSLOADER_LOCK_UNLOCK_API
Preprocessor switch to enable/disable the Lock/UnLock API
-If ON, Enable the FlsLoader Lock/UnLock API
-If OFF,Disable FlsLoader Lock/UnLock API
*/
#define FLSLOADER_LOCK_UNLOCK_API        (STD_OFF)
/*
Configuration: FLSLOADER_VERSION_INFO_API
Preprocessor switch to enable/disable the versioninfo API
-If ON, Include the FlsLoader GetVersionInfo API
-If OFF,Disable FlsLoader GetVersionInfo API
*/
#define FLSLOADER_VERSION_INFO_API       (STD_OFF)

/* Configuration: FlsLoader PFlash0 Protection configuration */
#define FLSLOADER_PF0_PROT         (NO_PROTECTION)

/* Configuration:this device variant does not have Pf1Sectors so FLSLOADER_PF1_PROT 
  generated with default value NO_PROTECTION  */
#define FLSLOADER_PF1_PROT        (NO_PROTECTION)

/* Configuration:this device variant does not have Pf2Sectors so FLSLOADER_PF2_PROT 
  generated with default value NO_PROTECTION  */
#define FLSLOADER_PF2_PROT         (NO_PROTECTION)

/* Configuration:this device variant does not have Pf3Sectors so FLSLOADER_PF3_PROT 
  generated with default value NO_PROTECTION   */
#define FLSLOADER_PF3_PROT         (NO_PROTECTION)


/*
Configuration: FlsLoader DFLASH0 Read/Write Protection configuration */
#define FLSLOADER_DF0_PROT        (NO_PROTECTION)


#if ((FLSLOADER_PF0_PROT == READ_PROTECTION )|| \
  (FLSLOADER_PF0_PROT == WRITE_PROTECTION)|| \
  (FLSLOADER_PF0_PROT == OTP_PROTECTION )|| \
  (FLSLOADER_PF0_PROT == WOP_PROTECTION)\
)
/* PFLASH0 UCB 4 double word password*/
#define FLSLOADER_PF0_PW00   ((uint32)0x00000000U)
#define FLSLOADER_PF0_PW01   ((uint32)0x00000000U)
#define FLSLOADER_PF0_PW10   ((uint32)0x00000000U)
#define FLSLOADER_PF0_PW11   ((uint32)0x00000000U)
#define FLSLOADER_PF0_PW20   ((uint32)0x00000000U)
#define FLSLOADER_PF0_PW21   ((uint32)0x00000000U)
#define FLSLOADER_PF0_PW30   ((uint32)0x00000000U)
#define FLSLOADER_PF0_PW31   ((uint32)0x00000000U)



/* Passwords in the form of Bytes used in array to fill in UCB_PFLASH  */
#define FLSLOADER_P0_PW00_B0 ((uint8)0x00U)
#define FLSLOADER_P0_PW00_B1 ((uint8)0x00U)
#define FLSLOADER_P0_PW00_B2 ((uint8)0x00U)
#define FLSLOADER_P0_PW00_B3 ((uint8)0x00U)

#define FLSLOADER_P0_PW01_B0 ((uint8)0x00U)
#define FLSLOADER_P0_PW01_B1 ((uint8)0x00U)
#define FLSLOADER_P0_PW01_B2 ((uint8)0x00U)
#define FLSLOADER_P0_PW01_B3 ((uint8)0x00U)

#define FLSLOADER_P0_PW10_B0 ((uint8)0x00U)
#define FLSLOADER_P0_PW10_B1 ((uint8)0x00U)
#define FLSLOADER_P0_PW10_B2 ((uint8)0x00U)
#define FLSLOADER_P0_PW10_B3 ((uint8)0x00U)

#define FLSLOADER_P0_PW11_B0 ((uint8)0x00U)
#define FLSLOADER_P0_PW11_B1 ((uint8)0x00U)
#define FLSLOADER_P0_PW11_B2 ((uint8)0x00U)
#define FLSLOADER_P0_PW11_B3 ((uint8)0x00U)

#define FLSLOADER_P0_PW20_B0 ((uint8)0x00U)
#define FLSLOADER_P0_PW20_B1 ((uint8)0x00U)
#define FLSLOADER_P0_PW20_B2 ((uint8)0x00U)
#define FLSLOADER_P0_PW20_B3 ((uint8)0x00U)

#define FLSLOADER_P0_PW21_B0 ((uint8)0x00U)
#define FLSLOADER_P0_PW21_B1 ((uint8)0x00U)
#define FLSLOADER_P0_PW21_B2 ((uint8)0x00U)
#define FLSLOADER_P0_PW21_B3 ((uint8)0x00U)

#define FLSLOADER_P0_PW30_B0 ((uint8)0x00U)
#define FLSLOADER_P0_PW30_B1 ((uint8)0x00U)
#define FLSLOADER_P0_PW30_B2 ((uint8)0x00U)
#define FLSLOADER_P0_PW30_B3 ((uint8)0x00U)

#define FLSLOADER_P0_PW31_B0 ((uint8)0x00U)
#define FLSLOADER_P0_PW31_B1 ((uint8)0x00U)
#define FLSLOADER_P0_PW31_B2 ((uint8)0x00U)
#define FLSLOADER_P0_PW31_B3 ((uint8)0x00U)

/* PFLASH0 Read/write protection for Sector x*/




#define FLSLOADER_PROCONP0_BYTE0 ((uint8)0)
#define FLSLOADER_PROCONP0_BYTE1 ((uint8)0)
#define FLSLOADER_PROCONP0_BYTE2 ((uint8)0)
#define FLSLOADER_PROCONP0_BYTE3 ((uint8)0)

#define FLSLOADER_PROCONOTP0_BYTE0 ((uint8)0)
#define FLSLOADER_PROCONOTP0_BYTE1 ((uint8)0)
#define FLSLOADER_PROCONOTP0_BYTE2 ((uint8)0)
#define FLSLOADER_PROCONOTP0_BYTE3 ((uint8)0)

#define FLSLOADER_PROCONWOP0_BYTE0 ((uint8)0)
#define FLSLOADER_PROCONWOP0_BYTE1 ((uint8)0)
#define FLSLOADER_PROCONWOP0_BYTE2 ((uint8)0)
#define FLSLOADER_PROCONWOP0_BYTE3 ((uint8)0)


#define FLSLOADER_PROCONP1_BYTE0 ((uint8)0) 
#define FLSLOADER_PROCONP1_BYTE1 ((uint8)0) 
#define FLSLOADER_PROCONP1_BYTE2 ((uint8)0)  
#define FLSLOADER_PROCONP1_BYTE3 ((uint8)0)

#define FLSLOADER_PROCONOTP1_BYTE0 ((uint8)0) 
#define FLSLOADER_PROCONOTP1_BYTE1 ((uint8)0) 
#define FLSLOADER_PROCONOTP1_BYTE2 ((uint8)0)  
#define FLSLOADER_PROCONOTP1_BYTE3 ((uint8)0)

#define FLSLOADER_PROCONWOP1_BYTE0 ((uint8)0) 
#define FLSLOADER_PROCONWOP1_BYTE1 ((uint8)0) 
#define FLSLOADER_PROCONWOP1_BYTE2 ((uint8)0)  
#define FLSLOADER_PROCONWOP1_BYTE3 ((uint8)0)



#define FLSLOADER_PROCONP2_BYTE0 ((uint8)0)  
#define FLSLOADER_PROCONP2_BYTE1 ((uint8)0)  
#define FLSLOADER_PROCONP2_BYTE2 ((uint8)0)   
#define FLSLOADER_PROCONP2_BYTE3 ((uint8)0) 

#define FLSLOADER_PROCONOTP2_BYTE0 ((uint8)0)  
#define FLSLOADER_PROCONOTP2_BYTE1 ((uint8)0)  
#define FLSLOADER_PROCONOTP2_BYTE2 ((uint8)0)   
#define FLSLOADER_PROCONOTP2_BYTE3 ((uint8)0) 

#define FLSLOADER_PROCONWOP2_BYTE0 ((uint8)0)  
#define FLSLOADER_PROCONWOP2_BYTE1 ((uint8)0)  
#define FLSLOADER_PROCONWOP2_BYTE2 ((uint8)0)   
#define FLSLOADER_PROCONWOP2_BYTE3 ((uint8)0) 



#define FLSLOADER_PROCONP3_BYTE0 ((uint8)0) 
#define FLSLOADER_PROCONP3_BYTE1 ((uint8)0) 
#define FLSLOADER_PROCONP3_BYTE2 ((uint8)0)  
#define FLSLOADER_PROCONP3_BYTE3 ((uint8)0)

#define FLSLOADER_PROCONOTP3_BYTE0 ((uint8)0) 
#define FLSLOADER_PROCONOTP3_BYTE1 ((uint8)0) 
#define FLSLOADER_PROCONOTP3_BYTE2 ((uint8)0)  
#define FLSLOADER_PROCONOTP3_BYTE3 ((uint8)0)

#define FLSLOADER_PROCONWOP3_BYTE0 ((uint8)0) 
#define FLSLOADER_PROCONWOP3_BYTE1 ((uint8)0) 
#define FLSLOADER_PROCONWOP3_BYTE2 ((uint8)0)  
#define FLSLOADER_PROCONWOP3_BYTE3 ((uint8)0)


#endif

#if (FLSLOADER_DF0_PROT == READ_PROTECTION)

#define FLSLOADER_PROCOND_BYTE0 ((uint8)(0x01U))
#define FLSLOADER_PROCOND_BYTE1 ((uint8)(0U))
#define FLSLOADER_PROCOND_BYTE2 ((uint8)(0U))
#define FLSLOADER_PROCOND_BYTE3 ((uint8)(0x80U))

#elif (FLSLOADER_DF0_PROT == WRITE_PROTECTION)
#define FLSLOADER_PROCOND_BYTE0 ((uint8)(0x01U))
#define FLSLOADER_PROCOND_BYTE1 ((uint8)(0U))
#define FLSLOADER_PROCOND_BYTE2 ((uint8)(0U))
#define FLSLOADER_PROCOND_BYTE3 ((uint8)(0U))

#else
#define FLSLOADER_PROCOND_BYTE0 ((uint8)(0U))
#define FLSLOADER_PROCOND_BYTE1 ((uint8)(0U))
#define FLSLOADER_PROCOND_BYTE2 ((uint8)(0U))
#define FLSLOADER_PROCOND_BYTE3 ((uint8)(0U))
#endif

#if (FLSLOADER_DF0_PROT != NO_PROTECTION)
/* DFLASH UCB 4 double word password*/
#define FLSLOADER_DF0_PW00    ((uint32)0x00000000U)
#define FLSLOADER_DF0_PW01    ((uint32)0x00000000U)
#define FLSLOADER_DF0_PW10    ((uint32)0x00000000U)
#define FLSLOADER_DF0_PW11    ((uint32)0x00000000U)
#define FLSLOADER_DF0_PW20    ((uint32)0x00000000U)
#define FLSLOADER_DF0_PW21    ((uint32)0x00000000U)
#define FLSLOADER_DF0_PW30    ((uint32)0x00000000U)
#define FLSLOADER_DF0_PW31    ((uint32)0x00000000U)


/* Passwords in the form of Bytes used in array to fill in UCB_PFLASH  */
#define FLSLOADER_DF0_PW00_B0 ((uint8)0x00U)
#define FLSLOADER_DF0_PW00_B1 ((uint8)0x00U)
#define FLSLOADER_DF0_PW00_B2 ((uint8)0x00U)
#define FLSLOADER_DF0_PW00_B3 ((uint8)0x00U)

#define FLSLOADER_DF0_PW01_B0 ((uint8)0x00U)
#define FLSLOADER_DF0_PW01_B1 ((uint8)0x00U)
#define FLSLOADER_DF0_PW01_B2 ((uint8)0x00U)
#define FLSLOADER_DF0_PW01_B3 ((uint8)0x00U)

#define FLSLOADER_DF0_PW10_B0 ((uint8)0x00U)
#define FLSLOADER_DF0_PW10_B1 ((uint8)0x00U)
#define FLSLOADER_DF0_PW10_B2 ((uint8)0x00U)
#define FLSLOADER_DF0_PW10_B3 ((uint8)0x00U)

#define FLSLOADER_DF0_PW11_B0 ((uint8)0x00U)
#define FLSLOADER_DF0_PW11_B1 ((uint8)0x00U)
#define FLSLOADER_DF0_PW11_B2 ((uint8)0x00U)
#define FLSLOADER_DF0_PW11_B3 ((uint8)0x00U)

#define FLSLOADER_DF0_PW20_B0 ((uint8)0x00U)
#define FLSLOADER_DF0_PW20_B1 ((uint8)0x00U)
#define FLSLOADER_DF0_PW20_B2 ((uint8)0x00U)
#define FLSLOADER_DF0_PW20_B3 ((uint8)0x00U)

#define FLSLOADER_DF0_PW21_B0 ((uint8)0x00U)
#define FLSLOADER_DF0_PW21_B1 ((uint8)0x00U)
#define FLSLOADER_DF0_PW21_B2 ((uint8)0x00U)
#define FLSLOADER_DF0_PW21_B3 ((uint8)0x00U)

#define FLSLOADER_DF0_PW30_B0 ((uint8)0x00U)
#define FLSLOADER_DF0_PW30_B1 ((uint8)0x00U)
#define FLSLOADER_DF0_PW30_B2 ((uint8)0x00U)
#define FLSLOADER_DF0_PW30_B3 ((uint8)0x00U)

#define FLSLOADER_DF0_PW31_B0 ((uint8)0x00U)
#define FLSLOADER_DF0_PW31_B1 ((uint8)0x00U)
#define FLSLOADER_DF0_PW31_B2 ((uint8)0x00U)
#define FLSLOADER_DF0_PW31_B3 ((uint8)0x00U)

#else

/* Passords in the form of Bytes used in array to fill in UCB_PFLASH  */
#define FLSLOADER_DF0_PW00_B0 ((uint8)(0U))
#define FLSLOADER_DF0_PW00_B1 ((uint8)(0U))
#define FLSLOADER_DF0_PW00_B2 ((uint8)(0U))
#define FLSLOADER_DF0_PW00_B3 ((uint8)(0U))

#define FLSLOADER_DF0_PW01_B0 ((uint8)(0U))
#define FLSLOADER_DF0_PW01_B1 ((uint8)(0U))
#define FLSLOADER_DF0_PW01_B2 ((uint8)(0U))
#define FLSLOADER_DF0_PW01_B3 ((uint8)(0U))

#define FLSLOADER_DF0_PW10_B0 ((uint8)(0U))
#define FLSLOADER_DF0_PW10_B1 ((uint8)(0U))
#define FLSLOADER_DF0_PW10_B2 ((uint8)(0U))
#define FLSLOADER_DF0_PW10_B3 ((uint8)(0U))

#define FLSLOADER_DF0_PW11_B0 ((uint8)(0U))
#define FLSLOADER_DF0_PW11_B1 ((uint8)(0U))
#define FLSLOADER_DF0_PW11_B2 ((uint8)(0U))
#define FLSLOADER_DF0_PW11_B3 ((uint8)(0U))

#define FLSLOADER_DF0_PW20_B0 ((uint8)(0U))
#define FLSLOADER_DF0_PW20_B1 ((uint8)(0U))
#define FLSLOADER_DF0_PW20_B2 ((uint8)(0U))
#define FLSLOADER_DF0_PW20_B3 ((uint8)(0U))

#define FLSLOADER_DF0_PW21_B0 ((uint8)(0U))
#define FLSLOADER_DF0_PW21_B1 ((uint8)(0U))
#define FLSLOADER_DF0_PW21_B2 ((uint8)(0U))
#define FLSLOADER_DF0_PW21_B3 ((uint8)(0U))

#define FLSLOADER_DF0_PW30_B0 ((uint8)(0U))
#define FLSLOADER_DF0_PW30_B1 ((uint8)(0U))
#define FLSLOADER_DF0_PW30_B2 ((uint8)(0U))
#define FLSLOADER_DF0_PW30_B3 ((uint8)(0U))

#define FLSLOADER_DF0_PW31_B0 ((uint8)(0U)
#define FLSLOADER_DF0_PW31_B1 ((uint8)(0U)
#define FLSLOADER_DF0_PW31_B2 ((uint8)(0U)
#define FLSLOADER_DF0_PW31_B3 ((uint8)(0U)
#endif



#endif /*FLSLOADER_CFG_H*/
