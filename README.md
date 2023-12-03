# Simply-CHIP8
## A Chip-8 Interpreter/Emulator

This is my first attempt at a major project written in C. I've decided to write a CHIP-8 interpreter since emulation has always been an interest of mine.

UPDATE: Project is now done! I've learned a lot by writing this interpreter. I will be:
- Doing a writeup on everything I've learned and my thinking process
- A complete rewrite of the program, as the source code is messy

### Usage
```
./simply-chip8 CHIP8_PROGRAM CPU_CYCLE_DELAY_IN_MS
```

### Currently Working:
- Reading program files
- Memory (and storing roms into memory)
- CPU Cycles
- All CPU Opcodes
- Display
- Keypad Input
- Delay timer

### In Progress:
- N/A

### Not Implementing:
- Sound timer

### Future Plans:
- Full rewrite
- Debug window

### Documentation Used:

https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Technical-Reference

https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set

https://tobiasvl.github.io/blog/write-a-chip-8-emulator

http://www.emulator101.com/a-quick-introduction-to-a-cpu.html

https://austinmorlan.com/posts/chip8_emulator/#loading-the-fonts (Draw Opcode)
