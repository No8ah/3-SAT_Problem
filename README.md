# 3 - SAT 问题

## "SAT"问题简介

SAT问题, 专业术语又叫做"Boolean Satisfiability Problem", 简称"SAT Problem"。
  
具体来说, SAT问题需要我们解决什么问题呢?

简单而言, 考虑一个布尔表达式"Boolean Expression": $$\quad clause_{1} \cap clause_{2} \cap \cdots \cap clause_{m}$$.
  
我们需要处理的问题是: $$\text{能不能找到这么一组赋值}\quad S.t. \quad clause_{1} \cap clause_{2} \cap \cdots \cap clause_{m} = True$$呢?

其中:
```math
\left\{\begin{aligned}
\quad & clause_{1} = x_{1} \cup x_{2} \cup \cdots \cup x_{n}\\
\quad & \vdots\\
\quad & clause_{l} = x_{1} \cup \overline{x_{2}} \cup \cdots \cup x_{n}\\
\quad & \vdots \\
\quad & clause_{m} = \overline{x_{1}} \cup x_{2} \cup \cdots \cup \overline{x_{n}}\\
\end{aligned}\right.
```

也就是说, 我们需要设计这么一个算法:

```math
\text{找出}x_{1},x_{2},\cdots,x_{n} \text{的一组赋值}\quad S.t. \quad clause_{1} \cap clause_{2} \cap \cdots \cap clause_{m} = True
```

这就是我们的这个"SAT Problem"需要解决的问题。

## "3 - SAT"问题介绍

那么什么是"3 - SAT"问题呢？

首先, 我们观察一下:

```math
\left\{\begin{aligned}
\quad & Boolean\ Expression = clause_{1} \cap clause_{2} \cap \cdots \cap clause_{m}\\
\quad & clause_{l} = x_{1} \cup \overline{x_{2}} \cup \cdots \cup x_{n}
\end{aligned}\right.
```

我们可以看到:

```math
\left\{\begin{aligned}
\quad & \text{有}m\text{个}clause\\
\quad & \\
\quad & \text{每个}clause\text{有}n\text{个变量}x_{something}
\end{aligned}\right.
```

那么这个"3 - SAT"问题中的"3"是个什么东西呢?

看下面这么一个例子:

```math
\left\{\begin{aligned}
\quad & clause_{1} = x_{1} \cup \overline{x}_{2}\cup \overline{x}_{5}\\
\quad & clause_{2} = x_{1} \cup x_{2}  \cup x_{4}\\
\quad & clause_{3} = x_{1} \cup x_{3} \cup x_{5}\\
\quad & clause_{4} = x_{2} \cup x_{4} \cup \overline{x}_{5}
\end{aligned}\right.
```

可以很清楚地看到这么一个事实:

```math
\left\{\begin{aligned}
\quad & m = 4 \text{个}clause\\
\quad & \text{每个}clause \text{有} \underline{3} \text{个}x_{something}\\
\quad & \text{总共有} 5 \text{个变量}x_{something} \quad (\text{其实可能多于5个,就比如}x_{6}\text{压根就不在}clause\text{里边})
\end{aligned}\right.
```

总而言之, 这个 $$\underline{3}$$ 是个重点, 就是"3 - SAT"问题中的"3"。

那么问题就很清楚了:

```math
\text{能不能找到这么一组赋值}\quad S.t. \quad x_{1}, x_{2}, \cdots , x_{5}, \cdots \quad MAKE \quad clause_{1}, clause_{2}, \cdots , clause_{4} \quad S.A.T. \quad clause_{1} \cap clause_{2} \cap \cdots \cap clause_{4} = True  
```

  这就是我们要解决的"3 - SAT"问题

  
