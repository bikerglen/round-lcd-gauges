// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xblend4.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XBlend4_CfgInitialize(XBlend4 *InstancePtr, XBlend4_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XBlend4_Start(XBlend4 *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_AP_CTRL) & 0x80;
    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XBlend4_IsDone(XBlend4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XBlend4_IsIdle(XBlend4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XBlend4_IsReady(XBlend4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XBlend4_EnableAutoRestart(XBlend4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XBlend4_DisableAutoRestart(XBlend4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_AP_CTRL, 0);
}

u32 XBlend4_Get_return(XBlend4 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_AP_RETURN);
    return Data;
}
void XBlend4_Set_background_offset(XBlend4 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_BACKGROUND_OFFSET_DATA, (u32)(Data));
    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_BACKGROUND_OFFSET_DATA + 4, (u32)(Data >> 32));
}

u64 XBlend4_Get_background_offset(XBlend4 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_BACKGROUND_OFFSET_DATA);
    Data += (u64)XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_BACKGROUND_OFFSET_DATA + 4) << 32;
    return Data;
}

void XBlend4_Set_foreground_offset(XBlend4 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_FOREGROUND_OFFSET_DATA, (u32)(Data));
    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_FOREGROUND_OFFSET_DATA + 4, (u32)(Data >> 32));
}

u64 XBlend4_Get_foreground_offset(XBlend4 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_FOREGROUND_OFFSET_DATA);
    Data += (u64)XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_FOREGROUND_OFFSET_DATA + 4) << 32;
    return Data;
}

void XBlend4_InterruptGlobalEnable(XBlend4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_GIE, 1);
}

void XBlend4_InterruptGlobalDisable(XBlend4 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_GIE, 0);
}

void XBlend4_InterruptEnable(XBlend4 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_IER);
    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_IER, Register | Mask);
}

void XBlend4_InterruptDisable(XBlend4 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_IER);
    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_IER, Register & (~Mask));
}

void XBlend4_InterruptClear(XBlend4 *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XBlend4_WriteReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_ISR, Mask);
}

u32 XBlend4_InterruptGetEnabled(XBlend4 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_IER);
}

u32 XBlend4_InterruptGetStatus(XBlend4 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XBlend4_ReadReg(InstancePtr->Control_BaseAddress, XBLEND4_CONTROL_ADDR_ISR);
}

