#pragma once

//also defined in userspace program to undo change
#define key 0xb5cf4c0f5

namespace Features
{
	void ReadPhysical(PTransferData Buffer)
	{
		//undo the data obfuscation done in userspace, so data does not get red flagged in transit, more detail in userspace
		uintptr_t XorKeySolved = Buffer->XorKey ^ key;
		Physical::ReadMemory((Buffer->Address2) ^ XorKeySolved, (Buffer->Address) ^ XorKeySolved, (void*)((Buffer->Buffer) ^ XorKeySolved), Buffer->Size);
	}

	void WritePhysical(PTransferData Buffer)
	{
		Physical::WriteMemory(Buffer->Address2, Buffer->Address, (void*)Buffer->Buffer, Buffer->Size);
	}
}