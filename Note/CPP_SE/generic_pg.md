# 泛型编程

按照习惯，函数和类的声明应放在文件中，实现放在一个主代码文件. 但是模板是随用随生成，并不存在真正函数实现代码。

**明确生成模板实例：**当关键词后没有模板参数列表，而是一个函数声明时，指示编译器根据此函数声明寻找合适的模板实现。

## 基础知识

+ 关注函数模板参数自动推导
+ 关注类模板参数自动推导（C++17）

函数模板推导的需要注意的点：

+ **只能**根据函数的调用的实参列表来推导模板参数值
+ 与函数返回值无关的模板参数其值也无法推导
+ 所有可推导模板参数必须是连续位于模板参数列表尾部



...

## 一、函数模板

函数模板有两阶段编译检查

1. 模板定义阶段，检查部分内容例如语法、类型名，函数名
2. 模板实例化阶段，检查依赖于类型参数的部分

### 1. 模板参数推断

类型推断中的类型转换：

+ 参数为**引用**，无法进行转换
+ 参数按**值**传递，decay简单转换可以接受
  + 诸如const int -> int
  + int& -> int
  + arr[4] -> int* (int arr[4])
  + ...

**返回类型推断**

C++14可以由编译器推断返回类型，无需尾置返回类型decltype 

```c++
template<typename T1, template T2>
auto max(T1 a, T2 b) {
	return b < a ? a : b;
}

//before C++14
template<typename T1, template T2>
auto max(T1 a, T2 b) -> decltype(true ? a : b);

//但是存在一个问题，如果T是引用类型，返回类型就会被推断为引用
template<typename T1, template T2>
auto max(T1 a, T2 b) -> typename std::decay<decltype(true ? a : b)>::type {
	return b < a ? a : b;
}
//这里需要用类型萃取出目标类型
```

**将返回类型声明为公共类型**

```
template<typename T1, template T2>
std::common_type_t<T1, T2> max(T1 a, T2 b) { //C++14
	return b < a ? a : b;
}
```

什么叫做**公共类型**呢？对于某些类型的特化来决定目标类型。

比如::max(2, 2.1)和::max(2.1, 2)都是返回double类型。



### 2. 默认模板参数

```c++
template<typename T1, typename T2, typename RT = 
std::decay_t<decltype(true ? T1() : T2())>>
RT max(T1 a, T2 b) {
	return b < a ? a : b;
}
//also the same with common type
```



### 3. 函数模板的重载

```c++
int max(int a, int b) {
	return b < a ? a : b;
}

template<typename T>
T max(T a, T b) {
	reutn b < a ? a : b;
}
```

一个非模板函数可以和一个同名函数共存，并且这个同名函数可以实例化出与非模板函数同类型的调用参数。

**优先选择更匹配的；同条件，优先选择非模板参数**。

```c++
::max<>(7, 42); //显示使用模板调用
::max('a', 42,7) //模板推断不允许自动类型推导
```



## 二、类模板

Concept：用来约束模板类型的语法糖。

C++11简单实用关键字static_assert和预定义的类型萃取做一些简单的检查：

```c++
template<typename T>
class C {
	static_assert(std::is_default_constructible<T>::value, "//...");
};
```



### 1. 模板特例化

可以对类模板的某一个模板参数进行例化。和函数模板的重载类似。

为了特化一个模板，在类模板前需有一个template<>

```c++
template<>
class Stack<std::string> {
	//...
};
```

**被特化的模板，所有成员函数的定义都应该被定义成“常规”成员函数，即出现T的地方，都应该替换成特化类模板的类型**。



**部分特例化**

比如可以特殊化一个Stack<>专门处理指针

```c++
template<typename T>
class Stack<T*> {
	//...
};
```



**多模板参数的部分特例化**

```c++
template<typename T1, typename T2>
class MyClass {
	//...
};

template<typename T>
class MyClass<T, T> {
    //...
};

template<typename T>
class MyClass<T, int> {
    //...
};
```



**默认类模板参数**

和函数模板一样，类模板的模板参数可以指定默认值。

```c++
template<typenae T, typename Cont = std::vector<T>>
class Stack {
	//...
};
```



### 2. 类型别名

通过给类模板定义个新的名字，便于使用。

**Typedefs和Alias声明**

```c++
typedef Stack<int> IntStack;
IntStack istack[10];
//or
using IntStack = Stack<int>
```

这样一个过程alias declaration，只是一个别名，没有定义新的变量。



**Alias Template**

补充：

+ class template
+ function template
+ variable template
+ alias template

```c++
template<typaname T>
using DequeStack = Stack<T, std::deque<T>>;
```

从C++14开始，给标准库中所有返回一个类型的type trait定义个快捷方式

```c++
std::add_const_t<T>
//typename std::add_const<T>::type

namespace std {
	template<typename T>
	using add_const_t = typename add_cosnt<T>::type;
}
```

这样一个特性是alias template发挥作用。



### 3. 类模板的类型推导

C++17后，不必再严格显示的之处所有的模板参数的类型。

+ 因为接受了int类型的构造函数，要向编译器要求生成默认构造函数及其全部默认行为；

**推断指引**

存在一个问题，当参数是按照T的引用传递时，参数类型不会被**decay**。

因此，当把引用修改为值传递时，参数会被decay。

可以通过 **推断指引**提供模板参数推断规则，或者**修正**以后的模板参数推断规则。

```c++
Stack(const char*) -> Stack<std::string>;
```

**指引语句必须出现和模板类定义相同的作用域或者命名空间内**。



### 4. 聚合类的初始化

C++17开始可以对聚合类模板使用类型推断指引

```c++
template<typename T>
struct ValueWithComment {
	T value;
	std::string commmet;
}

//C++17
ValueWithCommmet(const char*, const char*) -> ValueWithComment<std::string>;
ValueWithComment vc2 = {"hello", "world"};
//若是没有推断指引的话，就不能使用上述初始化方法，因为没有相应的构造函数来完成类型推断。
```



## 三、非类型模板参数





## 类模板

+ 在一个类模板内出现的自身模板名，等价于该模板被调用所生成的实例。
+ 如果类模板参数有默认值，可以在模板实参列表中省略，但是**尖括号**无法省略。

简要介绍几个类模板的用法：

### 成员函数模板

> 类中的成员函数模板可以在类中当场实现，也可以在类外单独实现。当然，类模板的成员函数可以有额外的模板参数。

成员函数模板的参数不能与类模板参数同名。

在类外实现函数的时候需要使用**两个template**。



### 友元函数/类模板

任何友元实体都可以访问任意私有资源。



### 类模板的静态成员

类模板静态成员变量的实现与类模板实现放在同一可见范围内。

