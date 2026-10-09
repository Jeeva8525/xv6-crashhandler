# lastwords(): a dying message for crashed processes (xv6-riscv)

## What it does

When a program crashes or gets killed, normally all you see is a fault or
"killed" with no idea what the program was doing.

This modification adds a new system call, `lastwords(msg)`. A program calls it
to leave a short note (up to 63 characters) describing what it is currently
doing. If the program later dies abnormally (fatal fault or `kill`), the kernel
prints the process ID, the process name, the cause, and the last note.

If a program never calls `lastwords`, nothing extra is printed.

## Example use in a program

```c
lastwords("reading config file");
// ... do the work ...
lastwords("writing config file, step 3");
```

Each call replaces the previous note, so the kernel always reports the most
recent one.

## Files changed

- kernel/proc.h      : added `lastwords[64]` field to struct proc
- kernel/syscall.h   : added SYS_lastwords
- kernel/syscall.c   : registered sys_lastwords in the syscall table
- kernel/sysproc.c   : added sys_lastwords() handler
- kernel/proc.c      : clears the note in allocproc(); prints it in kill()
- kernel/trap.c      : prints the note on a fatal fault in usertrap()
- user/usys.pl       : added the user-side stub
- user/user.h        : added the lastwords() prototype
- user/crashtest.c, user/killtest.c, user/nolastwords.c : test programs
- Makefile           : added the three test programs to UPROGS

## How to build and run

```
cd ~/xv6-riscv
make qemu
```

To exit xv6: press Ctrl+A, release, then press X.

## Tests

### Test 1: crash (fault path), crashtest

Sets a note, then writes to an invalid address.

```
$ crashtest
```

Expected:

```
usertrap(): unexpected scause 0xf pid=5
            sepc=0x... stval=0x0000000010000000
lastwords: pid 5 (crashtest) died: fault
  last words: "writing config file, step 3"
```

### Test 2: kill (kill path), killtest

Sets a note, prints its PID, then waits forever. Run it in the background,
then kill it using the PID it printed.

```
$ killtest &
killtest running, pid 6
$ kill 6
```

Expected:

```
lastwords: pid 6 (killtest) died: killed
  last words: "waiting forever in loop"
```

### Test 3: no note (negative case), nolastwords

Crashes without ever calling lastwords().

```
$ nolastwords
```

Expected: only the normal fault lines, and no "lastwords:" lines.

```
usertrap(): unexpected scause 0xf pid=7
            sepc=0x... stval=0x0000000010000000
```

Note: PID numbers will differ on each run.
