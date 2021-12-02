# C++对象布局

参考公众号**[程序喵大人]**

几个问题？

+ 普通类对象的布局
+ 带虚函数的类对象的布局
+ 单继承下不含覆盖函数（区别隐藏）的类对象的布局
+ 单继承下含有覆盖对象的类对象的布局
+ 多继承下不含有覆盖函数的类对象的布局
+ 多继承下含有覆盖函数的类对象的布局
+ 多继承下不同继承顺序产生的类对象的布局
+ 虚继承的类对象的布局
+ 菱形继承下类对象的布局
+ 为什么引入虚继承
+ 为什么虚析构表中有两个析构函数
+ 为什么构造函数不能是虚函数
+ 为什么基类析构函数需要是虚函数



查看对象的布局

```c++
g++ -fdump-class-hierarchy xxx.cc
```



## 虚函数的类对象的布局

```c++
struct Base {   
	Base() = default;   
	virtual ~Base() = default;      
	void FuncA() {}
	
   	virtual void FuncB() {       
   		printf("FuncB\n");  
   	}
   	int a;   
   	int b;
};
int main() {   
	Base a;   
	return 0;
}
// 这里可以查看对象的布局和相应虚函数表的布局
clang -Xclang -fdump-record-layouts -stdlib=libc++ -c model.cc
clang -Xclang -fdump-vtable-layouts -stdlib=libc++ -c model.cc
```

```
*** Dumping AST Record Layout        
	0 | struct Base        
	0 |   (Base vtable pointer)        
	8 |   int a       
   12 |   int b          
	  | [sizeof=16, dsize=16, align=8,          
	  |  nvsize=16, nvalign=8]
*** Dumping IRgen Record Layout
```

结论：这个含有虚函数的结构体大小16。

对象**头部**：前8个字节是虚函数表的指针，指向虚函数相应函数指针地址；

a占4字节，b占4字节。



**虚函数表布局**

```
Vtable for 'Base' (5 entries).  
0 | offset_to_top (0)  
1 | Base RTTI      
  	-- (Base, 0) vtable address --  
2 | Base::~Base() [complete]  
3 | Base::~Base() [deleting]  
4 | void Base::FuncB()
```



Item：

**offset_to_top(0)**: 当前虚函数表地址距离对象顶部地址的偏移量

**RTTI指针**：指向存储运行时类型信息地址type_info的地址，用于运行时类型识别，用于typeid和dynamic_cast。



## 单继承不含覆盖函数的类对象的布局

```c++
struct Base {
	Base() = default;
	virtual ~Base() = default;
	
	void FuncA() {}
	virutal void FuncB() {
        cout << "base: funcb" << '\n';
    }
	
	int a;
	int b;
};

struct Derive : public Base {
};
```

子类对象布局：

```
*** Dumping AST Record Layout
        0 | struct Derive
        0 |   struct Base (primary base)
        0 |     (Base vtable pointer)
        8 |     int a
       12 |     int b
          | [sizeof=16, dsize=16, align=8,
          |  nvsize=16, nvalign=8]

*** Dumping IRgen Record Layout
```

这个含有虚函数的结构体大小和上面这个结构是一样的。**注意 base vptr**。

子类对象虚函数表布局：

```
Vtable for 'Derive' (5 entries).
  0 | offset_to_top (0)
  1 | Derive RTTI
      -- (Base, 0) vtable address --
      -- (Derive, 0) vtable address --
  2 | Derive::~Derive() [complete]
  3 | Derive::~Derive() [deleting]
  4 | void Base::FuncB()
```



## 单继承下含有覆盖函数的类对象的布局

```c++
//Base the same
struct Derive : public Base {
	void FuncB() {
		cout << "derive: funcb" << '\n';
	}
}
```

子类对象布局：

```c++

*** Dumping AST Record Layout
        0 | struct Derive
        0 |   struct Base (primary base)
        0 |     (Base vtable pointer)
        8 |     int a
       12 |     int b
          | [sizeof=16, dsize=16, align=8,
          |  nvsize=16, nvalign=8]

*** Dumping IRgen Record Layout
```

