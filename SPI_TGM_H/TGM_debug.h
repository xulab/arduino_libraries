#ifndef _TGM_DEBUG_H
#define _TGM_DEBUG_H
#include "TGM_type.h"

#ifdef DEBUG
void printToneStep(toneStep * ts) {
	Serial.print("ts->LoopNum = ");
	Serial.println(ts->LoopNum);
	Serial.print("ts->LoopInd = ");
	Serial.println(ts->LoopInd);

	Serial.print("ts->ISI.arrayPar = ");
	Serial.println((uint32_t)ts->ISI.arrayPar);
	Serial.print("ts->ISI.array_Size = ");
	Serial.println(ts->ISI.array_Size);
	Serial.print("ts->ISI.drtPra = ");
	Serial.println(ts->ISI.drtPar);
	Serial.print("ts->ISI.flag = ");
	Serial.println(ts->ISI.flag);

	Serial.print("ts->fq.arrayPar = ");
	Serial.println((uint32_t)ts->fq.arrayPar);
	Serial.print("ts->fq.array_Size = ");
	Serial.println(ts->fq.array_Size);
	Serial.print("ts->fq.drtPra = ");
	Serial.println(ts->fq.drtPar);
	Serial.print("ts->fq.flag = ");
	Serial.println(ts->fq.flag);

	Serial.print("ts->vol.arrayPar = ");
	Serial.println((uint32_t)ts->vol.arrayPar);
	Serial.print("ts->vol.array_Size = ");
	Serial.println(ts->vol.array_Size);
	Serial.print("ts->vol.drtPra = ");
	Serial.println(ts->vol.drtPar);
	Serial.print("ts->vol.flag = ");
	Serial.println(ts->vol.flag);
}

void print_tone(ton * t) {
	Serial.print("sizeof(ton) = ");
	Serial.println(sizeof(ton));
	Serial.print("t->nSize = ");
	Serial.println(t->nSize);
	Serial.print("t->version = ");
	Serial.println(t->version);
	Serial.print("t->tone_flag = ");
	Serial.println(t->tone_flag);
	Serial.print("t->sweep = ");
	Serial.println(t->sweep);
	Serial.print("t->volume_mode = ");
	Serial.println(t->volume_mode);
	Serial.print("t->volume = ");
	Serial.println(t->volume);
	Serial.print("t->prepare_mode = ");
	Serial.println(t->prepare_mode);
	Serial.print("t->step_up_flag = ");
	Serial.println(t->step_up_flag);
	Serial.print("t->step_down_flag = ");
	Serial.println(t->step_down_flag);
	Serial.print("t->duration = ");
	Serial.println(t->duration);
	Serial.print("t->frequency0 = ");
	Serial.println(t->frequency0);
	Serial.print("t->frequency1 = ");
	Serial.println(t->frequency1);
	Serial.print("t->frequency2 = ");
	Serial.println(t->frequency2);
	Serial.print("t->frequency3 = ");
	Serial.println(t->frequency3);
	Serial.print("t->frequency4 = ");
	Serial.println(t->frequency4);
	Serial.print("t->frequency5 = ");
	Serial.println(t->frequency5);
	Serial.print("t->frequency6 = ");
	Serial.println(t->frequency6);
	Serial.print("t->frequency7 = ");
	Serial.println(t->frequency7);
	Serial.print("t->frequency8 = ");
	Serial.println(t->frequency8);
	Serial.print("t->frequency9 = ");
	Serial.println(t->frequency9);
	Serial.print("t->fq_step = ");
	Serial.println(t->fq_step);
	Serial.print("t->sweep_base = ");
	Serial.println(t->sweep_base);
	Serial.print("t->durationL = ");
	Serial.println(t->durationL);
	Serial.print("t->pre_sound_delay = ");
	Serial.println(t->pre_sound_delay);
	Serial.print("t->frequencyL0 = ");
	Serial.println(t->frequencyL0);
	Serial.print("t->frequencyL1 = ");
	Serial.println(t->frequencyL1);
	Serial.print("t->frequencyL2 = ");
	Serial.println(t->frequencyL2);
	Serial.print("t->frequencyL3 = ");
	Serial.println(t->frequencyL3);
	Serial.print("t->frequencyL4 = ");
	Serial.println(t->frequencyL4);
	Serial.print("t->frequencyL5 = ");
	Serial.println(t->frequencyL5);
	Serial.print("t->frequencyL6 = ");
	Serial.println(t->frequencyL6);
	Serial.print("t->frequencyL7 = ");
	Serial.println(t->frequencyL7);
	Serial.print("t->frequencyL8 = ");
	Serial.println(t->frequencyL8);
	Serial.print("t->frequencyL9 = ");
	Serial.println(t->frequencyL9);
	Serial.print("t->clicks_dur = ");
	Serial.println(t->clicks_dur);
	Serial.print("t->clicks_period = ");
	Serial.println(t->clicks_period);
}
#endif // DEBUG

#endif
