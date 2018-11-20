#define SEC_START __attribute__((__section__(".startup")))
extern void _start(void);
void  SEC_START startup(void)
{
  _start();
}

