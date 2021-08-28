// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xblend4.h"

extern XBlend4_Config XBlend4_ConfigTable[];

XBlend4_Config *XBlend4_LookupConfig(u16 DeviceId) {
	XBlend4_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XBLEND4_NUM_INSTANCES; Index++) {
		if (XBlend4_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XBlend4_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XBlend4_Initialize(XBlend4 *InstancePtr, u16 DeviceId) {
	XBlend4_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XBlend4_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XBlend4_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

