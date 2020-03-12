#ifndef _DICE_COMMUNICATION_H_
#define _DICE_COMMUNICATION_H_

#include "stdint.h"
#include "stdbool.h"
#include "custs1_task.h"

#define NTF_STRING_SIZE 																18
#define DICE_SIDE_COUNT																	6

void dice_chance_init(void);
void dice_chance_deinit(void);

void dice_chance_set_ntf_string(const char caDice_chance[NTF_STRING_SIZE]);
void dice_chance_prepare_ntf_string(void);
char *dice_chance_get_ntf_string(void);

void dice_chance_send(void);
void dice_chance_recieve_handler(struct custs1_val_write_ind const *param);

#endif


