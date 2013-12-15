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
    void _exception0 (void);
    void _exception1 (void);
    void _exception2 (void);
    void _exception3 (void);
    void _exception4 (void);
    void _exception5 (void);
    void _exception6 (void);
    void _exception7 (void);
    void _exception8 (void);
    void _exception9 (void);
    void _exception10(void);
    void _exception11(void);
    void _exception12(void);
    void _exception13(void);
    void _exception14(void);
    void _exception15(void);
    void _exception16(void);
    void _exception17(void);
    void _exception18(void);
    void _exception19(void);
    void _exception20(void);
    void _exception21(void);
    void _exception22(void);
    void _exception23(void);
    void _exception24(void);
    void _exception25(void);
    void _exception26(void);
    void _exception27(void);
    void _exception28(void);
    void _exception29(void);
    void _exception30(void);
    void _exception31(void);

    void _irq0 (void);
    void _irq1 (void);
    void _irq2 (void);
    void _irq3 (void);
    void _irq4 (void);
    void _irq5 (void);
    void _irq6 (void);
    void _irq7 (void);
    void _irq8 (void);
    void _irq9 (void);
    void _irq10(void);
    void _irq11(void);
    void _irq12(void);
    void _irq13(void);
    void _irq14(void);
    void _irq15(void);
}
