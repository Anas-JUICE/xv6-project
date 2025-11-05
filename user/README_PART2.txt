Instructions to install files into your repo (Part-2 branch):

1. From your repo root (on branch Part-2), copy the kernel and user files into place.
   Example (adjust paths if needed):

   cp kernel/*.c kernel/*.h /path/to/your/repo/kernel/
   cp user/*.c user/*.h /path/to/your/repo/user/

2. Edit files that require small manual patches:
   - kernel/syscall.h: add SYS_clone and SYS_join with unused syscall numbers.
   - kernel/syscall.c: add extern declarations for sys_clone/sys_join and add table entries.
   - kernel/defs.h: add prototypes for clone and join.
   - user/usys.S or usys.pl: add wrappers for clone and join so user programs can call them.
   - Makefile: add uthreadtest to UPROGS so it is built (append _uthreadtest to UPROGS list).

3. Build:
   make clean
   make qemu

4. Inside xv6:
   $ uthreadtest

Note: This package is targeted at xv6-public (x86). If you encounter build errors about undefined symbols (e.g., nextpid, copyout, sleep), you will need to integrate the clone/join code inside your existing proc.c where those symbols are defined, or adapt the helper calls to your kernel's existing functions.
