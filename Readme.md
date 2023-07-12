# SDL2 游戏开发
本项目是我学习SDL2游戏开发的代码库，按照LazyFoo的推荐，我开发了几个小游戏用于练习。

## 教程
1. [如何入门游戏编程](https://lazyfoo.net/articles/article01/index.php) 
2. [SDL2 教程](https://lazyfoo.net/tutorials/SDL/index.php)

## 项目结构
0. ### main.cpp
    playground，用于写一些测试代码，比如你想测试下某个SDL API或者某个新学的C++语法
1. ### 1～10开头的文件夹
   小游戏的代码
2. ### YEngine文件夹
   用于存放所有小游戏的“通用”文件，也就是一个名为YEngine的“游戏引擎”


## 如何构建
项目使用CMake，在开始学习SDL游戏开发前，请先简单的学习一下如何用CMake来构建C++项目。


## 在Web里运行

- 使用命令：`emcc src/main.cpp src/YEngine/LTexture.cpp  -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_LIBJPEG=1 -s SDL2_IMAGE_FORMATS='["jpg","png"]' --preload-file resources -o hello.html` 把游戏编译成wasm html应用
- 运行: `emrun --browser chrome hello.html`

>> ⚠️注意：现在运行hello.html会发现一直处于running,因为main函数里的while是无限循环。因此需要修改main loop的逻辑，将其改为适合wasm app，详细参考：[html一直被hang的问题](https://emscripten.org/docs/getting_started/FAQ.html#why-does-my-html-app-hang), [如何运行Main Loop](https://emscripten.org/docs/getting_started/FAQ.html#faq-how-run-event-loop)
