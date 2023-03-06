# EasySlither

高级语言程序设计课设项目

一个 [Slither.io](http://slither.io/) 的 [EasyX](https://easyx.cn/) 实现

## 主要特性

- 使用 C++ 语言与 [EasyX](https://easyx.cn/) 图形库实现

- 充分利用最新 C++ 规范的**面向对象**、[**模块**](https://zh.cppreference.com/w/cpp/language/modules)和**并发**等方面特性

- 同时实现客户端和服务端
  - 可本地同时运行两端，进行单人游戏
  - 可通过 UDP 协议连接到运行服务端的远程服务器，实现多人游戏
  - *（画饼）（需要第三方库支持）*
    完整实现[原版游戏的通信协议](https://github.com/ClitherProject/Slither.io-Protocol/blob/master/Protocol.md)，可通过
    WebSocket 协议连接原版服务器

- 实现较为简单的电脑玩家，人数不足时自动补充

- 支持鼠标、键盘、Xbox 手柄三种方式输入

- （画饼）支持自定义蛇蛇皮肤
