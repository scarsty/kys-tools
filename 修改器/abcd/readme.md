使用C++编写的一些小工具。

# makeRanger

从xlsx文件生成存档。

# trans50

包含3个功能，覆盖了原sfe2kdefscript，talkmaker的功能。

## 将原版对话转为txt文件

```
trans50 --talk --in path
```
其中path为talk.idx和talk.grp所在的文件夹，编码限制为BIG5。执行之后会在path文件夹中生成talkutf8.txt文件，每个对话一行。

需注意如果用编辑器查看的话，行号为对话的编号加一。

该功能会去掉*，改为游戏中自动计算换行。如果有特殊需求建议自己修改代码处理。


## 将原版指令转为lua脚本。

```
trans50 --kdef --in path --out path_out
```
其中path为kdef.idx、kdef.grp所在的文件夹，如果该文件夹中含有talkutf8.txt，会优先读取，否则读取talk.idx和talk.grp。

执行之后会在输出目录中的event文件夹得到所有剧情事件的lua脚本。这些脚本可以被kys-pascal和kys-cpp直接支持。需注意kys-cpp的50指令部分支持不完整。

也可以自己修改ini文件，得到适合自己版本的脚本。

## 将50指令转成可读性稍好的程序段

```
trans50 --50 --in path --talkpath talkpath --out path_out
```
path为上一步转换脚本的文件夹，talkpath为查找对话文件的位置，实际上可以认为是上一个功能输入文件夹。

执行之后会在输出目录的event50文件夹下面得到50指令翻译后的脚本，如果原脚本不含50指令则不会有输出。此处得到的脚本更容易阅读，但因一些关键字不同，不保证能正常执行。如有进一步需求建议自行修改相关代码。