**base vptr** -> 虚函数表

子类对象虚函数表布局：

```
Vtable for 'Derive' (5 entries).
  0 | offset_to_top (0)
  1 | Derive RTTI
      -- (Base, 0) vtable address --
      -- (Derive, 0) vtable address --
  2 | Derive::~Derive() [complete]
  3 | Derive::~Derive() [deleting]
  4 | void Derive::FuncB()
```

可以注意到：在子类对象的虚函数表中，FuncB函数已经是Derive的FuncB了，子类重写了父类的函数。



## 多继承下不含有覆盖函数的类对象的布局

```c++

struct BaseA {
   BaseA() = default;
   virtual ~BaseA() = default;
   
   void FuncA() {}

   virtual void FuncB() {
       printf("BaseA FuncB\n");
  }

   int a;
   int b;
};

struct BaseB {
   BaseB() = default;
   virtual ~BaseB() = default;
   
   void FuncA() {}

   virtual void FuncC() {
       printf("BaseB FuncC\n");
  }

   int a;
   int b;
};

struct Derive : public BaseA, public BaseB{
};
```

子类对象布局：

```
*** Dumping AST Record Layout
        0 | struct Derive
        0 |   struct BaseA (primary base)
        0 |     (BaseA vtable pointer)
        8 |     int a
       12 |     int b
       16 |   struct BaseB (base)
       16 |     (BaseB vtable pointer)
       24 |     int a
      28 |     int b
         | [sizeof=32, dsize=32, align=8,
         |  nvsize=32, nvalign=8]
```

此时含有了两个虚表指针，一版继承了几个带有虚函数的类，对象布局中就有几个虚表指针，并且继承基类的数据。

虚函数表布局：

```
Vtable for 'Derive' (10 entries).
  0 | offset_to_top (0)
  1 | Derive RTTI
      -- (BaseA, 0) vtable address --
      -- (Derive, 0) vtable address --
  2 | Derive::~Derive() [complete]
  3 | Derive::~Derive() [deleting]
  4 | void BaseA::FuncB()
  5 | offset_to_top (-16)
  6 | Derive RTTI
      -- (BaseB, 16) vtable address --
  7 | Derive::~Derive() [complete]
      [this adjustment: -16 non-virtual]
  8 | Derive::~Derive() [deleting]
      [this adjustment: -16 non-virtual]
  9 | void BaseB::FuncC()
```

这里的**RTTI**中有了两项，表示BaseA和Derive的虚表地址是相同的，BaseA类里的虚函数和Derive类里的虚函数都在这个链条下，截至到offset_to_top(-16)之前都是BaseA和Derive的虚函数表。

后面的这个RTTI：只有一项，表示BaseB的虚函数表，后面也有两个虚析构函数



## 多继承下含有覆盖函数的类对象的布局

```c++

struct BaseA {
   BaseA() = default;
   virtual ~BaseA() = default;
   
   void FuncA() {}

   virtual void FuncB() {
       printf("BaseA FuncB\n");
  }

   int a;
   int b;
};

struct BaseB {
   BaseB() = default;
   virtual ~BaseB() = default;
   
   void FuncA() {}

   virtual void FuncC() {
       printf("BaseB FuncC\n");
  }

   int a;
   int b;
};

struct Derive : public BaseA, public BaseB{
   void FuncB() override {
       printf("Derive FuncB \n");
  }

   void FuncC() override {
       printf("Derive FuncC \n");
  }
};
```

子类对象布局：

```
*** Dumping AST Record Layout
        0 | struct Derive
        0 |   struct BaseA (primary base)
        0 |     (BaseA vtable pointer)
        8 |     int a
       12 |     int b
       16 |   struct BaseB (base)
       16 |     (BaseB vtable pointer)
       24 |     int a
       28 |     int b
          | [sizeof=32, dsize=32, align=8,
          |  nvsize=32, nvalign=8]

*** Dumping IRgen Record Layout
```

