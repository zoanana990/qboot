# timer

## System clock (SYSCLK)
- Three different clock sources can be used to drive the system clock
  - HSI oscillator clock
  - HSE oscillator clock
  - Two main PLL clocks
- The devices have two following secondary clock sources
  - 32 kHz low-speed internal RC (LSI RC), which drives the independent watchdog and, optionally, 
    the RTC used for auto wakeup from the stop/standby mode
  - 32.768 kHz low_speed external crystal which optionally drives the RTC clock (RTCCLK)

## In NUCLOE-F746 board
- HSI -> 16 MHz
- HSE -> Don't Know
- PLL
  - High Speed clock, up to 216 MHz
  - used for USB, RNG etc. peripheral, used to generate 48 MHz
- LSE clock -> 32.768 kHz
- LSI clock -> 32 kHz

## Default clock state
- After reset of the MCU
  - HSI is on
  - HSE is off
  - PLL is off
  - LSE is off
  - LSI is off
- So, systick sourced by HSI
  - i.e. SYSCLK = 16 MHz
- Each clock source can be switched on or off independently when it is noe used, to optimize power consumption

### HSI
- The HSI is used as system clock source after startup from reset, wake-up from stop and standby mode, or in case of 
  failure of the HSE used directly or indirectly as system clock
- The HSI RC oscillator has the advantage of providing a clock source at low cost (no external components)
- It also has a faster startup time than the HSE crystal oscillator
- However, even with calibration the frequency is less accurate than an external crystal oscillator or ceramic resonator
- The HSI signal can also be used as backup source if the HSE crystal oscillator fairs
