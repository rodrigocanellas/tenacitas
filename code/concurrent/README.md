# Definition #
**tenacitas.concurrent** is a header only C++ library that makes it easier to write concurrent applications.
# Development Tools #
**tenacitas.concurrent** is written in C++ 11, it needs a GNU compatible compiler, and a POSIX Operating System.
# Directory Structure #
Please, read the [tenacitas README.md](../../README.md) file to understand the build structure used in all **tenacitas** components.
# Using the library #
In `tenacitas/code/concurrent` directory we have

    ├── dispatcher.h
    ├── inc
    │   ├── async_loop_functions.h
    │   ├── async_loop.h
    │   ├── sleeping_loop_controller.h
    │   ├── thread_pool_controller.h
    │   ├── time_out_function.h
    │   └── traits.h
    ├── LICENSE
    ├── README.md
    ├── sleeping_loop.h
    ├── thread_pool.h
    ├── thread.h
    └── tst
        ├── async_loop_000
        │   └── main.cpp
        ├── async_loop_001
        │   └── main.cpp
        ├── async_loop_002
        │   └── main.cpp
        ├── async_loop_003
        │   └── main.cpp
        ├── async_loop_004
        │   └── main.cpp
        ├── async_loop_005
        │   └── main.cpp
        ├── async_loop_006
        │   └── main.cpp
        ├── async_loop_007
        │   └── main.cpp
        ├── async_loop_008
        │   └── main.cpp
        ├── async_loop_009
        │   └── main.cpp
        ├── async_loop_010
        │   └── main.cpp
        ├── async_loop_011
        │   └── main.cpp
        ├── dispatcher_000
        │   └── main.cpp
        ├── message_dispatcher
        │   ├── main.cpp
        │   └── test007.h
        ├── sleeping_loop_000
        │   └── main.cpp
        ├── sleeping_loop_001
        │   └── main.cpp
        ├── sleeping_loop_002
        │   └── main.cpp
        ├── sleeping_loop_003
        │   └── main.cpp
        ├── sleeping_loop_004
        │   └── main.cpp
        ├── sleeping_loop_005
        │   └── main.cpp
        ├── thread_pool_000
        │   └── main.cpp
        ├── thread_pool_001
        │   └── main.cpp


The header files that one will need to ```#include``` are the ones in ```tenacitas/code/concurrent```. The header files in ```tenacitas/code/concurrent/inc``` are files used internaly. 

You will need to include the ```.../tenacitas/code``` to your compiler include path.

In `tenacitas/code/concurrent/thread.h` there is a simple wrapper on the ```std::thread``` class, that takes care of joining the thread for you.
In `tenacitas/code/concurrent/sleeping_loop.h` there is the ```slepping_loop``` template class, that allows a function to executed in a assynchronous loop.
In `tenacitas/code/concurrent/thread_pool` there is another ```thread_pool` template class, that implements a thread pool.
In `tenacitas/code/concurrent/dispatcher` there is the ```dispatcher``` template class,that implements the publisher/subscriber pattern.

In each of these files there is a detailed explanation on how to use them.

You may also look at the test files, under `tenacitas/code/concurrent/tst` to learn how to use them.
 
# Building the library #
Under `tenacitas/builders` we have:

    └── qtcreator

So, for now, there is only a build system based on [QtCreator](https://www.qt.io/). 
Inside `tenacitas/builder/qtcreator` there is, among other files:

    ├── common.pri
    ├── concurrent.pro

`common.pri` is a file with definitions common to build all the components.

The `tenacitas/builders/qtcreator/concurrent.pro` is the project file you need to load on [QtCreator](https://www.qt.io/) in order to build **tenacitas.concurrent**.
If necessary, please read the instructions in [QtCreator README.md](../../builders/qtcreator/README.md)

**Enjoy!**
