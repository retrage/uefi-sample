/* SPDX-License-Identifier: BSD-3-Clause */

#include <Uefi.h>
#include <GlobalTable.h>

EFI_STATUS
EFIAPI
EfiMain (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  gImageHandle = ImageHandle;
  gST = SystemTable;
  gBS = gST->BootServices;

  return EFI_SUCCESS;
}
