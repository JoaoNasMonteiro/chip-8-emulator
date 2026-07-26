Let's analyse the crash
crash-1d678cddb0badd45f2ac88992b654a032150cc03

First we run our code with gdb and try to see where it crashes


``` Bash
$ gdb --args ./code/v0/bin/chip_emulator_fuzzer assessment/fuzzing/v0/crash-1d678cddb0badd45f2ac88992b654a032150cc03

(gdb) run
Starting program: /home/jjp/chip-8-emulator/code/v0/bin/chip_emulator_fuzzer assessment/fuzzing/v0/crash-1d678cddb0badd45f2ac88992b654a032150cc03
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
INFO: Running with entropic power schedule (0xFF, 100).
INFO: Seed: 3312083576
INFO: Loaded 1 modules   (544 inline 8-bit counters): 544 [0x5555556e7b18, 0x5555556e7d38),
INFO: Loaded 1 PC tables (544 PCs): 544 [0x5555556e7d38,0x5555556e9f38),
[New Thread 0x7ffff24b96c0 (LWP 36892)]
/home/jjp/chip-8-emulator/code/v0/bin/chip_emulator_fuzzer: Running 1 inputs 1 time(s) each.
Running: assessment/fuzzing/v0/crash-1d678cddb0badd45f2ac88992b654a032150cc03
chip8.c:351:31: runtime error: index 4096 out of bounds for type 'uint8_t[4096]' (aka 'unsigned char[4096]')
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior chip8.c:351:31
=================================================================
==36889==ERROR: AddressSanitizer: global-buffer-overflow on address 0x5555560740dc at pc 0x55555569eb0
6 bp 0x7fffffffd910 sp 0x7fffffffd908
READ of size 1 at 0x5555560740dc thread T0
[Detaching after fork from child process 36893]
    #0 0x55555569eb05 in i_drw_vx_vy_n /home/jjp/chip-8-emulator/code/v0/chip8.c:351:31
    #1 0x55555569eb05 in cpu_step /home/jjp/chip-8-emulator/code/v0/chip8.c:226:9
    #2 0x55555569f06e in LLVMFuzzerTestOneInput /home/jjp/chip-8-emulator/code/v0/fuzz.c:28:9
    #3 0x5555555a6cd4 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/j
jp/chip-8-emulator/code/v0/bin/chip_emulator_fuzzer+0x52cd4) (BuildId: 58bf763376af7d73d75b31b60164fc4
09fc64018)
    #4 0x55555558fe06 in fuzzer::RunOneTest(fuzzer::Fuzzer*, char const*, unsigned long) (/home/jjp/ch
ip-8-emulator/code/v0/bin/chip_emulator_fuzzer+0x3be06) (BuildId: 58bf763376af7d73d75b31b60164fc409fc6
4018)
    #5 0x5555555958ba in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned lo
ng)) (/home/jjp/chip-8-emulator/code/v0/bin/chip_emulator_fuzzer+0x418ba) (BuildId: 58bf763376af7d73d7
5b31b60164fc409fc64018)
    #6 0x5555555c0076 in main (/home/jjp/chip-8-emulator/code/v0/bin/chip_emulator_fuzzer+0x6c076) (Bu
ildId: 58bf763376af7d73d75b31b60164fc409fc64018)
    #7 0x7ffff782a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #8 0x7ffff782a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #9 0x55555558a9d4 in _start (/home/jjp/chip-8-emulator/code/v0/bin/chip_emulator_fuzzer+0x369d4) (
BuildId: 58bf763376af7d73d75b31b60164fc409fc64018)

0x5555560740dc is located 0 bytes after global variable 'cpu' defined in '/home/jjp/chip-8-emulator/code/v0/fuzz.c:7' (0x5555560728a0) of size 6204
SUMMARY: AddressSanitizer: global-buffer-overflow /home/jjp/chip-8-emulator/code/v0/chip8.c:351:31 in i_drw_vx_vy_n
Shadow bytes around the buggy address:
  0x555556073e00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x555556073e80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x555556073f00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x555556073f80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x555556074000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x555556074080: 00 00 00 00 00 00 00 00 00 00 00[04]f9 f9 f9 f9
  0x555556074100: f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9
  0x555556074180: f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9
  0x555556074200: f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9
  0x555556074280: f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9
  0x555556074300: f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==36889==ABORTING
[Thread 0x7ffff24b96c0 (LWP 36892) exited]
[Inferior 1 (process 36889) exited with code 01]

(gdb) print cpu
$1 = {memory = '\000' <repeats 4095 times>, display_buffer = '\000' <repeats 2047 times>,
  registers = '\000' <repeats 15 times>, I = 0, delay_timer = 0 '\000', sound_timer = 0 '\000',
  pc = 0, sp = 0 '\000', stack = {0 <repeats 16 times>}, keypad = 0, is_halted = 0 '\000', config = {
    mode = 0 '\000'}}
```

