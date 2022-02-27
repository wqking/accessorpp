# Class Accessor reference

## Description

Accessor is used to get and set value, it's a combination of Getter and Setter.  
Usually you should use Accessor instead of Getter or Setter.

## Header

accessorpp/accessor.h

## Template parameters

```c++
template <
    typename Type,
    typename PoliciesType = DefaultPolicies
>
class Accessor :
```
`Type`:  the underlying value type.  
`PoliciesType`: the policies.  

## Policies

accessorpp uses policy based design to configure and extend Accessor behavior. The last template parameter in Accessor is the policies class. Accessor has default policies class named `DefaultPolicies`.  
A policy is a type in the policies class. All policies must be public visible, so `struct` is commonly used to define the policies class.  
All policies are optional. If any policy is omitted, the default value is used.  In fact `DefaultPolicies` is just an empty struct. 

### Policy Storage

The policy `Storage` determine how the underlying data is stored. It can have two kinds of types,
`accessorpp::InternalStorage`: store the data in the Accessor. This is the default type.  
`accessorpp::ExternalStorage': the Accessor doesn't hold the data, how the data is accessed depending on the getter and setter in the Accessor.  
InternalStorage and ExternalStorage defines different constructors and member functions in Accessor. You may treat them as different Accessor classes.

Example code,  
```c++
struct MyPolicies
{
    using Storage = accessorpp::InternalStorage;
};
accessorpp::Accessor<int, MyPolicies> accessor;
```

```c++
struct MyPolicies
{
    using Storage = accessorpp::ExternalStorage;
};
int value = 0;
accessorpp::Accessor<int, MyPolicies> accessor(&value, &value);
```

### Policy OnChangingCallback and OnChangedCallback  

OnChangingCallback specifies the event handler type that's called before the underlying value is changed. OnChangedCallback specifies the event handler type that's called before the underlying value is changed.  
Ideally the `CallbackList` in [my eventpp library](https://github.com/wqking/eventpp) is perfect for such callback, otherwise `std::function` can be used.  

The callback can have three kinds of prototype, accessorpp will invoke the proper prototype automatically.  
```
std::function<void ()>; // or eventpp::CallbackList<void ()>
std::function<void (const Type & newValue)>; // or eventpp::CallbackList<void (const Type & newValue)>
std::function<void (const Type & newValue, CallbackData data)>; // or eventpp::CallbackList<void (const Type & newValue), CallbackData data>
```

The callback type must be default constructible. To add a handler to the accessor, call `void onChanging()` for OnChangingCallback and `void onChanged()` for OnChangedCallback.  

The default callback type is `void`, that doesn't trigger any events.

Example code,  
```c++
// Use std::function
struct MyPolicies
{
    using OnChangedCallback = std::function<void (std::string)>;
};
accessorpp::Accessor<std::string, MyPolicies> accessor;
accessor.onChanged() = [](const std::string & newValue) {
    std::cout << "New value is " << newValue << std::endl;
};
accessor = "Hello";
```

```c++
// Use eventpp::CallbackList
struct MyPolicies
{
    using OnChangedCallback = eventpp::CallbackList<void (std::string)>;
};
accessorpp::Accessor<std::string, MyPolicies> accessor;
accessor.onChanged().append([](const std::string & newValue) {
    std::cout << "New value is " << newValue << std::endl;
});
accessor = "Hello";
```

### Policy CallbackData  


## Constructors for InternalStorage

### Default constructor  

```c++
Accessor(const Type & newValue = Type()) noexcept;
```

Default constructor.  
`newValue` is the initial value.  

### Construct from getter and setter
```c++
template <typename G, typename S>
Accessor(G && getter, S && setter, const ValueType & newValue = ValueType()) noexcept;
```

The argument `getter` and `setter` are used to construct underlying `accessorpp::Getter` and `accessorpp::Setter`.  
`newValue` is the initial value.  
`getter` can be any type that's accepted by `accessor::Getter`.  
`setter` can be any type that's accepted by `accessor::Setter`.  
`getter` can be `accessor::DefaultGetter`, means the default getter is used.  
`setter` can be `accessor::DefaultSetter`, means the default setter is used.  
`setter` can be `accessor::NoSetter`, means there no setter used, so the accessor is read only. Setting to an accessor which setter is `NoSetter` will throw exception.  

Note: the getter and setter should call `accessor.directGet` and `accessor.directSet` to access the internal value in the accessor. It's possible that the getter and setter gets and sets external value, then the internal storage of the value is wasted.  

Example code,  
```c++
accessorpp::Accessor<int> accessor(
    [&accessor]() {
        std::cout << "Getting value" << std::endl;
        return accessor.directGet();
    },
    [&accessor](const int value) {
        std::cout << "Setting value " << value << std::endl;
        accessor.directSet(value);
    }
);
const int value = accessor;
accessor = value + 1;
```

### Construct from class members  
```c++
template <typename G, typename IG, typename S, typename IS>
Accessor(
        G && getter, IG && getterInstance,
        S && setter, IS && setterInstance,
        const ValueType & newValue = ValueType()
    ) noexcept;
```

`newValue` is the initial value.  
`getter` and `getterInstance` are used to construct underlying `accessorpp::Getter`.  
`setter` and `setterInstance` are used to construct underlying `accessorpp::Setter`.  

Example code,  
```c++
struct MyClass
{
    void setValue(const int newValue) {
        std::cout << "Setting value " << newValue << std::endl;
        value = newValue;
    }
    int value;
};
MyClass instance { 5 };
accessorpp::Accessor<int> accessor(
    &MyClass::value, &instance, 
    &MyClass::setValue, &instance
);
accessor = instance.value + 1;
```

Above constructor equals to,  
```c++
accessorpp::Accessor<int> accessor(
    accessorpp::Getter<int>(&MyClass::value, &instance),
    accessorpp::Setter<int>(&MyClass::setValue, &instance)
);
```

### Copy constructor  
```c++
Accessor(const Accessor & other) noexcept;
```

Copy constructor.

### Move constructor  
```c++
Accessor(Accessor && other) noexcept;
```

Move constructor.

## Member functions for InternalStorage

```c++
const Type & directGet() const;
Type & directGet();
```

Get the internal value directly. This is used to implement customized getter.

```c++
void directSet(const Type & newValue);
```

Set the internal value directly. This is used to implement customized setter.  
`directSet` doesn't respect read-only accessor, so it can set the value in a read-only accessor.  
`directSet` doesn't trigger any onChanging/onChanged events.  

```c++
constexpr bool isReadOnly() const;
```

Return true if the accessor is read-only. Setting to a read-only accessor will throw `std::logic_error`.  

## Constructors and member functions for ExternalStorage

```c++
Accessor() noexcept;
template <typename G, typename S>
Accessor(G && getter, S && setter) noexcept;
template <typename G, typename IG, typename S, typename IS>
Accessor(
        G && getter, IG && getterInstance,
        S && setter, IS && setterInstance
    ) noexcept;
Accessor(const Accessor & other) noexcept;
Accessor(Accessor && other) noexcept;
constexpr bool isReadOnly() const;
```

The difference between ExternalStorage and InternalStorage is, constructors for ExternalStorage don't have the argument for initial value(`newValue`).  ExternalStorage doesn't have functions `directGet` and `directSet`.  
