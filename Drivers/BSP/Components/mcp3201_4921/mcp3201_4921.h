/**
 ******************************************************************************
 * @file    lis2dtw12.h
 * @author  MEMS Software Solutions Team
 * @brief   MCP3201_4921 header driver file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MCP3201_4921_H
#define MCP3201_4921_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup MCP3201_4921 MCP3201_4921
 * @{
 */

/** @defgroup MCP3201_4921_Exported_Types MCP3201_4921 Exported Types
 * @{
 */

typedef int32_t (*MCP3201_4921_Init_Func)(void);
typedef int32_t (*MCP3201_4921_DeInit_Func)(void);
typedef int32_t (*MCP3201_4921_GetTick_Func)(void);
typedef void (*MCP3201_4921_Enable_Func)(uint8_t);
typedef void (*MCP3201_4921_Delay_Func)(uint32_t);
typedef int32_t (*MCP3201_4921_WriteReg_Func)(uint16_t, uint16_t, uint8_t*,
		uint16_t);
typedef int32_t (*MCP3201_4921_ReadReg_Func)(uint16_t, uint16_t, uint8_t*,
		uint16_t);

typedef int32_t (*dev_write_ptr)(void*, uint8_t, uint8_t*, uint16_t);
typedef int32_t (*dev_read_ptr)(void*, uint8_t, uint8_t*, uint16_t);
typedef void (*dev_mdelay_ptr)(uint32_t millisec);
typedef void (*dev_enable_ptr)(uint8_t);

typedef struct
{
	/** Component mandatory fields **/
	dev_write_ptr write_reg;
	dev_read_ptr read_reg;
	/** Component optional fields **/
	dev_mdelay_ptr mdelay;
	/** Customizable optional pointer **/
	dev_enable_ptr enable;
	void *handle;
} dev_ctx_t;


typedef enum
{
	MCP3201_4921_INT1_PIN,
} MCP3201_4921_SensorIntPin_t;

typedef struct
{
	MCP3201_4921_Init_Func Init;
	MCP3201_4921_DeInit_Func DeInit;
	uint32_t BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
	uint8_t Address;
	MCP3201_4921_WriteReg_Func WriteReg;
	MCP3201_4921_ReadReg_Func ReadReg;
	MCP3201_4921_GetTick_Func GetTick;
	MCP3201_4921_Enable_Func Enable;
	MCP3201_4921_Delay_Func Delay;
} MCP3201_4921_IO_t;

typedef struct
{
	int16_t transmit;
} MCP3201_4921_T_t;

typedef struct
{
	int32_t receive;
} MCP3201_4921_R_t;

typedef struct
{
	unsigned int ChannelStatus :1;
	unsigned int BufferStatus :1;
	unsigned int GainStatus :1;
	unsigned int ShutdownStatus :1;
} MCP3201_4921_Event_Status_t;

typedef enum
{
	MCP3201_4921_DISABLE, MCP3201_4921_ENABLE
} MCP3201_4921_Operate_t;

typedef struct
{
	MCP3201_4921_IO_t IO;
	dev_ctx_t Ctx;
	uint8_t is_initialized;
	float t_odr;
	float r_odr;
	MCP3201_4921_Operate_t operate;
	MCP3201_4921_Event_Status_t ev;
} MCP3201_4921_Object_t;

typedef struct
{
	uint16_t Receive;
	uint16_t Transmit;
	uint32_t TransmitMaxFS;
	uint32_t ReceiveMaxFS;
	float R_MaxOdr;
	float T_MaxOdr;

} MCP3201_4921_Capabilities_t;

typedef struct
{
	int32_t (*Init)(MCP3201_4921_Object_t*);
	int32_t (*DeInit)(MCP3201_4921_Object_t*);
	int32_t (*GetCapabilities)(MCP3201_4921_Object_t*,
			MCP3201_4921_Capabilities_t*);
} MCP3201_4921_CommonDrv_t;

typedef struct
{
	int32_t (*Enable)(MCP3201_4921_Object_t*);
	int32_t (*Disable)(MCP3201_4921_Object_t*);
	int32_t (*GetSensitivity)(MCP3201_4921_Object_t*, float*);
	int32_t (*GetOutputDataRate)(MCP3201_4921_Object_t*, float*);
	int32_t (*SetOutputDataRate)(MCP3201_4921_Object_t*, float);
	int32_t (*GetFullScale)(MCP3201_4921_Object_t*, int32_t*);
	int32_t (*SetFullScale)(MCP3201_4921_Object_t*, int32_t);
	int32_t (*GetReceiveLevel)(MCP3201_4921_Object_t*, MCP3201_4921_R_t*);
} MCP3201_4921_R_Drv_t;

