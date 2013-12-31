#pragma once

#define alwaysinline  inline __attribute__((always_inline))

#define _SYSCALL_0(n, fun)                             \
static alwaysinline int fun(void)                      \
{                                                      \
    int ret;                                           \
    asm volatile ("int $0x80" : "=a" (ret) : "0" (n)); \
    return ret;                                        \
}

#define _SYSCALL_1(n, fun, T1)                                   \
static alwaysinline int fun(T1 p1)                               \
{                                                                \
    int ret;                                                     \
    asm volatile ("int $0x80" : "=a" (ret) : "0" (n), "c" (p1)); \
    return ret;                                                  \
}

#define _SYSCALL_2(n, fun, T1, T2)                                         \
static alwaysinline int fun(T1 p1, T2 p2)                                  \
{                                                                          \
    int ret;                                                               \
    asm volatile ("int $0x80" : "=a" (ret) : "0" (n), "c" (p1), "d" (p2)); \
    return ret;                                                            \
}

#define _SYSCALL_3(n, fun, T1, T2, T3)                                               \
static alwaysinline int fun(T1 p1, T2 p2, T3 p3)                                     \
{                                                                                    \
    int ret;                                                                         \
    asm volatile ("int $0x80" : "=a" (ret) : "0" (n), "c" (p1), "d" (p2), "b" (p3)); \
    return ret;                                                                      \
}

#define _SYSCALL_4(n, fun, T1, T2, T3, T4)                                                     \
static alwaysinline int fun(T1 p1, T2 p2, T3 p3, T4 p4)                                        \
{                                                                                              \
    int ret;                                                                                   \
    asm volatile ("int $0x80" : "=a" (ret) : "0" (n), "c" (p1), "d" (p2), "b" (p3), "D" (p4)); \
    return ret;                                                                                \
}

#define _SYSCALL_5(n, fun, T1, T2, T3, T4, T5)                                                           \
static alwaysinline int fun(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)                                           \
{                                                                                                        \
    int ret;                                                                                             \
    asm volatile ("int $0x80" : "=a" (ret) : "0" (n), "c" (p1), "d" (p2), "b" (p3), "D" (p4), "S" (p5)); \
    return ret;                                                                                          \
}
