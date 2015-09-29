from os import environ

DEBUG = int(ARGUMENTS.get('DEBUG', 1))
if DEBUG:
    CPPFLAGS = ['-DDEBUG']
    ASFLAGS  = CCFLAGS = LINKFLAGS = ['-ggdb3']
else:
    ASFLAGS = CPPFLAGS = []
    CCFLAGS = LINKFLAGS = ['-Os', '-flto']
    LINKFLAGS += ['-s']

ARCH = ARGUMENTS.get('ARCH', 'x86')
if ARCH == 'x86':
    TRIPLET = 'i686-elf'

env = Environment(ENV        = {'PATH': environ['PATH']},
                  CC         = '%s-gcc' % TRIPLET,

                  ASFLAGS    = ASFLAGS,
                  CCFLAGS    = ['-Wall', '-Wextra', '-fno-asynchronous-unwind-tables', '-fdiagnostics-color'] + CCFLAGS,
                  CPPFLAGS   = CPPFLAGS,
                  LINKFLAGS  = LINKFLAGS,

                  ASPPCOMSTR = 'AS    $TARGETS <= $SOURCES',
                  CCCOMSTR   = 'CC    $TARGETS <= $SOURCES',
                  LINKCOMSTR = 'LD    $TARGETS <= $SOURCES')
Export('env', 'ARCH')

kernel = SConscript('kernel/SConscript')
