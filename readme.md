# accessorpp -- C++ library for data binding and property

accessorpp is a C++ library for implementing property and data binding.

## Facts and features

- **Powerful**
    - Support arbitrary data type as property or data binding.
	- Support event dispatching on changing data.
    - Configurable using policies.
- **Robust**
    - Well tested. Backed by unit tests.
- **Flexible and easy to use**
    - Header only, no source file, no need to build. Does not depend on other libraries.
    - Requires C++ 11.
    - Written in portable and standard C++, no hacks or quirks.

## License

Apache License, Version 2.0  

## Version 0.1.0
![CI](https://github.com/wqking/accessorpp/workflows/CI/badge.svg)

The master branch is usable and stable.  

## Source code

[https://github.com/wqking/accessorpp](https://github.com/wqking/accessorpp)

## Supported compilers

Tested with MSVC 2019, MinGW (Msys) GCC 7.2, Ubuntu GCC 5.4.
In brief, MSVC, GCC, Clang that has well support for C++11, or released after 2019, should be able to compile the library.

## C++ standard requirements
* To Use the library  
    * The library: C++11.  
* To develop the library
    * Unit tests: C++11.
	* Tutorials: C++11.
	* Benchmakrs: C++11.

## Quick start

### Namespace

`accessorpp`

### Use accessorpp in your project

accessorpp is header only library. Just clone the source code, then add the 'include' folder inside accessorpp to your project, then you can use the library.  
You don't need to link to any source code.  

