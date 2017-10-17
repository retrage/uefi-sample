#include "EfiCommon.h"
#include "EfiApi.h"
#include "EfiError.h"
#include "EfiTypes.h"

#define EFI_SYSTEM_TABLE ___EFI_SYSTEM_TABLE

EFI_STATUS
EFIAPI
EfiMain (
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
    )
{
    SystemTable->BootServices->ExitBootServices(ImageHandle, NULL);

    return EFI_SUCCESS;
}
