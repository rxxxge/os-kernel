#include <system.h>

// Define GDT entry, prevent compiler "optimization" by packing.
// Prevents the compiler to do things that it thinks is best
struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

/* Special pointer which includes the limit: The max bytes
 * taken up by the GDT, minus 1. Needs to be packed */
struct gdt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* GDT with 3 entries and special gdt pointer */
struct gdt_entry gdt[3];
struct gdt_ptr gp;

/* Defined in start.asm. Use to properly reload the new segment registers */
extern void gdt_flush();

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Setup the granularity and set up access flags */
    gdt[num].granularity |= (gran & 0x0F);
    gdt[num].access = access;
}

/* Should be called by main. This will setup the special GDT
 * pointer, set up the first 3 entries in our GDT, and then 
 * finally call gdt_flush() in our assembler file in order 
 * to tell the processor where the new GDT is and update the new segment registers */
void gdt_install() {
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = &gdt;

    /* Our NULL descriptor */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* The second entry is Code Segment. The base address is 0,
    * the limit is 4 GBytes, it uses 4KByte granularity,
    * uses 32-bit opcodes, and is a Code Segment descriptor.
    */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* The third entry is Data Segment. Exactly the same as code segment,
    * but the descriptor type in this entry's access byte says it's a Data Segment */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    /* Flush out old GDT and install new changes */
    gdt_flush();
}
