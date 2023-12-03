# Simply-CHIP8
## A Chip-8 Interpreter/Emulator

https://github.com/simplynarx/Simply-CHIP8/assets/123662551/a3a468c0-a77d-449e-854b-ced1755c0fdc

This is my first attempt at a major project written in C. I've decided to write a CHIP-8 interpreter since emulation has always been an interest of mine.

Update: With this project finished, I'm unsatisfied with the inconsistency of quality in the source code. A full rewrite will follow soon.

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
