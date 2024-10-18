# OpenGL Experiment

## Introduction

This is a small experiment that I did when exploring with linear algebra projection using OpenGL

## Code Files

-   draw_v1.c: initial version that i coded up without taking care of memory management, so it is not memory safe
-   draw_v2.c: a more refined version that frees memory and make use of struct

## Getting Started

#### 1. Install required dependencies

Make sure `gcc` is installed on your system. You can install `gcc` using the following commands based on your operating system:

-   **macOS**:
    ```bash
    brew install gcc glfw
    ```

#### 2. Compile the code

-   **macOS**:
    ```bash
    gcc draw_v2.c -o draw_v2 -Wno-deprecated-declarations -lglfw -framework OpenGL
    ```

#### 3. Run the binary

-   **macOS**:
    ```bash
    ./draw_v2.c
    ```

## Built With

<a href="https://en.cppreference.com/w/c" target="_blank" rel="noreferrer">
    <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/c/c-original.svg" alt="c" width="40" height="40"/>
</a>
<a href="https://gcc.gnu.org/" target="_blank" rel="noreferrer">
    <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/gcc/gcc-original.svg" alt="gcc" width="40" height="40"/>
</a>

## License

This project is licensed under the [MIT License](LICENSE.md).
