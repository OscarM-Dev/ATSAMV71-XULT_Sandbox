#include <stdio.h>
#include <string.h>
#include <math.h>
#include "AUDIO_FFT.h"
#include "fft.h" 
//#include "arm_math.h" 

#define AUDIO_FFT_SIZE        1024u
#define AUDIO_SAMPLE_RATE_HZ  8000u

static float audio_fft_input[AUDIO_FFT_SIZE * 2];   /* 2*N: real + imaginary */
static float audio_fft_output[AUDIO_FFT_SIZE];      /* Power per bin */
static uint32_t audio_fft_peak_bin;
static float    audio_fft_peak_power;


static float AUDIO_FFT_BinToFrequency(uint32_t bin_index)
{
    return ((float)bin_index * (float)AUDIO_SAMPLE_RATE_HZ) / (float)AUDIO_FFT_SIZE;
}


void AUDIO_FFT_ProcessCapturedAudio(const int32_t *data, uint32_t num_samples)
{
    uint32_t i;
    uint32_t N;

    printf("\n\r=== AUDIO FFT (from CODEC) ===\n\r");
    printf("Available samples from CODEC: %lu\n\r", (unsigned long)num_samples);

    if (num_samples < AUDIO_FFT_SIZE)
    {
        printf("Not enough samples for FFT (%lu < %u). Skipping FFT.\n\r",
               (unsigned long)num_samples, AUDIO_FFT_SIZE);
        return;
    }

    N = AUDIO_FFT_SIZE;
    memset(audio_fft_input, 0, sizeof(audio_fft_input));
    memset(audio_fft_output, 0, sizeof(audio_fft_output));

    for (i = 0; i < N; i++)
    {
        float sample_norm = (float)data[i] / 32768.0f;

        audio_fft_input[2U * i]     = sample_norm;
        audio_fft_input[2U * i + 1] = 0.0f;
    }

    fft(audio_fft_input, audio_fft_output, N, &audio_fft_peak_bin, &audio_fft_peak_power);

    printf("FFT size: %u points\n\r", AUDIO_FFT_SIZE);
    printf("Sample rate: %u Hz\n\r", AUDIO_SAMPLE_RATE_HZ);
    printf("Freq resolution: %.2f Hz/bin\n\r",
           (float)AUDIO_SAMPLE_RATE_HZ / (float)AUDIO_FFT_SIZE);

    printf("Peak bin: %lu\n\r", (unsigned long)audio_fft_peak_bin);
    printf("Peak frequency: %.2f Hz\n\r",
           AUDIO_FFT_BinToFrequency(audio_fft_peak_bin));
    printf("Peak power: %.6f\n\r", audio_fft_peak_power);

    printf("\n\rBin,Frequency_Hz,Power\n\r");
    for (i = 0; i < (AUDIO_FFT_SIZE / 2u); i++)
    {
        printf("%lu,%.2f,%.6f\n\r",
               (unsigned long)i,
               AUDIO_FFT_BinToFrequency(i),
               audio_fft_output[i]);
    }

    printf("=== AUDIO FFT done ===\n\r\n\r");
}
