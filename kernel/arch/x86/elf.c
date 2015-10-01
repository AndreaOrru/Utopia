#include <arch.h>    // PAGE_SIZE.
#include <string.h>  // memcpy, memset.
#include <vmem.h>    // map.
#include <elf.h>

void* elf_load(const ELFHeader* elf)
{
    ELFProgHeader* ph_tbl = (void*) elf + elf->e_phoff;

    for (uint16_t i = 0; i < elf->e_phnum; i++)
    {
        if (ph_tbl[i].p_type == PT_LOAD)
        {
            unsigned flags = PAGE_USER;
            if (ph_tbl[i].p_flags & PF_W)
                flags |= PAGE_WRITE;

            void*    base   = (void*) ph_tbl[i].p_vaddr;
            uint32_t memsz  = ph_tbl[i].p_memsz;
            uint32_t filesz = ph_tbl[i].p_filesz;

            for (void* i = base; i < base + memsz; i += PAGE_SIZE)
                map(i, NULL, flags);

            memcpy(base, (void*) elf + ph_tbl[i].p_offset, filesz);
            memset(base + filesz, 0, memsz - filesz);
        }
    }

    return (void*) elf->e_entry;
}
