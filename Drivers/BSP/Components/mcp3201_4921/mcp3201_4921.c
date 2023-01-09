/**
  ******************************************************************************
 * @file    mcp3201_4921.c
  * @author  MEMS Software Solutions Team
 * @brief   MCP3201_4921 driver file
  ******************************************************************************
  * @attention
  *
 * Copyright (c) 2022 EmOne.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mcp3201_4921.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup MCP3201_4921 MCP3201_4921
  * @{
  */

/** @defgroup MCP3201_4921_Exported_Variables MCP3201_4921 Exported Variables
  * @{
  */

MCP3201_4921_CommonDrv_t MCP3201_4921_COMMON_Driver =
{ MCP3201_4921_Init, MCP3201_4921_DeInit, MCP3201_4921_GetCapabilities, };

MCP3201_4921_R_Drv_t MCP3201_4921_R_Driver =
{ MCP3201_4921_R_Enable, MCP3201_4921_R_Disable, MCP3201_4921_R_GetSensitivity,
		MCP3201_4921_R_GetOutputDataRate, MCP3201_4921_R_SetOutputDataRate,
		MCP3201_4921_R_GetFullScale, MCP3201_4921_R_SetFullScale,
		MCP3201_4921_R_GetLevelRaw, };

MCP3201_4921_T_Drv_t MCP3201_4921_T_Driver =
{ MCP3201_4921_T_Enable, MCP3201_4921_T_Disable,
		MCP3201_4921_T_GetOutputDataRate, MCP3201_4921_T_SetOutputDataRate,
		MCP3201_4921_T_SetValue, };

/**
 * @}
 */

/** @defgroup MCP3201_4921_Private_Function_Prototypes MCP3201_4921 Private Function Prototypes
 * @{
 */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData,
		uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData,
		uint16_t Length);

/**
 * @}
 */

/** @defgroup MCP3201_4921_Exported_Functions MCP3201_4921 Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t MCP3201_4921_RegisterBusIO(MCP3201_4921_Object_t *pObj,
		MCP3201_4921_IO_t *pIO)
{
	int32_t ret = MCP3201_4921_OK;

	if (pObj == NULL)
	{
		ret = MCP3201_4921_ERROR;
	}
	else
	{
		pObj->IO.Init = pIO->Init;
		pObj->IO.DeInit = pIO->DeInit;
		pObj->IO.BusType = pIO->BusType;
		pObj->IO.Address = pIO->Address;
		pObj->IO.WriteReg = pIO->WriteReg;
		pObj->IO.ReadReg = pIO->ReadReg;
		pObj->IO.GetTick = pIO->GetTick;
		pObj->IO.Enable = pIO->Enable;

		pObj->Ctx.read_reg = ReadRegWrap;
		pObj->Ctx.write_reg = WriteRegWrap;
		pObj->Ctx.mdelay = pIO->Delay;
		pObj->Ctx.handle = pObj;

		if (pObj->IO.Init == NULL)
		{
			ret = MCP3201_4921_ERROR;
		}
		else if (pObj->IO.Init() != MCP3201_4921_OK)
		{
			ret = MCP3201_4921_ERROR;
		}
		else
		{
//			if (pObj->IO.BusType == MCP3201_4921_SPI_3WIRES_BUS) /* SPI 3-Wires */
//			{
//				/* Enable the SPI 3-Wires support only the first time */
//				if (pObj->is_initialized == 0U)
//				{
//					/* Enable SPI 3-Wires on the component */
//					uint8_t data = 0x05;
//
//					if (MCP3201_4921_Write_Reg(pObj, MCP3201_4921_CTRL2,
//							data) != MCP3201_4921_OK)
//					{
//						ret = MCP3201_4921_ERROR;
//					}
//				}
//			}
		}
	}

	return ret;
}

/**
 * @brief  Initialize the MCP3201_4921 sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t MCP3201_4921_Init(MCP3201_4921_Object_t *pObj)
{
	/* Channel selection DACA or DACB Select bit */
	pObj->ev.ChannelStatus = 0;
	/* Buffer VREF Input Buffer Control bit */
	pObj->ev.BufferStatus = 0;
	/* Output Gain Select bit */
	pObj->ev.GainStatus = 0;
	/* Output Power Down Control bit */
	pObj->ev.ShutdownStatus = 0;

	if (MCP3201_4921_R_Enable(pObj) != MCP3201_4921_OK)
	{
		return MCP3201_4921_ERROR;
	}
//
//	if (MCP3201_4921_R_Enable(pObj) != MCP3201_4921_OK)
//	{
//		return MCP3201_4921_ERROR;
//	}

	pObj->is_initialized = 1;

	return MCP3201_4921_OK;
}

/**
 * @brief  Deinitialize the MCP3201_4921 sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t MCP3201_4921_DeInit(MCP3201_4921_Object_t *pObj)
{
	/* Disable the component */
	if (pObj->is_initialized == 1U)
	{
		if (MCP3201_4921_R_Disable(pObj) != MCP3201_4921_OK)
		{
			return MCP3201_4921_ERROR;
		}

//		if (MCP3201_4921_T_Disable(pObj) != MCP3201_4921_OK)
//		{
//			return MCP3201_4921_ERROR;
//		}
	}

	pObj->is_initialized = 0;

	return MCP3201_4921_OK;
}

/**
 * @brief  Get MCP3201_4921 sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to MCP3201_4921 sensor capabilities
 * @retval 0 in case of success, an error code otherwise
 */
int32_t MCP3201_4921_GetCapabilities(MCP3201_4921_Object_t *pObj,
		MCP3201_4921_Capabilities_t *Capabilities)
{
	/* Prevent unused argument(s) compilation warning */
	(void) (pObj);

	Capabilities->R_MaxOdr = 4096;
	Capabilities->Receive = 0;
	Capabilities->ReceiveMaxFS = 1;
	Capabilities->T_MaxOdr = 4096;
	Capabilities->Transmit = 0;
	Capabilities->TransmitMaxFS = 1;

	return MCP3201_4921_OK;
}

/**
 * @brief  Enable the MCP3201_4921 R sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t MCP3201_4921_R_Enable(MCP3201_4921_Object_t *pObj)
{

	/* Check if the compopOnent is already disabled */
	pObj->IO.Enable(true);
	return MCP3201_4921_OK;
}

/**
 * @brief  Disable the MCP3201_4921 accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t MCP3201_4921_R_Disable(MCP3201_4921_Object_t *pObj)
{
	/* Check if the component is already disabled */
	pObj->IO.Enable(false);
	return MCP3201_4921_OK;

}

