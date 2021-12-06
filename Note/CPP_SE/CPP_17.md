# CPP 17

## chapter1 基本语言特性

### **结构化绑定**

```c++
struct MyStruct {
	int i = 0;
	std::string s;
};

auto [u, v] = ms;
//auto [u, v](ms);
```

变量u和v的声明方式为结构化绑定。

```c++
//遍历std::map<>

for (const auto& [key, val] : my_map) {
	//...
}
```



## Chapter2 带初始化的if和switch语句

**带初始化if** （适用编译期if）

```c++
if (std::ofstream strm = getLogStrm(); coll.empty()) {
	strm << "no data\n";
} else {
	for (const auto& elem : coll) {
		strm << elem << "\n";
	}
}
//strm destruct
```



cool 

```c++
std::map<string, int> coll
for (auto [pos, ok] = coll.insert({"new", 42}); !ok) {
	const auto& [key, val] = *pos;
	//...
}
```



**带初始化switch**

```c++
namespace fs = std::filesystem;

switch (fs::path p{name}; status(p).type()) {
	case fs::file_type::not_found:
		//...
}
```



## Chapter3 内联变量

```c++
class MyClass {
	inline static std::string msg("OK");
	//...
}

inline MyClass myGlobalObj;
```

+ C++不允许在类内初始化非常量静态成员；可以在类外外部定义并初始化非常量静态成员，但是被多个CPP文件同时包含又会引发新的错误

一次定义原则（ODR）：一个变量或实体的定义只能出现在一个编译单元内-除非该变量或实体被定义为inline.

使用inline修饰之后，即使被多个CPP文件包含，也只会存在一个全局对象。

### **内联变量与thread_local**

通过使用thread_local可以为每个线程创建一个内联变量：

```C++
struct ThreadData {
	inline static thread_local std::string name; //every thread has its own name
};

inline thread_local std::vector<std::string> cache; //every thread has one cache
```



## Chapter4 聚合体扩展

###  C++17开始聚合体可以拥有基类



## Chapter5 强调省略拷贝或传递未实质化的对象

+ 当以值传递或返回一个临时对象的时候必须省略该改临时对象的拷贝
+ 效果：传递一个未实质化的对象



移动构造函数只有在没有**用户声明的拷贝构造函数**（或者赋值操作符或析构函数）是才会隐式存在。

但是C++17临时变量初始化对象省略拷贝变成**强制性**的。



C++11起值类型体系

lvalue（左值），prvalue（纯右值），xvalue（将亡值）

lvalue：

+ 含有单个变量、函数或成员的表达式
+ 只含有字符串字面量的表达式
+ ...

rvalue：

+ 处字符串字面量和用户自定义字面量之外的字面量组成的表达式
+ 一个返回值的函数的返回值
+ 一个lambda

xvalue：

+ rvalue -> type&& special for std::move



C++17起值类型体系

广义上将，只有两种类型的表达式：

+ gvalue: 描述函数或对象的表达式
+ prvalue：用于初始化的表达式



## Chapter6 Lambda表达式扩展

###  constexpr lambda

lambda表达式会尽可能的隐式声明constexpr，只是用有效的编译期上下文（字面量、没有静态量、没有虚函数，没有try/catch，没有new/delete的lambda都可以被用于编译期。

```c++
auto squared = [](auto val) constexpr { //constexpr为确定是否能用于编译期
	return val * val;
};
std::array<int, squared(5)> arr;
```

也即会被转化为如下闭包类型（closure type）：

```c++
class CompileSpecificName　{
public:
	//...
	template<typename T>
	constexpr auto operator()(T val) const {
		return val * val;
	}
};
```

**这里自动生成的闭包类型的函数调用运算符被自动声明为constexpr**



**使用constexpr lambda**

```c++
auto hashed = [](const char *str) {
	std::size_t hash = 5381;
	while(*str != '\0') {
		hash = hash * 33 ^ *str++;
	}
	return hash;
}
```

使用这个lambda，可以在编译期初始化不同字符串的哈希值，并定义为枚举：

```c++
enum head {beer = hashed("beer"),
		   wine = hashed("wine")};
```

或者在编译期计算case标签

**向lambda传递this指针**

