#include "Func.h"
#include "xor.h"
#include "check.h"

extern uintptr_t cr3;

//also defined in driver, always matching and hardcoded
#define skey 0xb5cf4c0f5

#define s CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2828, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

//copied from driver, so they use the same buffer protocol, more is explained in driver
//again, code field is used here instead of IOCTL code, in order to evade certain detections
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

namespace memory {
	HANDLE driver_handle;
	INT32 process_id;

	bool find_driver() {
		driver_handle = CreateFileW(_(L"\\\\.\\SDriver"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (!driver_handle || (driver_handle == INVALID_HANDLE_VALUE))
			return false;

		return true;
	}

	void test_s() {
		_TransferData arguments = { 0 };
		arguments.Code = 0x2828;
		DeviceIoControl(driver_handle, stest, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	//random 64 bit number
	static uint64_t rand64() {
		return ((uint64_t) rand() | ((uint64_t) rand()) << 32);
	}

	void read_physical(PVOID address, PVOID buffer, DWORD size) {
		//undergoes a data obfuscation process that will be undone once transmitted to the driver
		_TransferData arguments = { NULL };
		//random key. this key will be able to be used to decode the data being sent
		arguments.XorKey = rand64();

		//the code telling the driver to read physical memory. note that IOCTL's standard code isn't used (it's SPSpoofCode, a random code) in order to evade certain detections
		arguments.Code = 0x28f9;
		//all data here will be XOR-ed with the key to make it more confusing to analyze
		arguments.Address2 = cr3 ^ arguments.XorKey;
		arguments.Address = (uintptr_t) address ^ arguments.XorKey;
		arguments.Buffer = (uintptr_t) buffer ^ arguments.XorKey;
		arguments.Size = size;

		//add some random data in the transfer to make data more confusing to analyze if intercepted
		arguments.JunkInt = rand() % 1000;
		arguments.JunkPointer = rand64();
		arguments.JunkPointer2 = rand64();

		//also obfuscate the key. a more complex encryption algorithm can be used here. should be easy to replace
		arguments.XorKey = arguments.XorKey ^ skey;

		//send command to driver handle
		DeviceIoControl(driver_handle, SPSpoofCode, &arguments, sizeof(arguments), &arguments, sizeof(arguments), NULL, NULL);
	}
	void write_physical(PVOID address, PVOID buffer, DWORD size) {
		_TransferData arguments = { NULL };
		arguments.XorKey = rand64();

		arguments.Code = 0x28f0;
		arguments.Address2 = cr3 ^ arguments.XorKey;
		arguments.Address = (uintptr_t)address ^ arguments.XorKey;
		arguments.Buffer = (uintptr_t)buffer ^ arguments.XorKey;
		arguments.Size = size;

		arguments.JunkInt = rand() % 1000;
		arguments.JunkPointer = rand64();
		arguments.JunkPointer2 = rand64();

		arguments.XorKey = arguments.XorKey ^ skey;

		DeviceIoControl(driver_handle, SPSpoofCode, &arguments, sizeof(arguments), &arguments, sizeof(arguments), NULL, NULL);
	}
	
}