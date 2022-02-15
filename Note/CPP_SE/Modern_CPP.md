# 一. Previous

## 1. 应该被摒弃的特性

+ 不再允许字符串字面值常量赋值给一个`char*`，应该使用`const char*` or `auto`。
+ C++98异常说明、unexpected_handler、set_unexpected()应被启用，使用`noexcept`。
+ `auto_ptr`被弃用，应使用`unique_ptr`。
+ register关键词被弃用，可以使用但不再具备任何实际含义。
+  bool类型的++操作被弃用。
+ 如一个类有析构函数，为其生成的拷贝构造函数和拷贝赋值运算符的特性被弃用了。
+ C语言风格的类型转换被弃用，使用`static_cast`,`reinterpret_cast`,`const_cast`
+ 弃用了C的一些标准库。
+ C++11bind function export特性被弃用。
+ ...(待补充)



# 二. 语言可用性强化

## 1. 常量

**nullptr**

nullptr出现为了替代NULL



**constexpr**

constr显示的声明函数 或对象构造函数在编译期成为常量表达式。

而从C++14开始，constr函数可以在内部使用局部变量、循环和分支等简单语句。

## 2. 变量及其初始化

**if/switch变量声明强化**

if/switch可以讲临时变量放入语句内



**初始化列表**

对象构造上，也可做为普通函数的形参。

```c++
void foo(std::initializer_list<int> list) {
	//...
}
```



**结构化绑定**

结构化绑定类似于多返回值的功能。

```c++
std::tuple<int, double, std::string> foo() {
	//...
}

auto [x, y, z] = foo();
```



## 3. 类型推导

**auto**

参见modern effective cpp



**decltype**

decltype关键字是解决auto关键字只能对**变量**进行类型推导的缺陷。

参见modern effective cpp



**尾返回类型推导**

利用auto关键字将返回类型后置。

`decltype(auto)`

主要用于转发函数或封装的返回类型进行推导，无需考虑decltype的参数表达式。



## 4. 控制流

**if constexpr**

C++17将constexpr关键字引入到if语句中，允许代码中的声明常量的表达式的判断条件。

```c++
template<typename T>
auto print_type_info(const T& t) {
	if constexpr(std::is_integral<T>::value) {
		return t + 1;
	} else {
		return t + 0.1;
	}
}
```



**区间for迭代**

C++11引入基于范围的迭代写法，类似python一样简介的循环语句。



## 5. 模板

模板的哲学在于将一切能在编译期处理的问题丢到编译期进行处理，仅在运行时处理那些最核心的动态业务，进而大幅优化运行期性能。

**外部模板**

传统C++中，模板只有在**使用时**才会被编译器实例化。换句话说，只要在每个编译单元中编译的代码中遇到了完整定义的模板，都会被实例化，就产生了重复实例化而导致的编译时间的增加。

为此，C++11引入了外部模板，扩充了原来的**强制**编译器在特定位置实例化模板的语法，使得显式地通知编译器何时进行模板的实例化：

```c++
template class std::vector<bool>; //强制实例化
extern template class std::vector<double>; //不在该当前编译文件中实例化模板
```



**尖括号”>“**



**类型别名模板**

**模板是用来产生类型的**。因此typedef没有办法为模板定义一个新的名称，原因是模板不是类型。

```c++
template<typename T, typename U>
class MagicType {
public:
	T dark;
	U magic;
};

template<typename T>
typedef MagicType<std::vector<T>, std::string> FakeDarkMagic; //error

using TrueDarkMagic = MagicType<std::vector<T>, std::string>; //true

```

**变长参数模板**

```c++
template<typename... Ts>
class Magic;
```

对参数模板进行解包。

**1. 递归模板函数**

```c++
template<typename T0>
void printf1(T0 value) {
	//...
}

template<typename T, typename... Ts>
void printf1(T value, Ts... args) {
	//...
	printf1(args...);
}
```

