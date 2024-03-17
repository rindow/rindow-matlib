The Rindow Matrix library for C
===============================
Rindow Math Matrix is the fundamental package for scientific matrix operation

- A powerful N-dimensional array object
- Sophisticated (broadcasting) functions
- A high-speed calculation library written in C.
- Useful linear algebra and random number capabilities
- Parallel calculation with OpenMP

You can call a high-speed calculation library written in C language to speed up matrix calculation processing.
Rindow Matlib includes many matrix operations functions used in machine learning.

Please see the documents about rindow mathematics on [Rindow Mathematics](https://rindow.github.io/mathematics/openblas/mathlibrary.html) web pages.

Requirements
============
- Windows 10 or later, Linux(Ubuntu 20.04 or later)


How to setup pre-build binaries
===============================

### How to setup for Windows
Download the pre-build binary file.

- https://github.com/rindow/rindow-matlib/releases

Unzip the file for Windows and copy rindowmatlib.dll to the directory set in PATH.

```shell
C> PATH %PATH%;C:\path\to\bin
```

### How to setup for Ubuntu
Download the pre-build binary file.

- https://github.com/rindow/rindow-matlib/releases

Please install using the apt command. 

```shell
$ sudo apt install ./rindow-matlib_X.X.X_amd64.deb
```

If you use this library under the PHP, you must set it to "serial" mode.

```shell
$ sudo update-alternatives --config librindowmatlib.so
There are 2 choices for the alternative librindowmatlib.so (providing /usr/lib/librindowmatlib.so).

  Selection    Path                                             Priority   Status
------------------------------------------------------------
* 0            /usr/lib/rindowmatlib-openmp/librindowmatlib.so   95        auto mode
  1            /usr/lib/rindowmatlib-openmp/librindowmatlib.so   95        manual mode
  2            /usr/lib/rindowmatlib-serial/librindowmatlib.so   90        manual mode

Press <enter> to keep the current choice[*], or type selection number: 2
```

How to build from source code on Windows
========================================
You can also build and use from source code.

### download source code

Download source code from release and unzip

- https://github.com/rindow/rindow-matlib/releases

### Build and Install on Windows

Build with Visual Studio.

```shell
C> cd \path\to\here
C> cmake -S . -B build
C> cmake --build build --config Release
C> cd build
C> ctest -C Release
C> cpack -C Release
C> cd ..\packages
```
Unzip the package file from packages directory.

```shell
C> PATH %PATH%;C:\path\to\bin
```

How to build from source code on Ubuntu
=======================================
You can also build and use from source code.

### download source code

Download source code from release and extract

- https://github.com/rindow/rindow-matlib/releases

### Build and Install on Ubuntu

Build with cmake.

```shell
$ cd \path\to\here
$ cmake -S . -B build
$ cmake --build build --config Release
$ (cd build; ctest -C Release)
$ (cd build; cpack -C Release)
```
Install with apt commnand.

```shell
$ sudo apt install ./packages/rindow-matlib_X.X.X_amd64.deb
```

If you use this library under the PHP, you must set it to "serial" mode.

```shell
$ sudo update-alternatives --config librindowmatlib.so
There are 2 choices for the alternative librindowmatlib.so (providing /usr/lib/librindowmatlib.so).

  Selection    Path                                             Priority   Status
------------------------------------------------------------
* 0            /usr/lib/rindowmatlib-openmp/librindowmatlib.so   95        auto mode
  1            /usr/lib/rindowmatlib-openmp/librindowmatlib.so   95        manual mode
  2            /usr/lib/rindowmatlib-serial/librindowmatlib.so   90        manual mode

Press <enter> to keep the current choice[*], or type selection number: 2
```

How to use
==========

### sample program
```cpp
#include <iostream>
#include <string>
#include <iomanip>
#include <rindow/matlib.h>

void printMatrix(int m, int n, void *matrix)
{
    float *v = (float *)matrix;
    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++) {
            std::cout << std::setw(4) << v[i*n+j] << ",";
        }
        std::cout << std::endl;
    }
}
int main(int ac, char **av)
{
    const int M = 2;
    const int N = 3;
    // float
    float sX[N] = {1, 2, 3};
    float sY[M][N] = {{1,10,100}, {-1,-10,-100}};
    float alpha = 2.0;
    std::cout << "X:" << std::endl;
    printMatrix(1, N, sX);
    std::cout << std::endl << "Y:" << std::endl;
    printMatrix(M, N, sY);
    std::cout << std::endl << "alpha:" << std::endl;
    std::cout << std::setw(4) << alpha << std::endl;
    rindow_matlib_s_add(
        RINDOW_MATLIB_NO_TRANS, // int32_t trans,
        M,      // int32_t m,
        N,      // int32_t n,
        alpha,    // float alpha,
        (float *)&sX,     // float *x,
        1,      // int32_t incX,
        (float *)&sY,     // float *a,
        N       // int32_t ldA
    );
    std::cout << std::endl << "Results:" << std::endl;
    printMatrix(M, N, sY);

    std::string mode;
    switch(rindow_matlib_common_get_parallel()) {
        case RINDOW_MATLIB_SEQUENTIAL: {
            mode = "SEQUENTIAL";
            break;
        }
        case RINDOW_MATLIB_THREAD: {
            mode = "THREAD";
            break;
        }
        case RINDOW_MATLIB_OPENMP: {
            mode = "OPENMP";
            break;
        } default: {
            mode = "UNKOWN";
            break;
        }
    }
    std::cout << mode  << std::endl;
    return 0;
}
```

### build the sample program on Windows.

C> cl /EHsc -I.\path\to\include sample.cpp \path\to\lib\rindowmatlib.lib

### build the sample program on Ubuntu.

$ g++ sample.cpp -lrindowmatlib -lm
