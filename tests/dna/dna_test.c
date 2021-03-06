/*
 * Copyright (C) 2011 Fergus Noble <fergusnoble@gmail.com>
 * Copyright (C) 2012 Colin Beighley <colin@swift-nav.com>
 *
 */

#include <stdio.h>
#include <libopencm3/stm32/f2/rcc.h>
#include <libopencm3/stm32/f2/flash.h>
#include <libopencm3/stm32/f2/gpio.h>

#include "init.h"
#include "main.h"
#include "sbp.h"
#include "acq.h"
#include "board/leds.h"
#include "board/m25_flash.h"
#include "board/nap/nap_common.h"
#include "peripherals/spi.h"

int main(void) {

  led_setup();
  led_on(LED_GREEN);
  led_on(LED_RED);

  /* Force FPGA to configure itself by driving PROGRAM_B low, then high */
  nap_conf_b_setup();
  nap_conf_b_clear();
  for (u32 i = 0; i < 50; i++)
    __asm__("nop");
  nap_conf_b_set();

  init();

  printf("\n\nFirmware info - git: " GIT_VERSION ", built: " __DATE__ " " __TIME__ "\n");
  printf("--- DNA TEST ---\n");

  while (1) {
    for (u32 i = 0; i < 10000; i++)
      __asm__("nop");
    DO_EVERY(50,
      led_toggle(LED_GREEN);
      led_toggle(LED_RED);
    );
    sbp_process_messages();
  }

  return 0;
}
