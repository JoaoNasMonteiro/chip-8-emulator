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

```
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
DEBUG: PC:022D | OP:D018 | DRW V0, V1, 8
DEBUG: PC:022F | OP:7108 | ADD V1, 0x08
DEBUG: PC:0231 | OP:F21E | ADD I, V2
DEBUG: PC:0233 | OP:D7DF | DRW V7, VD, F
DEBUG: PC:0235 | OP:EDD2 | invalid operation
DEBUG: PC:0237 | OP:8FF7 | SUBN VF, VF
DEBUG: PC:023B | OP:9EFF | SNE VE, VF
DEBUG: PC:022B | OP:122D | JP 0x22D
DEBUG: PC:022D | OP:D018 | DRW V0, V1, 8
DEBUG: PC:022F | OP:7108 | ADD V1, 0x08
DEBUG: PC:0231 | OP:F21E | ADD I, V2
DEBUG: PC:0233 | OP:D7DF | DRW V7, VD, F
DEBUG: PC:0235 | OP:EDD2 | invalid operation
DEBUG: PC:0237 | OP:8FF7 | SUBN VF, VF
DEBUG: PC:023B | OP:9EFF | SNE VE, VF
DEBUG: PC:022B | OP:122D | JP 0x22D
```


the PC is odd because of the first jump instruction

Do note that the value of I at the time of the crash is 6197 (0x1835), which is way bigger than what any single instruction can set it (Annn can only set it to a max of 4095), and the large number is what makes this code crash. Let's explore the execution flow of the program and try to understand how it works

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

(basically it jumps to an odd address, making the pc out of allignement. it keeps incrementing I and eventually dxyn tries toa ccess a value oob. The cool thing is actualyl the jump to an odd address that makes the bytes out of alignement rather than the oob read itself)



