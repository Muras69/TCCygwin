# TCCygwin

**Tiny C Compiler for the Cygwin runtime — native Windows, tiny footprint, fast builds.**

![Platform: Windows x86_64](https://img.shields.io/badge/platform-Windows%20x86__64-0078D6)
![Languages: C and Assembly](https://img.shields.io/badge/languages-C%20%2B%20Assembly-555555)
[![Runtime: Cygwin](https://img.shields.io/badge/runtime-Cygwin-008080)](https://cygwin.com/)
[![Compiler license: LGPL v2.1](https://img.shields.io/badge/compiler%20license-LGPL%20v2.1-blue)](COPYING-LGPLv21)

TCCygwin is a lightweight C compiler built as a native Windows application that produces programs using the Cygwin POSIX runtime, `cygwin1.dll`. It combines the small footprint and fast compilation of **Tiny C Compiler (TCC)** with Cygwin's POSIX capabilities for lightweight C development and compiler bootstrapping on Windows.

*A tiny compiler with POSIX superpowers.*

[Features](#features) · [Packages](#packages) · [Build](#build-from-source) · [Runtime setup](#runtime-setup) · [Documentation](#usage-and-documentation) · [License](#license)

## Features

- **C compilation** with an integrated preprocessor and assembler.
- **Cygwin POSIX API support** in the generated programs through `cygwin1.dll`.
- **Native Windows compiler** with a compact distribution: approximately **2 MB per unpacked binary package**.
- **Self-hosting builds** using the compiler to compile itself.
- **Automated bootstrapping** with one main batch script that prepares and builds all three compiler variants.

For detailed language support, options, and limitations, see the compiler documentation in [`doc/`](doc/).

## Packages

Prebuilt compiler packages are available alongside the source package, **`TCCygwin-b1b8637`**.

| Package | Purpose |
| :--- | :--- |
| `x86_64-win32-tcc` | Original native Windows TCC build, ready to use as the bootstrap compiler. |
| `TCCygwin-3.6.10-1` | Native Windows compiler targeting the Cygwin `3.6.10-1` runtime. |
| `TCCygwin-3.7.0-0.605` | Native Windows compiler targeting the Cygwin `3.7.0-0.605` runtime. |

Each unpacked binary package is approximately **2 MB**. The Cygwin runtime is obtained separately; see [Runtime setup](#runtime-setup).

## Technology and source baseline

The compiler is implemented in **C with assembly components**, based on TinyCC, and built through a self-hosting bootstrap process.

| Item | Value |
| :--- | :--- |
| Upstream project | [Tiny C Compiler](https://bellard.org/tcc/) |
| Development repository | [TinyCC on repo.or.cz](https://repo.or.cz/tinycc.git) |
| Source branch | `mob` |
| Working version | `0.9.28rc` |
| Source tree identifier | `b1b863756f3dd24380ca24bbcfdc848673e1951a` |
| Source package | `TCCygwin-b1b8637` |
| Build scripts | Windows batch (`.bat`) |

These identifiers describe the source baseline used for this package.

## Build from source

The source package includes an already-built native Windows bootstrap compiler.

1. Extract `TCCygwin-b1b8637`.
2. Open **Command Prompt** in the directory containing the build scripts.
3. Run the main build script:

   ```bat
   BUILD_TCCygwin.bat
   ```

**`BUILD_TCCygwin.bat` is the recommended entry point for the complete build.** It bootstraps `x86_64-win32-tcc` four times, then builds both TCCygwin variants.

The source package also provides scripts for building the native Windows compiler separately:

| Script | Purpose |
| :--- | :--- |
| `BUILD_TCC.bat` | Basic build of the native Windows TCC compiler. |
| `BUILD_TCC_bootstrapping.bat` | Bootstrap build of the native Windows compiler; preferred over the basic build when building that compiler separately. |
| **`BUILD_TCCygwin.bat`** | **Complete build: native compiler bootstrapping followed by both TCCygwin variants.** |

You only need to invoke the main script for the complete build; the table above is not a sequence of commands to run manually.

## Runtime setup

**TCCygwin itself runs natively on Windows. Programs compiled for its Cygwin target require `cygwin1.dll` at runtime.**

### Obtain `cygwin1.dll`

Select the runtime corresponding to your TCCygwin package:

| Runtime | Kernel.org mirror | Checkdomain.de mirror |
| :--- | :--- | :--- |
| `3.6.10-1` | [Download archive](https://mirrors.kernel.org/sourceware/cygwin/x86_64/release/cygwin/cygwin-3.6.10-1-x86_64.tar.xz) | [Download archive](https://mirror.checkdomain.de/cygwin/x86_64/release/cygwin/cygwin-3.6.10-1-x86_64.tar.xz) |
| `3.7.0-0.605` | [Download archive](https://mirrors.kernel.org/sourceware/cygwin/x86_64/release/cygwin/cygwin-3.7.0-0.605.g04f9e88ce783-x86_64.tar.xz) | [Download archive](https://mirror.checkdomain.de/cygwin/x86_64/release/cygwin/cygwin-3.7.0-0.605.g04f9e88ce783-x86_64.tar.xz) |

Additional download locations are listed on the [Cygwin mirrors page](https://cygwin.com/mirrors.html). Availability of these specific archive versions may change over time.

Open the archive with an archive manager and extract **`usr/bin/cygwin1.dll`**. This is the runtime DLL needed by the generated application; the compiler does not need it merely to run as a native Windows program.

### Arrange the application files

For a minimal application, place the generated `.exe` and `cygwin1.dll` beside each other. For a structured environment, use a dedicated `bin` directory, for example:

| Example path | Contents |
| :--- | :--- |
| `my-environment/bin/my-program.exe` | Your compiled application. |
| `my-environment/bin/cygwin1.dll` | The corresponding Cygwin runtime. |

An application may also need other libraries, configuration files, or directories according to the APIs and dependencies it uses. Placing the executable beside the DLL supplies the runtime dependency; it does not create a complete Cygwin environment.

## Usage and documentation

Compiler descriptions and usage examples are provided in **[`doc/`](doc/)**. Refer to that directory for the invocation syntax and options of the compiler variant you select.

To use a prebuilt package:

1. Extract the desired compiler package.
2. Follow the compilation examples in `doc/`.
3. Place the corresponding `cygwin1.dll` alongside your generated application, as described in [Runtime setup](#runtime-setup).
4. Run the application from Command Prompt. For an output file named `my-program.exe`:

   ```bat
   my-program.exe
   ```

## Authors and acknowledgments

- **TCCygwin:** Przemysław Korcik — project adaptation and build integration.
- **Tiny C Compiler:** [Fabrice Bellard](https://bellard.org/tcc/), original creator, and the TinyCC developers and contributors.
- **Cygwin:** [Cygwin developers and contributors](https://cygwin.com/), for the POSIX runtime and related components.

## License

The compiler is distributed under **GNU LGPL v2.1**. See [`COPYING-LGPLv21`](COPYING-LGPLv21) for the license text.

Cygwin-related components have their own applicable licensing terms. These include `crt0.c`, `INCLUDE_3610100/`, and `INCLUDE_3700605/` within the `cygwin/` directory. Consult the relevant source notices and the accompanying license files:

- `COPYING-CYGWIN`
- `COPYING-GPLv20`
- `COPYING-GPLv30`
- `COPYING-LGPLv21`
- `COPYING-LGPLv30`

The compiler license does not replace the licensing terms of the bundled Cygwin components.
