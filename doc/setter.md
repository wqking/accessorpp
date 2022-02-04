# Class Setter reference

## Description

Setter is used to write value.  
Usually you should use Accessor instead of Setter.

## Header

accessorpp/setter.h

## Template parameters

```c++
template <typename Type>
class Setter;
```
`Type`:  the underlying value type.  

## Constructors

### Default constructor  
```c++
Setter() noexcept;
```

Default constructor. 

### Construct from data address  
```c++
template <typename U>
explicit Setter(U * address) noexcept;
```

When setting to the setter, the setter sets value to `address`.  
Example code,  
```c++
int value = 3;
accessorpp::Setter<int> setter(&value);
setter = 5;
// output 5
std::cout << value << std::endl;
setter = 8;
// output 8
std::cout << value << std::endl;
```

### Construct from class member address  
```c++
template <typename U, typename C>
Setter(const U C::* address, const C * instance) noexcept;
```

When setting to the setter, the setter sets the value to member field `instance->*address`.
Example code,  
```c++
struct MyClass
{
    int value;
};
MyClass instance;
accessorpp::Setter<int> setter(&MyClass::value, &instance);
setter = 18;
// output 18
std::cout << instance.value << std::endl;
setter = 19;
// output 19
std::cout << instance.value << std::endl;
```

### Construct from function  
```c++
template <typename F>
explicit Setter(F func) noexcept;
```

'func' is a callable, such as function pointer, std::function, function object, or lambda. The prototype is `void (Type)`.  
When setting to the setter, the setter invokes `func(value)`.
Example code,  
```c++
int n = 0;
accessorpp::Setter<int> setter([&n](const int value) { n = value;  });
setter = 9;
// output 9
std::cout << n << std::endl;
setter = 10;
// output 10
std::cout << n << std::endl;
```

### Construct from class member function  
```c++
template <typename F, typename C>
explicit Setter(F func, C * instance) noexcept;
```

`func` is a member function which prototype is `void (Type)`, `instance` is the object pointer.  
When setting to the setter, the setter invokes `instance->*func(value)`.
Example code,  
```c++
struct MyClass
{
    void setValue(const int newValue) {
        value = newValue;
    }
    int value;
};
MyClass instance;
accessorpp::Setter<int> setter(&MyClass::setValue, &instance);
setter = 15;
// output 15
std::cout << instance.value << std::endl;
setter = 16;
// output 16
std::cout << instance.value << std::endl;
```

### Copy constructor  
```c++
Setter(const Setter & other) noexcept;
```

Copy constructor.

### Move constructor  
```c++
Setter(Setter && other) noexcept;
```

Move constructor.

## Member functions

```c++
Setter & operator = (const ValueType & value);
void set(const ValueType & value);
```

Both sets the underlying value.

```c++
template <typename Type>
std::istream & operator >> (std::istream & stream, Setter<Type> & setter);
```

Overloaded input stream operator.
