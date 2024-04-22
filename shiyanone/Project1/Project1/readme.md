# 这是用来解决老师布置的问题的

## one.cpp简介

- one.cpp是最终的程序

- 实现了对保留字、用户标识符、无符号整数、无符号浮点数（包括科学计数法）、无法识别的字符

  的分类识别。

- 关键思想：

  - 对文件先读取一行

  - 能观察到，保留字如果没有分隔符分开，必定会有空格与其他分开

    - 所以 先用 geline(,,' ')读取每一个“粗单词”

    - 然后进一步分割这时就要考虑怎么分割了：

      - 标识符遇见分隔符，标识符先归约
      - 分隔符遇见非分隔符，分隔符先归约
      - 分隔符遇见分隔符，继续读进，判断是否是组合分隔符，直到不是或者满足上述条件，进行规约

      - 数字遇到.的话，需要按照“伪小数[^A]”处理。
      - 这样分割后，就可以进行下一步的判断了

  - 判断

    - 先判断是否是保留字或者操作符，因为有集合，特好判断。
    - 再依次判断，用户标识符
    - 无符号整数
    - 无符号浮点数
    - 无法识别的字符

  - 存储数据即可。

  

[^A]: 包括这几种情况：是小数，无法识别字符，科学计数法

## input.txt简介

是存储待判断程序的文件，因为默认是UTF-8的字符体系，所有汉字等字符无法判断。



## output.txt简介

存储的是二元式，整个程序判断后的。



## idlist.txt简介

找到的用户标识符  三元式存储 (value, type, length )



## uintlist.txt简介

找到的无符号整数  三元式存储 (value, type, length )



## ufdlist.txt简介

找到的无符号浮点数  三元式存储 (value, type, length )



## 验证局部.cpp简介

用来验证一些函数、及一些用法