///**
// * @brief  Get the MCP3201_4921 accelerometer sensor sensitivity
//  * @param  pObj the device pObj
//  * @param  Sensitivity pointer
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_GetSensitivity(MCP3201_4921_Object_t *pObj,
//		float *Sensitivity)
//{
//	int32_t ret = MCP3201_4921_OK;
//  lis2dtw12_fs_t full_scale;
//  lis2dtw12_mode_t mode;
//
//  /* Read actual full scale selection from sensor. */
//	if (lis2dtw12_full_scale_get(&(pObj->Ctx), &full_scale) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Read actual power mode selection from sensor. */
//	if (lis2dtw12_power_mode_get(&(pObj->Ctx), &mode) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  switch (mode)
//  {
//	case MCP3201_4921_CONT_LOW_PWR_12bit:
//	case MCP3201_4921_SINGLE_LOW_PWR_12bit:
//	case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_12bit:
//	case MCP3201_4921_SINGLE_LOW_LOW_NOISE_PWR_12bit:
//      switch (full_scale)
//      {
//		case MCP3201_4921_2g:
//			*Sensitivity = MCP3201_4921_ACC_SENSITIVITY_FOR_FS_2G_LOPOW1_MODE;
//          break;
//
//		case MCP3201_4921_4g:
//			*Sensitivity = MCP3201_4921_ACC_SENSITIVITY_FOR_FS_4G_LOPOW1_MODE;
//          break;
//
//		case MCP3201_4921_8g:
//			*Sensitivity = MCP3201_4921_ACC_SENSITIVITY_FOR_FS_8G_LOPOW1_MODE;
//          break;
//
//		case MCP3201_4921_16g:
//			*Sensitivity = MCP3201_4921_ACC_SENSITIVITY_FOR_FS_16G_LOPOW1_MODE;
//          break;
//
//        default:
//          *Sensitivity = -1.0f;
//			ret = MCP3201_4921_ERROR;
//          break;
//      }
//      break;
//
//	case MCP3201_4921_HIGH_PERFORMANCE:
//	case MCP3201_4921_CONT_LOW_PWR_4:
//	case MCP3201_4921_CONT_LOW_PWR_3:
//	case MCP3201_4921_CONT_LOW_PWR_2:
//	case MCP3201_4921_SINGLE_LOW_PWR_4:
//	case MCP3201_4921_SINGLE_LOW_PWR_3:
//	case MCP3201_4921_SINGLE_LOW_PWR_2:
//	case MCP3201_4921_HIGH_PERFORMANCE_LOW_NOISE:
//	case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_4:
//	case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_3:
//	case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_2:
//	case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_4:
//	case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_3:
//	case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_2:
//      switch (full_scale)
//      {
//		case MCP3201_4921_2g:
//			*Sensitivity = MCP3201_4921_ACC_SENSITIVITY_FOR_FS_2G_OTHER_MODES;
//          break;
//
//		case MCP3201_4921_4g:
//			*Sensitivity = MCP3201_4921_ACC_SENSITIVITY_FOR_FS_4G_OTHER_MODES;
//          break;
//
//		case MCP3201_4921_8g:
//			*Sensitivity = MCP3201_4921_ACC_SENSITIVITY_FOR_FS_8G_OTHER_MODES;
//          break;
//
//		case MCP3201_4921_16g:
//			*Sensitivity = MCP3201_4921_ACC_SENSITIVITY_FOR_FS_16G_OTHER_MODES;
//          break;
//
//        default:
//          *Sensitivity = -1.0f;
//			ret = MCP3201_4921_ERROR;
//          break;
//      }
//      break;
//
//    default:
//      *Sensitivity = -1.0f;
//		ret = MCP3201_4921_ERROR;
//      break;
//  }
//
//  return ret;
//}
//
///**
// * @brief  Get the MCP3201_4921 accelerometer sensor output data rate
//  * @param  pObj the device pObj
//  * @param  Odr pointer where the output data rate is written
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_GetOutputDataRate(MCP3201_4921_Object_t *pObj,
//		float *Odr)
//{
//	int32_t ret = MCP3201_4921_OK;
//  lis2dtw12_odr_t odr_low_level;
//  lis2dtw12_mode_t mode;
//
//  /* Get current output data rate. */
//	if (lis2dtw12_data_rate_get(&(pObj->Ctx), &odr_low_level)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Read actual power mode selection from sensor. */
//	if (lis2dtw12_power_mode_get(&(pObj->Ctx), &mode) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  switch (odr_low_level)
//  {
//	case MCP3201_4921_XL_ODR_OFF:
//	case MCP3201_4921_XL_SET_SW_TRIG:
//	case MCP3201_4921_XL_SET_PIN_TRIG:
//      *Odr = 0.0f;
//      break;
//
//	case MCP3201_4921_XL_ODR_1Hz6_LP_ONLY:
//      switch (mode)
//      {
//		case MCP3201_4921_HIGH_PERFORMANCE:
//		case MCP3201_4921_HIGH_PERFORMANCE_LOW_NOISE:
//          *Odr = 12.5f;
//          break;
//
//		case MCP3201_4921_CONT_LOW_PWR_4:
//		case MCP3201_4921_CONT_LOW_PWR_3:
//		case MCP3201_4921_CONT_LOW_PWR_2:
//		case MCP3201_4921_CONT_LOW_PWR_12bit:
//		case MCP3201_4921_SINGLE_LOW_PWR_4:
//		case MCP3201_4921_SINGLE_LOW_PWR_3:
//		case MCP3201_4921_SINGLE_LOW_PWR_2:
//		case MCP3201_4921_SINGLE_LOW_PWR_12bit:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_4:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_3:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_2:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_12bit:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_4:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_3:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_2:
//		case MCP3201_4921_SINGLE_LOW_LOW_NOISE_PWR_12bit:
//          *Odr = 1.6f;
//          break;
//
//        default:
//          *Odr = -1.0f;
//			ret = MCP3201_4921_ERROR;
//          break;
//      }
//      break;
//
//	case MCP3201_4921_XL_ODR_12Hz5:
//      *Odr = 12.5f;
//      break;
//
//	case MCP3201_4921_XL_ODR_25Hz:
//      *Odr = 25.0f;
//      break;
//
//	case MCP3201_4921_XL_ODR_50Hz:
//      *Odr = 50.0f;
//      break;
//
//	case MCP3201_4921_XL_ODR_100Hz:
//      *Odr = 100.0f;
//      break;
//
//	case MCP3201_4921_XL_ODR_200Hz:
//      *Odr = 200.0f;
//      break;
//
//	case MCP3201_4921_XL_ODR_400Hz:
//      switch (mode)
//      {
//		case MCP3201_4921_HIGH_PERFORMANCE:
//		case MCP3201_4921_HIGH_PERFORMANCE_LOW_NOISE:
//          *Odr = 400.0f;
//          break;
//
//		case MCP3201_4921_CONT_LOW_PWR_4:
//		case MCP3201_4921_CONT_LOW_PWR_3:
//		case MCP3201_4921_CONT_LOW_PWR_2:
//		case MCP3201_4921_CONT_LOW_PWR_12bit:
//		case MCP3201_4921_SINGLE_LOW_PWR_4:
//		case MCP3201_4921_SINGLE_LOW_PWR_3:
//		case MCP3201_4921_SINGLE_LOW_PWR_2:
//		case MCP3201_4921_SINGLE_LOW_PWR_12bit:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_4:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_3:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_2:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_12bit:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_4:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_3:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_2:
//		case MCP3201_4921_SINGLE_LOW_LOW_NOISE_PWR_12bit:
//          *Odr = 200.0f;
//          break;
//
//        default:
//          *Odr = -1.0f;
//			ret = MCP3201_4921_ERROR;
//          break;
//      }
//      break;
//
//	case MCP3201_4921_XL_ODR_800Hz:
//      switch (mode)
//      {
//		case MCP3201_4921_HIGH_PERFORMANCE:
//		case MCP3201_4921_HIGH_PERFORMANCE_LOW_NOISE:
//          *Odr = 800.0f;
//          break;
//
//		case MCP3201_4921_CONT_LOW_PWR_4:
//		case MCP3201_4921_CONT_LOW_PWR_3:
//		case MCP3201_4921_CONT_LOW_PWR_2:
//		case MCP3201_4921_CONT_LOW_PWR_12bit:
//		case MCP3201_4921_SINGLE_LOW_PWR_4:
//		case MCP3201_4921_SINGLE_LOW_PWR_3:
//		case MCP3201_4921_SINGLE_LOW_PWR_2:
//		case MCP3201_4921_SINGLE_LOW_PWR_12bit:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_4:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_3:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_2:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_12bit:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_4:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_3:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_2:
//		case MCP3201_4921_SINGLE_LOW_LOW_NOISE_PWR_12bit:
//          *Odr = 200.0f;
//          break;
//
//        default:
//          *Odr = -1.0f;
//			ret = MCP3201_4921_ERROR;
//          break;
//      }
//      break;
//
//	case MCP3201_4921_XL_ODR_1k6Hz:
//      switch (mode)
//      {
//		case MCP3201_4921_HIGH_PERFORMANCE:
//		case MCP3201_4921_HIGH_PERFORMANCE_LOW_NOISE:
//          *Odr = 1600.0f;
//          break;
//
//		case MCP3201_4921_CONT_LOW_PWR_4:
//		case MCP3201_4921_CONT_LOW_PWR_3:
//		case MCP3201_4921_CONT_LOW_PWR_2:
//		case MCP3201_4921_CONT_LOW_PWR_12bit:
//		case MCP3201_4921_SINGLE_LOW_PWR_4:
//		case MCP3201_4921_SINGLE_LOW_PWR_3:
//		case MCP3201_4921_SINGLE_LOW_PWR_2:
//		case MCP3201_4921_SINGLE_LOW_PWR_12bit:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_4:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_3:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_2:
//		case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_12bit:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_4:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_3:
//		case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_2:
//		case MCP3201_4921_SINGLE_LOW_LOW_NOISE_PWR_12bit:
//          *Odr = 200.0f;
//          break;
//
//        default:
//          *Odr = -1.0f;
//			ret = MCP3201_4921_ERROR;
//          break;
//      }
//      break;
//
//    default:
//      *Odr = -1.0f;
//		ret = MCP3201_4921_ERROR;
//      break;
//  }
//
//  return ret;
//}
//
///**
// * @brief  Set the MCP3201_4921 accelerometer sensor output data rate
//  * @param  pObj the device pObj
//  * @param  Odr the output data rate value to be set
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_SetOutputDataRate(MCP3201_4921_Object_t *pObj,
//		float Odr)
//{
//  /* By default we use High Performance mode and Low Noise disabled */
//	return MCP3201_4921_ACC_SetOutputDataRate_With_Mode(pObj, Odr,
//			MCP3201_4921_HIGH_PERFORMANCE_MODE, MCP3201_4921_LOW_NOISE_DISABLE);
//}
//
///**
// * @brief  Set the MCP3201_4921 accelerometer sensor output data rate
//  * @param  pObj the device pObj
//  * @param  Odr the output data rate value to be set
//  * @param  Mode the operating mode to be used
//  * @param  Noise the low noise option
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_SetOutputDataRate_With_Mode(
//		MCP3201_4921_Object_t *pObj, float Odr,
//		MCP3201_4921_Operating_Mode_t Mode, MCP3201_4921_Low_Noise_t Noise)
//{
//  /* Check if the component is enabled */
//  if (pObj->acc_is_enabled == 1U || pObj->temp_is_enabled == 1U)
//  {
//		return MCP3201_4921_ACC_SetOutputDataRate_When_Enabled(pObj, Odr, Mode,
//				Noise);
//  }
//  else
//  {
//		return MCP3201_4921_ACC_SetOutputDataRate_When_Disabled(pObj, Odr, Mode,
//				Noise);
//  }
//}
//
///**
// * @brief  Get the MCP3201_4921 accelerometer sensor full scale
//  * @param  pObj the device pObj
//  * @param  FullScale pointer where the full scale is written
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_GetFullScale(MCP3201_4921_Object_t *pObj,
//		int32_t *FullScale)
//{
//	int32_t ret = MCP3201_4921_OK;
//  lis2dtw12_fs_t fs_low_level;
//
//  /* Read actual full scale selection from sensor. */
//	if (lis2dtw12_full_scale_get(&(pObj->Ctx), &fs_low_level)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  switch (fs_low_level)
//  {
//	case MCP3201_4921_2g:
//      *FullScale =  2;
//      break;
//
//	case MCP3201_4921_4g:
//      *FullScale =  4;
//      break;
//
//	case MCP3201_4921_8g:
//      *FullScale =  8;
//      break;
//
//	case MCP3201_4921_16g:
//      *FullScale = 16;
//      break;
//
//    default:
//      *FullScale = -1;
//		ret = MCP3201_4921_ERROR;
//      break;
//  }
//
//  return ret;
//}
//
///**
// * @brief  Set the MCP3201_4921 accelerometer sensor full scale
//  * @param  pObj the device pObj
//  * @param  FullScale the functional full scale to be set
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_SetFullScale(MCP3201_4921_Object_t *pObj,
//		int32_t FullScale)
//{
//  lis2dtw12_fs_t new_fs;
//
//  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
//     the parameter passed to the function is not known at the moment of analysis */
//	new_fs = (FullScale <= 2) ? MCP3201_4921_2g :
//				(FullScale <= 4) ? MCP3201_4921_4g :
//				(FullScale <= 8) ? MCP3201_4921_8g : MCP3201_4921_16g;
//
//	if (lis2dtw12_full_scale_set(&(pObj->Ctx), new_fs) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
/**
 * @brief  Get the MCP3201_4921 R sensor raw data
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t MCP3201_4921_R_GetLevelRaw(MCP3201_4921_Object_t *pObj,
		MCP3201_4921_R_t *Value)
{
//	uint16_t data_raw;
//	lis2dtw12_mode_t mode;
	int32_t ret = MCP3201_4921_OK;
//
//	/* Read raw data values. */
//	if (lis2dtw12_acceleration_raw_get(&(pObj->Ctx),
//			data_raw.i16bit) != MCP3201_4921_OK)
//	{
//		return MCP3201_4921_ERROR;
//	}
	pObj->Ctx.read_reg(pObj->Ctx.handle, 0, (uint8_t*) &Value->receive, 1);


	return ret;
}

///**
// * @brief  Get the MCP3201_4921 accelerometer sensor axes
//  * @param  pObj the device pObj
//  * @param  Acceleration pointer where the values of the axes are written
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_GetAxes(MCP3201_4921_Object_t *pObj,
//		MCP3201_4921_ACC_Axes_t *Acceleration)
//{
//	MCP3201_4921_ACC_AxesRaw_t data_raw;
//  float sensitivity = 0.0f;
//
//  /* Read raw data values. */
//	if (MCP3201_4921_ACC_GetAxesRaw(pObj, &data_raw) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	/* Get MCP3201_4921 actual sensitivity. */
//	if (MCP3201_4921_ACC_GetSensitivity(pObj, &sensitivity) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Calculate the data. */
//  Acceleration->x = (int32_t)((float)((float)data_raw.x * sensitivity));
//  Acceleration->y = (int32_t)((float)((float)data_raw.y * sensitivity));
//  Acceleration->z = (int32_t)((float)((float)data_raw.z * sensitivity));
//
//	return MCP3201_4921_OK;
//}
//
///**
// * @brief  Enable the MCP3201_4921 temperature sensor
//  * @param  pObj the device pObj
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_TEMP_Enable(MCP3201_4921_Object_t *pObj)
//{
//  /* Check if the component is already enabled */
//  if (pObj->temp_is_enabled == 1U)
//  {
//		return MCP3201_4921_OK;
//  }
//
//	/* Check if the MCP3201_4921 accelerometer sensor is already enabled. */
//  /* If yes, skip the enable function, if not call enable function */
//  if (pObj->acc_is_enabled == 0U)
//  {
//    /* Output data rate selection. */
//		if (MCP3201_4921_ACC_SetOutputDataRate_When_Enabled(pObj, pObj->acc_odr,
//				pObj->acc_operating_mode, pObj->acc_low_noise)
//				!= MCP3201_4921_OK)
//    {
//			return MCP3201_4921_ERROR;
//    }
//  }
//
//  pObj->temp_is_enabled = 1;
//
//	return MCP3201_4921_OK;
//}
//
///**
// * @brief  Disable the MCP3201_4921 temperature sensor
//  * @param  pObj the device pObj
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_TEMP_Disable(MCP3201_4921_Object_t *pObj)
//{
//  /* Check if the component is already disabled */
//  if (pObj->temp_is_enabled == 0U)
//  {
//		return MCP3201_4921_OK;
//  }
//
//	/* Check if the MCP3201_4921 accelerometer sensor is still enable. */
//  /* If yes, skip the disable function, if not call disable function */
//  if (pObj->acc_is_enabled == 0U)
//  {
//    /* Output data rate selection - power down. */
//		if (lis2dtw12_data_rate_set(&(pObj->Ctx), MCP3201_4921_XL_ODR_OFF)
//				!= MCP3201_4921_OK)
//    {
//			return MCP3201_4921_ERROR;
//    }
//  }
//
//  pObj->temp_is_enabled = 0;
//
//	return MCP3201_4921_OK;
//}
//
///**
// * @brief  Get the MCP3201_4921 temperature sensor output data rate
//  * @param  pObj the device pObj
//  * @param  Odr pointer where the output data rate is written
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_TEMP_GetOutputDataRate(MCP3201_4921_Object_t *pObj,
//		float *Odr)
//{
//	int32_t ret = MCP3201_4921_OK;
//  float acc_odr;
//  lis2dtw12_mode_t mode;
//
//	/* Get the MCP3201_4921 accelerometer sensor output data rate */
//	if (MCP3201_4921_ACC_GetOutputDataRate(pObj, &acc_odr) != MCP3201_4921_OK)
//  {
//    *Odr = -1.0f;
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Read actual power mode selection from sensor. */
//	if (lis2dtw12_power_mode_get(&(pObj->Ctx), &mode) != MCP3201_4921_OK)
//  {
//    *Odr = -1.0f;
//		return MCP3201_4921_ERROR;
//  }
//
//	*Odr = (mode == MCP3201_4921_HIGH_PERFORMANCE) ? 50.0f :
//			(mode == MCP3201_4921_HIGH_PERFORMANCE_LOW_NOISE) ? 50.0f
//         : (acc_odr >= 50.0f)                             ? 50.0f
//         :                                                  acc_odr;
//
//  return ret;
//}
//
///**
// * @brief  Set the MCP3201_4921 accelerometer sensor output data rate
//  * @param  pObj the device pObj
//  * @param  Odr the output data rate value to be set
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_TEMP_SetOutputDataRate(MCP3201_4921_Object_t *pObj,
//		float Odr)
//{
//  float acc_odr;
//  lis2dtw12_mode_t mode;
//	int32_t ret = MCP3201_4921_OK;
//
//  float new_acc_odr;
//	MCP3201_4921_Operating_Mode_t new_mode;
//	MCP3201_4921_Low_Noise_t new_noise;
//
//	/* Get the MCP3201_4921 accelerometer sensor output data rate */
//	if (MCP3201_4921_ACC_GetOutputDataRate(pObj, &acc_odr) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Read actual power mode selection from sensor. */
//	if (lis2dtw12_power_mode_get(&(pObj->Ctx), &mode) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  switch (mode)
//  {
//	case MCP3201_4921_HIGH_PERFORMANCE:
//		new_noise = MCP3201_4921_LOW_NOISE_DISABLE;
//		new_mode = MCP3201_4921_HIGH_PERFORMANCE_MODE;
//      break;
//
//	case MCP3201_4921_HIGH_PERFORMANCE_LOW_NOISE:
//		new_noise = MCP3201_4921_LOW_NOISE_ENABLE;
//		new_mode = MCP3201_4921_HIGH_PERFORMANCE_MODE;
//      break;
//
//	case MCP3201_4921_CONT_LOW_PWR_4:
//	case MCP3201_4921_SINGLE_LOW_PWR_4:
//		new_noise = MCP3201_4921_LOW_NOISE_DISABLE;
//		new_mode = MCP3201_4921_LOW_POWER_MODE4;
//      break;
//
//	case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_4:
//	case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_4:
//		new_noise = MCP3201_4921_LOW_NOISE_ENABLE;
//		new_mode = MCP3201_4921_LOW_POWER_MODE4;
//      break;
//
//	case MCP3201_4921_CONT_LOW_PWR_3:
//	case MCP3201_4921_SINGLE_LOW_PWR_3:
//		new_noise = MCP3201_4921_LOW_NOISE_DISABLE;
//		new_mode = MCP3201_4921_LOW_POWER_MODE3;
//      break;
//
//	case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_3:
//	case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_3:
//		new_noise = MCP3201_4921_LOW_NOISE_ENABLE;
//		new_mode = MCP3201_4921_LOW_POWER_MODE3;
//      break;
//
//	case MCP3201_4921_CONT_LOW_PWR_2:
//	case MCP3201_4921_SINGLE_LOW_PWR_2:
//		new_noise = MCP3201_4921_LOW_NOISE_DISABLE;
//		new_mode = MCP3201_4921_LOW_POWER_MODE2;
//      break;
//
//	case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_2:
//	case MCP3201_4921_SINGLE_LOW_PWR_LOW_NOISE_2:
//		new_noise = MCP3201_4921_LOW_NOISE_ENABLE;
//		new_mode = MCP3201_4921_LOW_POWER_MODE2;
//      break;
//
//	case MCP3201_4921_CONT_LOW_PWR_12bit:
//	case MCP3201_4921_SINGLE_LOW_PWR_12bit:
//		new_noise = MCP3201_4921_LOW_NOISE_DISABLE;
//		new_mode = MCP3201_4921_LOW_POWER_MODE1;
//      break;
//
//	case MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_12bit:
//	case MCP3201_4921_SINGLE_LOW_LOW_NOISE_PWR_12bit:
//		new_noise = MCP3201_4921_LOW_NOISE_ENABLE;
//		new_mode = MCP3201_4921_LOW_POWER_MODE1;
//      break;
//
//    default:
//		ret = MCP3201_4921_ERROR;
//      break;
//  }
//
//	if (ret != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  if (Odr <= 25.0f)
//  {
//    new_acc_odr = (Odr <=  1.6f) ?  1.6f
//                  : (Odr <= 12.5f) ? 12.5f
//                  :                  25.0f;
//
//		new_mode =
//				(new_mode == MCP3201_4921_HIGH_PERFORMANCE_MODE) ?
//						MCP3201_4921_LOW_POWER_MODE4
//               :                                                 new_mode;
//  }
//  else
//  {
//		new_acc_odr =
//				(new_mode == MCP3201_4921_HIGH_PERFORMANCE_MODE) ? acc_odr
//                  : (acc_odr > 25.0f)                             ? acc_odr
//                  :                                                 50.0f;
//  }
//
//	/* Set the MCP3201_4921 accelerometer sensor output data rate */
//	if (MCP3201_4921_ACC_SetOutputDataRate_With_Mode(pObj, new_acc_odr,
//			new_mode, new_noise) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  return ret;
//}
//
///**
// * @brief  Get the MCP3201_4921 accelerometer sensor axes
//  * @param  pObj the device pObj
//  * @param  Acceleration pointer where the values of the axes are written
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_TEMP_GetValue(MCP3201_4921_Object_t *pObj,
//		float *Temperature)
//{
//  uint8_t reg[2];
//  int16_t temperature_raw;
//
//  float sensitivity = 16.0f;  /* 16 LSB/degC */
//
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_OUT_T_L, reg, 2)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  temperature_raw = (int16_t)reg[1];
//  temperature_raw = (temperature_raw * 256) + (int16_t)reg[0];
//  /* you need to divide to 16 because the output is only on most significant 12 bits */
//  temperature_raw = temperature_raw / 16;
//
//  *Temperature = ((float)temperature_raw / sensitivity) + 25.0f;
//
//	return MCP3201_4921_OK;
//}
//
/**
 * @brief  Get the MCP3201_4921 register value
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t MCP3201_4921_Read_Reg(MCP3201_4921_Object_t *pObj, uint8_t Reg,
		uint8_t *Data)
{
	if (pObj->Ctx.read_reg(pObj->Ctx.handle, Reg, Data, 2) != MCP3201_4921_OK)
	{
		return MCP3201_4921_ERROR;
	}

	return MCP3201_4921_OK;
}

/**
 * @brief  Set the MCP3201_4921 register value
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t MCP3201_4921_Write_Reg(MCP3201_4921_Object_t *pObj, uint8_t Reg,
		uint8_t Data)
{
	if (pObj->Ctx.write_reg(pObj->Ctx.handle, Reg, Data, 1) != MCP3201_4921_OK)
	{
		return MCP3201_4921_ERROR;
	}

	return MCP3201_4921_OK;
}

///**
//  * @brief  Set the interrupt latch
//  * @param  pObj the device pObj
//  * @param  Status value to be written
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_Set_Interrupt_Latch(MCP3201_4921_Object_t *pObj,
//		uint8_t Status)
//{
//  if (Status > 1U)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	if (lis2dtw12_int_notification_set(&(pObj->Ctx), (lis2dtw12_lir_t) Status)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Enable DRDY interrupt mode
//  * @param  pObj the device pObj
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Enable_DRDY_Interrupt(MCP3201_4921_Object_t *pObj)
//{
//  lis2dtw12_ctrl5_int2_pad_ctrl_t int2_pad_ctrl;
//
//  /* Enable DRDY interrupts on INT1 */
//	if (lis2dtw12_data_ready_mode_set(&(pObj->Ctx), MCP3201_4921_DRDY_PULSED)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//	if (lis2dtw12_pin_int2_route_get(&(pObj->Ctx), &int2_pad_ctrl)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//  int2_pad_ctrl.int2_drdy = 1;
//	if (lis2dtw12_pin_int2_route_set(&(pObj->Ctx), &int2_pad_ctrl)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Disable DRDY interrupt mode
//  * @param  pObj the device pObj
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Disable_DRDY_Interrupt(MCP3201_4921_Object_t *pObj)
//{
//  lis2dtw12_ctrl5_int2_pad_ctrl_t int2_pad_ctrl;
//
//	if (lis2dtw12_pin_int2_route_get(&(pObj->Ctx), &int2_pad_ctrl)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//  int2_pad_ctrl.int2_drdy = 0;
//	if (lis2dtw12_pin_int2_route_set(&(pObj->Ctx), &int2_pad_ctrl)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Set the filterMode value
//  * @param  pObj the device pObj
//  * @param  filterMode value to be written
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Set_Filter_Mode(MCP3201_4921_Object_t *pObj,
//		uint8_t filterMode)
//{
//	if (lis2dtw12_filter_bandwidth_set(&(pObj->Ctx),
//			(lis2dtw12_bw_filt_t) filterMode) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Enable wake up detection
//  * @param  pObj the device pObj
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Enable_Wake_Up_Detection(MCP3201_4921_Object_t *pObj)
//{
//	int32_t ret = MCP3201_4921_OK;
//  lis2dtw12_ctrl4_int1_pad_ctrl_t val;
//
//  /* Output Data Rate selection */
//	if (MCP3201_4921_ACC_SetOutputDataRate(pObj, 200.0f) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Full scale selection */
//	if (MCP3201_4921_ACC_SetFullScale(pObj, 2) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* WAKE_DUR setting */
//	if (lis2dtw12_wkup_dur_set(&(pObj->Ctx), 0x00) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Set wake up threshold. */
//	if (lis2dtw12_wkup_threshold_set(&(pObj->Ctx), 0x02) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	if (lis2dtw12_pin_int1_route_get(&(pObj->Ctx), &val) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  val.int1_wu = PROPERTY_ENABLE;
//
//	if (lis2dtw12_pin_int1_route_set(&(pObj->Ctx), &val) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  return ret;
//}
//
///**
//  * @brief  Disable wake up detection
//  * @param  pObj the device pObj
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Disable_Wake_Up_Detection(MCP3201_4921_Object_t *pObj)
//{
//  lis2dtw12_ctrl4_int1_pad_ctrl_t ctrl4_int1_reg;
//  lis2dtw12_ctrl5_int2_pad_ctrl_t ctrl5_int2_reg;
//  lis2dtw12_ctrl_reg7_t ctrl_reg7;
//
//  /* Disable wake up event on INT1 pin. */
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_CTRL4_INT1_PAD_CTRL,
//			(uint8_t*) &ctrl4_int1_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  ctrl4_int1_reg.int1_wu = PROPERTY_DISABLE;
//
//	if (lis2dtw12_write_reg(&(pObj->Ctx), MCP3201_4921_CTRL4_INT1_PAD_CTRL,
//			(uint8_t*) &ctrl4_int1_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Read INT2 Sleep Change */
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_CTRL5_INT2_PAD_CTRL,
//			(uint8_t*) &ctrl5_int2_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /*Disable Interrupts bit if none event is still enabled */
//  if (ctrl5_int2_reg.int2_sleep_chg == 0 && ctrl4_int1_reg.int1_wu == 0 && ctrl4_int1_reg.int1_6d == 0)
//  {
//		if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_CTRL_REG7,
//				(uint8_t*) &ctrl_reg7, 1) != MCP3201_4921_OK)
//    {
//			return MCP3201_4921_ERROR;
//    }
//
//    ctrl_reg7.interrupts_enable = PROPERTY_DISABLE;
//
//		if (lis2dtw12_write_reg(&(pObj->Ctx), MCP3201_4921_CTRL_REG7,
//				(uint8_t*) &ctrl_reg7, 1) != MCP3201_4921_OK)
//    {
//			return MCP3201_4921_ERROR;
//    }
//  }
//
//  /* Reset wake up threshold. */
//	if (lis2dtw12_wkup_threshold_set(&(pObj->Ctx), 0x00) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* WAKE_DUR setting */
//	if (lis2dtw12_wkup_dur_set(&(pObj->Ctx), 0x00) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Set wake up threshold
//  * @param  pObj the device pObj
//  * @param  Threshold wake up detection threshold
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Set_Wake_Up_Threshold(MCP3201_4921_Object_t *pObj,
//		uint8_t Threshold)
//{
//  /* Set wake up threshold. */
//	if (lis2dtw12_wkup_threshold_set(&(pObj->Ctx), Threshold)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Set wake up duration
//  * @param  pObj the device pObj
//  * @param  Duration wake up detection duration
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Set_Wake_Up_Duration(MCP3201_4921_Object_t *pObj,
//		uint8_t Duration)
//{
//  /* Set wake up duration. */
//	if (lis2dtw12_wkup_dur_set(&(pObj->Ctx), Duration) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Enable inactivity detection
//  * @param  pObj the device pObj
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Enable_Inactivity_Detection(
//		MCP3201_4921_Object_t *pObj)
//{
//	int32_t ret = MCP3201_4921_OK;
//  lis2dtw12_ctrl5_int2_pad_ctrl_t val;
//
//  /* Output Data Rate selection */
//	if (MCP3201_4921_ACC_SetOutputDataRate(pObj, 50.0f) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Full scale selection */
//	if (MCP3201_4921_ACC_SetFullScale(pObj, 2) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* SLEEP_DUR setting */
//	if (lis2dtw12_act_sleep_dur_set(&(pObj->Ctx), 0x01) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Enable inactivity detection. */
//	if (lis2dtw12_act_mode_set(&(pObj->Ctx), MCP3201_4921_DETECT_ACT_INACT)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	if (lis2dtw12_pin_int2_route_get(&(pObj->Ctx), &val) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  val.int2_sleep_chg = PROPERTY_ENABLE;
//
//	if (lis2dtw12_pin_int2_route_set(&(pObj->Ctx), &val) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  return ret;
//}
//
///**
//  * @brief  Disable inactivity detection
//  * @param  pObj the device pObj
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Disable_Inactivity_Detection(
//		MCP3201_4921_Object_t *pObj)
//{
//  lis2dtw12_ctrl4_int1_pad_ctrl_t ctrl4_int1_reg;
//  lis2dtw12_ctrl5_int2_pad_ctrl_t ctrl5_int2_reg;
//  lis2dtw12_ctrl_reg7_t ctrl_reg7;
//
//  /* Disable inactivity event on INT2 pin */
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_CTRL5_INT2_PAD_CTRL,
//			(uint8_t*) &ctrl5_int2_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  ctrl5_int2_reg.int2_sleep_chg = PROPERTY_DISABLE;
//
//	if (lis2dtw12_write_reg(&(pObj->Ctx), MCP3201_4921_CTRL5_INT2_PAD_CTRL,
//			(uint8_t*) &ctrl5_int2_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Read INT1 Wake Up event and INT1 6D Orientation event */
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_CTRL4_INT1_PAD_CTRL,
//			(uint8_t*) &ctrl4_int1_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /*Disable Interrupts bit if none event is still enabled */
//  if (ctrl5_int2_reg.int2_sleep_chg == 0 && ctrl4_int1_reg.int1_wu == 0 && ctrl4_int1_reg.int1_6d == 0)
//  {
//		if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_CTRL_REG7,
//				(uint8_t*) &ctrl_reg7, 1) != MCP3201_4921_OK)
//    {
//			return MCP3201_4921_ERROR;
//    }
//
//    ctrl_reg7.interrupts_enable = PROPERTY_DISABLE;
//
//		if (lis2dtw12_write_reg(&(pObj->Ctx), MCP3201_4921_CTRL_REG7,
//				(uint8_t*) &ctrl_reg7, 1) != MCP3201_4921_OK)
//    {
//			return MCP3201_4921_ERROR;
//    }
//  }
//
//  /* Disable inactivity detection. */
//	if (lis2dtw12_act_mode_set(&(pObj->Ctx), MCP3201_4921_NO_DETECTION)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* SLEEP_DUR setting */
//	if (lis2dtw12_act_sleep_dur_set(&(pObj->Ctx), 0x00) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Set sleep duration
//  * @param  pObj the device pObj
//  * @param  Duration wake up detection duration
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Set_Sleep_Duration(MCP3201_4921_Object_t *pObj,
//		uint8_t Duration)
//{
//  /* Set sleep duration. */
//	if (lis2dtw12_act_sleep_dur_set(&(pObj->Ctx), Duration) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Enable 6D orientation detection
//  * @param  pObj the device pObj
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Enable_6D_Orientation(MCP3201_4921_Object_t *pObj)
//{
//	int32_t ret = MCP3201_4921_OK;
//  lis2dtw12_ctrl4_int1_pad_ctrl_t val;
//
//  /* Output Data Rate selection */
//	if (MCP3201_4921_ACC_SetOutputDataRate(pObj, 200.0f) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Full scale selection */
//	if (MCP3201_4921_ACC_SetFullScale(pObj, 2) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* 6D orientation threshold. */
//	if (lis2dtw12_6d_threshold_set(&(pObj->Ctx), 2) != MCP3201_4921_OK) /* 60 degrees */
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Enable 6D orientation event on INT1 pin */
//	if (lis2dtw12_pin_int1_route_get(&(pObj->Ctx), &val) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  val.int1_6d = PROPERTY_ENABLE;
//
//	if (lis2dtw12_pin_int1_route_set(&(pObj->Ctx), &val) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  return ret;
//}
//
///**
//  * @brief  Disable 6D orientation detection
//  * @param  pObj the device pObj
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Disable_6D_Orientation(MCP3201_4921_Object_t *pObj)
//{
//  lis2dtw12_ctrl4_int1_pad_ctrl_t ctrl4_int1_reg;
//  lis2dtw12_ctrl5_int2_pad_ctrl_t ctrl5_int2_reg;
//  lis2dtw12_ctrl_reg7_t ctrl_reg7;
//
//  /* Disable 6D orientation event on INT1 pin */
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_CTRL4_INT1_PAD_CTRL,
//			(uint8_t*) &ctrl4_int1_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  ctrl4_int1_reg.int1_6d = PROPERTY_DISABLE;
//
//	if (lis2dtw12_write_reg(&(pObj->Ctx), MCP3201_4921_CTRL4_INT1_PAD_CTRL,
//			(uint8_t*) &ctrl4_int1_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Read INT2 Sleep Change */
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_CTRL5_INT2_PAD_CTRL,
//			(uint8_t*) &ctrl5_int2_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /*Disable Interrupts bit if none event is still enabled */
//  if (ctrl5_int2_reg.int2_sleep_chg == 0 && ctrl4_int1_reg.int1_wu == 0 && ctrl4_int1_reg.int1_6d == 0)
//  {
//		if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_CTRL_REG7,
//				(uint8_t*) &ctrl_reg7, 1) != MCP3201_4921_OK)
//    {
//			return MCP3201_4921_ERROR;
//    }
//
//    ctrl_reg7.interrupts_enable = PROPERTY_DISABLE;
//
//		if (lis2dtw12_write_reg(&(pObj->Ctx), MCP3201_4921_CTRL_REG7,
//				(uint8_t*) &ctrl_reg7, 1) != MCP3201_4921_OK)
//    {
//			return MCP3201_4921_ERROR;
//    }
//  }
//
//  /* Reset 6D orientation threshold. */
//	if (lis2dtw12_6d_threshold_set(&(pObj->Ctx), 0) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Set 6D orientation threshold
//  * @param  pObj the device pObj
//  * @param  Threshold 6D orientation detection threshold
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Set_6D_Orientation_Threshold(
//		MCP3201_4921_Object_t *pObj, uint8_t Threshold)
//{
//  if (Threshold > 3)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	if (lis2dtw12_6d_threshold_set(&(pObj->Ctx), Threshold) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Get the status of XLow orientation
//  * @param  pObj the device pObj
//  * @param  XLow the status of XLow orientation
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Get_6D_Orientation_XL(MCP3201_4921_Object_t *pObj,
//		uint8_t *XLow)
//{
//  lis2dtw12_sixd_src_t data;
//
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_SIXD_SRC,
//			(uint8_t*) &data, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  *XLow = data.xl;
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Get the status of XHigh orientation
//  * @param  pObj the device pObj
//  * @param  XHigh the status of XHigh orientation
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Get_6D_Orientation_XH(MCP3201_4921_Object_t *pObj,
//		uint8_t *XHigh)
//{
//  lis2dtw12_sixd_src_t data;
//
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_SIXD_SRC,
//			(uint8_t*) &data, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  *XHigh = data.xh;
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Get the status of YLow orientation
//  * @param  pObj the device pObj
//  * @param  YLow the status of YLow orientation
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Get_6D_Orientation_YL(MCP3201_4921_Object_t *pObj,
//		uint8_t *YLow)
//{
//  lis2dtw12_sixd_src_t data;
//
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_SIXD_SRC,
//			(uint8_t*) &data, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  *YLow = data.yl;
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Get the status of YHigh orientation
//  * @param  pObj the device pObj
//  * @param  YHigh the status of YHigh orientation
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Get_6D_Orientation_YH(MCP3201_4921_Object_t *pObj,
//		uint8_t *YHigh)
//{
//  lis2dtw12_sixd_src_t data;
//
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_SIXD_SRC,
//			(uint8_t*) &data, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  *YHigh = data.yh;
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Get the status of ZLow orientation
//  * @param  pObj the device pObj
//  * @param  ZLow the status of ZLow orientation
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Get_6D_Orientation_ZL(MCP3201_4921_Object_t *pObj,
//		uint8_t *ZLow)
//{
//  lis2dtw12_sixd_src_t data;
//
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_SIXD_SRC,
//			(uint8_t*) &data, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  *ZLow = data.zl;
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Get the status of ZHigh orientation
//  * @param  pObj the device pObj
//  * @param  ZHigh the status of ZHigh orientation
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Get_6D_Orientation_ZH(MCP3201_4921_Object_t *pObj,
//		uint8_t *ZHigh)
//{
//  lis2dtw12_sixd_src_t data;
//
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_SIXD_SRC,
//			(uint8_t*) &data, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  *ZHigh = data.zh;
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Get the status of all hardware events
//  * @param  pObj the device pObj
//  * @param  Status the status of all hardware events
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Get_Event_Status(MCP3201_4921_Object_t *pObj,
//		MCP3201_4921_Event_Status_t *Status)
//{
//  lis2dtw12_status_t status_reg;
//  lis2dtw12_ctrl4_int1_pad_ctrl_t ctrl4_int1_reg;
//  lis2dtw12_ctrl5_int2_pad_ctrl_t ctrl5_int2_reg;
//
//	(void) memset((void*) Status, 0x0, sizeof(MCP3201_4921_Event_Status_t));
//
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_STATUS,
//			(uint8_t*) &status_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_CTRL4_INT1_PAD_CTRL,
//			(uint8_t*) &ctrl4_int1_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_CTRL5_INT2_PAD_CTRL,
//			(uint8_t*) &ctrl5_int2_reg, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  if (ctrl4_int1_reg.int1_wu == 1U)
//  {
//    if (status_reg.wu_ia == 1U)
//    {
//      Status->WakeUpStatus = 1;
//    }
//  }
//
//  if (ctrl4_int1_reg.int1_6d == 1U)
//  {
//    if (status_reg._6d_ia == 1U)
//    {
//      Status->D6DOrientationStatus = 1;
//    }
//  }
//
//  if (ctrl5_int2_reg.int2_sleep_chg == 1U)
//  {
//    if (status_reg.sleep_state == 1U)
//    {
//      Status->SleepStatus = 1;
//    }
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Set self test
//  * @param  pObj the device pObj
//  * @param  Val the value of ST in reg CTRL3
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Set_SelfTest(MCP3201_4921_Object_t *pObj, uint8_t Val)
//{
//  lis2dtw12_st_t reg;
//
//  if (Val > 2U)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	reg = (Val == 0U) ? MCP3201_4921_XL_ST_DISABLE :
//			(Val == 1U) ?
//					MCP3201_4921_XL_ST_POSITIVE : MCP3201_4921_XL_ST_NEGATIVE;
//
//	if (lis2dtw12_self_test_set(&(pObj->Ctx), reg) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
// * @brief  Get the MCP3201_4921 ACC data ready bit value
//  * @param  pObj the device pObj
//  * @param  Status the status of data ready bit
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Get_DRDY_Status(MCP3201_4921_Object_t *pObj,
//		uint8_t *Status)
//{
//	if (lis2dtw12_flag_data_ready_get(&(pObj->Ctx), Status) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
// * @brief  Get the MCP3201_4921 ACC initialization status
//  * @param  pObj the device pObj
//  * @param  Status 1 if initialized, 0 otherwise
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_ACC_Get_Init_Status(MCP3201_4921_Object_t *pObj,
//		uint8_t *Status)
//{
//  if (pObj == NULL)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  *Status = pObj->is_initialized;
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Get the number of samples contained into the FIFO
//  * @param  pObj the device pObj
//  * @param  NumSamples the number of samples contained into the FIFO
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_FIFO_Get_Num_Samples(MCP3201_4921_Object_t *pObj,
//		uint16_t *NumSamples)
//{
//  lis2dtw12_fifo_samples_t FIFO_Samples;
//
//	if (lis2dtw12_read_reg(&(pObj->Ctx), MCP3201_4921_FIFO_SAMPLES,
//			(uint8_t*) &FIFO_Samples, 1) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  if (FIFO_Samples.diff == 0x20)
//  {
//    *NumSamples = 32;
//  }
//  else
//  {
//    *NumSamples = FIFO_Samples.diff;
//  }
//
//	return MCP3201_4921_OK;
//}
//
///**
//  * @brief  Set the FIFO mode
//  * @param  pObj the device pObj
//  * @param  Mode FIFO mode
//  * @retval 0 in case of success, an error code otherwise
//  */
//int32_t MCP3201_4921_FIFO_Set_Mode(MCP3201_4921_Object_t *pObj, uint8_t Mode)
//{
//	int32_t ret = MCP3201_4921_OK;
//
//  /* Verify that the passed parameter contains one of the valid values. */
//  switch ((lis2dtw12_fmode_t)Mode)
//  {
//	case MCP3201_4921_BYPASS_MODE:
//	case MCP3201_4921_FIFO_MODE:
//	case MCP3201_4921_STREAM_TO_FIFO_MODE:
//	case MCP3201_4921_BYPASS_TO_STREAM_MODE:
//	case MCP3201_4921_STREAM_MODE:
//      break;
//
//    default:
//		ret = MCP3201_4921_ERROR;
//      break;
//  }
//
//	if (ret == MCP3201_4921_ERROR)
//  {
//    return ret;
//  }
//
//	if (lis2dtw12_fifo_mode_set(&(pObj->Ctx), (lis2dtw12_fmode_t) Mode)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  return ret;
//}
//
///**
//  * @}
//  */
//
///** @defgroup MCP3201_4921_Private_Functions MCP3201_4921 Private Functions
//  * @{
//  */
//
///**
// * @brief  Set the MCP3201_4921 accelerometer sensor output data rate when enabled
//  * @param  pObj the device pObj
//  * @param  Odr the functional output data rate to be set
//  * @param  Mode the operating mode to be used
//  * @param  Noise the low noise option
//  * @retval 0 in case of success, an error code otherwise
//  */
//static int32_t MCP3201_4921_ACC_SetOutputDataRate_When_Enabled(
//		MCP3201_4921_Object_t *pObj, float Odr,
//		MCP3201_4921_Operating_Mode_t Mode, MCP3201_4921_Low_Noise_t Noise)
//{
//  lis2dtw12_odr_t new_odr;
//  lis2dtw12_mode_t new_power_mode;
//
//  switch (Mode)
//  {
//	case MCP3201_4921_HIGH_PERFORMANCE_MODE:
//    default:
//      switch (Noise)
//      {
//		case MCP3201_4921_LOW_NOISE_DISABLE:
//        default:
//			new_power_mode = MCP3201_4921_HIGH_PERFORMANCE;
//          break;
//		case MCP3201_4921_LOW_NOISE_ENABLE:
//			new_power_mode = MCP3201_4921_HIGH_PERFORMANCE_LOW_NOISE;
//          break;
//      }
//
//      /* If High Performance mode minimum ODR is 12.5Hz */
//      if (Odr < 12.5f)
//      {
//        Odr = 12.5f;
//      }
//      break;
//	case MCP3201_4921_LOW_POWER_MODE4:
//      switch (Noise)
//      {
//		case MCP3201_4921_LOW_NOISE_DISABLE:
//        default:
//			new_power_mode = MCP3201_4921_CONT_LOW_PWR_4;
//          break;
//		case MCP3201_4921_LOW_NOISE_ENABLE:
//			new_power_mode = MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_4;
//          break;
//      }
//
//      /* If Low Power mode maximum ODR is 200Hz */
//      if (Odr > 200.0f)
//      {
//        Odr = 200.0f;
//      }
//      break;
//	case MCP3201_4921_LOW_POWER_MODE3:
//      switch (Noise)
//      {
//		case MCP3201_4921_LOW_NOISE_DISABLE:
//        default:
//			new_power_mode = MCP3201_4921_CONT_LOW_PWR_3;
//          break;
//		case MCP3201_4921_LOW_NOISE_ENABLE:
//			new_power_mode = MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_3;
//          break;
//      }
//
//      /* If Low Power mode maximum ODR is 200Hz */
//      if (Odr > 200.0f)
//      {
//        Odr = 200.0f;
//      }
//      break;
//	case MCP3201_4921_LOW_POWER_MODE2:
//      switch (Noise)
//      {
//		case MCP3201_4921_LOW_NOISE_DISABLE:
//        default:
//			new_power_mode = MCP3201_4921_CONT_LOW_PWR_2;
//          break;
//		case MCP3201_4921_LOW_NOISE_ENABLE:
//			new_power_mode = MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_2;
//          break;
//      }
//
//      /* If Low Power mode maximum ODR is 200Hz */
//      if (Odr > 200.0f)
//      {
//        Odr = 200.0f;
//      }
//      break;
//	case MCP3201_4921_LOW_POWER_MODE1:
//      switch (Noise)
//      {
//		case MCP3201_4921_LOW_NOISE_DISABLE:
//        default:
//			new_power_mode = MCP3201_4921_CONT_LOW_PWR_12bit;
//          break;
//		case MCP3201_4921_LOW_NOISE_ENABLE:
//			new_power_mode = MCP3201_4921_CONT_LOW_PWR_LOW_NOISE_12bit;
//          break;
//      }
//
//      /* If Low Power mode maximum ODR is 200Hz */
//      if (Odr > 200.0f)
//      {
//        Odr = 200.0f;
//      }
//      break;
//  }
//
//	new_odr =
//			(Odr <= 1.6f) ? MCP3201_4921_XL_ODR_1Hz6_LP_ONLY :
//			(Odr <= 12.5f) ? MCP3201_4921_XL_ODR_12Hz5 :
//			(Odr <= 25.0f) ? MCP3201_4921_XL_ODR_25Hz :
//			(Odr <= 50.0f) ? MCP3201_4921_XL_ODR_50Hz :
//			(Odr <= 100.0f) ? MCP3201_4921_XL_ODR_100Hz :
//			(Odr <= 200.0f) ? MCP3201_4921_XL_ODR_200Hz :
//			(Odr <= 400.0f) ? MCP3201_4921_XL_ODR_400Hz :
//			(Odr <= 800.0f) ?
//					MCP3201_4921_XL_ODR_800Hz : MCP3201_4921_XL_ODR_1k6Hz;
//
//  /* Output data rate selection. */
//	if (lis2dtw12_data_rate_set(&(pObj->Ctx), new_odr) != MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Power mode selection. */
//	if (lis2dtw12_power_mode_set(&(pObj->Ctx), new_power_mode)
//			!= MCP3201_4921_OK)
//  {
//		return MCP3201_4921_ERROR;
//  }
//
//  /* Store the current Odr, Mode and Noise values */
//  pObj->acc_odr = Odr;
//	pObj->temp_odr = (Mode == MCP3201_4921_HIGH_PERFORMANCE_MODE) ? 50.0f
//                   : (Odr >= 50.0f)                            ? 50.0f
//                   :                                             Odr;
//
//  pObj->acc_operating_mode = Mode;
//  pObj->acc_low_noise = Noise;
//
//	return MCP3201_4921_OK;
//}
//
/**
 * @brief  Wrap Read register component function to Bus IO function
 * @param  Handle the device handler
 * @param  Reg the register address
 * @param  pData the stored data pointer
 * @param  Length the length
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData,
		uint16_t Length)
{
	MCP3201_4921_Object_t *pObj = (MCP3201_4921_Object_t*) Handle;

	return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
 * @brief  Wrap Write register component function to Bus IO function
 * @param  Handle the device handler
 * @param  Reg the register address
 * @param  pData the stored data pointer
 * @param  Length the length
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData,
		uint16_t Length)
{
	MCP3201_4921_Object_t *pObj = (MCP3201_4921_Object_t*) Handle;

	return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
