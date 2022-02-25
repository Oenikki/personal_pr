# FAQs of CPP

1. operator T&() or operator T()

第一种方式需要做一次拷贝，而第二次存在做不做拷贝的选择。

operator两种用法：

+ operator overloading

```c++
template<typename>
class Foo {
public:
	const T operator+(const T& rhs) {
		return this->m_ + rhs;
	}
private:
	T m_;
};
```

+ operator casting

```c++
class B;//...
class Bar {
public:
	opeartor B*() { return this->b_; }
	operator const B*() { return this->b_; }
	operator B&() { return *this->b_; }
private:
	B *b_;
};

Bar bar;
if (bar) {} // if (bar.operator B*())
```

2. 

