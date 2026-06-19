#ifndef COSMOS_BYTES_H
#define COSMOS_BYTES_H

#include <stdint.h>
#include "schneider_lang.h"

// Konstanten deines 26-Byte Protokolls
#define RAW_FRAME_SIZE 26
#define ANCHOR_BYTE 0x2A
#define FOOTER_BYTE 0xFF

// Funktionen für das System
_172 _184 cb_checksum(_71 _184* block);
_172 _44 cb_validate(_71 _184* block);
_172 _50 cb_build(_184* out_block, _182 sensor_val, _184 r, _184 g, _184 b, _184 a);
_172 _182 cb_extract_sensor(_71 _184* block);

#endif