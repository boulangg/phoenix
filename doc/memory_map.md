# Memory Mapping

## Physical Mapping

The kernel follows the following physical mapping at boot up:

<pre>
MAX_MEM ->  | -------------------- |
            |      Free Memory     |
? ->        | -------------------- |
            |  Kernel Page Tables  |
? ->        | -------------------- |
            |      Kernel Data     |
? ->        | -------------------- |
            |      Kernel Code     |
16 Mo ->    | -------------------- |
            |      Free Memory     |
? ->        | -------------------- |
            |    Bootstrap code    |
1 Mo ->     | -------------------- |
            |       Bios Area      |
0 Mo ->     | -------------------- |
</pre>

* __Kernel Page Tables__: Early page table that is reuse to form the kernel
page table.
* __Kernel (Code & Data)__: Kernel minus the bootstrap part, that can only be
executed in 64 bits mode.
* __Bootstrap Code__: Kernel code that initialize the 64 bit long-mode (from
  the protected mode initialized by GRUB) and prepare the kernel page table.
* __BIOS AREA__: Area initialized with BIOS code. Also contain Video memory for
Console output.

The kernel follows the following virtual mapping:

The upper half of the virtual memory address space is reserved for the kernel.
The lower half will be used by user applications.

<pre>
-1 octect ->            | -------------------- |
                        | Kernel (code & data) |
-1 Go ->                | -------------------- |
                        |                      |
kernel_ist1_top ->      | -------------------- |
                        |    Main Interrupt    |
                        |        Stack         |
kernel_ist1_bottom ->   | -------------------- |
                        |                      |
kernel_stack_top ->     | -------------------- |
                        |  Main Kernel Stack   |
kernel_stack_bottom ->  | -------------------- |
                        |                      |
kernel_heap_end ->      | -------------------- |
                        |    Kernel Dynamic    |
                        |    Allocation Area   |
kernel_heap_start ->    | -------------------- |
                        |                      |
-128 To + MAX_MEM ->    | -------------------- |
                        | Full Linear Mapping  |
-128 To ->              | -------------------- |
                        |                      |
                        |  Non-canonical Addr  |
                        |    (unavailable)     |
                        |                      |
128 To ->               | -------------------- |
                        |      User Space      |
0 Mo ->                 | -------------------- |
</pre>

* __Kernel (Code & Data)__: The kernel is mapped and executed on the last Go
of the virtual address space.
* __Main Interrupt Stack__: Stack used by interrupt vector.
* __Main Kernel Stack__: Stack used internally by the kernel.
* __Kernel Dynamic Allocation Area__: Address Space reserved for malloc/new
inside the kernel.
* __Non-canonical Addr__: Address Space unavailable due to hardware constraints.
* __User Space__: Address space dedicated to user applications.