C++11 & 14可以通过值或引用捕获this，不管是拷贝还是引用本质上获得的都是引用。问题就是当**lambda的生命周期比对象的生命周期更长时**，会导致问题。

C++14

```c++
auto l1 = [thisCopy = *this] {//...}
```

C++17

```c++
auto l1 = [*this] {//...}
```

捕获this将意味着该lambda生成的闭包将存储当前对象的一份拷贝。

### 以常量引用捕获



## Chapter7 新属性和属性特性

#### [[nodiscard]]

鼓励编译器在某个函数的返回值未被使用时给出警告。防止某些返回值未被使用导致的不当行为：

+ 内存泄露
+ 未知或出乎意料的行为
+ 不必要的开销



#### [[maybe_unused]]

避免编译器在某个变量未使用时发出警告



#### [[fallthrough]]

避免编译器在switch语句中某一个标签缺少break语句发出警告



### 通用的属性扩展

弃用一个命名空间：

```c++
namespace [[deprecated]] DraftAPI {
	//...
}
```



## Chapter8 语言特性

### 嵌套的命名空间

```C++
namespace A::B::C {
//...
}
//->
namespace A {
	namespace B {
		namespace C {
			//...
		}
	}
}
```

#### 

### 有定义的表达式求值顺序

运算：

+ e1[e2]
+ e1.e2
+ e1.*e2
+ e1->*e2
+ e1 << e2
+ e1 >> d2

e1一定在e2之前求值

+ e2 = e1
+ e2 += e1
+ e2 *= e1

e1一定在e2前求值



### 更宽松的用整型初始化枚举值的规则

```c++
enum MyInt : char {};
MyInt i1{42};
```



### 修正auto类型的列表初始化

```c++
//C++11
auto a{42} //std::initializer_list<int>

//C++17
auto a{42} //int

auto c = {42} //std::initializer_list<int>
```



### 十六进制浮点数字面量

### UTF-8字符字面量

#### 

### 异常声明作为类型的一部分

```c++
void foo();
void foo() noexcept; //不同类型
```



## Chapter9 模板特性 类模板参数推导

在C++17之前，必须明确指出类模板的所有参数；但是在C++17之后，可以通过类模板参数推导

```c++
std::complex c{5.1, 3,3};
std::mutex mtx;
std::lock_guard lg{mtx};

std::vector v1{1, 2, 3};
```



### 使用类模板参数推导

```c++
std::complex c1{1.1, 2.2}; //std::complex<double>
```



#### 推导lambda的类型

```c++
template<typename CB>
class CountCalls {
private:
	CB callback;
	long calls = 0;
public:
	CountCall(Cb cb) : callback(cb) {}
	template<typename... Args>
	decltype(auto) operator()(Args&&... args) {
		++calls;
		return callback(std::forward<Args>(args)...);
	}
	long count() const {
		return calls;
	}
};
```

```c++
CountCalls sc{[](auto x, auto y) { return x > y;}};
```

**没有类模板部分参数推导**



#### 使用类模板参数推导代替快捷函数

```c++
vector<int> v;
auto p = std::make_pair(v.begin(), v.end());

//->
std::pair p{v.begin(), v.end()};
```



### 推导指引



## Chapter10 编译器if

```c++
if constexpr(...)
```

编译器可以计算编译期的条件表达式来在编译期决定一个使用if语句的then还是else部分。其余代码将被**丢弃**，甚至不会生成。

```c++
template<typename T>
std::string asString(T x) {
	if constr (std::is_same_v<T, std::string>) {
		return x; //如果T不能自动转换为string该语句无效
	} else if constr (std::is_arithmetic_v(T)) {
		return std::to_string(x);
	} else {
		return std::to_string(x);
	}
}
```

模板在实例化时整个模块作为一个整体编译。然而if语句的条件表达式的检查是运行时的特性。**即使在编译器就能确定表达式的值一定是false，then的部分也必须能通过编译**。

使用编译期if语句时，then和else不可能用到的部分将成为**丢弃的语句**。



## Chapter11 折叠表达式

refer to CPP template 2nd

...

## Chapter12 处理字符串字面量模板参数

refer to CPP template 2nd

...

## Chapter13 占位符类型作为模板参数

refer to CPP template 2nd

...

## Chapter14 扩展using声明

refer to CPP template 2nd

...