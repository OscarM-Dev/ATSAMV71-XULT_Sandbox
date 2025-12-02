/**
 * @file audio_FFT.c
 * @author David Michel
 * @brief This file contains the FFT processing of captured audio data.
 * 
 */
/* ************************************************************************** */

/* Includes.
/* ************************************************************************** */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "AUDIO_FFT.h"
#include "fft.h" 
//#include "arm_math.h" 

/* ************************************************************************** */
/* Private Macros.
/* ************************************************************************** */
#define AUDIO_FFT_SIZE        2048u
#define AUDIO_SAMPLE_RATE_HZ  8000u

/* ************************************************************************** */
/* Global data.
/* ************************************************************************** */
static float audio_fft_input[AUDIO_FFT_SIZE * 2];
static float audio_fft_output[AUDIO_FFT_SIZE];
static float audio_fft_accum[AUDIO_FFT_SIZE / 2];
static uint32_t audio_fft_peak_bin;
static float audio_fft_peak_power;

/**
 * @brief This function converts a FFT bin index to its corresponding frequency in Hz.
 * @note  Resolution =  8kHz / 2048 bins = 3.90625 Hz/bin
 */
static float AUDIO_FFT_BinToFrequency(uint32_t bin_index)
{
    return ((float)bin_index * (float)AUDIO_SAMPLE_RATE_HZ) / (float)AUDIO_FFT_SIZE;
}

/**
 * @brief This function processes captured audio data with the FFT using STFT approach.
 */
void AUDIO_FFT_ProcessCapturedAudio(const int32_t *data, uint32_t num_samples)
{
    //Parameters
    uint32_t N      = AUDIO_FFT_SIZE;
    uint32_t hop    = N / 2u;
    uint32_t offset = 0;
    uint32_t win    = 0;
    uint32_t i;

    printf("\n\r=== AUDIO FFT (STFT over full capture) ===\n\r");
    printf("Total CODEC samples: %lu\n\r", (unsigned long)num_samples);
    printf("FFT size: %u, hop: %u (50%% overlap)\n\r", N, hop);

    if (num_samples < N) {
        printf("Not enough samples for FFT (%lu < %u)\n\r", (unsigned long)num_samples, N);
        return;
    }

    //Clear accumulation buffer.
    memset(audio_fft_accum, 0, sizeof(audio_fft_accum));

    while (offset + N <= num_samples) {
        for (i = 0; i < N; i++) {
            float sample_norm = (float)data[offset + i] / 32768.0f; // Normalize to [-1.0, 1.0]
            audio_fft_input[2u * i] = sample_norm; // Real part
            audio_fft_input[2u * i + 1] = 0.0f; // Imaginary part
        }
        
        //Perform FFT
        fft(audio_fft_input, audio_fft_output, N, &audio_fft_peak_bin, &audio_fft_peak_power);
        
        //Accumulate power spectrum
        for (i = 0; i < N / 2u; i++) {
            audio_fft_accum[i] += audio_fft_output[i];
        }
        
        // Print peak info for this window
        printf("Window %lu: peak_bin=%lu, freq=%.2f Hz, power=%.6f\n\r", (unsigned long)win, (unsigned long)audio_fft_peak_bin, AUDIO_FFT_BinToFrequency(audio_fft_peak_bin), audio_fft_peak_power);
        win++;
        offset += hop;
    }

    if (win == 0) {
        printf("No window was processed.\n\r");
        return;
    }

    for (i = 0; i < N / 2u; i++) {
        audio_fft_accum[i] /= (float)win;
    }

    // Prints results for all bins for plotting
    printf("\n\r=== Average spectrum from %lu windows (~%.2f s) ===\n\r", (unsigned long)win, (float)num_samples / (float)AUDIO_SAMPLE_RATE_HZ);
    printf("Bin,Frequency_Hz,Power_avg\n\r");
    for (i = 0; i < N / 2u; i++) {
        printf("%lu,%.2f,%.6f\n\r", (unsigned long)i, AUDIO_FFT_BinToFrequency(i), audio_fft_accum[i]);
    }

    printf("=== AUDIO FFT STFT done ===\n\r\n\r");
}