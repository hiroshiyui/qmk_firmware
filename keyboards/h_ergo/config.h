#pragma once

// ─── Split transport ──────────────────────────────────────────────────────────
// Full-duplex UART over the USB-C interconnect cable (TX/RX/GND only).
// GP0 = UART0 TX, GP1 = UART0 RX on the Raspberry Pi Pico.
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1
#define SERIAL_USART_SPEED 460800

// The half that has an active USB connection to the host becomes the master.
// Both Picos have USB ports, so this cleanly selects whichever is plugged in first.
#define SPLIT_USB_DETECT

// ─── Asymmetric matrix ────────────────────────────────────────────────────────
// keyboard.json lists 10 col pins, which sets MATRIX_COLS=10 (needed for the
// right half's 10-column layout).  The left half's hand-wired matrix only has
// 7 column traces (rows 0-5 × cols 0-6 → 42 positions for 36 keys).
//
// Override MATRIX_COL_PINS so the left Pico only drives 7 col lines,
// then declare MATRIX_COL_PINS_RIGHT so the right Pico drives all 10.
//
// Left:  GP10–GP16  (7 cols, rows R0/R1 fully populated; R2-R5 partially)
// Right: GP10–GP19  (10 cols, R0 fully populated with 10 keys)
#undef MATRIX_COL_PINS
#define MATRIX_COL_PINS {GP10, GP11, GP12, GP13, GP14, GP15, GP16}
#define MATRIX_COL_PINS_RIGHT {GP10, GP11, GP12, GP13, GP14, GP15, GP16, GP17, GP18, GP19}
