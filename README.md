# tenacitas

A place to study C++

![tenacitas](etapas_sw_landscape.png)

# Directory Structure
The **tenacitas** repository directory structure allows different build tools to be used to build any of its components. This is accomplished by having the code in a separate directory tree from the build tools configured.
## tenacitas/code ##
Under the `tenacitas` directory we have:

    ├── builders
    ├── code

And under `tenacitas/code`:

    ├── calendar
    ├── concurrent
    ├── logger
    ├── string
    └── type

And in each of these directories there will be only source code (.h and .cpp) files.

For example, under ```tenacitas/code/logger``` we have:

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

## tenacitas/builders ##
Under `tenacitas/builders` we have:

    └── qtcreator

So, for now, there is only a build system based on [QtCreator](https://www.qt.io/). 
Inside `tenacitas/builder/qtcreator` there are, among other files and directories:

    ├── common.pri
    ├── concurrent.pro
    ├── logger.pro

`common.pri` is a file with definitions common to build all the components.

`tenacitas/builder/qtcreator/logger.pro` is the project file to be loaded into [QtCreator](https://www.qt.io/) in order to build **tenacitas.logger**.
`tenacitas/builder/qtcreator/concurrent.pro` is the project file to be loaded into [QtCreator](https://www.qt.io/) in order to build **tenacitas.concurrent**.

So, if one wants to use another build system, for example, [Code Blocks](http://www.codeblocks.org/), it is necessary to create a directory under `tenacitas/builders`, say `tenacitas/builders/code_blocks`, and put all the necessary files there.

To learn more about using [QtCreator](https://www.qt.io/), please read the [QtCreator README.md](builders/qtcreator/README.md) file.

To learn how to build library **tenacitas.logger**, please read the [logger README.md](code/logger/README.md) file.
To learn how to build library **tenacitas.concurrent**, please read the [logger README.md](code/concurrent/README.md) file.

# Software Structure

*tenacitas* softwares follow a basic structure, described in [Software Structure](doc/sw_structure/README.md)