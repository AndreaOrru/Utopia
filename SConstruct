from os import environ

DEBUG = int(ARGUMENTS.get('debug', 1))

env = Environment(ENV        = {'PATH': environ['PATH']},

                  AS         = 'nasm',
                  ASFLAGS    = ['-felf'] + ['-Fdwarf', '-g'] if DEBUG else [],

                  CC         = 'i586-elf-gcc',
                  CFLAGS     = ['-std=gnu11'],
                  CCFLAGS    = ['-gdwarf-2'] if DEBUG else [],
                  CPPFLAGS   = ['-Wall', '-Wextra'],
                  LIBS       = ['gcc'],

                  ASCOMSTR   = 'AS\t$SOURCES -> $TARGETS',
                  CCCOMSTR   = 'CC\t$SOURCES -> $TARGETS',
                  LINKCOMSTR = 'LD\t$SOURCES -> $TARGETS')
Export('env')

kernel  = SConscript('kernel/SConscript')
servers = SConscript('servers/SConscript')