Right off the bat this telss us a lot of things

firstly the

chip8.c:351:31: runtime error: index 4096 out of bounds for type 'uint8_t[4096]' (aka 'unsigned char[4096]')

line tells us that tehre was some sort of oob access (either write or read, it does not tell us) by using index 4096 in a uint8_t[4096] array. Our only uint8_t variable is cpu.memory, so it has to involve that.

A bit lower down we see in this line

`#0 0x55555569eb05 in i_drw_vx_vy_n /home/jjp/chip-8-emulator/code/v0/chip8.c:351:31`

that the crash has had something to do with the function `i_drw_vx_vy_n`

By looking at it's signature we can see that, in fact, the function accesses cpu.memory, and because it only reads to it we can assume that the illegal access was in fact a read.

```c
static inline void i_drw_vx_vy_n(chip8_cpu_t *cpu, uint8_t x, uint8_t y, uint8_t n) {
    uint8_t start_x = cpu->registers[x] % DISPLAY_WIDTH;
    uint8_t start_y = cpu->registers[y] % DISPLAY_HEIGHT;

    cpu->registers[0xF] = 0;

    for (int row = 0; row < n; row++) {
        uint8_t sprite_byte = cpu->memory[cpu->I + row];

        for (int col = 0; col < 8; col++) {
            uint8_t sprite_pixel = (sprite_byte >> (7 - col)) & 1;

            if (sprite_pixel == 0) {
                continue;
            }

            uint16_t current_x = start_x + col;
            uint16_t current_y = start_y + row;

            if (current_x >= DISPLAY_WIDTH || current_y >= DISPLAY_HEIGHT) {
                continue;
            }

            uint16_t buff_index = (current_y * DISPLAY_WIDTH) + current_x;

            if (cpu->display_buffer[buff_index] == 1) {
                cpu->registers[0xF] = 1;
            }

            cpu->display_buffer[buff_index] ^= 1;
        }
    }
}
```
in the line

```c
uint8_t sprite_byte = cpu->memory[cpu->I + row];
```

we can see that we are acessing the memory array indexing it by I + the row number witohut any kind of sanitization. So a ROM like this:

Running GDB with a ROM like this:

```hex
00000000: AFFF D002                                ....
```

we get this output:

```bash
/home/jjp/chip-8-emulator/code/v0/bin/chip_emulator_fuzzer: Running 1 inputs 1 time(s) each.
Running: ../../assessment/fuzzing/v0/malicious_roms/dxyn_oob_read.ch8
chip8.c:351:31: runtime error: index 4096 out of bounds for type 'uint8_t[4096]' (aka 'unsigned char[4
096]')
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior chip8.c:351:31
Executed ../../assessment/fuzzing/v0/malicious_roms/dxyn_oob_read.ch8 in 2 ms
```

If we run gdb with `set environment ASAN_OPTIONS=abort_on_error=1` we can print out the state of our CPU right before the crash:

