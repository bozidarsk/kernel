#include <efi.h>
#include <efilib.h>
#include <elf.h>

#include "include/kernel/acpi.h"

#define breakpoint() ({ __asm__ volatile("int3"); })
#define halt() ({ __asm__ volatile("hlt"); })

bool ExitedBootServices = false;

uint8_t tmpelf[ELF_SIZE];
uint8_t tmpxsdt[4096];

__attribute__((aligned(16)))
uint8_t tmpstack[4096];

void Printf(const CHAR16* fmt, ...) 
{
	if (!ExitedBootServices) 
	{
		va_list args;
		va_start(args, fmt);
		Print(fmt, args);
		va_end(args);
	}
}

__attribute__((noreturn))
void Errorf(const CHAR16* message, ...) 
{
	if (!ExitedBootServices) 
	{
		va_list args;
		va_start(args, message);
		Print(message, args);
		va_end(args);
	}

	do 
	{
		breakpoint();
		halt();
	} while (true);
}

void* CopyMemory(void* destination, void* source, size_t size) 
{
	if (!destination || !source)
		return destination;

	for (size_t i = 0; i < size; i++)
		((uint8_t*)destination)[i] = ((uint8_t*)source)[i];

	return destination;
}

void* SetMemory(void* destination, int32_t value, size_t size) 
{
	if (!destination)
		return destination;

	for (size_t i = 0; i < size; i++)
		((uint8_t*)destination)[i] = (uint8_t)value;

	return destination;
}

