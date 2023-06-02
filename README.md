# C木马程序

本应用程序实现
- 主控端（clr）
- 服务器（server）
- 受控端（cle）

主控端需要输入用户密码才能与服务器连接

用户密码在服务器启动时确定，默认为:

|用户|密码|
|--|--|
|root|admin|


主控端和受控端都能指定服务器IP（通常需要指定，默认为 127.0.0.1）
- 主控端在启动时通过命令行参数指定
- 受控端在编译时指定

主控端可以查看所有受控端连接，并指定发动攻击的对象

被攻击对象的机器会马上关机。

## 运行
1. 查看服务器运行机器的 ip 地址（如果受控端不是本机，通常情况下如此），运行
    ```shell
   ./getip.sh
    ```
2. 打开 [./project/controllee/CMakeLists.txt](./project/controllee/CMakeLists.txt)
3. 将找到的子网 ip 填入 `set(SERVER_IP ****)`处。
   比如你的子网ip是 *10.123.232.33*，则将该行代码改为：
   ```cmake
    set(SERVER_IP 10.123.232.33)
    ```
4. 运行 [./build.sh](./build.sh) 编译（注意需要输入密码）
    ```shell
    ./build.sh
   ```
5. 在后台运行服务器
   ```shell
    ./bin/server &
    ```
6. 在目标机器上（可以是本机）运行受控端
   ```shell
    ./bin/cle &
    ```
7. 运行主控端
    ```shell
    ./bin/clr --sip=`./getip.sh`
    ```
8. 现在我们启动了主控端，需要登录服务器。按照提示用默认用户名和密码登录
9. 登录之后，可以输入命令与服务器交互了。可以输入 `help` 命令查看有哪些命令可用
10. 输入 `attack <id>`可以指定要攻击的受控端

   <div style="border: 2px solid;">
   <div style="font-size: 2em; color: red">
   注意
   </div>被攻击的机器会立即关机！！！
   </div>

11. 由于后台运行着服务器，我们需要关闭它。运行找到服务器进程的 pid
   ```shell
   lsof -i:29000
   ```
12. 运行（将下面的 `<pid>` 替换为找到的 pid）杀死服务器进程
   ```shell
   kill -1 <pid>
   ```