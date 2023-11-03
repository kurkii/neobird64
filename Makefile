BUILD_DIR = build
KERN = kern
CFLAGS += -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fPIE \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
	-I kern/include
LDFLAGS += -m elf_x86_64 \
    -nostdlib \
    -static \
    -pie \
    --no-dynamic-linker \
    -z text \
    -z max-page-size=0x1000 \
    -T linker.ld
NASMFLAGS = -f elf64

all:



	# make build directory
	mkdir -p $(BUILD_DIR)

	# build & link boot and kernel files


	amd64-elf-gcc -c $(KERN)/kern.c -o $(BUILD_DIR)/kern.o $(CFLAGS)

	amd64-elf-gcc -c $(KERN)/include/video.c -o $(BUILD_DIR)/video.o $(CFLAGS)
	amd64-elf-gcc -c $(KERN)/include/printf.c -o $(BUILD_DIR)/printf.o $(CFLAGS)
	amd64-elf-gcc -c $(KERN)/include/log.c -o $(BUILD_DIR)/log.o $(CFLAGS)
	amd64-elf-gcc -c $(KERN)/flanterm/flanterm.c -o $(BUILD_DIR)/flanterm.o $(CFLAGS)
	amd64-elf-gcc -c $(KERN)/flanterm/backends/fb.c -o $(BUILD_DIR)/fb.o $(CFLAGS)

	amd64-elf-gcc -c $(KERN)/include/string.c -o $(BUILD_DIR)/string.o $(CFLAGS)

	amd64-elf-gcc -c $(KERN)/sys/gdt/gdt.c -o $(BUILD_DIR)/gdt.o $(CFLAGS)
	nasm 	 		 $(KERN)/sys/gdt/gdt.asm -o $(BUILD_DIR)/gdt_asm.o $(NASMFLAGS)

	amd64-elf-gcc -c $(KERN)/sys/idt/idt.c -o $(BUILD_DIR)/idt.o $(CFLAGS)
	nasm 	 		 $(KERN)/sys/idt/idt.asm -o $(BUILD_DIR)/idt_asm.o $(NASMFLAGS)

	amd64-elf-gcc -c $(KERN)/sys/acpi/acpi.c -o $(BUILD_DIR)/acpi.o $(CFLAGS)
	amd64-elf-gcc -c $(KERN)/sys/acpi/apic.c -o $(BUILD_DIR)/apic.o $(CFLAGS)
	

	amd64-elf-ld -o $(BUILD_DIR)/neobird64.elf  $(BUILD_DIR)/*.o $(LDFLAGS)

	# Create a directory which will be our ISO root.
	mkdir -p iso_root

	# Copy the relevant files over.
	cp -v $(BUILD_DIR)/neobird64.elf limine.cfg limine/limine-bios.sys \
		limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso_root/

	# Create the EFI boot tree and copy Limine's EFI executables over.
	mkdir -p iso_root/EFI/BOOT
	cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
	cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/

	# Create the bootable ISO.
	xorriso -as mkisofs -b limine-bios-cd.bin \
			-no-emul-boot -boot-load-size 4 -boot-info-table \
			--efi-boot limine-uefi-cd.bin \
			-efi-boot-part --efi-boot-image --protective-msdos-label \
			iso_root -o $(BUILD_DIR)/neobird64.iso

	# Install Limine stage 1 and 2 for legacy BIOS boot.
	./limine/limine bios-install $(BUILD_DIR)/neobird64.iso
