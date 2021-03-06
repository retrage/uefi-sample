/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * The Minimal snprintf() implementation
 *
 * Copyright (c) 2013,2014 Michal Ludvig <michal@logix.cz>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the auhor nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----
 *
 * This is a minimal snprintf() implementation optimised
 * for embedded systems with a very limited program memory.
 * mini_snprintf() doesn't support _all_ the formatting
 * the glibc does but on the other hand is a lot smaller.
 * Here are some numbers from my STM32 project (.bin file size):
 *      no snprintf():      10768 bytes
 *      mini snprintf():    11420 bytes     (+  652 bytes)
 *      glibc snprintf():   34860 bytes     (+24092 bytes)
 * Wasting nearly 24kB of memory just for snprintf() on
 * a chip with 32kB flash is crazy. Use mini_snprintf() instead.
 *
 */

#include "printf.h"

static unsigned int
mini_strlen(const char *s)
{
	unsigned int len = 0;
	while (s[len] != '\0') len++;
	return len;
}

static unsigned int
mini_itoa(int value, unsigned int radix, unsigned int uppercase, unsigned int unsig,
	 char *buffer, unsigned int zero_pad)
{
	char	*pbuffer = buffer;
	int	negative = 0;
	unsigned int	i, len;

	/* No support for unusual radixes. */
	if (radix > 16)
		return 0;

	if (value < 0 && !unsig) {
		negative = 1;
		value = -value;
	}

	/* This builds the string back to front ... */
	do {
		int digit = value % radix;
		*(pbuffer++) = (digit < 10 ? '0' + digit : (uppercase ? 'A' : 'a') + digit - 10);
		value /= radix;
	} while (value > 0);

	for (i = (pbuffer - buffer); i < zero_pad; i++)
		*(pbuffer++) = '0';

	if (negative)
		*(pbuffer++) = '-';

	*(pbuffer) = '\0';

	/* ... now we reverse it (could do it recursively but will
	 * conserve the stack space) */
	len = (pbuffer - buffer);
	for (i = 0; i < len / 2; i++) {
		char j = buffer[i];
		buffer[i] = buffer[len-i-1];
		buffer[len-i-1] = j;
	}

	return len;
}

struct mini_buff {
	char *buffer, *pbuffer;
	unsigned int buffer_len;
};

struct mini_guid {
	unsigned long data1;
	unsigned short data2;
	unsigned short data3;
	unsigned char data4[8];
};

const char *lguid_fmt = "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x";
const char *uguid_fmt = "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X";

struct mini_efi_status_type {
	unsigned long long status;
	char *string;
};

/* FIXME: support 32-bit */
const struct mini_efi_status_type mini_efi_status[] = {
	{ 0x0ULL               , "EFI_SUCCESS"               },
	{ 0x8000000000000001ULL, "EFI_LOAD_ERROR"            },
	{ 0x8000000000000002ULL, "EFI_INVALID_PARAMETER"     },
	{ 0x8000000000000003ULL, "EFI_UNSUPPORTED"           },
	{ 0x8000000000000004ULL, "EFI_BAD_BUFFER_SIZE"       },
	{ 0x8000000000000005ULL, "EFI_BUFFER_TOO_SMALL"      },
	{ 0x8000000000000006ULL, "EFI_NOT_READY"             },
	{ 0x8000000000000007ULL, "EFI_DEVICE_ERROR"          },
	{ 0x8000000000000008ULL, "EFI_WRITE_PROTECTED"       },
	{ 0x8000000000000009ULL, "EFI_OUT_OF_RESOURCES"      },
	{ 0x800000000000000aULL, "EFI_VOLUME_CORRUPTED"      },
	{ 0x800000000000000bULL, "EFI_VOLUME_FULL"           },
	{ 0x800000000000000cULL, "EFI_NO_MEDIA"              },
	{ 0x800000000000000dULL, "EFI_MEDIA_CHANGED"         },
	{ 0x800000000000000eULL, "EFI_NOT_FOUND"             },
	{ 0x800000000000000fULL, "EFI_ACCESS_DENIED"         },
	{ 0x8000000000000010ULL, "EFI_NO_RESPONSE"           },
	{ 0x8000000000000011ULL, "EFI_NO_MAPPING"            },
	{ 0x8000000000000012ULL, "EFI_TIMEOUT"               },
	{ 0x8000000000000013ULL, "EFI_NOT_STARTED"           },
	{ 0x8000000000000014ULL, "EFI_ALREADY_STARTED"       },
	{ 0x8000000000000015ULL, "EFI_ABORTED"               },
	{ 0x8000000000000016ULL, "EFI_ICMP_ERROR"            },
	{ 0x8000000000000017ULL, "EFI_TFTP_ERROR"            },
	{ 0x8000000000000018ULL, "EFI_PROTOCOL_ERROR"        },
	{ 0x8000000000000019ULL, "EFI_INCOMPATIBLE_VERSION"  },
	{ 0x800000000000001aULL, "EFI_SECURITY_VIOLATION"    },
	{ 0x800000000000001bULL, "EFI_CRC_ERROR"             },
	{ 0x800000000000001cULL, "EFI_END_OF_MEDIA"          },
	{ 0x800000000000001fULL, "EFI_END_OF_FILE"           },
	{ 0x8000000000000020ULL, "EFI_INVALID_LANGUAGE"      },
	{ 0x8000000000000021ULL, "EFI_COMPROMISED_DATA"      },
	{ 0x8000000000000023ULL, "EFI_HTTP_ERROR"            },
	{ 0x8000000000000064ULL, "EFI_NETWORK_UNREACHABLE"   },
	{ 0x8000000000000065ULL, "EFI_HOST_UNREACHABLE"      },
	{ 0x8000000000000066ULL, "EFI_PROTOCOL_UNREACHABLE"  },
	{ 0x8000000000000067ULL, "EFI_PORT_UNREACHABLE"      },
	{ 0x8000000000000068ULL, "EFI_CONNECTION_FIN"        },
	{ 0x8000000000000069ULL, "EFI_CONNECTION_RESET"      },
	{ 0x800000000000006aULL, "EFI_CONNECTION_REFUSED"    },
	{ 0x1ULL               , "EFI_WARN_UNKNOWN_GLYPH"    },
	{ 0x2ULL               , "EFI_WARN_DELETE_FAILURE"   },
	{ 0x3ULL               , "EFI_WARN_WRITE_FAILURE"    },
	{ 0x4ULL               , "EFI_WARN_BUFFER_TOO_SMALL" },
	{ 0x5ULL               , "EFI_WARN_STALE_DATA"       },
	{ 0x6ULL               , "EFI_WARN_FILE_SYSTEM"      },
};

