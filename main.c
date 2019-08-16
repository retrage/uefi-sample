/* SPDX-License-Identifier: BSD-3-Clause */

#include <Uefi.h>
#include <GlobalTable.h>

int main(void);

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

  return main();
}

int main()
{
  return EFI_SUCCESS;
}