**2. 变参模板展开 C++17**

```c++
template<typename T0, typename... T>
void printf2(T0 t0, T... t) {
	//...
	if constexpr(sizeof...(t) > 0)
		printf2(t...);
}
```

**3. 初始化列表展开**

```c++
template<typename T, typename... Ts>
auto printf3(T value, Ts... args) {
	//...
	(void) std::initializer_list<T>{([&args] {
		cout << args << endl;
	}(), value)...};
}
```



**折叠表达式**

```c++
template<typename... T>
auto sum(T... t) {
	return (t + ...);
}
```



**非类型模板参数推导**

可以让不同的字面值量成为模板参数，即为非类型模板参数。

```c++
template<typename T, int BufSize>
class Buffer_t {
public:
	T& alloc();
	void free(T& item);
private:
	T data[BufSize];
};
```

C++17引入了类型推导，通过使用占用符auto不再需要明确指明类型。

```c++
template<auto value>
void foo() {
	//...
}
```



## 6. 面向对象

**委托构造**

C++11引入了委托构造的概念，使得构造函数可以在同一个类中一个构造函数调用另一个构造函数。

```c++
class Base {
public:
	int value1;
	int value2;
	Base() {
		value1 = 1;
	}
	Base(int value) : Base() { //委托Base()构造函数
		value2 = value;
	}
};
```



**继承构造**

C++11利用using引入了继承构造函数。

```C++
class Base {
public:
	int value1;
	int value2;
	Base() {
		value1 = 1;
	}
	Base(int value) : Base() { //委托Base()构造函数
		value2 = value;
	}
};

class Subclass : public Base {
public:
	using Base::Base; //继承构造
};
```



**显式虚函数重载**

**override**

```c++
struct Base {
	virtual void foo(int);
};

struct SubClass : Base {
	virutal void foo(int) override;
};
```

**final**

```c++
struct Base {
	virtual void foo() final;
};

struct SubClass1 final : Base {
};
```



**显式禁用默认函数**

显式的声明 采用或拒绝编译器自带的函数。



**强制型枚举**

C++11引入了枚举类，使用enum class语法进行声明。

+ 类型安全，不能被隐式转换为整数；
+ 不能和整数数字比较，不可以对不同枚举类型的枚举值比较

枚举类型后面使用冒号及关键字来指定枚举中枚举值的类型 。

在获得枚举值的时，需要**显式地进行类型转换**。

```c++
template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value,
						 std::ostream>::type& stream, const T& e) {
	return stream << static_case<typename std::underlying_type<T>::type>(e);
}
```



# 三、语言运行期的强化

## 1. Lambda表达式

类似匿名函数的特性。

语法：

```C++
[捕获列表](参数列表) mutable(可选) 异常属性 -> 返回类型 {
	// 函数体
}
```

**1. 值捕获、引用捕获、隐式捕获**

**2. 表达式捕获**

C++14允许了右值捕获，被声明的捕获变量类型会根据表达式进行判断，判断方式与使用auto本质上相同。

```c++
void lambda_expression_capture() {
	auto important = std::make_unique<int>(1);
	auto add = [v1 = 1, v2 = std::move(important)](int x, int y) -> int {
		return x + y + v1 + *v2;
	};
}
//important是独占指针，不能被=捕获到
```



**泛型lambda**

C++14 lambda函数的形式参数可以使用auto关键字来产生意义上的泛型

```c++
auto add = [](auto x, auto y) {
	return x + y;
}
```



## 2. 函数对象包装器

**std::function**

lambda表达式的本质是一个和函数对象类型相似的类类型（闭包类型）的对象。当lambda表达式捕获列表为空时，闭包对象还能够转换为函数指针值进行传递。

```c++
using foo = void(int);
void functional(foo f) {
	f(1);
}

auto f = [](int value) {
	//...
}

functional(f);
//or
f(1);
```

C++11统一了概念，将能够被调用的对象的类型，统称为可调用类型。