static char *
_efi_status_str(unsigned long long status)
{
	unsigned int i;
	unsigned int len;
	len = sizeof(mini_efi_status) / sizeof(mini_efi_status[0]);
	for (i = 0; i < len; i++) {
		if (mini_efi_status[i].status == status)
			return mini_efi_status[i].string;
	}
	return "";
}

static int
_putc(int ch, struct mini_buff *b)
{
	if ((unsigned int)((b->pbuffer - b->buffer) + 1) >= b->buffer_len)
		return 0;
	*(b->pbuffer++) = ch;
	*(b->pbuffer) = '\0';
	return 1;
}

static int
_puts(char *s, unsigned int len, struct mini_buff *b)
{
	unsigned int i;

	if (b->buffer_len - (b->pbuffer - b->buffer) - 1 < len)
		len = b->buffer_len - (b->pbuffer - b->buffer) - 1;

	/* Copy to buffer */
	for (i = 0; i < len; i++)
		*(b->pbuffer++) = s[i];
	*(b->pbuffer) = '\0';

	return len;
}

int
mini_vsnprintf(char *buffer, unsigned int buffer_len, const char *fmt, __builtin_va_list va)
{
	struct mini_buff b;
	char bf[48];
	char ch;

	b.buffer = buffer;
	b.pbuffer = buffer;
	b.buffer_len = buffer_len;

	while ((ch=*(fmt++))) {
		if ((unsigned int)((b.pbuffer - b.buffer) + 1) >= b.buffer_len)
			break;
		if (ch!='%')
			_putc(ch, &b);
		else {
			char zero_pad = 0;
			char *ptr;
			unsigned short *wptr;
			struct mini_guid *g;
			unsigned int len;

			ch=*(fmt++);

			/* Zero padding requested */
			if (ch=='0') {
				ch=*(fmt++);
				if (ch == '\0')
					goto end;
				if (ch >= '0' && ch <= '9')
					zero_pad = ch - '0';
				ch=*(fmt++);
			}

			switch (ch) {
				case 0:
					goto end;

				case 'u':
				case 'd':
					len = mini_itoa(__builtin_va_arg(va, unsigned int), 10, 0, (ch=='u'), bf, zero_pad);
					_puts(bf, len, &b);
					break;

				case 'x':
				case 'X':
					len = mini_itoa(__builtin_va_arg(va, unsigned int), 16, (ch=='X'), 1, bf, zero_pad);
					_puts(bf, len, &b);
					break;

				case 'c' :
					_putc((char)(__builtin_va_arg(va, int)), &b);
					break;

				case 's' :
					ptr = __builtin_va_arg(va, char*);
					_puts(ptr, mini_strlen(ptr), &b);
					break;

                case 'S' :
					wptr = __builtin_va_arg(va, unsigned short *);
                    while (*wptr != L'\0')
						_putc((char)*(wptr++), &b);
					break;

				case 'g' :
				case 'G' :
					g = __builtin_va_arg(va, struct mini_guid*);
					len = mini_snprintf(bf, 48,
						(ch=='G') ? uguid_fmt : lguid_fmt,
						g->data1, g->data2, g->data3,
						g->data4[0], g->data4[1], g->data4[2], g->data4[3],
						g->data4[4], g->data4[5], g->data4[6], g->data4[7]);
					_puts(bf, len, &b);
					break;

				case 'r' :
					ptr = _efi_status_str(__builtin_va_arg(va, unsigned long long));
					_puts(ptr, mini_strlen(ptr), &b);
					break;

				default:
					_putc(ch, &b);
					break;
			}
		}
	}
end:
	return b.pbuffer - b.buffer;
}


int
mini_snprintf(char* buffer, unsigned int buffer_len, const char *fmt, ...)
{
	int ret;
	__builtin_va_list va;
    __builtin_va_start(va, fmt);
	ret = mini_vsnprintf(buffer, buffer_len, fmt, va);
	__builtin_va_end(va);

	return ret;
}
