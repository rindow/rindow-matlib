The Rindow Matrix library for C
===============================
Rindow Math Matrix is the fundamental package for scientific matrix operation

- A powerful N-dimensional array object
- Sophisticated (broadcasting) functions
- A high-speed calculation library written in C/C++.
- Useful linear algebra and random number capabilities
- Parallel calculation with native multi-threading or OpenMP

You can call a high-speed calculation library written in C language to speed up matrix calculation processing.
Rindow Matlib includes many matrix operations functions used in machine learning.

Please see the documents about rindow mathematics 
on [Rindow Mathematics](https://rindow.github.io/mathematics/openblas/mathlibrary.html) web pages.

Requirements
============

- Windows(10/11) , Linux(Ubuntu 22.04-, Debian 12-), MacOS(13/14) or later

How to setup pre-build binaries
===============================

### How to setup for Windows

Download the pre-build binary file.

- https://github.com/rindow/rindow-matlib/releases

Unzip the file for Windows and copy rindowmatlib.dll to the directory set in PATH.

The standard DLLs in the Bin directory is the thread version. Subdirectories contain OpenMP and Serial versions.

```shell
C> PATH %PATH%;C:\path\to\bin
```

### How to setup for Linux

Download the pre-build binary file.

- https://github.com/rindow/rindow-matlib/releases

Please install using the apt command. 
```shell
$ sudo apt install ./rindow-matlib_X.X.X_amd64.deb
```

### Troubleshooting for Linux
Since rindow-matlib currently uses ptheads, so you should choose the pthread version for OpenBLAS as well.
In version 1.0 of Rindow-matlib we recommended the OpenMP version, but now we have changed our policy and are recommending the pthread version.

Using the OpenMP version of OpenBLAS can cause conflicts and become unstable and slow.
This issue does not occur on Windows.

If you have already installed the OpenMP version of OpenBLAS, you can delete it and install pthread version.
```shell
$ sudo apt install libopenblas0-pthread liblapacke
$ sudo apt remove libopenblas0-openmp
```

But if you can't remove it, you can switch to it using the update-alternatives command.

```shell
$ sudo update-alternatives --config libopenblas.so.0-x86_64-linux-gnu
$ sudo update-alternatives --config liblapack.so.3-x86_64-linux-gnu
```

If you really want to use the OpenMP version of OpenBLAS, please switch to the serial version of rindow-matlib.

But, If you really want to use the pthread version of OpenBLAS, please switch to the serial version of rindow-matlib.

```shell
$ sudo update-alternatives --config librindowmatlib.so
There are 2 choices for the alternative librindowmatlib.so (providing /usr/lib/librindowmatlib.so).

  Selection    Path                                             Priority   Status
------------------------------------------------------------
* 0            /usr/lib/rindowmatlib-openmp/librindowmatlib.so   95        auto mode
  1            /usr/lib/rindowmatlib-openmp/librindowmatlib.so   95        manual mode
  2            /usr/lib/rindowmatlib-serial/librindowmatlib.so   90        manual mode
  3            /usr/lib/rindowmatlib-thread/librindowmatlib.so   100       manual mode

Press <enter> to keep the current choice[*], or type selection number: 2
```
Choose the "rindowmatlib-serial".

### How to setup for MacOS

Download the pre-build binary file.

- https://github.com/rindow/rindow-matlib/releases

Extract the tar.gz file to the tmp directory and navigate to the directory.

```shell
$ tar -xzf rindow-matlib-X.X.X-Darwin.tar.gz -C /tmp
$ cd /tmp/rindow-matlib-X.X.X-Darwin
```

Next, copy the include and lib directories to /usr/local.

```shell
$ sudo cp -r usr/include /usr/local/
$ sudo cp -r usr/lib /usr/local/
$ brew install libomp
```

How to build from source code on Windows
========================================
You can also build and use from source code.

### Download source code

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

How to build from source code on Linux
=======================================
You can also build and use from source code.

### Download source code

Download source code from release and extract

- https://github.com/rindow/rindow-matlib/releases

### Build and Install on Linux

Build with cmake.

```shell
$ sudo apt install build-essential cmake
$ cd \path\to\here
$ cmake -S . -B build
$ cmake --build build --config Release
$ (cd build; ctest -C Release)
$ (cd build; cpack -C Release)
```

Install with apt command.

```shell
$ sudo apt install ./packages/rindow-matlib_X.X.X_amd64.deb
```

How to build from source code on MacOS
=======================================

You can also build and use from source code.

### Download source code

Download source code from release and extract

- https://github.com/rindow/rindow-matlib/releases

### Build and Install on MacOS

Install openmp library with brew before build with cmake.

```shell
$ brew install libomp
$ cd \path\to\here
$ cmake -S . -B build
$ cmake --build build --config Release
$ (cd build; ctest -C Release)
$ (cd build; cpack -C Release)
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

### Build the sample program on Windows.

```shell
C> cl /EHsc -I.\path\to\include sample.cpp \path\to\lib\rindowmatlib.lib
```

### Build the sample program on Linux.

```shell
$ g++ sample.cpp -lrindowmatlib -lm
```

### Build the sample program on MacOS.

```shell
$ c++ sample.cpp -lrindowmatlib -lm
```

