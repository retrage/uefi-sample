/* SPDX-License-Identifier: BSD-3-Clause */

#include "string.h"

#include <Uefi.h>
#include <GlobalTable.h>

UINTN
StrnLen (
  IN CONST CHAR16 *String,
  IN UINTN        MaxLength
  )
{
  UINTN Length;

  for (Length = 0; Length <= MaxLength && *String != L'\0'; Length++, String++)
    ;
  return Length;
}

INTN
StrCmp (
  IN CONST CHAR16 *Lhs,
  IN CONST CHAR16 *Rhs
  )
{
  for (; *Lhs == *Rhs && *Lhs; Lhs++, Rhs++)
    ;
  return *(CHAR16 *)Lhs - *(CHAR16 *)Rhs;
}

CHAR16 *
StrnCpy (
  IN OUT CHAR16   *Destination,
  IN CONST CHAR16 *Source,
  IN UINTN        Length
  )
{
  for (; *Source != L'\0' && Length; *Destination++ = *Source++, Length--)
    ;
  gBS->SetMem (Destination, sizeof (CHAR16) * Length, L'\0');
  return Destination;
}

CHAR16 *
StrnCat (
  IN OUT CHAR16   *Destination,
  IN CONST CHAR16 *Source,
  IN UINTN        Length
  )
{
  CHAR16  *Head;

  Head = Destination;
  Destination += StrnLen (Destination, STR_MAX_LEN);
  while (Length  && *Source)
    Length--, *Destination++ = *Source++;
  *Destination++ = L'\0';
  return Head;
}

UINTN
AsciiStrnLen (
  IN CONST CHAR8  *String,
  IN UINTN        MaxLength
  )
{
  UINTN Length;

  for (Length = 0; Length <= MaxLength && *String != '\0'; Length++, String++)
    ;
  return Length;
}

INTN
AsciiStrCmp (
  IN CONST CHAR8  *Lhs,
  IN CONST CHAR8  *Rhs
  )
{
  for (; *Lhs == *Rhs && *Lhs; Lhs++, Rhs++)
    ;
  return *(CHAR8 *)Lhs - *(CHAR8 *)Rhs;
}

CHAR8 *
AsciiStrnCpy (
  IN OUT CHAR8    *Destination,
  IN CONST CHAR8  *Source,
  IN UINTN        Length
  )
{
  for (; *Source != '\0' && Length; *Destination++ = *Source++, Length--)
    ;
  gBS->SetMem (Destination, sizeof (CHAR8) * Length, '\0');
  return Destination;
}

CHAR8 *
AsciiStrnCat (
  IN OUT CHAR8    *Destination,
  IN CONST CHAR8  *Source,
  IN UINTN        Length
  )
{
  CHAR8 *Head;

  Head = Destination;
  Destination += AsciiStrnLen (Destination, STR_MAX_LEN);
  while (Length  && *Source)
    Length--, *Destination++ = *Source++;
  *Destination++ = '\0';
  return Head;
}