```hex
00000000: 1225 5350 4143 4520 494E 5641 4445 5253  .%SPACE INVADERS
00000010: 2030 2E39 3120 4279 2044 6176 6964 2057   0.91 By David W
00000020: 494E 5445 5260 0061 0062 08C2 A3C3 9DC3  INTER`.a.b......
00000030: 9018 7108 C3B2 1EC3 97C3 9FC3 ADC3 92C2  ..q.............
00000040: 8FC3 B7C2 9EC3 BF30 4012 2D69 056C 156E  .......0@.-i.l.n
00000050: 0023 C291 600A C3B0 15C3 B007 3000 124B  .#..`.......0..K
00000060: 23C2 917E 0112 4566 0068 1C69 006A 046B  #..~..Ef.h.i.j.k
00000070: 0A6C 046D 3C6E 0F00 C3A0 2375 2351 C3BD  .l.m<n....#u#Q..
00000080: 1560 04C3 A0C2 9E12 7D23 7538 0078 C3BF  .`......}#u8.x..
00000090: 2375 6006 C3A0 C29E 12C2 8B23 7538 3978  #u`........#u89x
000000a0: 0123 7536 0012 C29F 6005 C3A0 C29E 12C3  .#u6....`.......
000000b0: A966 0165 1BC2 84C2 80C2 A3C3 99C3 9451  .f.e...........Q
000000c0: C2A3 C399 C394 5175 C3BF 35C3 BF12 C2AD  ......Qu..5.....
000000d0: 6600 12C3 A9C3 9451 3F01 12C3 A9C3 9451  f......Q?......Q
000000e0: 6600 C283 4073 03C2 83C2 B562 C3B8 C283  f...@s.....b....
000000f0: 2262 0833 0012 C389 237D C282 0643 0812  "b.3....#}...C..
00000100: C393 3310 12C3 9523 7DC2 8206 3318 12C3  ..3....#}...3...
00000110: 9D23 7DC2 8206 4320 12C3 A733 2812 C3A9  .#}...C ...3(...
00000120: 237D 3E00 1307 7906 4918 6900 6A04 6B0A  #}>...y.I.i.j.k.
00000130: 6C04 7DC3 B46E 0F00 C3A0 2351 2375 C3BD  l.}..n....#Q#u..
00000140: 1512 6FC3 B707 3700 126F C3BD 1523 51C2  ..o...7..o...#Q.
00000150: 8BC2 A43B 1213 1B7C 026A C3BC 3B02 1323  ...;...|.j..;..#
00000160: 7C02 6A04 2351 3C18 126F 00C3 A0C2 A4C3  |.j.#Q<..o......
00000170: 9D60 1461 0862 0FC3 901F 7008 C3B2 1E30  .`.a.b....p....0
00000180: 2C13 3360 C3BF C3B0 15C3 B007 3000 1341  ,.3`........0..A
00000190: C3B0 0A00 C3A0 C2A7 06C3 BE65 1225 C2A3  ...........e.%..
000001a0: C381 C3B9 1E61 0823 69C2 8106 2369 C281  .....a.#i...#i..
000001b0: 0623 69C2 8106 2369 7BC3 9000 C3AE C280  .#i...#i{.......
000001c0: C3A0 C280 1230 00C3 9BC3 867B 0C00 C3AE  .....0.....{....
000001d0: C2A3 C399 601C C398 0400 C3AE 2351 C28E  ....`.......#Q..
000001e0: 2323 5160 05C3 B018 C3B0 15C3 B007 3000  ##Q`..........0.
000001f0: 13C2 8900 C3AE 6A00 C28D C3A0 6B04 C3A9  ......j.....k...
00000200: C2A1 1257 C2A6 0CC3 BD1E C3B0 6530 C3BF  ...W........e0..
00000210: 13C2 AF6A 006B 046D 016E 0113 C297 C2A5  ...j.k.m.n......
00000220: 0AC3 B01E C39B C386 7B08 7D01 7A01 3A07  ........{.}.z.:.
00000230: 13C2 9700 C3AE 3C7E C3BF C3BF C299 C299  ......<~........
00000240: 7EC3 BFC3 BF24 24C3 A77E C3BF 3C3C 7EC3  ~....$$..~..<<~.
00000250: 9BC2 8142 3C7E C3BF C39B 1038 7CC3 BE00  ...B<~.....8|...
00000260: 007F 003F 007F 0000 0001 0101 0303 0303  ...?............
00000270: 0000 3F20 2020 2020 2020 203F 0808 C3BF  ..?        ?....
00000280: 0000 C3BE 00C3 BC00 C3BE 0000 007E 4242  .............~BB
00000290: 6262 6262 0000 C3BF 0000 0000 0000 0000  bbbb............
000002a0: C3BF 0000 C3BF 007D 0041 7D05 7D7D 0000  .......}.A}.}}..
000002b0: C382 C382 C386 446C 2838 0000 C3BF 0000  ......Dl(8......
000002c0: 0000 0000 0000 C3BF 0000 C3BF 00C3 B710  ................
000002d0: 14C3 B7C3 B704 0400 007C 44C3 BEC3 82C3  .........|D.....
000002e0: 82C3 82C3 8200 00C3 BF00 0000 0000 0000  ................
000002f0: 00C3 BF00 00C3 BF00 C3AF 2028 C3A8 C3A8  .......... (....
00000300: 2F2F 0000 C3B9 C285 C385 C385 C385 C385  //..............
00000310: C3B9 0000 C3BF 0000 0000 0000 0000 C3BF  ................
00000320: 0000 C3BF 00C2 BE00 2030 20C2 BEC2 BE00  ........ 0 .....
00000330: 00C3 B704 C3A7 C285 C285 C284 C3B4 0000  ................
00000340: C3BF 0000 0000 0000 00C3 BF00 0000 C3BF  ................
00000350: 0000 7F00 3F00 7F00 0000 C3AF 28C3 AF00  ....?.......(...
00000360: C3A0 606F 0000 C3BF 0000 0000 0000 0000  ..`o............
00000370: C3BF 0000 C3BF 0000 C3BE 00C3 BC00 C3BE  ................
00000380: 0000 00C3 8000 C380 C380 C380 C380 C380  ................
00000390: 0000 C3BC 0404 0404 0404 0404 C3BC 1010  ................
000003a0: C3BF C3B9 C281 C2B9 C28B C29A C29A C3BA  ................
000003b0: 00C3 BAC2 8AC2 9AC2 9AC2 9BC2 99C3 B8C3  ................
000003c0: A625 25C3 B434 3434 0017 1434 3736 26C3  .%%..444...476&.
000003d0: 87C3 9F50 505C C398 C398 C39F 00C3 9F11  ...PP\..........
000003e0: 1F12 1B19 C399 7C44 C3BE C286 C286 C286  ......|D........
000003f0: C3BC C284 C3BE C282 C282 C3BE C3BE C280  ................
00000400: C380 C380 C380 C3BE C3BC C282 C382 C382  ................
00000410: C382 C3BC C3BE C280 C3B8 C380 C380 C3BE  ................
00000420: C3BE C280 C3B0 C380 C380 C380 C3BE C280  ................
00000430: C2BE C286 C286 C3BE C286 C286 C3BE C286  ................
00000440: C286 C286 1010 1010 1010 1818 1848 4878  .............HHx
00000450: C29C C290 C2B0 C380 C2B0 C29C C280 C280  ................
00000460: C380 C380 C380 C3BE C3AE C292 C292 C286  ................
00000470: C286 C286 C3BE C282 C286 C286 C286 C286  ................
00000480: 7CC2 82C2 86C2 86C2 867C C3BE C282 C3BE  |........|......
00000490: C380 C380 C380 7CC2 82C3 82C3 8AC3 847A  ......|........z
000004a0: C3BE C286 C3BE C290 C29C C284 C3BE C380  ................
000004b0: C3BE 0202 C3BE C3BE 1030 3030 30C2 82C2  .........0000...
000004c0: 82C3 82C3 82C3 82C3 BEC2 82C2 82C2 82C3  ................
000004d0: AE38 10C2 86C2 86C2 96C2 92C2 92C3 AEC2  .8..............
000004e0: 8244 3838 44C2 82C2 82C2 82C3 BE30 3030  .D88D........000
000004f0: C3BE 021E C3B0 C280 C3BE 0000 0000 0606  ................
00000500: 0000 0060 60C3 8000 0000 0000 0018 1818  ...``...........
00000510: 1800 187C C386 0C18 0018 0032 C3BE C3BE  ...|.......2....
00000520: 0000 C3BE C282 C286 C286 C286 C3BE 0808  ................
00000530: 0818 1818 C3BE 02C3 BEC3 80C3 80C3 BEC3  ................
00000540: BE02 1E06 06C3 BEC2 84C3 84C3 84C3 BE04  ................
00000550: 04C3 BEC2 80C3 BE06 06C3 BEC3 80C3 80C3  ................
00000560: 80C3 BEC2 82C3 BEC3 BE02 0206 0606 7C44  ..............|D
00000570: C3BE C286 C286 C3BE C3BE C282 C3BE 0606  ................
00000580: 0644 C3BE 4444 C3BE 44C2 A8C2 A8C2 A8C2  .D..DD..D.......
00000590: A8C2 A8C2 A8C2 A86C 5A00 0C18 C2A8 304E  .......lZ.....0N
000005a0: 7E00 1218 666C C2A8 5A66 5424 6600 4848  ~...fl..ZfT$f.HH
000005b0: 1812 C2A8 06C2 90C2 A812 007E 3012 C2A8  ...........~0...
000005c0: C284 304E 7218 66C2 A8C2 A8C2 A8C2 A8C2  ..0Nr.f.........
000005d0: A8C2 A8C2 9054 78C2 A848 786C 72C2 A812  .....Tx..Hxlr...
000005e0: 186C 7266 54C2 90C2 A872 2A18 C2A8 304E  .lrfT....r*...0N
000005f0: 7E00 1218 666C C2A8 7254 C2A8 5A66 187E  ~...fl..rT..Zf.~
00000600: 184E 72C2 A872 2A18 3066 C2A8 304E 7E00  .Nr..r*.0f..0N~.
00000610: 6C30 544E C29C C2A8 C2A8 C2A8 C2A8 C2A8  l0TN............
00000620: C2A8 C2A8 4854 7E18 C2A8 C290 5478 66C2  ....HT~.....Txf.
00000630: A86C 2A30 5AC2 A8C2 8430 722A C2A8 C398  .l*0Z....0r*....
00000640: C2A8 004E 12C2 A8C3 A4C2 A2C2 A800 4E12  ...N..........N.
00000650: C2A8 6C2A 5454 72C2 A8C2 8430 722A C2A8  ..l*TTr....0r*..
00000660: C39E C29C C2A8 722A 18C2 A80C 5448 5A78  ......r*....THZx
00000670: 7218 66C2 A866 185A 5466 726C C2A8 722A  r.f..f.ZTfrl..r*
00000680: 0072 C2A8 722A 18C2 A830 4E7E 0012 1866  .r..r*...0N~...f
00000690: 6CC2 A800 6618 C2A8 304E 0C66 1800 6C30  l...f...0N.f..l0
000006a0: 4E24 C2A8 722A 1830 66C2 A81E 5466 0C18  N$..r*.0f...Tf..
000006b0: C29C C2A8 2454 5412 C2A8 4278 0C3C C2A8  ....$TT...Bx.<..
000006c0: C2AE C2A8 C2A8 C2A8 C2A8 C2A8 C2A8 C2A8  ................
000006d0: C3BF 0000 0000 0000 0000 0000 0000 0000  ................
000006e0: 00                                       .
```
