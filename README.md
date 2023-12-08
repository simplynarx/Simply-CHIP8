# Simply-CHIP8: A Basic Chip-8 Interpreter/Emulator

https://github.com/simplynarx/Simply-CHIP8/assets/123662551/a3a468c0-a77d-449e-854b-ced1755c0fdc

Simply-Chip8 is a basic Chip-8 interpreter/emulator, written from scratch in C and built for Linux. It emulates all CPU instructions as they appeared on the COSMAC VIP!

Note: This is my first attempt at a major project written in C. I've decided to write a CHIP-8 interpreter since emulation has always been an interest of mine. A rewrite is in progress.

### Usage
```
./simply-chip8 [FILE]... [OPTIONS]...
```
### Options 

| Option | Description |
| --- | --- |
| -i | Specifies the number of instructions per second. If the input is less than 60, set to default value (200) |

### Keypad Layout

| CHIP-8 Keypad | Emulator Keypad |
| --- | --- |
| 1 2 3 C | 1 2 3 4 |
| 4 5 6 D | Q W E R |
| 7 8 9 E | A S D F |
| A 0 B F | Z X C V |

### Not Yet Implemented
- Sound timer

### No Plans For Implementation
- SuperChip additional instructions

### Future Plans:
- Custom Colors
- Debug window

### Documentation Used:

https://tobiasvl.github.io/blog/write-a-chip-8-emulator

https://austinmorlan.com/posts/chip8_emulator/#loading-the-fonts (Draw Opcode)

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.0
