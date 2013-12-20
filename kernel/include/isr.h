#pragma once
#include <stdint.h>

typedef struct
{
    uint32_t regs[8];
    uint32_t num, error;
    uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed)) State;

typedef void (*IsrHandler)(State*);

extern void exception0 (void);
extern void exception1 (void);
extern void exception2 (void);
extern void exception3 (void);
extern void exception4 (void);
extern void exception5 (void);
extern void exception6 (void);
extern void exception7 (void);
extern void exception8 (void);
extern void exception9 (void);
extern void exception10(void);
extern void exception11(void);
extern void exception12(void);
extern void exception13(void);
extern void exception14(void);
extern void exception15(void);
extern void exception16(void);
extern void exception17(void);
extern void exception18(void);
extern void exception19(void);
extern void exception20(void);
extern void exception21(void);
extern void exception22(void);
extern void exception23(void);
extern void exception24(void);
extern void exception25(void);
extern void exception26(void);
extern void exception27(void);
extern void exception28(void);
extern void exception29(void);
extern void exception30(void);
extern void exception31(void);

extern void irq0 (void);
extern void irq1 (void);
extern void irq2 (void);
extern void irq3 (void);
extern void irq4 (void);
extern void irq5 (void);
extern void irq6 (void);
extern void irq7 (void);
extern void irq8 (void);
extern void irq9 (void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);