虚函数表布局：

```
Vtable for 'Derive' (11 entries).
  0 | offset_to_top (0)
  1 | Derive RTTI
      -- (BaseA, 0) vtable address --
      -- (Derive, 0) vtable address --
  2 | Derive::~Derive() [complete]
  3 | Derive::~Derive() [deleting]
  4 | void Derive::FuncB()
  5 | void Derive::FuncC()
  6 | offset_to_top (-16)
  7 | Derive RTTI
      -- (BaseB, 16) vtable address --
  8 | Derive::~Derive() [complete]
      [this adjustment: -16 non-virtual]
  9 | Derive::~Derive() [deleting]
      [this adjustment: -16 non-virtual]
 10 | void Derive::FuncC()
      [this adjustment: -16 non-virtual]
```

**offset_to_top(0)**：表示当前这个虚函数表（BaseA，Derive）地址距离对象顶部地址的偏移量，因为对象的头部就是虚函数表的指针，所以偏移量为0。

再注意这里的**RTTI**中有了两项，表示BaseA和Derive的虚表地址是相同的，BaseA类里的虚函数和Derive类里的虚函数都在这个链条下，截至到**offset_to_top(-16)**之前都是BaseA和Derive的虚函数表。

**offset_to_top(-16)**：表示当前这个虚函数表（BaseB）地址距离对象顶部地址的偏移量，因为对象的头部就是虚函数表的指针，所以偏移量为-16。当基类BaseB的引用或指针base实际接受的是Derive类型的对象，执行base->FuncC()时候，由于FuncC()已经被重写，而此时的this指针指向的是BaseB类型的对象，需要对this指针进行调整，就是**offset_to_top(-16)**，所以this指针向上调整了16字节，之后调用FuncC()，就调用到了被重写后Derive虚函数表中的FuncC()函数。这些带adjustment标记的函数都是需要进行指针调整的。

**需要注意的是子类对象和父类对象不会共享一张虚表，各司其职。**



## 多继承下不同的继承顺序下类对象的布局

结论：继承顺序不同，子类的虚表布局也会不同



## 虚继承的布局

```c++
struct Base {
   Base() = default;
   virtual ~Base() = default;
   
   void FuncA() {}

   virtual void FuncB() {
       printf("BaseA FuncB\n");
  }

   int a;
   int b;
};

struct Derive : virtual public Base{
   void FuncB() override {
       printf("Derive FuncB \n");
  }
};
```

对象布局：

```
*** Dumping AST Record Layout
        0 | struct Derive
        0 |   (Derive vtable pointer)
        8 |   struct Base (virtual base)
        8 |     (Base vtable pointer)
       16 |     int a
       20 |     int b
          | [sizeof=24, dsize=24, align=8,
          |  nvsize=8, nvalign=8]

*** Dumping IRgen Record Layout
```

虚继承下，对象布局和普通单继承有所不同，普通继承下子类和基类共用一个虚表地址；**而在虚继承下，子类和虚基类分别有一个虚表的指针**。

虚函数表：

```
Vtable for 'Derive' (13 entries).
  0 | vbase_offset (8)
  1 | offset_to_top (0)
  2 | Derive RTTI
      -- (Derive, 0) vtable address --
  3 | void Derive::FuncB()
  4 | Derive::~Derive() [complete]
  5 | Derive::~Derive() [deleting]
  6 | vcall_offset (-8)
  7 | vcall_offset (-8)
  8 | offset_to_top (-8)
  9 | Derive RTTI
      -- (Base, 8) vtable address --
 10 | Derive::~Derive() [complete]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 11 | Derive::~Derive() [deleting]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 12 | void Derive::FuncB()
      [this adjustment: 0 non-virtual, -32 vcall offset offset]
```

