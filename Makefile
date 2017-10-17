CC = x86_64-w64-mingw32-gcc
CFLAGS = -m32 -shared -nostdlib -mno-red-zone -fno-stack-protector -Wall \
         -e EfiMain -g -I. -I./edk/Foundation/Efi/Include \
         -I./edk/Foundation/Include -I./edk/Foundation/Efi \
	 -I./edk/Foundation/Framework/Include

all: main.efi

%.efi: %.dll
	objcopy --target=efi-app-i386 $< $@

%.dll: %.c
	$(CC) $(CFLAGS) $< -o $@

qemu: main.efi OVMF.fd image/EFI/BOOT/BOOTX64.EFI
	qemu-system-x86_64 -nographic -bios OVMF.fd -hda fat:image

image/EFI/BOOT/BOOTX64.EFI:
	mkdir -p image/EFI/BOOT
	ln -sf ../../../main.efi image/EFI/BOOT/BOOTX64.EFI

OVMF.fd:
	wget http://downloads.sourceforge.net/project/edk2/OVMF/OVMF-X64-r15214.zip
	unzip OVMF-X64-r15214.zip OVMF.fd
	rm OVMF-X64-r15214.zip

clean:
	rm -f main.efi OVMF.fd
	rm -rf image