void PrintMemoryMap(void)
{
    EFI_STATUS Status;
    UINTN MemoryMapSize = 0;
    EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;
    
    // Get memory map size
    Status = uefi_call_wrapper(BS->GetMemoryMap, 5, &MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    
    MemoryMapSize += 2*  DescriptorSize;
    Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, MemoryMapSize, (VOID**)&MemoryMap);
    if (EFI_ERROR(Status)) Errorf(L"PrintMemoryMap(): BS->AllocatePool(): failed to allocate memory map - %r\n", Status);
    
    Status = uefi_call_wrapper(BS->GetMemoryMap, 5, &MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    if (EFI_ERROR(Status)) 
    {
        Errorf(L"PrintMemoryMap(): BS->GetMemoryMap(): failed to get memory map - %r\n", Status);
        uefi_call_wrapper(BS->FreePool, 1, MemoryMap);
        return;
    }
    
    Printf(L"\n=== Memory Map ===\n");
    Printf(L"Type                     Start            End              Pages\n");
    Printf(L"-----------------------------------------------------------------------\n");
    
    UINT8* MapPtr = (UINT8*)MemoryMap;
    UINT8* MapEnd = MapPtr + MemoryMapSize;
    
    while (MapPtr < MapEnd) 
    {
        EFI_MEMORY_DESCRIPTOR* Desc = (EFI_MEMORY_DESCRIPTOR*)MapPtr;
        
        const CHAR16* TypeName;
        switch (Desc->Type) 
        {
            case EfiReservedMemoryType:      TypeName = L"Reserved"; break;
            case EfiLoaderCode:              TypeName = L"LoaderCode"; break;
            case EfiLoaderData:              TypeName = L"LoaderData"; break;
            case EfiBootServicesCode:        TypeName = L"BootSrvCode"; break;
            case EfiBootServicesData:        TypeName = L"BootSrvData"; break;
            case EfiRuntimeServicesCode:     TypeName = L"RuntimeCode"; break;
            case EfiRuntimeServicesData:     TypeName = L"RuntimeData"; break;
            case EfiConventionalMemory:      TypeName = L"Conventional"; break;
            case EfiUnusableMemory:          TypeName = L"Unusable"; break;
            case EfiACPIReclaimMemory:       TypeName = L"ACPIReclaim"; break;
            case EfiACPIMemoryNVS:           TypeName = L"ACPI_NVS"; break;
            case EfiMemoryMappedIO:          TypeName = L"MMIO"; break;
            case EfiMemoryMappedIOPortSpace: TypeName = L"MMIOPort"; break;
            case EfiPalCode:                 TypeName = L"PalCode"; break;
            default:                         TypeName = L"Unknown"; break;
        }
        
        EFI_PHYSICAL_ADDRESS End = Desc->PhysicalStart + (Desc->NumberOfPages * 4096);
        
        Printf(L"%-20s 0x%016lx 0x%016lx %6ld\n", TypeName, Desc->PhysicalStart, End, Desc->NumberOfPages);
        
        MapPtr += DescriptorSize;
    }
    
    Printf(L"=======================================================================\n\n");
    
    uefi_call_wrapper(BS->FreePool, 1, MemoryMap);
}

void Read(EFI_HANDLE image, const CHAR16* path) 
{
	Printf(L"reading elf file '" ELF_PATH "'\n");

	EFI_LOADED_IMAGE* loadedImage = NULL;				    /* image interface */
	EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;	    /* image interface GUID */
	EFI_FILE_IO_INTERFACE* IOVolume;						/* file system interface */
	EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID; /* file system interface GUID */
	EFI_FILE_HANDLE Volume;								    /* the volume's interface */
	EFI_STATUS Status;

	/* get the loaded image protocol interface for our "image" */
	Status = uefi_call_wrapper(BS->HandleProtocol, 3, image, &lipGuid, (void**) &loadedImage);
	if (EFI_ERROR(Status)) Errorf(L"Read(): BS->HandleProtocol(): %r\n", Status);

	/* get the volume handle */
	Status = uefi_call_wrapper(BS->HandleProtocol, 3, loadedImage->DeviceHandle, &fsGuid, (VOID*)&IOVolume);
	if (EFI_ERROR(Status)) Errorf(L"Read(): BS->HandleProtocol(): %r\n", Status);
	Status = uefi_call_wrapper(IOVolume->OpenVolume, 2, IOVolume, &Volume);
	if (EFI_ERROR(Status)) Errorf(L"Read(): IOVolume->OpenVolume(): %r\n", Status);

	EFI_FILE_HANDLE FileHandle;

	/* open the file */
	Status = uefi_call_wrapper(Volume->Open, 5, Volume, &FileHandle, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
	if (EFI_ERROR(Status)) Errorf(L"Read(): Volume->Open(): %r\n", Status);

	/* read from the file */
	UINT64 ReadSize = ELF_SIZE;

	Status = uefi_call_wrapper(FileHandle->Read, 3, FileHandle, &ReadSize, tmpelf);
	if (EFI_ERROR(Status)) Errorf(L"Read(): FileHandle->Read(): %r\n", Status);

	/* close the file */
	Status = uefi_call_wrapper(FileHandle->Close, 1, FileHandle);
	if (EFI_ERROR(Status)) Errorf(L"Read(): FileHandle->Close(): %r\n", Status);
}

void* Load() 
{
	Printf(L"loading elf file '" ELF_PATH "'\n");

	Elf64_Ehdr* ElfHeader = (Elf64_Ehdr*)tmpelf;

	if (false
		|| ElfHeader->e_ident[EI_MAG0] != ELFMAG0
		|| ElfHeader->e_ident[EI_MAG1] != ELFMAG1
		|| ElfHeader->e_ident[EI_MAG2] != ELFMAG2
		|| ElfHeader->e_ident[EI_MAG3] != ELFMAG3
	) Errorf(L"Load(): invalid elf magic\n");

	if (ElfHeader->e_ident[EI_CLASS] != ELFCLASS64)
		Errorf(L"Load(): elf class must be ELF64\n");

	Elf64_Phdr* ProgramHeaders = (Elf64_Phdr*)((UINT8*)tmpelf + ElfHeader->e_phoff);

	for (int i = 0; i < ElfHeader->e_phnum; i++) 
	{
		Elf64_Phdr* phdr = ProgramHeaders + i;
		
		if (phdr->p_type != PT_LOAD)
			continue;

		Printf(L"segment %d wants paddr=%p vaddr=%p size=%p\n", i, phdr->p_paddr, phdr->p_vaddr, phdr->p_memsz);

		if (phdr->p_memsz == 0) 
		{
			Printf(L"segment %d is zero-sized - skipping\n", i);
			continue;
		}

		EFI_PHYSICAL_ADDRESS SegmentAddr = phdr->p_paddr;

		CopyMemory((void*)SegmentAddr, (UINT8*)tmpelf + phdr->p_offset, phdr->p_filesz);

		if (phdr->p_memsz > phdr->p_filesz)
			SetMemory((void*)(SegmentAddr + phdr->p_filesz), 0, phdr->p_memsz - phdr->p_filesz);

		Printf(L"segment %d loaded at %p (requested %p)\n", i, SegmentAddr, phdr->p_paddr);
	}

	Printf(L"entry point found at %p\n", ElfHeader->e_entry);
	return (void*)ElfHeader->e_entry;
}

void ExitBootServices(EFI_HANDLE ImageHandle) 
{
	Printf(L"exiting boot services\n");

	EFI_STATUS Status;

	// Get memory map
	UINTN MemoryMapSize = 0;
	EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
	UINTN MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;

	// Get memory map size
	Status = uefi_call_wrapper(BS->GetMemoryMap, 5, &MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);

	MemoryMapSize += 2 * DescriptorSize; // Extra space
	Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, MemoryMapSize, (VOID**)&MemoryMap);

	Status = uefi_call_wrapper(BS->GetMemoryMap, 5, &MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
	if (EFI_ERROR(Status)) Errorf(L"ExitBootServices(): failed to get memory map - %r\n", Status);

	// Exit boot services
	Status = uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, MapKey);
	if (EFI_ERROR(Status)) Errorf(L"ExitBootServices(): failed to exit boot services - %r\n", Status);

	ExitedBootServices = true;
}

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) 
{
	InitializeLib(ImageHandle, SystemTable);

	// PrintMemoryMap();

	for (UINTN i = 0; i < SystemTable->NumberOfTableEntries; i++) 
	{
		if (!CompareGuid(&(SystemTable->ConfigurationTable[i].VendorGuid), &AcpiTableGuid))
			continue;

		if ((*(uint64_t*)SystemTable->ConfigurationTable[i].VendorTable & 0xffffffffffffff) != 0x52545020445352) // RSD PTR
			continue;

		XSDP* xsdp = (XSDP*)SystemTable->ConfigurationTable[i].VendorTable;
		XSDT* xsdt = (XSDT*)xsdp->xsdtAddress;

		if (xsdt->header.size > sizeof(tmpxsdt))
			Errorf(L"xsdt->header.size > sizeof(tmpxsdt)\n");

		CopyMemory(tmpxsdt, xsdt, xsdt->header.size);

		Printf(L"xsdt found addr=%p size=%d signature=%p\n", xsdt, xsdt->header.size, *(uint64_t*)(&xsdt->header.signature));
	}

	Printf(L"booting '" ELF_PATH "'\n");

	Read(ImageHandle, ELF_PATH);

	ExitBootServices(ImageHandle);
	__asm__ volatile("cli");

	__asm__ volatile("mov %0, %%rsp" : : "r" (tmpstack + sizeof(tmpstack)));

	void* entry = Load();

	__asm__ volatile("mov %0, %%rdi" : : "r" (tmpelf));
	__asm__ volatile("mov %0, %%rsi" : : "r" (sizeof(tmpelf)));
	__asm__ volatile("mov %0, %%rdx" : : "r" (tmpxsdt));
	__asm__ volatile("mov %0, %%rcx" : : "r" ((uint64_t)((XSDT*)tmpxsdt)->header.size));
	__asm__ volatile("jmp *%0" : : "r" (entry));

	while (true) __asm__ volatile("hlt");
}
