# memanalyse
memanalyse 是我的大学毕业设计，一个用来检测堆溢出，UAF,重复释放，空指针解引用，内存泄漏等内存错误的工具  
memanalyse is a tool to detect memory error like heap flow, use after free, double free and null point dereference  
在 Ubuntu14 下完成开发与测试，只能对**32位**程序进行检测，依赖于[DynamoRIO-i386-Linux-7.0.17598-0](https://github.com/DynamoRIO/dynamorio/releases/tag/cronbuild-7.0.17598)  
memanalyse is devoloped and tested in Ubuntu14. It can only analyse 32bit program, and depends on [DynamoRIO-i386-Linux-7.0.17598-0](https://github.com/DynamoRIO/dynamorio/releases/tag/cronbuild-7.0.17598)
## How to Build
0. `$ git clone https://github.com/Marsman1996/memanalyse.git`
1. build dynamoRIO client  
   ```
   $ cd client
   $ cmake -DDynamoRIO_DIR=~/DynamoRIO-i386-Linux-7.0.17598-0/cmake/ .
   $ make
   ```
   这样就在client文件夹中生成了 *libmyclient.so*  
   And there is *libmyclient.so* in client folder
2. build memanalyse  
   ```
   $ cd ..
   $ make
   ```
   这样就在Debug文件夹中生成了分析程序 *all*  
   And there is analyse program *all* in Debug folder
## How to Run
1. 使用DynamoRIO及刚刚生成的插件运行测试程序  
   `$ $DINAMORIO_PATH/drrun -c client/libmyclient.so -- $TESTED_PROG`
2. 将生成的trace文件改名为 *instrace.log* 放入 *log* 文件夹中
   复制显示的加载module部分放入 *log/module.log*
   复制显示的内存分配函数部分放入 *log/alloc.log*
3. 运行分析程序，得到结果
   ```
   $ cd Debug
   $ ./all
   ```
## REFERENCE
DynamoRIO client中的trace获取部分基于 [Dr_memtrace](https://github.com/mcgrady1/Dr_memtrace) BY [mcgrady1](https://github.com/mcgrady1)