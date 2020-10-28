/* SPDX-License-Identifier: BSD-3-Clause */

#ifndef UTILS_H_
#define UTILS_H_

#include <Uefi.h>

VOID *
AllocateZeroPool (
  IN UINTN  Size
  );

BOOLEAN
CompareGuid (
  IN EFI_GUID *Guid1,
  IN EFI_GUID *Guid2
  );

VOID
ZeroMem (
  IN OUT VOID   *Buffer,
  IN     UINTN  Size
  );

CHAR16
GetChar (
  VOID
  );

VOID
AsciiPuts (
  IN CONST CHAR8 *AsciiString
  );

INTN
AsciiPrint (
  IN CONST CHAR8 *AsciiFormat,
  ...
  );

INTN
AsciiSPrint (
  OUT CHAR8       *AsciiString,
  IN CONST CHAR8  *AsciiFormat,
  ...
  );

#endif /* UTILS_H_ */
