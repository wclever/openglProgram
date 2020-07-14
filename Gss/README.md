这版本为工程主要实现版本，使用原生opengl实现

lib库编译：cmake-gui + mingw32-make

首先在编译文件夹下新建build文件夹，将cmake结果放入build中

在cmd中进入build文件夹，然后用mingw32-make -j 4编译，或者mingw32-make单线程构建
