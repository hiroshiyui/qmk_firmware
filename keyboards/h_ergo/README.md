# h_ergo — Hiroshi's Ergonomic Keyboard

Split TKL mechanical keyboard — hand-wired, Raspberry Pi Pico per side.

- **87 keys total**: 36 left + 51 right
- **Switch spacing**: 19.05 mm (ANSI 1U)
- **Connection**: USB-C interconnect cable between halves (UART TX/RX/GND; no VCC)
- **Host**: USB Micro (left back wall) or USB-C (right back wall) — either half can be master

The CAD source for the case and plate is `samples/split_tkl_keyboard.rb` in the [rrcad](https://github.com/yhh/rrcad) repository.

## Layout

```
┌──────┬────┬────┬────┬────┬────┬────╥────┬────┬────┬────┬────┬────┬──────┬──────┬──────┬──────┐
│ Esc  │ F1 │ F2 │ F3 │ F4 │ F5 │ F6 ║ F7 │ F8 │ F9 │F10 │F11 │F12 │PrtSc │ScrLk │Pause │ Ins  │
├──────┼────┼────┼────┼────┼────┼────╫────┼────┼────┼────┼────┼────┼─────────────┼──────┤
│  `   │  1 │  2 │  3 │  4 │  5 │  6 ║  7 │  8 │  9 │  0 │  - │  = │  Backspace  │ Home │
├───────┬───┴┬───┴┬───┴┬───┴┬───┴╥───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬────────────┼──────┤
│  Tab  │  Q │  W │  E │  R │  T ║  Y │  U │  I │  O │  P │  [ │  ] │     \      │ PgUp │
├────────┬───┴┬───┴┬───┴┬───┴┬───╫───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴────────────┼──────┤
│  Caps  │  A │  S │  D │  F │  G ║  H │  J │  K │  L │  ; │  ' │    Enter       │ Del  │
├─────────┬──┴┬───┴┬───┴┬───┴┬───╫───┬┴───┬┴───┬┴───┬┴───┬┴──────────┬────┬──────┤
│ LShift  │ Z │  X │  C │  V │  B ║  N │  M │  , │  . │  / │  RShift  │ ↑  │ End  │
├──────┬───┴┬──────┴┬───────────╫──┬──────┬──────────┬──────┐    ├────┼────┼──────┤
│LCtrl │Win │ LAlt  │   Space   ║Sp│ RAlt │    Fn    │RCtrl │    │ ←  │ ↓  │  →   │ PgDn │
└──────┴────┴───────┴───────────╨──┴──────┴──────────┴──────┘    └────┴────┴──────┴──────┘
```

## Matrix

The two halves have **asymmetric column counts** — matched exactly to the hand-wired trace layout:

| Half  | Rows | Cols | Matrix positions | Keys used |
|-------|------|------|-----------------|-----------|
| Left  | 6    | 7    | 42              | 36        |
| Right | 6    | 10   | 60              | 51        |

`MATRIX_ROWS = 12` (rows 0–5 = left, rows 6–11 = right), `MATRIX_COLS = 10`.

### Per-row key counts

| Row | Left keys | Right keys |
|-----|-----------|------------|
| R0 (Fn)     | 7  | 10 |
| R1 (Number) | 7  | 8  |
| R2 (QWERTY) | 6  | 9  |
| R3 (Home)   | 6  | 8  |
| R4 (Shift)  | 6  | 8  |
| R5 (Bottom) | 4  | 8  |

## GPIO Pin Assignment (both Picos use the same physical pins)

| Function         | Pins            | Notes                                   |
|------------------|-----------------|-----------------------------------------|
| Row scan         | GP4 – GP9       | 6 pins, shared both sides               |
| Col scan (left)  | GP10 – GP16     | 7 pins; defined via `MATRIX_COL_PINS`   |
| Col scan (right) | GP10 – GP19     | 10 pins; defined via `MATRIX_COL_PINS_RIGHT` |
| UART TX          | GP0             | UART0; connects to opposite half RX     |
| UART RX          | GP1             | UART0; connects to opposite half TX     |

The left Pico drives 13 matrix GPIO pins (GP4–GP16); the right drives 16 (GP4–GP19).
GP0/GP1 are reserved on both sides for the split link; GP2/GP3 are free for future use.

## Split Link

Full-duplex UART at 460800 baud.  Cable wiring: `TX ↔ RX`, `RX ↔ TX`, `GND ↔ GND`.  No VCC on the cable — each Pico is powered independently via its own USB port.

Master election uses `SPLIT_USB_DETECT`: whichever half has an active USB host connection becomes master. Either half can be plugged into the computer.

## Layers

### Layer 0 — Base (QWERTY)

Standard ANSI layout. Both Space bars send `KC_SPC`.

### Layer 1 — Fn (hold right `Fn` key)

| Key combo     | Action              |
|---------------|---------------------|
| Fn + `-`      | Volume down         |
| Fn + `=`      | Volume up           |
| Fn + `PgUp`   | Mute                |
| Fn + `↑`      | Play / Pause        |
| Fn + `←`      | Previous track      |
| Fn + `↓`      | Mute                |
| Fn + `→`      | Next track          |
| Fn + `Esc`    | `QK_BOOT` (left half bootloader)  |
| Fn + `Pause`  | `QK_BOOT` (right half bootloader) |

## Preliminary Verifications

Before soldering switches, flash both Picos and verify circuit logic with jumper wires.

### 1. Split link

- Wire the interconnect cable **TX ↔ RX, RX ↔ TX, GND ↔ GND** (no VCC).
- Plug only the left Pico into the computer — confirm it enumerates as a HID device (master).
- Plug only the right Pico — confirm it also enumerates as master when alone.
- Connect both halves via the cable and plug in either side — the plugged-in half should become master and both halves should be recognized.

### 2. Master election (`SPLIT_USB_DETECT`)

- With both halves connected by the cable, plug USB into the **left** Pico → left is master.
- Swap to the **right** Pico → right becomes master.
- Verify that keystrokes from the slave half are forwarded correctly in both configurations.

### 3. Matrix — jumper testing

Short each row/col pin pair with a jumper wire to simulate keypresses and confirm positions match the layout (use `qmk console` or a keytest tool):

- **Left** (GP4–GP9 × GP10–GP16): 7 col pins; simulate all 36 key positions.
- **Right** (GP4–GP9 × GP10–GP19): 10 col pins; simulate all 51 key positions.
- Confirm that GP17–GP19 produce **no output** on the left Pico — those pins are intentionally unwired on the left half.

### 4. Bootloader shortcut

Before casing the board, confirm the in-firmware bootloader entry works so the BOOTSEL button remains accessible without disassembly:

- Hold `Fn + Esc` on the left half → left Pico should reboot into UF2 bootloader.
- Hold `Fn + Pause` on the right half → right Pico should reboot into UF2 bootloader.

---

## Building & Flashing

```sh
# Build
qmk compile -kb h_ergo -km default

# Flash left half (hold BOOTSEL on left Pico before plugging in USB)
qmk flash -kb h_ergo -km default

# Flash right half (hold BOOTSEL on right Pico)
qmk flash -kb h_ergo -km default
```

Alternatively, hold `Fn + Esc` (left) or `Fn + Pause` (right) to reboot into the bootloader without touching the board.

## File Structure

```
keyboards/h_ergo/
├── keyboard.json          — processor, USB IDs, matrix pins, layout
├── config.h               — asymmetric col pins, UART config, master election
└── keymaps/
    └── default/
        └── keymap.c       — base QWERTY + Fn layer
```
