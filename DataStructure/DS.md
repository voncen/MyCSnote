### 指针
存储另一变量的地址,16进制数

```c++
int    *ip;    /* 一个整型的指针 */
double *dp;    /* 一个 double 型的指针 */
float  *fp;    /* 一个浮点型的指针 */
char   *ch;    /* 一个字符型的指针 */
/*星号是用来指定一个变量是指针*/
```
#### 空指针
内存地址 0 有特别重要的意义，它表明该指针不指向一个可访问的内存位置。但按照惯例，如果指针包含空值（零值），则假定它不指向任何东西。

#### 指针的运算

在执行完上述的运算之后，ptr 将指向位置 1004，因为 ptr 每增加一次，它都将指向下一个整数位置，即当前位置往后移 4 个字节。这个运算会在不影响内存位置中实际值的情况下，移动指针到下一个内存位置。如果 ptr 指向一个地址为 1000 的字符，上面的运算会导致指针指向位置 1001，因为下一个字符位置是在 1001。

增加一个字节数，减少一个字节数

```c++
#include <iostream>
 
using namespace std;
const int MAX = 3;
 
int main ()
{
   int  var[MAX] = {10, 100, 200};
   int  *ptr;
 
   
   ptr = &var[MAX-1];// 指针中最后一个元素的地址
   for (int i = MAX; i > 0; i--)
   {
      cout << "Address of var[" << i << "] = ";
      cout << ptr << endl;
 
      cout << "Value of var[" << i << "] = ";
      cout << *ptr << endl;
 
      // 移动到下一个位置
      ptr--;
   }
   return 0;
}
```

#### 指针vs数组
不完全互换
在C++中，数组名代表数组中第一个元素（即序号为0的元素）的地址
```c++
#include <iostream>
 
using namespace std;
const int MAX = 3;
 
int main ()
{
   int  var[MAX] = {10, 100, 200};
 
   for (int i = 0; i < MAX; i++)
   {
      *var = i;    // 这是正确的语法
      var++;       // 这是不正确的
   }
   return 0;
}
```
把指针运算符 * 应用到 var 上是完全可以的，但修改 var 的值是非法的。这是因为 var 是一个指向数组开头的常量，不能作为左值。

由于一个数组名对应一个指针常量，只要不改变数组的值，仍然可以用指针形式的表达式。例如，下面是一个有效的语句，把 var[2] 赋值为 500：

```c++
*(var + 2) = 500;
```

#### 数组存储指针
```c++
int *ptr[MAX];
// ptr 是一个数组， 有 MAX 个整数指针

```
可以用一个指向字符的指针数组来存储一个字符串列表
```c++
#include <iostream>
 
using namespace std;
const int MAX = 4;
 
int main ()
{
 const char *names[MAX] = {
                   "Zara Ali",
                   "Hina Ali",
                   "Nuha Ali",
                   "Sara Ali",
   };
 
   for (int i = 0; i < MAX; i++)
   {
      cout << "Value of names[" << i << "] = ";
      cout << names[i] << endl;
   }
   return 0;
}
```
#### 指针的指针


定义结构：struct

#### FIFO
##### 队列
队列：先入先出
###### 循环队列
可以看作是圆圈
两个指针：head,tail
满：tail在向后移动时和head重合，接下来head向后移动
空：tail和head都置于-1
##### 栈