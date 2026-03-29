// h_ergo default keymap
// Split TKL: 36-key left half + 51-key right half, Raspberry Pi Pico per side.
//
// Layer 0 — Base (QWERTY)
// Layer 1 — Fn  (media, volume, navigation extras, firmware flash)
//
// Key order matches the LAYOUT array in keyboard.json:
//   Row 0: Esc … F6 | F7 … Ins          (17 keys)
//   Row 1: ` … 6   | 7 … Home           (15 keys)
//   Row 2: Tab … T | Y … PgUp           (15 keys)
//   Row 3: Caps … G | H … Del           (14 keys)
//   Row 4: LSft … B | N … End           (14 keys)
//   Row 5: LCtl … Spc | Spc … PgDn     (12 keys)

#include QMK_KEYBOARD_H

enum h_ergo_layers {
    _BASE = 0,
    _FN,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* ── Layer 0: Base QWERTY ────────────────────────────────────────────────
     * ,──────┬────┬────┬────┬────┬────┬────╥────┬────┬────┬────┬────┬────┬──────┬──────┬──────┬──────╮
     * │ Esc  │ F1 │ F2 │ F3 │ F4 │ F5 │ F6 ║ F7 │ F8 │ F9 │F10 │F11 │F12 │PrtSc │ScrLk │Pause │ Ins  │
     * ├──────┼────┼────┼────┼────┼────┼────╫────┼────┼────┼────┼────┼────┼──────┴──────┼──────┤
     * │  `   │  1 │  2 │  3 │  4 │  5 │  6 ║  7 │  8 │  9 │  0 │  - │  = │  Backspace  │ Home │
     * ├──────┴┬───┴┬───┴┬───┴┬───┴┬───┴╥───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬────────────┼──────┤
     * │  Tab  │  Q │  W │  E │  R │  T ║  Y │  U │  I │  O │  P │  [ │  ] │    \       │ PgUp │
     * ├───────┴┬───┴┬───┴┬───┴┬───┴┬───╫───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴────────────┼──────┤
     * │  Caps  │  A │  S │  D │  F │  G ║  H │  J │  K │  L │  ; │  ' │    Enter       │ Del  │
     * ├────────┴┬───┴┬───┴┬───┴┬───┴┬──╫───┬┴───┬┴───┬┴───┬┴───┬┴────┴───────┬────┬──────┤
     * │ LShift  │  Z │  X │  C │  V │  B ║  N │  M │  , │  . │  / │  RShift   │ ↑  │ End  │
     * ├──────┬──┴─┬──┴───┬┴────┴────┴──╫──┬┴───┴┬───┴────┬──────┬────┬────┬────┼────┼──────┤
     * │LCtrl │Win │ LAlt │    Space     ║Sp│ RAlt│  Fn    │RCtrl │    │ ←  │ ↓  │ →  │ PgDn │
     * └──────┴────┴──────┴─────────────╨──┴─────┴────────┴──────┘    └────┴────┴────┴──────┘
     */
    [_BASE] = LAYOUT(
        // Row 0: Fn row
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_SCRL, KC_PAUS, KC_INS,
        // Row 1: Number row
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,
        KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_HOME,
        // Row 2: QWERTY
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
        // Row 3: Home row
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_DEL,
        // Row 4: Shift row
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,
        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_END,
        // Row 5: Bottom row
        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC,
        KC_SPC,  KC_RALT, MO(_FN), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN
    ),

    /* ── Layer 1: Fn ─────────────────────────────────────────────────────────
     * Fn + arrow cluster → media/volume
     * Fn + Esc           → QK_BOOT (enter bootloader for flashing)
     * Fn + Pause         → QK_BOOT (right-half alternative)
     * All other keys     → transparent (pass through to Base)
     *
     * ,──────┬────┬────┬────┬────┬────┬────╥────┬────┬────┬────┬────┬────┬──────┬──────┬──────┬──────╮
     * │ BOOT │    │    │    │    │    │    ║    │    │    │    │    │    │      │      │ BOOT │      │
     * ├──────┼────┴────┴────┴────┴────┴────╫────┴────┴────┴────┴────┴────┴──────┴──────┼──────┤
     * │      │                             ║                             │             │      │
     * ├──────┴┬────────────────────────────╫────────────────────────────┴─────────────┼──────┤
     * │       │                            ║                                           │ Mute │
     * ├───────┴────────────────────────────╫────────────────────────────────────────── ┼──────┤
     * │       │                            ║                                    │      │      │
     * ├────────┴───────────────────────────╫──────────────────────────────────┬─┴──┬──────┤
     * │         │                          ║               │         │        │Mply│      │
     * ├──────┬──┴─┬──────┬────────────────╫──┬────────────┬──────┬──┘  ┌────┼────┼──────┤
     * │      │    │      │                ║  │            │      │     │Mprv│Mute│Mnxt│      │
     * └──────┴────┴──────┴────────────────╨──┴────────────┴──────┘     └────┴────┴────┴──────┘
     */
    [_FN] = LAYOUT(
        // Row 0
        QK_BOOT, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT, _______,
        // Row 1
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, KC_VOLD, KC_VOLU, _______, _______,
        // Row 2
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, KC_MUTE,
        // Row 3
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,
        // Row 4
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, KC_MPLY, _______,
        // Row 5
        _______, _______, _______, _______,
        _______, _______, _______, _______, KC_MPRV, KC_MUTE, KC_MNXT, _______
    ),
};
