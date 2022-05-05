
OSNAME = myos

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
LDS = kernel.ld
ASM = nasm
CC = gcc

CFLAGS = -ffreestanding -fshort-wchar -std=c11 -nostdlib -Wno-builtin-macro-redefined -nostdinc -pedantic -Wall -Wextra -mno-red-zone -I ./src
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib
ASMFLAGS = -f elf64

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin
FONTDIR = fonts
BOOTEFI := $(GNUEFI)/bootloader/result/main.efi

FONT = $(FONTDIR)/zap-light16.psf

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

INTERRUPTS_OBJECT_PATH = $(OBJDIR)/idt/interrupts.o
INTERRUPTS_SOURCE_PATH = $(SRCDIR)/idt/interrupts.c

SRC = $(call rwildcard,$(SRCDIR),*.c)          
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)          
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)


kernel: $(OBJS) link

$(INTERRUPTS_OBJECT_PATH): $(INTERRUPTS_SOURCE_PATH)
	@echo !=== INTERRUPTS COMPILING ===!
	@mkdir -p $(@D)
	$(CC) -mgeneral-regs-only -ffreestanding -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo !==== COMPILING ====!
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@
 
$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@echo !====== NASM =======!
	$(ASM) $(ASMFLAGS) $^ -o $@  

link:
	@echo !==== LINKING ====!
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)

# Remove compiled object files
clean:
	@rm -r $(OBJDIR)/*

setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)
	@mkdir $(FONTDIR)

buildimg:
	# Create empty image (sector=512 bytes, sectors number=93750)
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	# Create MSDOS partition (FAT32 filesystem)
	mformat -i $(BUILDDIR)/$(OSNAME).img -f 1440 ::
	# Create MSDOS partition subdirectories for default EFI boot place
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	# Copy compiled EFI bootloader into image (to default boot place)
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	# Copy startup.nsh into root EFI directory of image
	mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	# Copy compiled kernel into EFI root directory
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/kernel.elf ::
	# Copy psf font file into EFI root directory
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(FONT) ::

run:
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS.fd" -net none
