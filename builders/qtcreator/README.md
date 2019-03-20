# QtCreator Build System #
This file describes how to set the [QtCreator](https://www.qt.io/) to build **tenacitas** components.

After opening the project file, such as ```tenacitas/builders/qtcreator/logger/logger.pro```, you will see a window more or less like this:

![QtCreator](doc/qt_creator_00.png)

You must turn `Shadow Build` off. Click on the ![ProjectIcon](doc/qt_creator_02.png) and uncheck the `Shadow Build` box, like this:
![ShadowBuildOff](doc/qt_creator_01.png).

Then click on `Build/Build All` menu option to build the library as well as all the tests.

## Products ##
When you build a project, or a group of projects, all the artifacts built are placed in `tenacitas/products` sub-directory, according to the _Kit_ and configuration (`Debug` or `Release`) you used. In this example, the `linux-g++` and `Debug` was used, which resulted in this directory structure under `tenacitas/products`, again using **tenacitas.logger** as an example:

    .
    └── linux-g++
        └── debug
            ├── lib
            │   ├── libtenacitas.logger.a
            │   ├── libtenacitas.logger.so -> libtenacitas.logger.so.0.0.1
            │   ├── libtenacitas.logger.so.0 -> libtenacitas.logger.so.0.0.1
            │   ├── libtenacitas.logger.so.0.0 -> libtenacitas.logger.so.0.0.1
            │   └── libtenacitas.logger.so.0.0.1
            └── tst
                ├── tenacitas.logger.test.cerr_log_creation
                ├── tenacitas.logger.test.cerr_log_how_to
                ├── tenacitas.logger.test.cerr_log_multi
                ├── tenacitas.logger.test.cerr_log_single
                ├── tenacitas.logger.test.cout_log_creation
                ├── tenacitas.logger.test.cout_log_how_to
                ├── tenacitas.logger.test.cout_log_multi
                ├── tenacitas.logger.test.cout_log_single
                ├── tenacitas.logger.test.file_log_creation
                ├── tenacitas.logger.test.file_log_how_to
                ├── tenacitas.logger.test.file_log_multi
                └── tenacitas.logger.test.file_log_single

