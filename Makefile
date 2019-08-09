TRIPLE=x86_64-pc-win32-ocff
CC=clang
CFLAGS=--target=$(TRIPLE) -Wall -Werror \
	   -mno-red-zone -fno-stack-protector -fshort-wchar \
	   -Iinclude/edk2/MdePkg/Include \
	   -Iinclude/edk2/MdePkg/Include/X64
AS=llvm-mc
ASFLAGS=triple=$(TRIPLE) -filetype=obj
LD=lld-link
LDFLAGS=-subsystem:efi_application -nodefaultlib

SRCS=$(wildcard *.c)
ASMS=$(wildcard *.s)
OBJS=$(SRCS:.c=.o) $(ASMS:.s=.o)
TARGET=main.efi

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -entry:EfiMain $^ -out:$@

qemu: $(TARGET) OVMF.fd image/EFI/BOOT/BOOTX64.EFI
	qemu-system-x86_64 -nographic -bios OVMF.fd -hda fat:image

image/EFI/BOOT/BOOTX64.EFI: $(TARGET)
	mkdir -p image/EFI/BOOT
	ln -sf ../../../main.efi image/EFI/BOOT/BOOTX64.EFI

OVMF.fd:
	wget http://downloads.sourceforge.net/project/edk2/OVMF/OVMF-X64-r15214.zip
	unzip OVMF-X64-r15214.zip OVMF.fd
	rm OVMF-X64-r15214.zip

clean:
	rm -f $(TARGET) OVMF.fd
	rm -rf image
