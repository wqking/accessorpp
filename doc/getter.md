# Class Getter reference

## Description

Getter is used to read value.  
Usually you should use Accessor instead of Getter.

## Header

accessorpp/getter.h

## Template parameters

```c++
template <typename Type>
class Getter;
```
`Type`:  the underlying value type.  

## Constructors

### Default constructor  
```c++
Getter() noexcept;
```

Default constructor. Note: if the `Type` is a reference, the default constructor won't compile.

### Construct from data address  
```c++
template <typename U>
explicit Getter(const U * address) noexcept;
```

When reading from the getter, the getter gets value from `address` and cast the value to `Type`.  
Example code,  
```c++
int value = 3;
accessorpp::Getter<int> getter(&value);
// output 3
std::cout << (int)getter << std::endl;
value = 5;
// output 5
std::cout << (int)getter << std::endl;
```

### Construct from class member address  
```c++
template <typename U, typename C>
Getter(const U C::* address, const C * instance) noexcept;
```

When reading from the getter, the getter gets value from member field `instance->*address` and cast the value to `Type`.
Example code,  
```c++
struct MyClass
{
    int value;
};
MyClass instance{ 8 };
accessorpp::Getter<int> getter(&MyClass::value, &instance);
// output 8
std::cout << (int)getter << std::endl;
instance.value = 9;
// output 9
std::cout << (int)getter << std::endl;
```

### Construct from class member address, pass instance explicitly  
```c++
template <typename U>
Getter(const U C::* address) noexcept;
```

To read the value, call `Getter::get(instance)` with the object instance.    
Example code,  
```c++
struct MyClass
{
    int value;
};
MyClass instance{ 8 };
accessorpp::Getter<int> getter(&MyClass::value);
// output 8, can't use (int)getter here
std::cout << getter.get(&instance) << std::endl;
instance.value = 9;
// output 9
std::cout << getter.get(&instance) << std::endl;
```

### Construct from constant data  
```c++
Getter(const Type & value) noexcept;
```

When reading from the getter, the getter always returns 'value', that's to say, the getter returns constant 'value'.
Example code,  
```c++
int n = 9;
// note n is passed by value;
accessorpp::Getter<int> getter(n);
// output 9
std::cout << (int)getter << std::endl;
n = 8;
// still output 9
std::cout << (int)getter << std::endl;
```

### Construct from function  
```c++
template <typename F>
explicit Getter(F func) noexcept;
```

'func' is a callable, such as function pointer, std::function, function object, or lambda. The prototype is `Type ()`.  
When reading from the getter, the getter returns `func()`.
Example code,  
```c++
int n = 9;
accessorpp::Getter<int> getter([&n]() -> int { return n++;  });
// output 9
std::cout << (int)getter << std::endl;
// output 10
std::cout << (int)getter << std::endl;
```

### Construct from class member function  
```c++
template <typename F, typename C>
explicit Getter(F func, C * instance) noexcept;
```

`func` is a member function which prototype is `Type ()`, `instance` is the object pointer.  
When reading from the getter, the getter returns `instance->*func()`.
Example code,  
```c++
struct MyClass
{
    int getValue() const {
        return value;
    }
    int value;
};
MyClass instance{ 18 };
accessorpp::Getter<int> getter(&MyClass::getValue, &instance);
// output 18
std::cout << (int)getter << std::endl;
instance.value = 19;
// output 19
std::cout << (int)getter << std::endl;
```

### Construct from class member function, pass instance explicitly  
```c++
template <typename F>
explicit Getter(F func) noexcept;
```

`func` is a member function which prototype is `Type ()`.  
To read the value, call `Getter::get(instance)` with the object instance.    
Example code,  
```c++
struct MyClass
{
    int getValue() const {
        return value;
    }
    int value;
};
MyClass instance{ 18 };
accessorpp::Getter<int> getter(&MyClass::getValue);
// output 18
std::cout << getter.get(&instance) << std::endl;
instance.value = 19;
// output 19
std::cout << getter.get(&instance) << std::endl;
```

### Copy constructor  
```c++
Getter(const Getter & other) noexcept;
```

Copy constructor.

### Move constructor  
```c++
Getter(Getter && other) noexcept;
```

Move constructor.

## Member functions

```c++
Type get(const void * instance = nullptr) const;
operator Type() const;
```

Both returns the underlying value.  
If the getter is a class member and instance is not passed in constructor, the `instance` in the `get` function must be a valid object instance. Otherwise, `instance` can be nullptr.

```c++
template <typename Type>
std::ostream & operator << (std::ostream & stream, const Getter<Type> & getter);
```

Overloaded output stream operator.
