#include <efi.h>
#include <elf.h>
#include <efilib.h>

#include "FrameBuffer.h"

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

static int memcmp(const void* lh, const void* rh, unsigned long long size)
{
	const unsigned char* lhPointer = lh;
	const unsigned char* rhPointer = rh;

	for (unsigned long long i = 0; i < size; ++i)
		if (lhPointer[i] < rhPointer[i]) 
			return -1;
		else if (lhPointer[i] > rhPointer[i]) 
			return 1;

	return 0;
}

FrameBuffer* InitializeGraphicsOutputProtocol() 
{
	static FrameBuffer frameBuffer;
	EFI_GUID gopGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;

	if(EFI_ERROR(uefi_call_wrapper(BS->LocateProtocol, 3, &gopGUID, NULL, (void**) &gop)))
	{
		Print(L"ERR: Unable to locate Graphic Output Protocol\n\r");
		return NULL;
	}
	else
		Print(L"OK: Graphic Output Protocol located\n\r");

	frameBuffer.baseAddress       = (void*)gop->Mode->FrameBufferBase;
	frameBuffer.bufferSize        = gop->Mode->FrameBufferSize;
	frameBuffer.width             = gop->Mode->Info->HorizontalResolution;
	frameBuffer.height            = gop->Mode->Info->VerticalResolution;
	frameBuffer.pixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

	return &frameBuffer;
}

static EFI_FILE* LoadFile(EFI_FILE* directory, CHAR16* path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_FILE* loadedFile;
	EFI_LOADED_IMAGE_PROTOCOL* loadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fileSystem;

	systemTable->BootServices->HandleProtocol(imageHandle, &gEfiLoadedImageProtocolGuid, (void**) &loadedImage);
	systemTable->BootServices->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**) &fileSystem);

	if (directory == NULL)
		fileSystem->OpenVolume(fileSystem, &directory);

	EFI_STATUS status = directory->Open(directory, &loadedFile, path, 
										EFI_FILE_MODE_READ, 
										EFI_FILE_READ_ONLY);

	if (status != EFI_SUCCESS)
		return NULL;
	return loadedFile;
}


PSFFont* LoadPSF1Font(EFI_FILE* directory, CHAR16* path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_FILE* font = LoadFile(directory, path, imageHandle, systemTable);
	if (font == NULL) 
		return NULL;

	PSFHeader* fontHeader;
	systemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSFHeader), (void**) &fontHeader);

	UINTN size = sizeof(PSFHeader);
	font->Read(font, &size, fontHeader);

	if (fontHeader->magic[0] != PSF1_MAGIC0 || 
		fontHeader->magic[1] != PSF1_MAGIC1)
		return NULL;

	UINTN glyphBufferSize = fontHeader->charSize * 256;
	if (fontHeader->mode == 1)
		glyphBufferSize = fontHeader->charSize * 512;

	void* glyphBuffer;
	{
		font->SetPosition(font, sizeof(PSFHeader));
		systemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**) &glyphBuffer);
		font->Read(font, &glyphBufferSize, glyphBuffer);
	}

	PSFFont* finishedFont;
	systemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSFFont), (void**) &finishedFont);
	finishedFont->header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;
	return finishedFont;

}

EFI_STATUS efi_main (EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	InitializeLib(imageHandle, systemTable);

	EFI_FILE* kernel = LoadFile(NULL, L"kernel.elf", imageHandle, systemTable);
	if (kernel == NULL)
		Print(L"ERR: Could not load kernel.\n\r");
	else
		Print(L"OK: Kernel loaded successfully.\n\r");

	Elf64_Ehdr header;
	{
		UINTN fileInfoSize;
		EFI_FILE_INFO* fileInfo;

		kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fileInfoSize, NULL);
		systemTable->BootServices->AllocatePool(EfiLoaderData, fileInfoSize, (void**) &fileInfo);
		kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fileInfoSize, (void**) &fileInfo);

		UINTN size = sizeof(header);
		kernel->Read(kernel, &size, &header);
	}

	if (memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64  ||
		header.e_ident[EI_DATA]  != ELFDATA2LSB ||
		header.e_type            != ET_EXEC     ||
		header.e_machine         != EM_X86_64   ||
		header.e_version         != EV_CURRENT)
		Print(L"ERR: Bad kernel format.\n\r");
	else
		Print(L"OK: Kernel verified.\n\r");

	Elf64_Phdr* programHeaders;
	{
		kernel->SetPosition(kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		systemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**) &programHeaders);
		kernel->Read(kernel, &size, programHeaders);
	}

	for (Elf64_Phdr* programHeader = programHeaders;
		(char*)programHeader < (char*) programHeaders + header.e_phnum * header.e_phentsize;
		programHeader = (Elf64_Phdr*) ((char*) programHeader + header.e_phentsize))
		switch (programHeader->p_type)
		{
			case PT_LOAD:
			{
				int pages = (programHeader->p_memsz + 0x1000 - 1) / 0x1000;
				Elf64_Addr segment = programHeader->p_paddr;
				systemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

				kernel->SetPosition(kernel, programHeader->p_offset);
				UINTN size = programHeader->p_filesz;
				kernel->Read(kernel, &size, (void*) segment);
				break;
			}
		}

	Print(L"OK: Kernel ready.\n\r");
	void (*kernelEntry)(FrameBuffer*, PSFFont*) = ((__attribute__((sysv_abi)) void (*)(FrameBuffer*, PSFFont*)) header.e_entry);

	PSFFont* font = LoadPSF1Font(NULL, L"zap-light16.psf", imageHandle, systemTable);
	if (font == NULL)
		Print(L"ERR: Could not load font\n\r");
	else
		Print(L"OK: Loaded font & Character Size = %d\n\r", font->header->charSize);

	FrameBuffer* frameBuffer = InitializeGraphicsOutputProtocol();
	{
		Print(L"==== Frame Buffer Info ====\n\r");
		Print(L"Base: 0x%X\n\r", frameBuffer->baseAddress);
		Print(L"Size: 0x%X\n\r", frameBuffer->bufferSize);
		Print(L"Width: %d\n\r", frameBuffer->width);
		Print(L"Height: %d\n\r", frameBuffer->height);
		Print(L"Pixels/Scanline: %d\n\r", frameBuffer->pixelsPerScanLine);
	}

	kernelEntry(frameBuffer, font);
	return EFI_SUCCESS;
}