`std::function`是一种通用的、多态的函数封装。它的实例可以对任何可调用目标进行存储、复制和调用操作。类型安全（函数的容器）。

```c++
int foo(int para) {
	return para;
}
std::functional<int(int)> func = foo;
```



**std::bind & std::placeholder**

`std::bind`用来绑定函数调用的参数的，解决的需求是有的时候可能不一定能够一次性获得调用某个函数的全部参数，通过这个函数，可以**将部分参数提前绑定到函数上成为新的对象**。

```c++
int foo(int a, int b, int c) {

}

auto bindFoo = std::find(foo, std::placeholders::_1, 1, 2);
```



## 3. 右值引用

**左值、右值的纯右值、将亡值、右值**

纯右值：纯粹的右值，要么字面值，要么是求值结果相当与字面值或匿名临时对象。

**note：字符串只有在类中 才是右值，而在普通函数中是左值**。

将亡值是C++11为了引入右值引用而提出的概念，也就是即将被销毁、却能被移动的值。



**右值引用和左值引用**

要拿到一个将亡值，就需要用到右值引用。  

```c++
string lv1 = "string";
std::string&& rv1 = std::move(lv1);

const std::string& lv2 = lv1 + lv1;
std::string&& rv2 = lv1 + lv2; //rv2是一个左值
```

+ 不允许非常量引用绑定到非左值；
+ 常量引用可以绑定非左值



**移动语义**

```c++
class A {
public:
	int *p;
	A() : pointer(new int(1)) {}
	A(A& a) : pointer(new int(*a.p)) {}
	A(A&& a) : pointer(a.pointer) {
		a.pointer = nullptr;
	}
	~A() { delete p; }
};

A return_rvalue(bool test) {
	A a, b;
	if (test)
		return a; //static_cast<A&&>(a);
	else 
		return b; //static_cast<A&&>(b);
}

int main() {
    A obj = return_rvalue(false);
}
```

1. 在return_rvalue中构造两个A对象，两个构造函数的输出；
2. 函数返回后，产生一个将亡值，被A的移动构造引用，从而延长了生命周期，并将这个右值中的指针拿到，保存在obj中，而将亡值的指针被设置为nullptr。



```c++
std::vector<std::string> v;
std::string str = "string";

v.push_back(str); //push_back(const T&)
v.push_back(std::move(str)); //push_back(const T&&)
```

  

**完美转发**

一个声明的**右值引用其实是一个左值**。 

引用坍缩（引用折叠）

| 函数形参类型 | 实参参数类型 | 推导后函数形参类型 |
| :----------: | :----------: | :----------------: |
|      T&      |    左引用    |         T&         |
|      T&      |    右引用    |         T&         |
|     T&&      |    左引用    |         T&         |
|     T&&      |    右引用    |        T&&         |

完美转发：在传递参数的时候，保持原来的参数类型，即左引用保持左引用，右引用保持右引用。

```c++
void reference(int& v) {
	cout << "left value" << endl;
}

void reference(int&& v) {
	cout << "right value" << endl;
}

template<typename T>
void pass(T&& v) {
	reference(v);
}

int main() {
	pass(1); //left value
	int l = 1;
	pass(l); //left value
	return 0;
}
```



```c++
template<typename T>
void pass(T&& v) {
	reference(v);
	reference(std::move(v));
	reference(std::forward<T>(v));
	reference(static_cast<T&&>(v));
}

pass(1);
/*
left value
right value
right value
right value
*/

int l = 1;
pass(l);
/*
left value
right value
left value
left value
*/
```

+ 传递的参数无论是左值还是右值，普通传参都会将参数作为左值转发，对于`std::move`总会接受到左值，从而转发调用了reference(int&&);
+ `std::forward`完美转发了函数的实参给内部调用的其他函数，和`std::move`一样，不做任何事情，move将左值转化为右值，forward将参数做一个类型转发



# 四、容器









