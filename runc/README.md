# runc 🚀

## Introduction 
`runc` 🚀 simplifies the process of running C programs, similar to how you would run Python programs. It leverages compiles the C source code and then executes the program seamlessly.

## Features
- **Simple Execution**: Run C programs directly from the command line with a single command.
- **Automated Compilation**: Automatically compiles C source files using `gcc` before execution.
- **User-Friendly**: Mimics the straightforwardness of running scripts in interpreted languages like Python.

## Getting Started
### CLI
#### 1. Install required dependencies
Make sure `gcc` is installed on your system. You can install `gcc` using the following commands based on your operating system:
- **Ubuntu/Debian**:
    ```bash
    sudo apt-get install gcc
    ```
- **macOS**:
    ```bash
    brew install gcc
    ```
#### 2. Get the binary
- **Ubuntu/Debian**: [runc_linux](./runc_linux)

- **macOS**: [runc_macos](./runc_macosx)

#### 3. Run a C Program
```bash
./runc program.c
```
Note: You can also put the binaries into /usr/local/bin to run it directly
```bash
runc program.c
```

## Built With
<a href="https://en.cppreference.com/w/c" target="_blank" rel="noreferrer">
    <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/c/c-original.svg" alt="c" width="40" height="40"/>
</a>
<a href="https://gcc.gnu.org/" target="_blank" rel="noreferrer">
    <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/gcc/gcc-original.svg" alt="gcc" width="40" height="40"/>
</a>



## TODO
- [ ] Add support for windows

## License
This project is licensed under the [MIT License](LICENSE.md).