/* SPDX-License-Identifier: BSD-3-Clause */

#ifndef UTILS_H_
#define UTILS_H_

unsigned int strnlen(const char *s, unsigned int maxlen);
char getchar();
void puts(const char *s);
int printf(const char *fmt, ...);
int sprintf(char *str, const char *fmt, ...);

#endif /* UTILS_H_ */
