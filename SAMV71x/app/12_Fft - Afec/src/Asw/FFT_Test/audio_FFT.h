#ifndef AUDIO_FFT_H
#define AUDIO_FFT_H

#include <stdint.h>

/**
 * @brief Processes captured audio (PCM int16) with the FFT.
 *
 * @param data         Pointer to the PCM buffer (for example CODEC_Data).
 * @param num_samples  Number of valid captured samples.
 */
void AUDIO_FFT_ProcessCapturedAudio(const int32_t *data, uint32_t num_samples);

#endif /* AUDIO_FFT_H */
