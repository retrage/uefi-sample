/* SPDX-License-Identifier: BSD-3-Clause */

#include "utils.h"

#include <Uefi.h>
#include <GlobalTable.h>

#include "printf.h"
#include "string.h"

BOOLEAN
CompareGuid (
  IN EFI_GUID *Guid1,
  IN EFI_GUID *Guid2
  )
{
  UINTN Index;

  if (Guid1 == NULL || Guid2 == NULL) {
    return FALSE;
  }

  if (Guid1->Data1 != Guid2->Data1) {
    return FALSE;
  }
  if (Guid1->Data2 != Guid2->Data2) {
    return FALSE;
  }
  if (Guid1->Data3 != Guid2->Data3) {
    return FALSE;
  }

  for (Index = 0; Index < 8; Index++) {
    if (Guid1->Data4[Index] != Guid2->Data4[Index]) {
      return FALSE;
    }
  }

  return TRUE;
}

CHAR16
GetChar (
  VOID
  )
{
  if (gST == NULL) {
    return L'\0';
  }

  EFI_STATUS status;
  EFI_INPUT_KEY key;

  do {
    status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
  } while (status == EFI_NOT_READY);

  return key.UnicodeChar;
}

VOID
AsciiPuts (
  IN CONST CHAR8  *AsciiString
  )
{
  UINTN   Length;
  UINTN   Index;
  CHAR16  String[STR_MAX_LEN];

  if (gST == NULL) {
    return;
  }

  Length = AsciiStrnLen (AsciiString, STR_MAX_LEN);

  for (Index = 0; Index < Length; Index++) {
    String[Index] = (CHAR16)*AsciiString++;
  }
  String[Length] = L'\0'; 

  gST->ConOut->OutputString(gST->ConOut, String);
}

INTN
AsciiPrint (
  IN CONST CHAR8  *AsciiFormat,
  ...
  )
{
  __builtin_va_list VA;
  CHAR8             AsciiString[STR_MAX_LEN];
  INTN              Ret;

  __builtin_va_start(VA, AsciiFormat);
  Ret = vsnprintf(AsciiString, STR_MAX_LEN, AsciiFormat, VA);
  __builtin_va_end(VA);

  AsciiPuts (AsciiString);

  return Ret;
}

INTN
AsciiSPrint (
  OUT CHAR8       *AsciiString,
  IN CONST CHAR8  *AsciiFormat,
  ...
  )
{
  __builtin_va_list VA;
  INTN              Ret;

  __builtin_va_start(VA, AsciiFormat);
  Ret = vsnprintf(AsciiString, STR_MAX_LEN, AsciiFormat, VA);
  __builtin_va_end(VA);

  return Ret;
}
