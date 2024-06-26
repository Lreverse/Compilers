
## 语义分析

主要任务：通过遍历lab1构建的语法树，建立符号表并进行类型检查
- 每当遇到语法单元`ExtDef`或者`Def`，就说明该结点的子结点们包含了变量或者函数的定义信息，这时候应当将这些信息通过对子结点们的遍历提炼出来并插入到符号表里。
- 每当遇到语法单元`Exp`，说明该结点及其子结点们会对变量或者函数进行使用，这个时候应当查符号表以确认这些变量或者函数是否存在以及它们的类型是什么。

### 错误识别

- [x] `Error type 1`：变量在使用时未经定义
- [x] `Error type 2`：函数在调用时未经定义
- [x] `Error type 3`：变量出现重复定义，或变量与前面定义过的结构体名字重复
- [x] `Error type 4`：函数出现重复定义（即同样的函数名出现了不止一次定义）
- [x] `Error type 5`：赋值号两边的表达式类型不匹配
- [x] `Error type 6`：赋值号左边出现一个只有右值的表达式
- [x] `Error type 7`：操作数类型不匹配或操作数类型与操作符不匹配（例如整型变量与数组变
量相加减，或数组（或结构体）变量与数组（或结构体）变量相加减）
- [x] `Error type 8`：return语句的返回类型与函数定义的返回类型不匹配
- [x] `Error type 9`：函数调用时实参与形参的数目或类型不匹配
- [x] `Error type 10`：对非数组型变量使用“[...]”（数组访问）操作符
- [x] `Error type 11`：对普通变量使用“(...)”或“()”（函数调用）操作符
- [x] `Error type 12`；数组访问操作符“[...]”中出现非整数（例如a[1.5]）
- [x] `Error type 13`：对非结构体型变量使用“.”操作符
- [x] `Error type 14`：访问结构体中未定义过的域
- [x] `Error type 15`：结构体中域名重复定义（指同一结构体中），或在定义时对域进行初始化（例如struct A { int a = 0; }）
- [x] `Error type 16`：结构体的名字与前面定义过的结构体或变量的名字重复
- [x] `Error type 17`：直接使用未定义过的结构体来定义变量

### 符号表

- 此处只设计一个符号表，所有变量都会插入到这个符号表中，包括函数形参
- 对于结构体声明，同样插入表里（~~虽然这样做很不好~~）
- 对于结构体变量，像普通变量一样处理，这与结构体声明是不一致的

#### 基本要求

- 变量
  - 记录变量名及其类型
- 函数
  - 记录返回类型、参数个数以及参数类型

#### 哈希表

|函数|说明|
|:--:|:--:|
|`hash_pjw`|P.J. Weinberger提出的哈希函数|
|`initHashT`|初始化哈希表，将所有的桶置为NULL|
|`createHnode`|建立一个哈希结点|
|`insertHnode`|向哈希表中插入一个结点(头插法)|
|`printHashT`|打印哈希表|
|`check`|检查重复定义，或者已经定义|
