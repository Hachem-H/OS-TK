# OS-TK
The Operating System Tool Kit is a platform to turn any CLI C/C++ application into a x64 operating system. Designed originally to create an OS called [Cummer-OS](https://github.com/Hachem-H/Cummer-OS) as joke for one of my friends's birthday, now expanded into a full blown architecture of toy kernel development. It recreates a good chunk of libc/libcpp, which works along a kernel backend. Its quite a modern take, using an `UEFI` bootloader, and `docker` to create a simple development environment to work in. This project is marked as a template, so all you have to do is create an OS-TK project is hit the "Use Template" button.

**NOTE 1: Not all features advertised are yet implemented.**
**NOTE 2: Old repository can be found [here](https://github.com/Hachem-H/OS-TK.bak)**

## Project structure
```sh
.
├── buildenv           # Dockerfile and scripts
├── include/commmon    # Common files between the bootloader and the kernel
├── kernel             # Kernel backend
│   ├── fs-efi         # Files needed for EFI boot
│   ├── src
│   ├── linker.ld      # Linker script and data layout
│   └── Makefile
├── vendor             # External code.
│   ├── OVMF           # OVMF binary for vms
│   └── gnu-efi        # GNU-EFI source
│       ├── [...]
│       ├── bootloader # Custom UEFI bootloader
│       └── [..]
├── lib                # Library reimplementations
│   ├── libc               # C Standard Library
│   └── libcpp             # C++ Standard Library
├── src                # Your custom OS goes here
├── Makefile
├── LICENSE
└── README.md
```

## Example
```c
// The application should run the exact same.
// The only difference being is the main function:
//			int main() -> void kmain()
// Of course, there is no (argc, argv).

#include <stdio.h>

void kmain()
{
    puts("Hello, World!");
    puts("Look ma, no kernel!");
}
```

## Possible future features
- Interpreted language runtimes, to allow simpler languages (like `Lua` and `Python`) to be used to develop custom OS-TK kernels. The implementation would probably involve a bootstrap, loading all the sources into the filesystem, then executing a kernel which then runs the language runtime/interpreter.
- Hardware features and drivers:
    - Possible networking
    - Possible audio
- Installer to burn a kernel onto a HDD/SDD directly instead of booting on a USB.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change. Please make sure to update tests as appropriate.
