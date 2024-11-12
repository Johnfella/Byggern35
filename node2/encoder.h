#include <stdint.h>
#include "can.h"
void encoder_init(void);
void encoder_reset(void);
int32_t encoder_get_pos(can_data *d);