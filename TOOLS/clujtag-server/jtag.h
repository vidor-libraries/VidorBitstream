#include "defines.h"

#define LED_ON      
#define LED_OFF     
#define LED_BLINK   

#ifdef __cplusplus
extern "C" {
#endif

void jtag_setup(void);
void jtag_shutdown(void);
int jtag_execute(uint8_t data);

#ifdef __cplusplus
}
#endif
