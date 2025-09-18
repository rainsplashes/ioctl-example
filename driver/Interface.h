#pragma once

#include <cstdint>


//this struct will be the same no matter what operation from the driver is desired, and therefore contains variables needed for any operation
//random variables are added in order to make the buffer more confusing to analyze if intercepted
typedef struct _TransferData
{
	int Code;
	int TargetPID;

	int JunkInt;

	uint64_t XorKey;

	int Flags;
	int Protection;

	int JunkInt2;

	uintptr_t ControlRegister3;

	uintptr_t JunkPointer;

	uintptr_t Address;
	uintptr_t Address2;
	uintptr_t Address3;
	uintptr_t Buffer;

	uintptr_t JunkPointer2;


	uintptr_t Size;
	uintptr_t Size2;
} TransferData, *PTransferData;

#define CodeTest 0x2828

#define CodeReadPhysical 0x28f9
#define CodeWritePhysical 0x28f0