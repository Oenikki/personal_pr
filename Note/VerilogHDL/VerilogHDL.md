# Verilog HDL

> Verilog HDL 数字设计与综合

**基础总览**

1. 层次建模
2. 基本概念
3. 模块和端口
4. 门级建模
5. 数据流建模
6. 行为级建模
7. 任务和函数
8. 实用建模技术

---

## 一、层次建模

数字电路设计中有两种基本的设计方法：

+ 自底向上
+ 自顶向下

verilog使用module代表一个基本的功能块，可以是一个元件，也可以是一个低层次模块的组合（复用）。

模块声明类似模板，创建对象叫做实例（instance）。不允许模块声明中嵌套模块。



Test bench: 对设计模块施加激励检查输出检测功能的正确性。

+ 在激励块中调用并直接驱动设计块（控制clk和reset）
+ 在另一虚拟的顶层模块中调用激励块和设计块。

For instance:

#设计快

```verilog
module ripple_carry_counter(q, clk, reset);
    output [3:0] q;
    input clk, reset;

    T_FF tff0(q[0], clk, reset);
    T_FF tff1(q[1], clk, reset);
    T_FF tff2(q[2], clk, reset);
    T_FF tff3(q[3], clk, reset);
endmodule
    
module T_FF(q, clk, reset);
   	output q;
    intput clk, reset;
    wire d;
    
    D_FF dff0(q, d, clk, rest);
    not n1(d, q);
endmodule

module D_FF(q, d, clk, reset);
    output q;
    input d, clk, reset;
    reg q;
    always@(posedge reset or negedge clk)
        if (reset)
            q <= 1'b0;
   	 	else
        	q <= d;
endmodule
```

#激励块

```verilog
module stimulus;

    reg clk;
    reg reset;
    wire[3:0] q;
    ripple_carry_counter r1(q, clk, reset);
    
    initial
        clk = 1'b0
    always
        #5 clk = ~clk;
    initial
    begin
        reset = 1'b1;
        #15 reset = 1'b0;
        #180 reset = 1'b1;
        #10 reset = 1'b0;
        #20 $finish;
    end
    
    initial
        $monitor($timer, " Output q = %d", q);
endmodule 
```



## 二、基本概念

### **词法约定**

**数字声明**

+ 指定位数的数字
  <size>'<base format><number>
  size：数字的为位宽度，只能用10进制表示
  base format：十进制'd 'D；十六进制'h 'H；二进制'b 'B；八进制'o 'O

+ 不指定位数的数字
  默认都是十进制数，不指定宽度与计算机有关

+ X和Z值

  不确定值X
  高阻值Z