**vbase_offset(8)**：对象在对象布局中与指向虚基类虚函数表的指针地址的偏移量

**vcall_offset(-8)**：当**虚基类**Base的引用或指针base实际接受的是Derive类型的对象，执行base->FuncB()时候，由于FuncB()已经被重写，而此时的this指针指向的是Base类型的对象，需要对this指针进行调整，就是vcall_offset(-8)，所以this指针向上调整了8字节，之后调用FuncB()，就调用到了被重写后的FuncB()函数。



## 虚继承未覆盖函数的对象的布局

```c++
struct Base {
   Base() = default;
   virtual ~Base() = default;
   
   void FuncA() {}

   virtual void FuncB() {
       printf("Base FuncB\n");
  }

   virtual void FuncC() {
       printf("Base FuncC\n");
  }

   int a;
   int b;
};

struct Derive : virtual public Base{
   void FuncB() override {
       printf("Derive FuncB \n");
  }
};
```

对象布局：

```
*** Dumping AST Record Layout
        0 | struct Derive
        0 |   (Derive vtable pointer)
        8 |   struct Base (virtual base)
        8 |     (Base vtable pointer)
       16 |     int a
       20 |     int b
          | [sizeof=24, dsize=24, align=8,
          |  nvsize=8, nvalign=8]

*** Dumping IRgen Record Layout
```

和虚继承情况下相同，普通单继承下子类和基类共用一个虚表地址，而虚继承下，子类和基类分别有一个虚表的指针。

虚函数表布局：

```
Vtable for 'Derive' (15 entries).
  0 | vbase_offset (8)
  1 | offset_to_top (0)
  2 | Derive RTTI
      -- (Derive, 0) vtable address --
  3 | void Derive::FuncB()
  4 | Derive::~Derive() [complete]
  5 | Derive::~Derive() [deleting]
  6 | vcall_offset (0)
  7 | vcall_offset (-8)
  8 | vcall_offset (-8)
  9 | offset_to_top (-8)
 10 | Derive RTTI
      -- (Base, 8) vtable address --
 11 | Derive::~Derive() [complete]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 12 | Derive::~Derive() [deleting]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 13 | void Derive::FuncB()
      [this adjustment: 0 non-virtual, -32 vcall offset offset]
 14 | void Base::FuncC()
```

**vbase_offset(8)**：对象在对象布局中与指向虚基类虚函数表的指针地址的偏移量

**vcall_offset(-8)**：当**虚基类**Base的引用或指针base实际接受的是Derive类型的对象，执行base->FuncB()时候，由于FuncB()已经被重写，而此时的this指针指向的是Base类型的对象，需要对this指针进行调整，就是vcall_offset(-8)，所以this指针向上调整了8字节，之后调用FuncB()，就调用到了被重写后的FuncB()函数。

**vcall_offset(0)**：当Base的引用或指针base实际接受的是Derive类型的对象，执行base->FuncC()时候，由于FuncC()没有被重写，所以不需要对this指针进行调整，就是vcall_offset(0)，之后调用FuncC()。



## 菱形继承下类对象的布局

```
struct Base {
   Base() = default;
   virtual ~Base() = default;
   
   void FuncA() {}

   virtual void FuncB() {
       printf("BaseA FuncB\n");
  }

   int a;
   int b;
};

struct BaseA : virtual public Base {
   BaseA() = default;
   virtual ~BaseA() = default;
   
   void FuncA() {}

   virtual void FuncB() {
       printf("BaseA FuncB\n");
  }

   int a;
   int b;
};

struct BaseB : virtual public Base {
   BaseB() = default;
   virtual ~BaseB() = default;
   
   void FuncA() {}

   virtual void FuncC() {
       printf("BaseB FuncC\n");
  }

   int a;
   int b;
};

struct Derive : public BaseB, public BaseA{
   void FuncB() override {
       printf("Derive FuncB \n");
  }

   void FuncC() override {
       printf("Derive FuncC \n");
  }
};
```

类对象布局：

