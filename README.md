# Simply-CHIP8: A Basic Chip-8 Interpreter/Emulator

<p align="center">
  <img src="https://github.com/simplynarx/Simply-CHIP8/assets/123662551/1207f9b9-e77c-4d44-b846-b727d8034c14" alt="animated">
</p>

Simply-Chip8 is a basic Chip-8 interpreter/emulator, written from scratch in C and built for Linux. It emulates all CPU instructions as they appeared on the COSMAC VIP!

### Usage
```
./simply-chip8 [FILE]... [OPTIONS]...
```
### Options 

| Option | Description |
| --- | --- |
| -i ... | Change the number of instructions per second. If the input is less than 60, set to default value (200) |
| -s ... | Change the video scale (i.e. "8" --> 8x video scale). If the input is less than or equal to 0, set to default value (16) | 

### Keypad Layout

| CHIP-8 Keypad | Emulator Keypad |
| --- | --- |
| 1 2 3 C | 1 2 3 4 |
| 4 5 6 D | Q W E R |
| 7 8 9 E | A S D F |
| A 0 B F | Z X C V |

### No Plans For Implementation
- SuperChip additional instructions

### Future Plans:
- Sound implementation
- Custom Colors
- Debug window

### Documentation Used:

https://tobiasvl.github.io/blog/write-a-chip-8-emulator

https://austinmorlan.com/posts/chip8_emulator/#loading-the-fonts (Draw Opcode)

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.0
