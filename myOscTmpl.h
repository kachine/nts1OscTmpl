/*
 * File: myOscTmpl.h
 *
 * NTS-1 Simple oscillator
 * (user oscillator template)
 *
 */
#include "userosc.h"
#include <limits.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MAX_PARAM        100
#define INIT_WAVEFORM    WAVEFORM_SAW
#define INIT_LEVEL_RATIO 0.8

enum {
  WAVEFORM_SAW = 0,
  WAVEFORM_SQU = 1,
  WAVEFORM_SIN = 2,
  WAVEFORM_NOISE = 3
};

struct _voice{
  uint8_t waveformType; // Waveform type, 0-3
  float   phaseRatio;   // Phase ratio, 0<=phaseRatio<1.0
  float   levelRatio;   // Level ratio, 0<=levelRatio<=1.0
};
struct _voice VOICE;    // Global variable to keep voice parameters

void OSC_INIT(uint32_t platform, uint32_t api);
void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames);
void OSC_NOTEON(const user_osc_param_t * const params);
void OSC_NOTEOFF(const user_osc_param_t * const params);
void OSC_PARAM(uint16_t index, uint16_t value);
