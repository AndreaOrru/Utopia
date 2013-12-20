from os import environ

DEBUG = int(ARGUMENTS.get('debug', 1))
CCFLAGS = '-ffreestanding ' + ('-gdwarf-2 ' if DEBUG else '')

env = Environment(ENV        = {'PATH': environ['PATH']},
                  CPPPATH    = 'kernel/include',

                  AS         = 'nasm',
                  ASFLAGS    = '-f elf ' + ('-F dwarf -g ' if DEBUG else ''),

                  CC         = 'i586-elf-gcc',
                  CFLAGS     = '-std=gnu11 ',
                  CCFLAGS    = CCFLAGS,
                  CPPFLAGS   = '-Wall -Wextra ',

                  LINKFLAGS  = CCFLAGS + '-nostdlib -T kernel/link.ld ',
                  LIBS       = 'gcc',

                  ASCOMSTR   = 'AS\t$SOURCES -> $TARGETS',
                  CCCOMSTR   = 'CC\t$SOURCES -> $TARGETS',
                  LINKCOMSTR = 'LD\t$SOURCES -> $TARGETS')

kernel = env.Program('kernel/kernel.bin', Glob('kernel/*.c')   + Glob('kernel/*.asm') +
                                          Glob('kernel/*/*.c') + Glob('kernel/*/*.asm'))
Depends(kernel, 'kernel/link.ld')
