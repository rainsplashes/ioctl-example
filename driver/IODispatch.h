#pragma once

namespace IODispatch
{
	NTSTATUS IODispatch(PDEVICE_OBJECT DeviceObject, PIRP IRP)
	{

		UNREFERENCED_PARAMETER(DeviceObject);

		//Note that I pass the command code in the buffer and not as IOCTL code, which is instead random, to avoid detection
		auto TransferBuffer = reinterpret_cast<PTransferData>(IRP->AssociatedIrp.SystemBuffer);
		switch (TransferBuffer->Code)
		{
		case CodeTest:
		{
			DbgPrintEx(0, 0, "Driver recieved IOCTL Code 0x2828");

			break;
		}
		case CodeReadPhysical:
		{
			Features::ReadPhysical(TransferBuffer);

			break;
		}
		case CodeWritePhysical:
		{
			Features::WritePhysical(TransferBuffer);

			break;
		}
		}

		IRP->IoStatus.Status = STATUS_SUCCESS;
		IRP->IoStatus.Information = sizeof(TransferData);

		IofCompleteRequest(IRP, IO_NO_INCREMENT);
		
		return IRP->IoStatus.Status;
	}
}