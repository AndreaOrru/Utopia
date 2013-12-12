#pragma once
#include <stdint.h>

struct InterruptStack
{
    uint32_t regs[8];
    uint32_t num, error;
    uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed));

typedef void (*IsrHandler)(InterruptStack);

extern "C"
{
    void exception0 (void);
    void exception1 (void);
    void exception2 (void);
    void exception3 (void);
    void exception4 (void);
    void exception5 (void);
    void exception6 (void);
    void exception7 (void);
    void exception8 (void);
    void exception9 (void);
    void exception10(void);
    void exception11(void);
    void exception12(void);
    void exception13(void);
    void exception14(void);
    void exception15(void);
    void exception16(void);
    void exception17(void);
    void exception18(void);
    void exception19(void);
    void exception20(void);
    void exception21(void);
    void exception22(void);
    void exception23(void);
    void exception24(void);
    void exception25(void);
    void exception26(void);
    void exception27(void);
    void exception28(void);
    void exception29(void);
    void exception30(void);
    void exception31(void);

    void irq0 (void);
    void irq1 (void);
    void irq2 (void);
    void irq3 (void);
    void irq4 (void);
    void irq5 (void);
    void irq6 (void);
    void irq7 (void);
    void irq8 (void);
    void irq9 (void);
    void irq10(void);
    void irq11(void);
    void irq12(void);
    void irq13(void);
    void irq14(void);
    void irq15(void);
}
