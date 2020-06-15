/* SPDX-License-Identifier: BSD-3-Clause */

#ifndef UTILS_H_
#define UTILS_H_

#include <Uefi.h>

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
