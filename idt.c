#include <system.h>

/* Define IDT entry */
struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;     /* Kernel segment goes here */
    unsigned char always0;  /* Always set to 0 */
    unsigned char flags;    /* Set using the "table" */
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

/* Defined in 'start.asm', and is used to load IDT */
extern void idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned short flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install() {
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    /* Clear the whole idt, initialize to zero */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */

    /* Points processor's internal register to the new IDT */
    idt_load();
}
