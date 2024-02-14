# neobird64

cool x86_64 kernel designed for bird

# checklist

- [x] IDT
- [x] GDT
- [x] IOAPIC & LAPIC
- [x] Timer & keyboard support
- [x] Memory management
- [ ] SMP & Scheduling
- [ ] Userspace
# how to compile

- edit makefile CC to fit your cross-compiler
- `make`
- there should now be a `neobird64.iso` file in `build/`
- run iso with `qemu-system-x86_64 -d int -M smm=off build/neobird64.iso`



 