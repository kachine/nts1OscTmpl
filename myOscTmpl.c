/*
 * File: myOscTmpl.c
 *
 * NTS-1 Simple oscillator
 * (user oscillator template)
 *
 */
#include <myOscTmpl.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// Initialize function
//  called once on instantiation of the oscillator
void OSC_INIT(uint32_t platform, uint32_t api)
{
  // Initialize voice parameters
  VOICE.waveformType = INIT_WAVEFORM;
  VOICE.phaseRatio = (float) osc_rand() / UINT_MAX; // Randomize phase ratio within 0 to 1
  VOICE.levelRatio = INIT_LEVEL_RATIO;
}

// Wave Generation function
//  callbacked for each sample (or frames)
void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames)
{
  // Pointer to output buffer
  q31_t * __restrict y = (q31_t *) yn;

  // Last address of output buffer
  const q31_t * y_e = y + frames;

  // MIDI note# of current process
  uint8_t note = params->pitch >> 8;

  // Corresponding frequency of the MIDI note#
  // Not only notenumber but also pitchbend and built-in LFO pitch modulation is taken into account
  float frequency = osc_notehzf(note);

  // Working memory to store current sample value
  // Effective range is -1.0 <= sample < 1.0 to convert into Q31 format later
  float sample = 0.f;

  // Process one sample by sample in frames
  while( y != y_e ) {
    // Generate wave sample
    switch(VOICE.waveformType){
      case WAVEFORM_SAW:
        sample = osc_sawf(VOICE.phaseRatio);
        break;
      case WAVEFORM_SQU:
        sample = osc_sqrf(VOICE.phaseRatio);
        break;
      case WAVEFORM_SIN:
        sample = osc_sinf(VOICE.phaseRatio);
        break;
      case WAVEFORM_NOISE:
        sample = osc_white();
        break;
      default: break;
    }

    // Convert a sample into Q31 format, and write to output
    *(y++) = f32_to_q31(VOICE.levelRatio * sample);

    // Step a phase
    VOICE.phaseRatio += frequency / k_samplerate; // right side is equivalent to osc_w0f_for_note(params->pitch >> 8, params->pitch & 0xFF)
    // Keep the phase ratio within 0 <= phaseRatio < 1
    VOICE.phaseRatio -= (uint32_t) VOICE.phaseRatio;
  }
}

// MIDI note-on event process function
//  * This function is not hooked if active note is already exist
void OSC_NOTEON(const user_osc_param_t * const params)
{
  // Nothing to do mandatory in NTS-1
}

// MIDI note-off event process function
//  * This function is not hooked if active note remains
void OSC_NOTEOFF(const user_osc_param_t * const params)
{
  // Nothing to do mandatory in NTS-1
}

// Parameter change event process funnction
void OSC_PARAM(uint16_t index, uint16_t value)
{
  // 0-200 for bipolar percent parameters. 0% at 100, -100% at 0.
  // 0-100 for unipolar percent and typeless parameters.
  // 10 bit resolution for shape/shift-shape.
  switch (index) {
    case k_user_osc_param_id1: // WAVEFORM (0-3; 1-4 in LED display)
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      VOICE.waveformType = (uint8_t) value;
      break;
    case k_user_osc_param_id2: // Output level (0-100)
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      VOICE.levelRatio = (float) value / MAX_PARAM;
      break;
    case k_user_osc_param_id3: // Not used in this sample
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      break;
    case k_user_osc_param_id4: // Not used in this sample
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      break;
    case k_user_osc_param_id5: // Not used in this sample
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      break;
    case k_user_osc_param_id6: // Not used in this sample
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      break;
    case k_user_osc_param_shape: // Not used in this sample
      // 10bit parameter, 0 <= value <= 1023
      // Mapped to OSC mode A knob(shape) and MIDI CC#54
      //   CC#54 value=0:   uint16_t value=0
      //   CC#54 value=32:  uint16_t value=256
      //   CC#54 value=64:  uint16_t value=512
      //   CC#54 value=126: uint16_t value=1008
      //   CC#54 value=127: uint16_t value=1023
      //    --> CC#54 value*8 = uint16_t value, except CC value=127
      // You can use param_val_to_f32(value) to convert value to the range within 0 to 1 in float format
      break;
    case k_user_osc_param_shiftshape: // Not used in this sample
      // Similar to k_user_osc_param_shape, but mapped to OSC mode B knob(alt) and MIDI CC#55
      break;
    default:
      break;
  }
}
#pragma GCC diagnostic pop
