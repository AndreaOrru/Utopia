from os import environ

DEBUG = int(ARGUMENTS.get('debug', 1))
CCFLAGS = '-ffreestanding ' + ('-gdwarf-2 ' if DEBUG else '')

env = Environment(ENV       = {'PATH': environ['PATH']},
                  CPPPATH   = 'kernel/include',

                  AS        = 'nasm',
                  ASFLAGS   = '-f elf ' + ('-F dwarf -g ' if DEBUG else ''),

                  CC        = 'i586-elf-gcc',
                  CFLAGS    = '-std=gnu11 ',
                  CCFLAGS   = CCFLAGS,
                  CPPFLAGS  = '-Wall -Wextra ',

                  CXX       = 'i586-elf-g++',
                  CXXFLAGS  = '-std=gnu++11 -fno-exceptions -fno-rtti ',

                  LINKFLAGS = CCFLAGS + '-nostdlib -T kernel/link.ld ',
                  LIBS      = 'gcc')

kernel = env.Program('kernel/kernel.bin', Glob('kernel/*.c')   + Glob('kernel/*.cpp')   + Glob('kernel/*.s') +
                                          Glob('kernel/*/*.c') + Glob('kernel/*/*.cpp') + Glob('kernel/*/*.s'))
Depends(kernel, 'kernel/link.ld')
