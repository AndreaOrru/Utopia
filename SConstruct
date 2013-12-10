from os import environ

CCFLAGS = '-ffreestanding -ggdb '

env = Environment(ENV       = {'PATH': environ['PATH']},
                  CPPPATH   = 'kernel/include',

                  AS        = 'nasm',
                  ASFLAGS   = '-f elf ',

                  CC        = 'i586-elf-gcc',
                  CFLAGS    = '-std=gnu11 ',
                  CCFLAGS   = CCFLAGS,
                  CPPFLAGS  = '-Wall -Wextra ',

                  CXX       = 'i586-elf-g++',
                  CXXFLAGS  = '-std=gnu++11 -fno-exceptions -fno-rtti ',

                  LINKFLAGS = CCFLAGS + '-nostdlib -T kernel/link.ld ',
                  LIBS      = 'gcc')

kernel = env.Program('kernel/kernel.bin', Glob('kernel/*.cpp')   + Glob('kernel/*.s') +
                                          Glob('kernel/*/*.cpp') + Glob('kernel/*/*.s'))
Depends(kernel, 'kernel/link.ld')