```bash
(gdb) print cpu
$1 = {
  memory = '\000' <repeats 80 times>, "𐐐\360 `  p\360\020\360\200\360\360\020\360\020\360\220\220\360\
020\020\360\200\360\020\360\360\200\360\220\360\360\020 @@\360\220\360\220\360\360\220\360\020\360\360
\220\360\220\220\340\220\340\220\340\360\200\200\200\360\340\220\220\220\340\360\200\360\200\360\360\2
00\360\200\200", '\000' <repeats 352 times>...,
  display_buffer = "\001\000\001\000\001\000\001", '\000' <repeats 59 times>, "\001\001\001\000\000\00
1", '\000' <repeats 56 times>, "\001\001\001\001\001\001\001\001", '\000' <repeats 58 times>, "\001\00
1", '\000' <repeats 62 times>..., registers = "\000X\b", '\000' <repeats 12 times>, I = 6197,
  delay_timer = 0 '\000', sound_timer = 0 '\000', pc = 565, sp = 0 '\000', stack = {
    0 <repeats 16 times>}, keypad = 0, is_halted = 0 '\000', config = {mode = 0 '\000'}}
```

note that the `pc` is at 563 (0x235). Accounting for the fact that the CPU advances the PC before executing the instruction it just read and for the fact that the first byte of the ROM is placed at 0x200 in the memory of the ROM, we should find the instruction that is responsible for the crash at offset

0x235 - 0x202 = 0x33

looking at the malicious crash rom, we can see

```hex
00000000: 1225 5350 4143 4520 494e 5641 4445 5253  .%SPACE INVADERS
00000010: 2030 2e39 3120 4279 2044 6176 6964 2057   0.91 By David W
00000020: 494e 5445 5260 0061 0062 08a3 ddd0 1871  INTER`.a.b.....
00000030: 08f2 1ed7 dfed d28f f79e ff30 4012 2d69  ...........0@.-i
00000040: 056c 156e 0023 9160 0af0 15f0 0730 0012  .l.n.#.`.....0..
00000050: 4b23 917e 0112 4566 0068 1c69 006a 046b  K#.~..Ef.h.i.j.k
00000060: 0a6c 046d 3c6e 0f00 e023 7523 51fd 1560  .l.m<n...#u#Q..`
00000070: 04e0 9e12 7d23 7538 0078 ff23 7560 06e0  ....}#u8.x.#u`..
```

indeed, the word (two consecutive bytes) at offset 0x33 is `D7 DF`, which the interpreter would interpret as a Dxyn (Draw the sprite at memory position I with n columns at position x and y).


Do note that the value of I at the time of the crash is 6197 (0x1835), which is way bigger than what any single instruction can set it (Annn can only set it to a max of 4095), and the large number is what makes this code crash. Let's explore the execution flow of the program and try to understand how it works

note also that the PC is odd because of the first jump instruction, 0x1225, that jumps to address 0x225, and since the PC increments by 2 each cpu_step then the parity never changes


```Bash
(gdb) set endian big
The target is set to big endian.
(gdb) x/100xh cpu.memory + 512
0x555556072ca0 <cpu+512>:       0x1225  0x5350  0x4143  0x4520  0x494e  0x5641  0x4445  0x5253
0x555556072cb0 <cpu+528>:       0x2030  0x2e39  0x3120  0x4279  0x2044  0x6176  0x6964  0x2057
0x555556072cc0 <cpu+544>:       0x494e  0x5445  0x5260  0x0061  0x0062  0x08a3  0xddd0  0x1871
0x555556072cd0 <cpu+560>:       0x08f2  0x1ed7  0xdfed  0xd28f  0xf79e  0xff30  0x4012  0x2d69
0x555556072ce0 <cpu+576>:       0x056c  0x156e  0x0023  0x9160  0x0af0  0x15f0  0x0730  0x0012
0x555556072cf0 <cpu+592>:       0x4b23  0x917e  0x0112  0x4566  0x0068  0x1c69  0x006a  0x046b
0x555556072d00 <cpu+608>:       0x0a6c  0x046d  0x3c6e  0x0f00  0xe023  0x7523  0x51fd  0x1560
0x555556072d10 <cpu+624>:       0x04e0  0x9e12  0x7d23  0x7538  0x0078  0xff23  0x7560  0x06e0
0x555556072d20 <cpu+640>:       0x9e12  0x8b23  0x7538  0x3978  0x0123  0x7536  0x0012  0x9f60
0x555556072d30 <cpu+656>:       0x05e0  0x9e12  0xe966  0x0165  0x1b84  0x80a3  0xd9d4  0x51a3
0x555556072d40 <cpu+672>:       0xd9d4  0x5175  0xff35  0xff12  0xad66  0x0012  0xe9d4  0x513f
0x555556072d50 <cpu+688>:       0x0112  0xe9d4  0x5166  0x0083  0x4073  0x0383  0xb562  0xf883
0x555556072d60 <cpu+704>:       0x2262  0x0833  0x0012  0xc923
```

As you can see the ROM in the program's memory is identical to the one in the disk file, so that rules out some sort of program modification

Let's map out the execution flow of the application:

It starts out with this hot mess:

```log
DEBUG: PC:0223 | OP:1225 | JP 0x225
DEBUG: PC:0225 | OP:6000 | LD V0, 0x00
DEBUG: PC:0227 | OP:6100 | LD V1, 0x00
DEBUG: PC:0229 | OP:6208 | LD V2, 0x08
DEBUG: PC:022B | OP:A3DD | LD I, 0x3DD
DEBUG: PC:022D | OP:D018 | DRW V0, V1, 8
DEBUG: PC:022F | OP:7108 | ADD V1, 0x08
DEBUG: PC:0231 | OP:F21E | ADD I, V2
DEBUG: PC:0233 | OP:D7DF | DRW V7, VD, F
DEBUG: PC:0235 | OP:EDD2 | invalid operation
DEBUG: PC:0237 | OP:8FF7 | SUBN VF, VF
DEBUG: PC:023B | OP:9EFF | SNE VE, VF
DEBUG: PC:022B | OP:122D | JP 0x22D
```
it jumps forwards to the odd address 0x225, loads 0x8 into V2, loads some initial value into I, draws the sprite at memory location I to the screen, adds V2 to I (increasing it's value), draws again and jumps backwards to 0x22D.

From then on it loops, but with each iteration the value at I gets slighly bigger (by 0x8 each cycle, in fact)

It then repeats this endlessly.. or at least at whatever point something breaks, in this case the value of I excedes the size of the memory and sice Dxyn accesses the ram by indexing it at I + some ooffset then we crash.

But why crash at I = 0x1835 and not at some value closer to 0x1000?

For that we'll have to look a bit into how C structs are laid out in memory and how ASAN works. in our code the cpu struct looks like this:

```C
typedef struct chip8_cpu {
    uint8_t memory[MEM_SIZE];
    uint8_t display_buffer[MAX_DISPLAY_SIZE];
    uint8_t registers[16];
    uint16_t I;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t pc;
    uint8_t sp;
    uint16_t stack[16];
    uint16_t keypad;
    uint8_t is_halted;
    struct chip8_cpu_config config;
} chip8_cpu_t;
```
to find out in bytes we can just add up the fields:
    - memory: 4096 * 1 byte = 4096
    - display_buffer = 2048 * 1 byte = 2048
    - registers: 16 * 1 byte = 16 bytes
    - I: 1 * 2 bytes = 2
    - dt, st, sp: 1 byte for each = 3 bytes
    - pc: 1 * 2 bytes = 2
    - stack: 16 * 2 bytes = 32
    - keypad: 2 bytes
    - is_halted: 1 byte
    - config: 1 byte
totaling 6203

or.. we could just print sezeof(chip8_cpu_t):

```bash
$ ./a.out
6204
```
what??

that has to do with struct padding. Basically the CPU enjoys having values stored in even, preferrably power-of-two positions in memory so that it can perform it's operations really fast. Computers, especially in the hardware world, love predictability and alignement

Let's walkk through how C would lay out our struct:
    1. First it lays out the 4096 bytes of memory, so from byte 0 to byte 4095. The next avaliable byte is 4096, which is even
    2. Then it lays out bytes from 4096 to 6145, these are the bytes for the display display_buffer
    3. It lays down from 6145 to 6159 for the registers 
    4. to 6161 for I 
    5. then 2 for dt and st, so until 6163 
    6. two for pc, so to 6165 
    7. one for sp, so it fills up untill 6166 
    8. The next available position in 6167, but that's an odd address, so the compiler adds ony bye of padding, filling memory up to byte 6149 
    9. Then the compiler continues lating ut meory normally, and because there isn't another instance where the next available byte falls in an odd byte it does not add any more padding bytes 

[source](https://blog.trailofbits.com/2024/05/16/understanding-addresssanitizer-better-memory-safety-for-your-code/)

ASAN (address sanitizers) works by adding many checks to your program at compile time that scream at you at run time if your program goofs up something memory related, such as accesses previously freed memory (use after free), forgets to deallocate memory (memory leak) or if it access memory out of bounds, which is our case

The downside is that it makes the program bigger and slower, so you usually don't build production software with it, and use it for validation and debugging only. 

to check for oob access ASan adds stack canaries, basically regions of memory that surround your program's own and that it can use to identify illegal access 

ASan placed it's stack canaries surrounding our struct, but not in the middle of it, so it only screamed at us when we tried to index memory at 6197, which by itself is not enough to overfloww the struct, but the instruction it was executing was D7 DF, so it was trying to draw a 15 column sprite, so it looped 6 times before trying to access address 6205 and ASan crashed out.

A fix wouuld be to simply check for oob acces whenever we index into the memory directly: 


Example unsafe pattern
```C 
static inline void i_drw_vx_vy_n(chip8_cpu_t *cpu, uint8_t x, uint8_t y,
                                 uint8_t n) {
    uint8_t start_x = cpu->registers[x] % DISPLAY_WIDTH;
    uint8_t start_y = cpu->registers[y] % DISPLAY_HEIGHT;

    cpu->registers[0xF] = 0;

    for (int row = 0; row < n; row++) {
        uint8_t sprite_byte = cpu->memory[cpu->I + row];

        for (int col = 0; col < 8; col++) {
            uint8_t sprite_pixel = (sprite_byte >> (7 - col)) & 1;

            if (sprite_pixel == 0) {
                continue;
            }

            uint16_t current_x = start_x + col;
            uint16_t current_y = start_y + row;

            if (current_x >= DISPLAY_WIDTH || current_y >= DISPLAY_HEIGHT) {
                continue;
            }

            uint16_t buff_index = (current_y * DISPLAY_WIDTH) + current_x;

            if (cpu->display_buffer[buff_index] == 1) {
                cpu->registers[0xF] = 1;
            }

            cpu->display_buffer[buff_index] ^= 1;
        }
    }
}

```

Example safe pattern 
```C

```

now the program crashes and does not let the memory run free 

```bash
DEBUG: PC:022B | OP:122D | JP 0x22D
DEBUG: PC:022D | OP:D018 | DRW V0, V1, 8
DEBUG: PC:022F | OP:7108 | ADD V1, 0x08
DEBUG: PC:0231 | OP:F21E | ADD I, V2
chip_emulator_fuzzer: chip8.c:352: void i_drw_vx_vy_n(chip8_cpu_t *, uint8_t, uint8_t, uint8_t): Assertion `(cpu->I + row) < MAX_ROM_SIZE' failed.
```
