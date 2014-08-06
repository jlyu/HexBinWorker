###### 开发要求：
系统平台：Win / MacOS / Linux / BSD，任选；  
开发语言：C++ / C# / Java，任选；  
界面框架：WPF / QT / WinForm / wxWidget，任选；（建议：QT / WPF）    


###### 程序要求：
1. 能读取Intel HEX文件格式，能读取二进制Bin文件格式;
2. 能够将以上两种文件格式相互转换并保存;
3. 同时以两种视图方式显示文件内容；（ASCII视图显示HEX格式内容，16进制视图显示二进制格式内容）;
4. 可在视图中对文件内容进行修改，并保证两种视图内容同步;
5. 串口设置;（包括自动侦测可用串口）
6. 实现ISP通讯协议，能够将文件烧入到ARM芯片。


###### 待解决问题
√ 1. 了解 Intel HEX 文件格式  
√ 2. 正则匹配 pcre.h 的使用  
√ 3. 验证解析 .hex 文件内容  
4. 验证解析 .bin 文件内容  
5. 考虑多块情况
6. 了解 ARM Cortex-M3 串口设置  
7. 实现 ISP 通讯协议  

![](https://raw.githubusercontent.com/ChainYu/HexBinWorker/master/docs/HexBinFileWorker.png)