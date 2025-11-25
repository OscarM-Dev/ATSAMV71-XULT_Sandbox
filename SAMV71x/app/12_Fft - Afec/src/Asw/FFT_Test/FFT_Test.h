/****************************************************************************************************/
/**
\file       FFT_Test.h
\brief      FFT validation with synthetic test signals - Function prototypes
\author     Generated for validation
\version    1.0
\date       2025
*/
/****************************************************************************************************/

#ifndef __FFT_TEST_H
#define __FFT_TEST_H

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
#include <stdint.h>

/*****************************************************************************************************
* Function declarations
*****************************************************************************************************/

/**
 * \brief Test FFT with 1 kHz pure tone
 * Expected: Single peak at ~1000 Hz
 */
void FFT_Test_1kHz(void);

/**
 * \brief Test FFT with 2.5 kHz pure tone
 * Expected: Single peak at ~2500 Hz
 */
void FFT_Test_2p5kHz(void);

/**
 * \brief Test FFT with composite signal (1 kHz + 2.5 kHz)
 * Expected: Two distinct peaks
 */
void FFT_Test_Composite(void);

/**
 * \brief Export spectrum data in CSV format for Excel
 * Use this to visualize the full frequency spectrum
 */
void FFT_Test_ExportSpectrum(void);

/**
 * \brief Run complete FFT validation test suite
 * Executes all tests and prints summary
 */
void FFT_Test_RunAll(void);

#endif /* __FFT_TEST_H */
