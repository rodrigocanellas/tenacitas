# Definition #
**tenacitas.logger** is a thread-safe C++ library that allows one to log messages to `cout`, `cerr` or a file.
# Development Tools #
**tenacitas.logger** is written in C++ 11, it needs a GNU compatible compiler, and a POSIX Operating System.
# Directory Structure #
Please, read the [tenacitas README.md](../../README.md) file to understand the build structure used in **tenacitas** components.
# Using the library #
In `tenacitas/code/logger` directory we have

    ├── cerr.h
    ├── cout.h
    ├── doc
    ├── file.h
    ├── inc
    │   └── bus
    │       ├── cerr_media.h
    │       ├── configure_cerr_log.h
    │       ├── configure_cout_log.h
    │       ├── configure_file_log.h
    │       ├── cout_media.h
    │       ├── file_controller.h
    │       ├── file_media.h
    │       ├── level.h
    │       └── log.h
    ├── LICENSE
    ├── README.md
    ├── src
    │   └── bus
    │       ├── cerr.cpp
    │       ├── cout.cpp
    │       ├── file_controller.cpp
    │       └── file.cpp
    └── tst
        ├── cerr_log_creation
        │   └── main.cpp
        ├── cerr_log_how_to
        │   └── main.cpp
        ├── cerr_log_multi
        │   └── main.cpp
        ├── cerr_log_single
        │   └── main.cpp
        ├── cout_log_creation
        │   └── main.cpp
        ├── cout_log_how_to
        │   └── main.cpp
        ├── cout_log_multi
        │   └── main.cpp
        ├── cout_log_single
        │   └── main.cpp
        ├── file_log_creation
        │   └── main.cpp
        ├── file_log_how_to
        │   └── main.cpp
        ├── file_log_multi
        │   └── main.cpp
        └── file_log_single
            └── main.cpp

The header files that one will need to ```#include``` are the ones in ```tenacitas/code/concurrent```. The header files in ```tenacitas/code/concurrent/inc``` are files used internaly. 

You will also need the ```tenacitas.logger.so``` library file. 

You will need to include the ```.../tenacitas/code``` to your compiler include path.

In `tenacitas/code/logger/cerr.h` there is a configuration function and macro commands that allows to thread-safe write messages to ```cerr```.
In `tenacitas/code/logger/cout.h` there is a configuration function and macro commands that allows to thread-safe write messages to ```cout```.
In `tenacitas/code/logger/file.h` there is a configuration function and macro commands that allows to thread-safe write messages to a text file.

In each of these files there is a detailed explanation on how to use them.

You may also look at the test files, under `tenacitas/code/logger/tst` to learn how to use them.

# Building the library #
Under `tenacitas/builders` we have:

    └── qtcreator
So, for now, there is only a build system based on [QtCreator](https://www.qt.io/). 
Inside `tenacitas/builder/qtcreator` there is, among other files::

    ├── common.pri
    ├── logger.pro

`common.pri` is a file with definitions common to build all the components.

The `tenacitas/builders/qtcreator/logger.pro` is the project file you need to load on [QtCreator](https://www.qt.io/) in order to build **tenacitas.concurrent**.
If necessary, please read the instructions in [QtCreator README.md](../../builders/qtcreator/README.md)

**Enjoy!**
