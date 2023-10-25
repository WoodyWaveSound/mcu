/**
 * MCU Framework and library
 *
 * Copyright (c) Woody Wave Sound and contributors. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */
#include <wws_mcu/encoder.h>


void ___wws_encoder_service_callback(wws_phase_t on, wws_service_t *serv)
{
  wws_encoder_t *const enc = serv->inst;

  if (on != WWS_ON_ROUTINE) return;

  switch (((wws_logic_read(enc->ccw) == enc->active) << 1) |
          ((wws_logic_read(enc->cw) == enc->active))) {

  case 0: /** stx */ enc->_stx = 1; break;
  case 1: /** cw, !ccw */ enc->_ccw = 1; break;
  case 2: /** !cw, ccw */ enc->_cw = 1; break;
  case 3: /** cw, ccw */
    if (enc->_stx && enc->_cw) enc->value++;
    else if (enc->_stx && enc->_ccw) enc->value--;
    enc->flags = 0; /** reset flags */
    break;
  default: break;
  }
}
