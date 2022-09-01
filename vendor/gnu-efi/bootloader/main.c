#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main (EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
	systemTable->ConOut->OutputString(systemTable->ConOut, L"Bootloader ready\n\r");
	systemTable->ConIn->Reset(systemTable->ConIn, FALSE);

	EFI_INPUT_KEY key;
	while (systemTable->ConIn->ReadKeyStroke(systemTable->ConIn, &key) == EFI_NOT_READY);

	return EFI_SUCCESS;
}
