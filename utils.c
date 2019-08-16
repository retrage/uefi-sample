/* SPDX-License-Identifier: BSD-3-Clause */

#include "utils.h"

#include <Uefi.h>
#include <GlobalTable.h>

#include "printf.h"

const unsigned int BUFSIZE = 1024;

unsigned int strnlen(const char *s, unsigned int maxlen)
{
  const char *a = s;
  for (; *s && (s - a <= maxlen); s++);
  return s - a;
}

char getchar()
{
  if (!gST)
    return '\0';

  EFI_STATUS status;
  EFI_INPUT_KEY key;

  do {
    status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
  } while (status == EFI_NOT_READY);

  return (char)(key.UnicodeChar & 0x00ff);
}

void puts(const char *s)
{
  if (!gST)
    return;

  unsigned int len = strnlen(s, BUFSIZE);

  CHAR16 ws[BUFSIZE];
  for (int i = 0; i < len; i++)
    ws[i] = (CHAR16)*s++;
  ws[len] = L'\0';

  gST->ConOut->OutputString(gST->ConOut, ws);
}

int printf(const char *fmt, ...)
{
  int ret;
  char buf[BUFSIZE];
  __builtin_va_list va;

  __builtin_va_start(va, fmt);
  ret = vsnprintf(buf, BUFSIZE, fmt, va);
  __builtin_va_end(va);

  puts(buf);

  return ret;
}

int sprintf(char *str, const char *fmt, ...)
{
  int ret;
  __builtin_va_list va;

  __builtin_va_start(va, fmt);
  ret = vsnprintf(str, BUFSIZE, fmt, va);
  __builtin_va_end(va);

  return ret;
}
