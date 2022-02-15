# **泛型Lambda**

refer to https://en.cppreference.com/w/cpp/language/lambda & item 32 in effective modern C++ 

**泛型编程：**

将数据和方法进行高度分离，表示同类问题的最小解。（ 变量模板、函数模板、类模板）



**泛型lambda：**

Lambda：本质上就是一个匿名的函数对象，实际上也是一个“类”。

+ 唯一的方法就是operator()
+ []中捕获的参数，就是“类”中定义的成员变量

因此，lambda就具备了**函数的部分特征**（函数模板）和**类的部分特征**（类模板）。



**以继承封装变化：**

不论写库或框架，都是在提炼「不变」的逻辑，将「变化」的逻辑交给用户配置。

可以是预留接口，让用户覆写接口；也可以是采用回调，让用户提供处理逻辑；抑或是提供配置文件，让用户填写变化的信息，再通过配置文件自动生成相应处理逻辑。



**Lambda重载**

lambda具备函数模板的特征，但是仍不可以重载，它只有唯一的方法operator()。但是可以提供多个lambda，造就多个函数对象，让参数不同，可以从“视觉层面”实现lambda重载。

**overload pattern**

```c++
template<class... Ts> struct overloaded: Ts... { using Ts::operator()...; };

template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
```

+ 第一行使用了可变参数模板，是的overloaded可以继承自多个lambda。使用了using-declaration，防止重载时产生歧义；
+ 第二行使用了C++17 CTAD（ClassTemplate Argument Deduction），以推导出overloaded的类型。

```c++
//for instance
const auto func = overloaded {
	[](const int& n) { std::cout << "int: "<< n << '\n'; },\
	[](const std::string& s) { std::cout << "String: " << s << '\n'; }
};

func(2);
func("lambda with string paramter");
```

这里又使用了聚合初始化，通过它可以直接调用基类中lambda的构造器，从而避免为overloaded显示编写构造函数向基类传递参数。

