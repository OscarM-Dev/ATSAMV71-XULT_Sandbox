#include "UartMiniTester.h"
#include "Uart.h"
#include "Uart_Cfg.h"

static const char* gMsgs[UART_CFG_CHANNELS] = {
  "CH0: Hello from UART\r\n",
  "CH1: Quick brown fox\r\n",
  "CH2: Dale Cruz Azul\r\n",
  "CH3: AUTOSAR UART test\r\n"
};
static uint8_t gIdx[UART_CFG_CHANNELS];

void UartMiniTester_Init(void)
{
  for (uint8_t i = 0u; i < UART_CFG_CHANNELS; i++) {
    gIdx[i] = 0u;
  }
}

void UartMiniTester_10ms(void)
{
  for (uint8_t ch = 0u; ch < UART_CFG_CHANNELS; ch++) {
    const char* msg = gMsgs[ch];
    uint8_t pos = gIdx[ch];
    if (msg[pos] != '\0') {
      (void)Uart_Write(ch, (const uint8_t*)&msg[pos], 1u);
      gIdx[ch] = pos + 1u;
    } else {
      gIdx[ch] = 0u;
    }
  }
}
