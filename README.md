# Minecraft & Shovel Engine

My own c++ minecraft implementation

### How to build?

This project supports only Macos platform right now, because of Metal API utility.

### Macos

First, you need to make sure that SDL2 installed on your mac. This project uses SDL2 version 2.0.12, but it may work
with older or newer ones. You can download it [here](https://www.libsdl.org/download-2.0.php) or just type:

```
$ brew install sdl2
```

Then generate XCode project

```
$ mkdir build
$ cd build
$ CMake -G Xcode ..
```

And then you can select engine scheme via `Product`->`Scheme`->`Choose scheme`

You may also (and it's recommended) use `Clion` IDE

This repo utilizes `clang-format`, so to make any PR's please, use it as a formatter