# 泛型编程

> 摘录自C++模板 第二版

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

模板参数可以是常规数值，不一定非得是某种具体类型。

区别在于：非类型模板参数，待定的不是类型，而是某个数值。

### 1. 类模板的非类型参数

```c++
template<typename T, std::size_t Maxsize>
class Stack {
public:
	Stack();
	//...
};

template<typename T, std::size_t Maxsize>
Stack<T, Maxsize>::Stack() {}
//...

//use demo
Stack<int, 20> int20Stack;
```

由此可见，第二个参数Maxsize是int类型。



### 2. 函数模板的非类型参数

```c++
template<int Val, typename T>
T addValue(T x) {
	return x + Val;
}

//case:
std::transform(source.begin(), source.end(), dest.begin(), addValue<5, int>);
```

```
template<typename Val, typenamt T = decltype(Val)>
T foo();
```



### 3. 非类型模板参数的限制

约束：

+ 整形变量（包括枚举）
+ 指向objects/functions/members的指针
+ objects或者functions的左值引用
+ std::nullptr_t (类型：nullptr)



**浮点数值或者class类型变量都不能作为非类型模板参数使用**。

**当传递对象的指针或引用作为模板参数时，对象不能是字符串常量，临时变量或它的成员及其子对象**



C++11中，对象必须有**外部链接**

C++14中，对象必须是**外部链接**或**内部链接**



```c++
template<const char *name>
class MyClass {
	//...
};

MyClass<"Hello"> x; //error, literal "hello" is not allowed

extern const char s03[] = "hi"; //external linkage
char const char s11[] = "hi"; //internal linkage

//case
MyClass<s03> m03;
MyClass<s11> m11;
static const char s17[] = "hi";
MyClass<s17> m17;
```



**非类型模板参数可以是任何编译期表达式**：

```
template<int I, bool B>
class C;

c<sizeof(int) + 4, sizeof(int) == 4> c;
```



### 4. 使用auto作为非模板类型参数的类型

C++17开始，可以不指定非类型模板参数的具体类型，而使用auto。

通过这一特性，可以更为**泛化**固定大小的Stack类。

```
template<typename T, auto Maxsize>
class Stack {
	//...
};

//auto size() const {
	//...
}


```



从C++14，也可以使用auto，让编译期**推断出具体的返回类型**。

```c++
#include <iostream>
#include <string>
#include <array>
using namespace std;

template<typename T, auto Maxsize>
class Stack {
public:
	using size_type = decltype(Maxsize);
public:
	Stack();
	void push(const T& elem);
	void pop();
	const T& top() const;
	bool empty() const {
		return num_elements == 0;
	}
	size_type size() const {
		return num_elements;
	}
private:
	std::array<T, Maxsize> elems;
	size_type num_elements = 0;
};

template<typename T, auto Maxsize>
Stack<T, Maxsize>::Stack() : num_elements(0) { }

template<typename T, auto Maxsize>
void Stack<T, Maxsize>::push(const T& elem) {
	elems[num_elements] = elem;
	++num_elements;
}

template<typename T, auto Maxsize>
void Stack<T, Maxsize>::pop() {
	--num_elements;
}

template<typename T, auto Maxsize>
const T& Stack<T, Maxsize>::top() const {
	return elems[num_elements - 1];
}

int main() {

	Stack<int, 20u> int20Stack;
	Stack<string, 7> stringStack;
	int20Stack.push(7);
	stringStack.push("hello");
	auto sz1 = int20Stack.size();
	auto sz2 = stringStack.size();
	if (!std::is_same_v<decltype(sz1), decltype(sz2)>) {
		cout << "size type differ" << endl;
	}
	cout << int20Stack.top() << endl;
	return 0;
}
```



## 四、变参模板（variadic template）

C++11始，可以接受有一组数量可变的参数。

变参模板在标准库中的作用：

+ 向一个由智能指针管理的，在堆中创建的对象的构造函数传递参数：

```c++
auto sp = std::make_shard<std::complex<float>>(1.1, 2.2);
```

+ 向一个由库启动的thread传递参数：

```c++
std::thread(foo, 42, "hello");
```

+ 向一个被push进vector中的对象的构造函数传递参数：

```c++
std::vector<Customer> v;
//...
v.emplace("Tim", "Jovi", 1962);
```



### 1. 变参模板

```C++
void print() {}

template<typename T, typename... Types>
void print(T first_arg, Types... args) {
	cout << first_arg << '\n';
	print(args...);
}
```



**sizeof... 运算符**

>  参数包所包含的参数数目

注意下述代码：

```c++
template<typename T, typename... Types>
void print(T first_arg, Types... args) {
	cout << first_arg << endl;
	if (sizeof...(args) > 0) { //error if sizeof...(args) == 0
		print(args...);
	}
}
```

通常函数模板中if语句的两个分支都会被实例化出。是否使用被实例化出的代码时在运行期决定的；而是否实例化代码是在编译期间决定的。

但是自C++17始，可以使用编译期if语句。



### 2. 折叠表达式

C++17始，提供了一种计算参数包中所有参数计算结果的二元运算符

```c++
template<typename... T>
auto foldSum(T... s) {
	return (... + s);
}
```



```c++
struct Node {
	Node() : val(0), left(nullptr), right(nullptr) {}
	int val;
	Node *left;
	Node *right;
};

auto left = &Node::left;
auto right = &Node::right;

template<typename T, typename... TP>
Node *traverse(T np, TP... paths) {
	return (np->* ... ->* paths);
}
```



Tips: **再次强调一下，若参数按照值传递，那么参数被拷贝，类型也会退化；而按照引用传递的，参数类型不会退化**。



### 3. 变参类模板和变参表达式

**变参表达式**

```c++
template<typename T>
void printDoubled(const T&... args) {
	print(args + args...);
}
```

将所有的参数都翻倍。



```c++
template<typename T1, typename... TN>
constexpr bool isHomogeneous(T1, TN...) {
	return (std::is_same<T1, TN>::value && ...);
}
```

判断参数包中的所有参数类型是否相同。



**变参下标**

```c++
template<typename C, typename... Idx>
void printElems(const C& coll, Idx... idx) {
	print(coll[idx]...);
}
```

按照一组变参下标来访问一个参数中的相应元素。



**变参类模板**

+ 多个模板参数指定class相应数据成员的类型

```c++
template<typename... Elements>class Tuple;
Tuple<int, std:string, char> t;
```

+ 定义一组下标类型（参考tuple get函数）

```c++
template<std::size_t...>
struct Indices {};

template<typename T, std::size_t... Idx>
void printByIdx(T t, Indices<Idx...>) {
	print(std::get<Idx>(t)...);
}
```



**变参推断指引**

```c++
namespace std {
	template<typename T, typename... U> array(T, U...)
	-> array<enable_if_t<(is_same_v<T, U>&& ...), T>, (1 + sizeof...(U))>;
}

//enable_if_t<> ->
is_same_v<T, U1> && is_same_v<T, U2> && is_same_v<T, U3> ...
```



**变参基类及其使用**

...



## 五、基础技巧

### 1. typename关键字







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

