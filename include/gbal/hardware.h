#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>

// ========== Screen dimensions [pixels] ==========
#define SCREEN_W 240
#define SCREEN_H 160

// ========== Memory map bases ==========
// BIOS ROM (read only), 0x00000000-0x00003FFF
#define MEM_BIOS ((uint8_t*)0x00000000)
// Work RAM 256KB (external/internal mapping), 0x02000000-0x0203FFFF. 16-bit bus (slower)
#define MEM_WRAM ((uint8_t*)0x02000000)
// On-chip Work RAM 32KB, 0x03000000-0x03007FFF. 32-bit bus, no wait (fast)
#define MEM_IWRAM ((uint8_t*)0x03000000)
// External (Game Pak) RAM, 0x0E000000-0x0E00FFFF. 8-bit bus: access bytes only
#define MEM_EXTRAM ((uint8_t*)0x0E000000)
// I/O registers, 0x04000000-0x040003FE. Bus access: 8/16/32 bit
#define MEM_IO 0x04000000

// Palette RAM (Background/Object). No 8bit write. Access only during HBLANK/VBLANK
#define MEM_PAL_BG ((uint16_t*)0x05000000) // Background palette
#define MEM_PAL_OBJ ((uint16_t*)0x05000200) // Sprite palette
// Video RAM (VRAM). No 8bit write. Access only during HBLANK/VBLANK
#define MEM_VRAM ((uint16_t*)0x06000000) // Background VRAM space
#define MEM_VRAM_OBJ ((uint16_t*)0x06010000) // Sprite VRAM
// bitmap modes (3-5): framebuffer covers lower half, sprite tiles >= 512 only
#define OBJ_TILE_BITMAP_MIN 512
#define MEM_OAM ((uint16_t*)0x07000000) // Object Attribute Memory

// ========== Helpers for tile-map addressing ==========
// Charblock: 16KB unit holding tile graphics (n = 0..3)
#define MEM_VRAM_CHARBLOCK(n) ((uint16_t*)(0x06000000 + (n) * 0x4000))
// Screenblock: 2KB unit holding a tilemap (n = 0..31)
#define MEM_VRAM_SCREENBLOCK(n) ((uint16_t*)(0x06000000 + (n) * 0x0800))

// ========== I/O registers access helpers ==========
// Use REG16(off) for registers whose size is 2 bytes, REG32(off) for registers whose size
// is 4 bytes
#define REG16(off) (*(volatile uint16_t*)(MEM_IO + (off)))
#define REG32(off) (*(volatile uint32_t*)(MEM_IO + (off)))

// Note: Registers are R/W unless annotated otherwise (write only / read only).
// ========== LCD I/O Registers ==========
#define REG_DISPCNT REG16(0x0000) // LCD Control
#define REG_DISPSTAT REG16(0x0004) // General LCD status
#define REG_VCOUNT REG16(0x0006) // Vertical counter (read only)
#define REG_BG0CNT REG16(0x0008) // BG0 Control
#define REG_BG1CNT REG16(0x000A) // BG1 Control
#define REG_BG2CNT REG16(0x000C) // BG2 Control
#define REG_BG3CNT REG16(0x000E) // BG3 Control
// BG scroll offsets (write only)
#define REG_BG0HOFS REG16(0x0010) // BG0 X-Offset
#define REG_BG0VOFS REG16(0x0012) // BG0 Y-Offset
#define REG_BG1HOFS REG16(0x0014) // BG1 X-Offset
#define REG_BG1VOFS REG16(0x0016) // BG1 Y-Offset
#define REG_BG2HOFS REG16(0x0018) // BG2 X-Offset
#define REG_BG2VOFS REG16(0x001A) // BG2 Y-Offset
#define REG_BG3HOFS REG16(0x001C) // BG3 X-Offset
#define REG_BG3VOFS REG16(0x001E) // BG3 Y-Offset
// Affine transformations registers (write only)
#define REG_BG2PA REG16(0x0020) // BG2 Affine matrix param. A (dx)
#define REG_BG2PB REG16(0x0022) // BG2 Affine matrix param. B (dmx)
#define REG_BG2PC REG16(0x0024) // BG2 Affine matrix param. C (dy)
#define REG_BG2PD REG16(0x0026) // BG2 Affine matrix param. D (dmy)
#define REG_BG2X REG32(0x0028) // BG2 reference point X-Coordinate
#define REG_BG2Y REG32(0x002C) // BG2 reference point Y-Coordinate
#define REG_BG3PA REG16(0x0030) // BG3 Affine matrix param. A (dx)
#define REG_BG3PB REG16(0x0032) // BG3 Affine matrix param. B (dmx)
#define REG_BG3PC REG16(0x0034) // BG3 Affine matrix param. C (dy)
#define REG_BG3PD REG16(0x0036) // BG3 Affine matrix param. D (dmy)
#define REG_BG3X REG32(0x0038) // BG3 reference point X-Coordinate
#define REG_BG3Y REG32(0x003C) // BG3 reference point Y-Coordinate
// Graphic effects
#define REG_WIN0H REG16(0x0040) // Window 0 Horizontal Dimensions (write only)
#define REG_WIN1H REG16(0x0042) // Window 1 Horizontal Dimensions (write only)
#define REG_WIN0V REG16(0x0044) // Window 0 Vertical Dimensions (write only)
#define REG_WIN1V REG16(0x0046) // Window 1 Vertical Dimensions (write only)
#define REG_WININ REG16(0x0048) // Inside of Window 0 and 1
#define REG_WINOUT REG16(0x004A) // Inside of Object Window & outside of windows
#define REG_MOSAIC REG16(0x004C) // Mosaic Size (write only)
#define REG_BLDCNT REG16(0x0050) // Colour special effects selection
#define REG_BLDALPHA REG16(0x0052) // Alpha Blending Coefficients
#define REG_BLDY REG16(0x0054) // Brightness (Fade-In/Out) Coefficient (write only)