```
*** Dumping AST Record Layout
        0 | struct Derive
        0 |   struct BaseB (primary base)
        0 |     (BaseB vtable pointer)
        8 |     int a
       12 |     int b
       16 |   struct BaseA (base)
       16 |     (BaseA vtable pointer)
       24 |     int a
       28 |     int b
       32 |   struct Base (virtual base)
       32 |     (Base vtable pointer)
       40 |     int a
       44 |     int b
          | [sizeof=48, dsize=48, align=8,
          |  nvsize=32, nvalign=8]

*** Dumping IRgen Record Layout
```



虚函数表：

```
Vtable for 'Derive' (20 entries).
  0 | vbase_offset (32)
  1 | offset_to_top (0)
  2 | Derive RTTI
      -- (BaseB, 0) vtable address --
      -- (Derive, 0) vtable address --
  3 | Derive::~Derive() [complete]
  4 | Derive::~Derive() [deleting]
  5 | void Derive::FuncC()
  6 | void Derive::FuncB()
  7 | vbase_offset (16)
  8 | offset_to_top (-16)
  9 | Derive RTTI
      -- (BaseA, 16) vtable address --
 10 | Derive::~Derive() [complete]
      [this adjustment: -16 non-virtual]
 11 | Derive::~Derive() [deleting]
      [this adjustment: -16 non-virtual]
 12 | void Derive::FuncB()
      [this adjustment: -16 non-virtual]
 13 | vcall_offset (-32)
 14 | vcall_offset (-32)
 15 | offset_to_top (-32)
 16 | Derive RTTI
      -- (Base, 32) vtable address --
 17 | Derive::~Derive() [complete]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 18 | Derive::~Derive() [deleting]
      [this adjustment: 0 non-virtual, -24 vcall offset offset]
 19 | void Derive::FuncB()
      [this adjustment: 0 non-virtual, -32 vcall offset offset]
```

**vbase_offset (32)**

**vbase_offset (16)**：对象在对象布局中与指向虚基类虚函数表的指针地址的偏移量

**offset_to_top (0)**

**offset_to_top (-16)**

**offset_to_top (-32)**：指向虚函数表的地址与对象顶部地址的偏移量。

**vcall_offset(-32)**：当**虚基类**Base的引用或指针base实际接受的是Derive类型的对象，执行base->FuncB()时候，由于FuncB()已经被重写，而此时的this指针指向的是Base类型的对象，需要对this指针进行调整，就是vcall_offset(-32)，所以this指针向上调整了32字节，之后调用FuncB()，就调用到了被重写后的FuncB()函数。



## 为什么使用虚继承

在虚继承下，只有一个共享的虚基类对象被继承，而无论改基类在派生类中出现了多少次。共享的基类子对象被称为虚基类。在虚继承下，基类子对象的复制及由此引起的二义性都被消除了。

## 为什么虚函数表中有两个析构函数？

前面的代码输出中我们可以看到虚函数表中有两个析构函数，一个标志为deleting，一个标志为complete，因为对象有两种构造方式，栈构造和堆构造，所以对应的实现上，对象也有两种析构方式，其中堆上对象的析构和栈上对象的析构不同之处在于，栈内存的析构不需要执行 delete 函数，会自动被回收。

## 为什么构造函数不能是虚函数？

构造函数就是为了在编译阶段确定对象的类型以及为对象分配空间，如果类中有虚函数，那就会在构造函数中初始化虚函数表，虚函数的执行却需要依赖虚函数表。如果构造函数是虚函数，那它就需要依赖虚函数表才可执行，而只有在构造函数中才会初始化虚函数表，鸡生蛋蛋生鸡的问题，很矛盾，所以构造函数不能是虚函数。

## 为什么基类析构函数要是虚函数？

一般基类的析构函数都要设置成虚函数，因为如果不设置成虚函数，在析构的过程中只会调用到基类的析构函数而不会调用到子类的析构函数，可能会产生内存泄漏。