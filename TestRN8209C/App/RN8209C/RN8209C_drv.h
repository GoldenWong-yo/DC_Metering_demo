#ifndef _RN8209C_DRV_H_
#define _RN8209C_DRV_H_

#include "RN8209C_reg_addr.h"





EN_Global_Status RN8209C_Write_Reg(EN_RN8209C_REG_ADDR RegAddr);
EN_Global_Status RN8209C_Read_Reg(EN_RN8209C_REG_ADDR RegAddr);

#endif

