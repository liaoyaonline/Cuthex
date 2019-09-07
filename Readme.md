# Cuthex
## 简单介绍
&emsp;&emsp;因为工作需要一个切割hex的工具，公司使用的viewhex切割起来极度麻烦。所以我写了这个小程序
他是通过读取config里面的配置来进行切割的，而且配置的话第一行表示切割的份数，第二行表示
起始地址和结束地址，中间用一个空格隔开。
## 使用方法
- 第一步，下载源码
git clone git@github.com:liaoyaonline/Cuthex.git
- 第二步，编译输出可执行文件
gcc Cuthex.c -o Cuthex
- 第三步，创建config.txt
00FC4000 00FC8000
上述表示切割出来一个文件，它的起始地址是00FC4000结束地址是00FC8000
- 第四步，使用格式
./Cuthex [filename.hex]
阔以了
