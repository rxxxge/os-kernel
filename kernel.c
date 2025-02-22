#include <system.h>

void *memcpy(void *dest, const void *src, size_t count) {
    /* copy 'count' bytes of data from 'src' to 'dest' and return 'dest' */
    const char *sp = (const char*)src;
    char *dp = (char *)dest;
    for (int i = 0; i < count; ++i) {
        dp[i] = sp[i];
    }

    return dest;
}

void *memset(void *dest, char val, size_t count) {
    /* set 'count' bytes in 'dest' to 'val'. return 'dest' */
    char *temp = (char *)dest;
    for (int i = 0; i < count; ++i) {
        temp[i] = val;
    }

    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count) {
    /* same as memset but with 16 bit 'val' and dest pointer */
    for (int i = 0; i < count; ++i) {
        dest[i] = val;
    }

    return dest;
}

size_t strlen(const char *str) {
    /* length of bytes in a string
     * iterates through character array before encounters \0
     * and returns how many characters */ 
    size_t count = 0;
    while (str[count] != '\0') {
        count++;
    } 

    return count;
}

unsigned char inportb(unsigned short _port) {
    unsigned char rv;
    __asm__ __volatile__("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb(unsigned short _port, unsigned char _data) {
    __asm__ __volatile__("outb %1, %0" : : "dN" (_port), "a" (_data));
}

int kmain(void) {
    /* Add commands here */
    gdt_install();
    // idt_install();

    init_video();
    settextcolor(0x0D, 0x00);
    puts("Hello Sofiaaaa\n");

    for (;;);

    return 0;
}
