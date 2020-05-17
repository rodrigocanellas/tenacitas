/*! \mainpage 
 *
 * \section projects Projects
 *  
 * <a href="../../calendar/html/index.html">calendar</a>
 * 
 * <a href="../../concurrent/html/index.html">concurrent</a>
 *
 * <a href="../../logger/html/index.html">logger</a>
 *
 * <a href="../../number/html/index.html">number</a>
 * 
 * <a href="../../program/html/index.html">program</a>
 *
 * <a href="../../tester/html/index.html">tester</a>
 *
 * \section build_structure Build Structure
 * 
 * 
 * The build system files are separeted from the source files, so that one can use the build system that is used to.
 *
 * From the top directory we have:
 *
 *
 *     drwxrwxr-x  3 rodrigo rodrigo  4096 dez  9 09:59 builders
 *     drwxrwxr-x 11 rodrigo rodrigo  4096 mai 17 15:13 doc
 *     -rw-rw-r--  1 rodrigo rodrigo 35149 ago 31  2019 LICENSE
 *     -rw-rw-r--  1 rodrigo rodrigo   204 mai 17 11:19 README.md
 *     drwxrwxr-x  8 rodrigo rodrigo  4096 mai 17 12:10 src
 *     drwxrwxr-x  8 rodrigo rodrigo  4096 mai 17 11:18 tst
 *
 * In the **doc** directory tree are the documentation files of the projects.
 * 
 * In the **src** directory tree are the source files of the projects.
 * 
 * In the **tst** directory tree are the source files of the tests of the projects.
 * 
 * In the **builders** directory tree are the build systems used to build the projects.
 *
 * Currently, **tenacitas** projects use QtCreator(tm), so below **builders** we have:
 *
 *     drwxrwxr-x 6 rodrigo rodrigo 4096 mai 16 18:56 qtcreator/
 *
 * In this directory we have, among other directories:
 *
 *     drwxrwxr-x 8 rodrigo rodrigo  4096 mai 16 18:56 projects/
 * 
 * And inside it:
 * 
 *
 *     drwxrwxr-x 5 rodrigo rodrigo 4096 mai 17 10:02 calendar/
 *     drwxrwxr-x 4 rodrigo rodrigo 4096 mai 17 10:02 concurrent/
 *     drwxrwxr-x 8 rodrigo rodrigo 4096 mai 17 10:02 logger/
 *     drwxrwxr-x 4 rodrigo rodrigo 4096 mai 17 11:57 number/
 *     drwxrwxr-x 4 rodrigo rodrigo 4096 mai 17 10:58 program/
 *     drwxrwxr-x 6 rodrigo rodrigo 4096 mai 17 10:02 tester/
 * 
 *
 * Which are the directories with the files necessary to build the projects using QtCreator(tm).
 *
 * 
 * For example, in the **logger** directory tree we have:
 *
 *
 *    ├── dep<br>
 *    │   └── dep.pro <br>
 *    ├── logger.cerr<br>
 *    │   ├── logger.cerr.pro<br>
 *    │   ├── Makefile<br>
 *    │   └── Makefile.logger_cerr<br>
 *    ├── logger.clog<br>
 *    │   ├── logger.clog.pro<br>
 *    │   └── Makefile<br>
 *    ├── logger.cout<br>
 *    │   ├── logger.cout.pro<br>
 *    │   └── Makefile<br>
 *    ├── logger.file<br>
 *    │   ├── logger.file.pro<br>
 *    │   └── Makefile<br>
 *    ├── logger.pro<br>
 *    ├── logger.pro.user<br>
 *    ├── Makefile<br>
 *    └── tst<br>
 *        ├── cerr_log<br>
 *        │   ├── cerr_log.pro<br>
 *        │   └── Makefile<br>
 *        ├── clog_log<br>
 *        │   ├── clog_log.pro<br>
 *        │   └── Makefile<br>
 *        ├── cout_log<br>
 *        │   ├── cout_log.pro<br>
 *        │   └── Makefile<br>
 *        ├── dep<br>
 *        │   ├── dep.pro<br>
 *        │   └── Makefile<br>
 *        ├── file_log<br>
 *        │   ├── file_log.pro<br>
 *        │   └── Makefile<br>
 *        ├── file_log_how_to<br>
 *        │   ├── file_log_how_to.pro<br>
 *        │   └── Makefile<br>
 *        ├── file_log_multi<br>
 *        │   ├── file_log_multi.pro<br>
 *        │   └── Makefile<br>
 *        ├── file_log_single<br>
 *        │   ├── file_log_single.pro<br>
 *        │   └── Makefile<br>
 *        ├── Makefile<br>
 *
 * \endverbatin
 */