typedef struct
{
	int32_t (*Enable)(MCP3201_4921_Object_t*);
	int32_t (*Disable)(MCP3201_4921_Object_t*);
	int32_t (*GetOutputDataRate)(MCP3201_4921_Object_t*, float*);
	int32_t (*SetOutputDataRate)(MCP3201_4921_Object_t*, float);
	int32_t (*SetValue)(MCP3201_4921_Object_t*, MCP3201_4921_T_t*);
} MCP3201_4921_T_Drv_t;

typedef union
{
	int16_t i16bit;
	uint8_t u8bit[2];
} mcp3921_4921_2b8_1b16_t;

/**
 * @}
 */

/** @defgroup MCP3201_4921_Exported_Constants MCP3201_4921 Exported Constants
 * @{
 */

#define MCP3201_4921_OK                       0
#define MCP3201_4921_ERROR                   -1

#define MCP3201_4921_I2C_BUS                 0U
#define MCP3201_4921_SPI_4WIRES_BUS          1U
#define MCP3201_4921_SPI_3WIRES_BUS          2U

/**
 * @}
 */

/** @addtogroup MCP3201_4921_Exported_Functions MCP3201_4921 Exported Functions
 * @{
 */

int32_t MCP3201_4921_RegisterBusIO(MCP3201_4921_Object_t *pObj,
		MCP3201_4921_IO_t *pIO);
int32_t MCP3201_4921_Init(MCP3201_4921_Object_t *pObj);
int32_t MCP3201_4921_DeInit(MCP3201_4921_Object_t *pObj);
int32_t MCP3201_4921_GetCapabilities(MCP3201_4921_Object_t *pObj,
		MCP3201_4921_Capabilities_t *Capabilities);

int32_t MCP3201_4921_Read_Reg(MCP3201_4921_Object_t *pObj, uint8_t reg,
		uint8_t *Data);
int32_t MCP3201_4921_Write_Reg(MCP3201_4921_Object_t *pObj, uint8_t reg,
		uint8_t Data);
int32_t MCP3201_4921_Set_Interrupt_Latch(MCP3201_4921_Object_t *pObj,
		uint8_t Status);

int32_t MCP3201_4921_R_Enable(MCP3201_4921_Object_t *pObj);
int32_t MCP3201_4921_R_Disable(MCP3201_4921_Object_t *pObj);
int32_t MCP3201_4921_R_GetSensitivity(MCP3201_4921_Object_t *pObj,
		float *Sensitivity);
int32_t MCP3201_4921_R_GetOutputDataRate(MCP3201_4921_Object_t *pObj,
		float *Odr);
int32_t MCP3201_4921_R_SetOutputDataRate(MCP3201_4921_Object_t *pObj, float Odr);
int32_t MCP3201_4921_R_GetFullScale(MCP3201_4921_Object_t *pObj,
		int32_t *FullScale);
int32_t MCP3201_4921_R_SetFullScale(MCP3201_4921_Object_t *pObj,
		int32_t FullScale);
int32_t MCP3201_4921_R_GetLevelRaw(MCP3201_4921_Object_t *pObj,
		MCP3201_4921_R_t *Value);

int32_t MCP3201_4921_R_Enable_DRDY_Interrupt(MCP3201_4921_Object_t *pObj);
int32_t MCP3201_4921_R_Disable_DRDY_Interrupt(MCP3201_4921_Object_t *pObj);

int32_t MCP3201_4921_R_Get_Event_Status(MCP3201_4921_Object_t *pObj,
		MCP3201_4921_Event_Status_t *Status);
int32_t MCP3201_4921_R_Set_SelfTest(MCP3201_4921_Object_t *pObj, uint8_t Val);
int32_t MCP3201_4921_R_Get_DRDY_Status(MCP3201_4921_Object_t *pObj,
		uint8_t *Status);
int32_t MCP3201_4921_R_Get_Init_Status(MCP3201_4921_Object_t *pObj,
		uint8_t *Status);

int32_t MCP3201_4921_T_Enable(MCP3201_4921_Object_t *pObj);
int32_t MCP3201_4921_T_Disable(MCP3201_4921_Object_t *pObj);
int32_t MCP3201_4921_T_GetOutputDataRate(MCP3201_4921_Object_t *pObj,
		float *Odr);
int32_t MCP3201_4921_T_SetOutputDataRate(MCP3201_4921_Object_t *pObj, float Odr);
int32_t MCP3201_4921_T_SetValue(MCP3201_4921_Object_t *pObj,
		MCP3201_4921_T_t *value);

/**
 * @}
 */

/** @addtogroup MCP3201_4921_Exported_Variables MCP3201_4921 Exported Variables
 * @{
 */

extern MCP3201_4921_CommonDrv_t MCP3201_4921_COMMON_Driver;
extern MCP3201_4921_R_Drv_t MCP3201_4921_R_Driver;
extern MCP3201_4921_T_Drv_t MCP3201_4921_T_Driver;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
