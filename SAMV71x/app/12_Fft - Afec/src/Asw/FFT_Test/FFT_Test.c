/****************************************************************************************************/
/**
\file       FFT_Test.c
\brief      FFT validation with synthetic test signals
\author     Generated for validation
\version    1.0
\date       2025
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

#include <math.h>
#include "FFT_Test.h"
#include "fft.h"
#include "arm_math.h"
#include <stdio.h>
#include <string.h>

/*****************************************************************************************************
* Module configuration constants
*****************************************************************************************************/
#define FFT_SIZE        1024u
#define SAMPLE_RATE_HZ  10000u  /* 10 kHz sampling for voice testing */
#define PI              3.14159265358979f
#define TWO_PI          6.28318530717959f

/*****************************************************************************************************
* Local variables
*****************************************************************************************************/
/* Input buffer: real samples interleaved with zeros for imaginary parts [R0, I0, R1, I1, ...] */
static float test_input[FFT_SIZE * 2];
/* Output power spectrum */
static float test_output[FFT_SIZE];
/* Results */
static uint32_t peak_bin;
static float peak_power;

/*****************************************************************************************************
* Local function prototypes
*****************************************************************************************************/
static void generate_sine_wave(float freq_hz, float amplitude, uint32_t num_samples);
static void generate_composite_signal(void);
static float bin_to_frequency(uint32_t bin_index);

/*****************************************************************************************************
* Function implementations
*****************************************************************************************************/

/**
 * \brief Fast approximation of sin(x).
 * \param x Input angle in radians.
 */
static float fast_sin(float x)
{
    while (x > PI)      x -= TWO_PI;
    while (x < -PI)     x += TWO_PI;
    float x2 = x * x;
    return x * (1.0f - x2 / 6.0f + (x2 * x2) / 120.0f);
}

/**
 * \brief Generate a pure sine wave at specified frequency
 * \param freq_hz     Frequency in Hz (e.g., 1000.0 for 1 kHz)
 * \param amplitude   Peak amplitude (0.0 to 1.0 recommended)
 * \param num_samples Number of samples to generate
 */
static void generate_sine_wave(float freq_hz, float amplitude, uint32_t num_samples)
{
    uint32_t i;
    float phase_increment = 2.0f * PI * freq_hz / (float)SAMPLE_RATE_HZ;
    
    for (i = 0; i < num_samples; i++)
    {
        /* Real part: sine wave sample */
        test_input[2*i] = amplitude * fast_sin(phase_increment * (float)i);
        /* Imaginary part: zero */
        test_input[2*i + 1] = 0.0f;
    }
}

/**
 * \brief Generate composite signal with multiple frequencies
 * Example: 1 kHz + 2.5 kHz to verify FFT can separate them
 */
static void generate_composite_signal(void)
{
    uint32_t i;
    float f1 = 1000.0f;   /* 1 kHz component */
    float f2 = 2500.0f;   /* 2.5 kHz component */
    float phase1 = 2.0f * PI * f1 / (float)SAMPLE_RATE_HZ;
    float phase2 = 2.0f * PI * f2 / (float)SAMPLE_RATE_HZ;
    
    for (i = 0; i < FFT_SIZE; i++)
    {
        /* Real part: sum of two sine waves */
        test_input[2*i] = 0.5f * fast_sin(phase1 * (float)i) + 0.3f * fast_sin(phase2 * (float)i);
        /* Imaginary part: zero */
        test_input[2*i + 1] = 0.0f;
    }
}

/**
 * \brief Convert FFT bin index to frequency in Hz
 * \param bin_index  FFT bin (0 to FFT_SIZE-1)
 * \return Frequency in Hz
 */
static float bin_to_frequency(uint32_t bin_index)
{
    return ((float)bin_index * (float)SAMPLE_RATE_HZ) / (float)FFT_SIZE;
}

/**
 * \brief Test 1: Pure 1 kHz sine wave
 * Expected result: Single peak at bin ≈ 102 (1000 Hz / (10000 Hz / 1024) ≈ 102.4)
 */
void FFT_Test_1kHz(void)
{
    printf("\n\r=== FFT Test: 1 kHz Pure Tone ===\n\r");
    
    /* Clear buffers before test */
    memset(test_input, 0, sizeof(test_input));
    memset(test_output, 0, sizeof(test_output));
    
    /* Generate 1 kHz sine wave */
    generate_sine_wave(1000.0f, 0.8f, FFT_SIZE);
    
    /* Run FFT */
    fft(test_input, test_output, FFT_SIZE, &peak_bin, &peak_power);
    
    /* Print results */
    printf("Peak bin: %lu\n\r", (unsigned long)peak_bin);
    printf("Peak frequency: %.2f Hz\n\r", bin_to_frequency(peak_bin));
    printf("Peak power: %.4f\n\r", peak_power);
    printf("Expected frequency: 1000 Hz\n\r");
    
    /* Validation: Peak should be around bin 102 ± 2 */
    if ((peak_bin >= 100) && (peak_bin <= 105))
    {
        printf("PASS: Peak detected at correct frequency\n\r");
    }
    else
    {
        printf("FAIL: Peak not at expected frequency\n\r");
    }
}

/**
 * \brief Test 2: Pure 2.5 kHz sine wave
 * Expected result: Single peak at bin ≈ 256 (2500 Hz / (10000 Hz / 1024) ≈ 256)
 */
