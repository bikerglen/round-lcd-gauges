// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XBLEND4_H
#define XBLEND4_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xblend4_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
    u16 DeviceId;
    u32 Control_BaseAddress;
} XBlend4_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XBlend4;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XBlend4_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XBlend4_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XBlend4_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XBlend4_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XBlend4_Initialize(XBlend4 *InstancePtr, u16 DeviceId);
XBlend4_Config* XBlend4_LookupConfig(u16 DeviceId);
int XBlend4_CfgInitialize(XBlend4 *InstancePtr, XBlend4_Config *ConfigPtr);
#else
int XBlend4_Initialize(XBlend4 *InstancePtr, const char* InstanceName);
int XBlend4_Release(XBlend4 *InstancePtr);
#endif

void XBlend4_Start(XBlend4 *InstancePtr);
u32 XBlend4_IsDone(XBlend4 *InstancePtr);
u32 XBlend4_IsIdle(XBlend4 *InstancePtr);
u32 XBlend4_IsReady(XBlend4 *InstancePtr);
void XBlend4_EnableAutoRestart(XBlend4 *InstancePtr);
void XBlend4_DisableAutoRestart(XBlend4 *InstancePtr);
u32 XBlend4_Get_return(XBlend4 *InstancePtr);

void XBlend4_Set_background_offset(XBlend4 *InstancePtr, u64 Data);
u64 XBlend4_Get_background_offset(XBlend4 *InstancePtr);
void XBlend4_Set_foreground_offset(XBlend4 *InstancePtr, u64 Data);
u64 XBlend4_Get_foreground_offset(XBlend4 *InstancePtr);

void XBlend4_InterruptGlobalEnable(XBlend4 *InstancePtr);
void XBlend4_InterruptGlobalDisable(XBlend4 *InstancePtr);
void XBlend4_InterruptEnable(XBlend4 *InstancePtr, u32 Mask);
void XBlend4_InterruptDisable(XBlend4 *InstancePtr, u32 Mask);
void XBlend4_InterruptClear(XBlend4 *InstancePtr, u32 Mask);
u32 XBlend4_InterruptGetEnabled(XBlend4 *InstancePtr);
u32 XBlend4_InterruptGetStatus(XBlend4 *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
