#include <stddef.h>
#include "string.h"
#include "vmem.h"
#include "x86.h"
#include "elf.h"

void* elf_load(ElfHeader* file)
{
    ElfProgHeader* phTbl = (void*)file + file->e_phoff;

    for (int i = 0; i < file->e_phnum; i++)
    {
        if (phTbl[i].p_type == PT_LOAD)
        {
            uint32_t flags = PAGE_USER;
            if (phTbl[i].p_flags & PF_W)
                flags |= PAGE_WRITE;

            void*  base   = (void*)phTbl[i].p_vaddr;
            size_t memsz  = phTbl[i].p_memsz;
            size_t filesz = phTbl[i].p_filesz;

            for (void* i = base; i < base + memsz; i += PAGE_SIZE)
                map(i, NULL, flags);

            memcpy(base, (void*)file + phTbl[i].p_offset, filesz);
            memset(base + filesz, 0, memsz - filesz);
        }
    }

    return (void*)file->e_entry;
}
