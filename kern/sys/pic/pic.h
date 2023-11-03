#define PIC1_CMD    0x20
#define PIC2_CMD    0xA0
#define PIC1_DATA   0x21
#define PIC2_DATA   0xA1

void pic_init(void);
void pic_sendeoi(unsigned char irq);
void pic_unmask(unsigned char irq);
void pic_mask(unsigned char irq);