void FFT_Test_2p5kHz(void)
{
    printf("\n\r=== FFT Test: 2.5 kHz Pure Tone ===\n\r");
    
    /* Clear buffers before test */
    memset(test_input, 0, sizeof(test_input));
    memset(test_output, 0, sizeof(test_output));
    
    /* Generate 2.5 kHz sine wave */
    generate_sine_wave(2500.0f, 0.8f, FFT_SIZE);
    
    /* Run FFT */
    fft(test_input, test_output, FFT_SIZE, &peak_bin, &peak_power);
    
    /* Print results */
    printf("Peak bin: %lu\n\r", (unsigned long)peak_bin);
    printf("Peak frequency: %.2f Hz\n\r", bin_to_frequency(peak_bin));
    printf("Peak power: %.4f\n\r", peak_power);
    printf("Expected frequency: 2500 Hz\n\r");
    
    /* Validation: Peak should be around bin 256 ± 2 */
    if ((peak_bin >= 254) && (peak_bin <= 258))
    {
        printf("PASS: Peak detected at correct frequency\n\r");
    }
    else
    {
        printf("FAIL: Peak not at expected frequency\n\r");
    }
}

/**
 * \brief Test 3: Composite signal (1 kHz + 2.5 kHz)
 * Expected result: Two peaks, one at ~102 and another at ~256
 */
void FFT_Test_Composite(void)
{
    uint32_t i;
    uint32_t peaks_found = 0;
    float threshold = 0.1f;  /* Minimum power to consider a peak */
    
    printf("\n\r=== FFT Test: Composite Signal (1kHz + 2.5kHz) ===\n\r");
    
    /* Clear buffers before test */
    memset(test_input, 0, sizeof(test_input));
    memset(test_output, 0, sizeof(test_output));
    
    /* Generate composite signal */
    generate_composite_signal();
    
    /* Run FFT */
    fft(test_input, test_output, FFT_SIZE, &peak_bin, &peak_power);
    
    /* Print primary peak */
    printf("Primary peak bin: %lu\n\r", (unsigned long)peak_bin);
    printf("Primary peak frequency: %.2f Hz\n\r", bin_to_frequency(peak_bin));
    printf("Primary peak power: %.4f\n\r", peak_power);
    
    /* Find all significant peaks in first half of spectrum (0 to Nyquist) */
    printf("\n\rAll significant peaks:\n\r");
    for (i = 1; i < FFT_SIZE / 2; i++)
    {
        /* Check if this is a local maximum above threshold */
        if ((test_output[i] > threshold) &&
            (test_output[i] > test_output[i-1]) &&
            (test_output[i] > test_output[i+1]))
        {
            printf("  Bin %lu: %.2f Hz, Power = %.4f\n\r", 
                   (unsigned long)i, bin_to_frequency(i), test_output[i]);
            peaks_found++;
        }
    }
    
    printf("Total peaks found: %lu\n\r", (unsigned long)peaks_found);
    printf("Expected: 2 peaks (at ~1000 Hz and ~2500 Hz)\n\r");
    
    if (peaks_found >= 2)
    {
        printf("PASS: Multiple frequency components detected\n\r");
    }
    else
    {
        printf("FAIL: Expected 2 peaks, found %lu\n\r", (unsigned long)peaks_found);
    }
}

/**
 * \brief Test 4: Export Test spectrum
 */
void FFT_Test_ExportSpectrum(uint32_t samples )
{
    uint32_t i;
    
    printf("\n\r=== FFT Spectrum Export (for Excel) ===\n\r");
    printf("Bin,Frequency_Hz,Power\n\r");
    

    for (i = 0; i < samples && i < FFT_SIZE/2; i++)
    {
        printf("%lu,%.2f,%.6f\n\r", 
               (unsigned long)i, 
               bin_to_frequency(i), 
               test_output[i]);
    }
}

/**
 * \brief Run all FFT validation tests
 */
void FFT_Test_RunAll(void)
{
    printf("\n\r");
    printf("======================================\n\r");
    printf("   FFT Library Validation Suite      \n\r");
    printf("======================================\n\r");
    printf("Sample Rate: %lu Hz\n\r", (unsigned long)SAMPLE_RATE_HZ);
    printf("FFT Size: %lu points\n\r", (unsigned long)FFT_SIZE);
    printf("Frequency Resolution: %.2f Hz/bin\n\r", 
           (float)SAMPLE_RATE_HZ / (float)FFT_SIZE);
    printf("Nyquist Frequency: %lu Hz\n\r", (unsigned long)(SAMPLE_RATE_HZ / 2));
    
    /* Run individual tests & export spectrum */
    printf("   >>>    Starting full TEST   <<<   ");
    FFT_Test_1kHz();
    FFT_Test_ExportSpectrum(200);
    printf("   >>>    TEST #1 finalized   <<<   ");
    FFT_Test_2p5kHz();
    FFT_Test_ExportSpectrum(400);
    printf("   >>>    TEST #2 finalized   <<<   ");
    FFT_Test_Composite();
    FFT_Test_ExportSpectrum(400);
    printf("   >>>    TEST #3 finalized   <<<   ");
    
    printf("\n\r======================================\n\r");
    printf("   Validation Complete                \n\r");
    printf("======================================\n\r\n\r");
}
