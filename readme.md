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

## Source code

[https://github.com/wqking/accessorpp](https://github.com/wqking/accessorpp)

## Supported compilers

Tested with MSVC 2022, MSVC 2019, MinGW (Msys) GCC 7.2, Ubuntu GCC 5.4.
In brief, MSVC, GCC, Clang that has well support for C++11, or released after 2019, should be able to compile the library.

## Quick start

### Namespace

`accessorpp`

### Use accessorpp in your project

accessorpp is header only library. Just clone the source code, then add the 'include' folder inside accessorpp to your project, then you can use the library.  
You don't need to link to any source code.  

### Using Accessor

Simple usage

```c++
#include "accessorpp/accessor.h"
accessorpp::Accessor<int> accessor;
// output 0
std::cout << (int)accessor << std::endl;
accessor = 5
// output 5
std::cout << (int)accessor << std::endl;
```

Customized getter/setter

```c++
accessorpp::Accessor<int> accessor(
    // This is the getter
    [&accessor]() {
        return accessor.directGet();
    },
    // This is the setter, it limits the value not exceeding 5.
    [&accessor](int value) {
        if(value > 5) {
            value = 5;
        }
        accessor.directSet(value);  
    }
);
accessor = 3;
// output 3
std::cout << (int)accessor << std::endl;
accessor = 6;
// output 5
std::cout << (int)accessor << std::endl;
```

## Documentations

* [Accessor](doc/accessor.md)
* [Getter](doc/getter.md)
* [Setter](doc/setter.md)

## Motivations

Back to more than 10 years ago, in my (wqking) another monster library cpgf, I added accessor as a sub library, which serves for and binds to cpgf. Now this is a new independent library, with beautiful C++11 syntax, concise and easy to use.
