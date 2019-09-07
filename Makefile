# SPDX-License-Identifier: BSD-3-Clause

TRIPLE=x86_64-pc-win32-ocff
CC=clang
CFLAGS=--target=$(TRIPLE) -Wall -Werror \
	   -mno-red-zone -fno-stack-protector -fshort-wchar \
	   -Iinclude/edk2/MdePkg/Include \
	   -Iinclude/edk2/MdePkg/Include/X64 \
	   -Iinclude
AS=llvm-mc
ASFLAGS=-triple=$(TRIPLE) -filetype=obj
LD=lld-link
LDFLAGS=-subsystem:efi_application -nodefaultlib

SRCS=$(wildcard *.c)
ASMS=$(wildcard *.s)
OBJS=$(SRCS:.c=.o) $(ASMS:.s=.o)
TARGET=main.efi

URL=https://github.com/retrage/edk2-nightly/raw/master/bin/RELEASEX64_OVMF.fd
ROM=OVMF.fd
APP=image/EFI/BOOT/BOOTX64.EFI
IMG=$(firstword $(subst /, ,$(dir $(APP))))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -entry:EfiMain $^ -out:$@

$(ROM):
	wget -q -O $(ROM) $(URL)

$(APP): $(TARGET)
	mkdir -p $(dir $(APP))
	cp $(TARGET) $(APP)

qemu: $(APP) $(ROM)
	qemu-system-x86_64 -bios $(ROM) -drive file=fat:rw:$(IMG) -nographic

clean:
	rm -rf $(TARGET) $(OBJS) $(IMG)

clean-all: clean
	rm -rf $(ROM)

.PHONY: all clean clean-all qemu
