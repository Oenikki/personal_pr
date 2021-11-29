# CPP 17

## chapter1

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



## Chapter2

### 带初始化的if和switch语句

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



## Chapter3

### 内联变量

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







