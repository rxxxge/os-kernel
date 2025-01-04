#ifndef __SYSTEM_H
#define __SYSTEM_H

#define size_t unsigned int

/* KERNEL.C */
void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, char val, size_t count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
size_t strlen(const char *str);
unsigned char inportb(unsigned short _port);
void outportb(unsigned short _port, unsigned char _data);

extern void cls();
extern void putch(unsigned char c);
extern void puts(const char *text);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video(void);

#endif