// ========== Keypad Input Registers ==========
#define REG_KEYINPUT REG16(0x0130) // Key Status (read only)
#define REG_KEYCNT REG16(0x0132) // Key Interrupt Control

// ========== Sound Registers ==========
#define REG_SOUND1CNT_L REG16(0x0060) // Channel 1 Sweep register (NR10)
#define REG_SOUND1CNT_H REG16(0x0062) // Channel 1 Duty/Length/Envelope (NR11, NR12)
#define REG_SOUND1CNT_X REG16(0x0064) // Channel 1 Frequency/Control (NR13, NR14)
#define REG_SOUND2CNT_L REG16(0x0068) // Channel 2 Duty/Length/Envelope (NR21, NR22)
#define REG_SOUND2CNT_H REG16(0x006C) // Channel 2 Frequency/Control (NR23, NR24)
#define REG_SOUND3CNT_L REG16(0x0070) // Channel 3 Stop/Wave RAM select (NR30)
#define REG_SOUND3CNT_H REG16(0x0072) // Channel 3 Length/Volume (NR31, NR32)
#define REG_SOUND3CNT_X REG16(0x0074) // Channel 3 Frequency/Control (NR33, NR34)
#define REG_SOUND4CNT_L REG16(0x0078) // Channel 4 Length/Envelope (NR41, NR42)
#define REG_SOUND4CNT_H REG16(0x007C) // Channel 4 Frequency/Control (NR43, NR44)
#define REG_SOUNDCNT_L REG16(0x0080) // Control Stereo/Volume/Enable (NR50, NR51)
#define REG_SOUNDCNT_H REG16(0x0082) // Control Mixing/DMA Control
#define REG_SOUNDCNT_X REG16(0x0084) // Control Sound on/off (NR52)
#define REG_SOUNDBIAS REG16(0x0088) // Sound PWM Control (set up by BIOS)
// Channel 3 Wave Pattern RAM (16 bytes, 2 banks)
#define MEM_WAVE_RAM ((volatile uint16_t*)(MEM_IO + 0x0090))
#define REG_FIFO_A REG32(0x00A0) // Channel A FIFO, Data 0-3 (write only)
#define REG_FIFO_B REG32(0x00A4) // Channel B FIFO, Data 0-3 (write only)

// ========== DMA Transfer Channels Registers ==========
#define REG_DMA0SAD REG32(0x00B0) // DMA 0 Source Address (write only)
#define REG_DMA0DAD REG32(0x00B4) // DMA 0 Destination Address (write only)
#define REG_DMA0CNT_L REG16(0x00B8) // DMA 0 Word Count (write only)
#define REG_DMA0CNT_H REG16(0x00BA) // DMA 0 Control
#define REG_DMA1SAD REG32(0x00BC) // DMA 1 Source Address (write only)
#define REG_DMA1DAD REG32(0x00C0) // DMA 1 Destination Address (write only)
#define REG_DMA1CNT_L REG16(0x00C4) // DMA 1 Word Count (write only)
#define REG_DMA1CNT_H REG16(0x00C6) // DMA 1 Control
#define REG_DMA2SAD REG32(0x00C8) // DMA 2 Source Address (write only)
#define REG_DMA2DAD REG32(0x00CC) // DMA 2 Destination Address (write only)
#define REG_DMA2CNT_L REG16(0x00D0) // DMA 2 Word Count (write only)
#define REG_DMA2CNT_H REG16(0x00D2) // DMA 2 Control
#define REG_DMA3SAD REG32(0x00D4) // DMA 3 Source Address (write only)
#define REG_DMA3DAD REG32(0x00D8) // DMA 3 Destination Address (write only)
#define REG_DMA3CNT_L REG16(0x00DC) // DMA 3 Word Count (write only)
#define REG_DMA3CNT_H REG16(0x00DE) // DMA 3 Control

