/* SPDX-License-Identifier: BSD-3-Clause */

#ifndef STRING_H_
#define STRING_H_

#include <Uefi.h>

#define STR_MAX_LEN ((UINTN)1024)

UINTN
StrnLen (
  IN CONST CHAR16 *String,
  IN UINTN        MaxLength
  );

INTN
StrCmp (
  IN CONST CHAR16 *Lhs,
  IN CONST CHAR16 *Rhs
  );

CHAR16 *
StrnCpy (
  IN OUT CHAR16   *Destination,
  IN CONST CHAR16 *Source,
  IN UINTN        Length
  );

CHAR16 *
StrnCat (
  IN OUT CHAR16   *Destination,
  IN CONST CHAR16 *Source,
  IN UINTN        Length
  );

UINTN
AsciiStrnLen (
  IN CONST CHAR8  *String,
  IN UINTN        MaxLength
  );

INTN
AsciiStrCmp (
  IN CONST CHAR8  *Lhs,
  IN CONST CHAR8  *Rhs
  );

CHAR8 *
AsciiStrnCpy (
  IN OUT CHAR8    *Destination,
  IN CONST CHAR8  *Source,
  IN UINTN        Length
  );

CHAR8 *
AsciiStrnCat (
  IN OUT CHAR8    *Destination,
  IN CONST CHAR8  *Source,
  IN UINTN        Length
  );

#endif /* STRING_H_ */