// ========== Timer Registers ==========
#define REG_TM0CNT_L REG16(0x0100) // Timer 0 Counter/Reload
#define REG_TM0CNT_H REG16(0x0102) // Timer 0 Control
#define REG_TM1CNT_L REG16(0x0104) // Timer 1 Counter/Reload
#define REG_TM1CNT_H REG16(0x0106) // Timer 1 Control
#define REG_TM2CNT_L REG16(0x0108) // Timer 2 Counter/Reload
#define REG_TM2CNT_H REG16(0x010A) // Timer 2 Control
#define REG_TM3CNT_L REG16(0x010C) // Timer 3 Counter/Reload
#define REG_TM3CNT_H REG16(0x010E) // Timer 3 Control

// ========== Interrupt, Waitstate, and Power-Down Control Registers ==========
#define REG_IE REG16(0x0200) // Interrupt Enable Register
#define REG_IF REG16(0x0202) // Interrupt Request Flags/IRQ Acknowledge
#define REG_WAITCNT REG16(0x0204) // Game Pak Waitstate Control
#define REG_IME REG16(0x0208) // Interrupt Master Enable Register
#define REG_HALTCNT (*(volatile uint8_t*)(MEM_IO + 0x0301)) // Power Down/Halt (write only)

// ========== Serial Communication Registers ==========
#define REG_SIODATA32 REG32(0x0120) // SIO Data (Normal-32bit mode; shared with below)
#define REG_SIOMULTI0 REG16(0x0120) // SIO Data 0 (Parent; Multi-Player Mode)
#define REG_SIOMULTI1 REG16(0x0122) // SIO Data 1 (1st child; Multi-Player Mode)
#define REG_SIOMULTI2 REG16(0x0124) // SIO Data 2 (2nd child; Multi-Player Mode)
#define REG_SIOMULTI3 REG16(0x0126) // SIO Data 3 (3rd child; Multi-Player Mode)
#define REG_SIOCNT REG16(0x0128) // SIO Control Register
#define REG_SIOMLT_SEND REG16(0x012A) // SIO Data (Local Multi-Player; shared with below)
#define REG_SIODATA8 REG16(0x012A) // SIO Data (Normal-8bit & UART Mode)
#define REG_RCNT REG16(0x0134) // SIO Mode Select/General Purpose Data
#define REG_JOYCNT REG16(0x0140) // SIO JOY Bus Control
#define REG_JOY_RECV REG32(0x0150) // SIO JOY Bus Receiver Data
#define REG_JOY_TRANS REG32(0x0154) // SIO JOY Bus Transmit Data
// SIO JOY Bus Receive Status (read; write behaviour undocumented)
#define REG_JOYSTAT REG16(0x0158)

// ========== OAM / Sprites ==========
// Object Attribute Memory: 128 object entries (ObjAttributes) overlapped with
// 32 affine matrices (ObjAffineMatrix). Same physical memory, two views.
// Bit meanings of the attributes live in the sprite HAL, not here.

// One object entry in OAM (Tonc: OBJ_ATTR)
typedef struct ObjAttributes {
    uint16_t attr0;
    uint16_t attr1;
    uint16_t attr2;
    uint16_t fill;   // unused by OBJ; holds one affine param when interleaved
} __attribute__((aligned(4))) ObjAttributes;

// One affine matrix, interleaved in the 'fill' slots of 4 ObjAttributes (Tonc: OBJ_AFFINE)
// Params pa..pd are signed fixed-point: int16_t, not uint16_t
typedef struct ObjAffineMatrix {
    uint16_t fill0[3];
    int16_t  pa;
    uint16_t fill1[3];
    int16_t  pb;
    uint16_t fill2[3];
    int16_t  pc;
    uint16_t fill3[3];
    int16_t  pd;
} __attribute__((aligned(4))) ObjAffineMatrix;

#define OAM_OBJECTS  ((volatile ObjAttributes*)MEM_OAM) // OAM_OBJECTS[i]: object i (0..127)
// OAM_AFFINE_MATRICES[i]: matrix i (0..31)
#define OAM_AFFINE_MATRICES ((volatile ObjAffineMatrix*)MEM_OAM)

// ========== Background / Palette types ==========
typedef uint16_t ScreenEntry;   // tilemap entry: tile idx + flip + palbank (Tonc: SCR_ENTRY)
typedef uint16_t PaletteBank[16]; // a 16-colour palette bank (4bpp)

#endif // HARDWARE_H