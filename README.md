# tenacitas
C++ 20 header only libraries


`tenacitas` is the result of my continuous study of C++ and Software Design.

It is made up of the following `namespace`s:


**async** - tries to make it easier to implement common asynchronous tasks, such as execute a function in a certain period of time (`sleeping_loop`), and a publisher/subscriber mechanism (`dispatcher`), mixed with a event queue, that allows to define multiple handlings for an event, each with multiple concurrent handlers, without searching for the event in any kind of container.

**container** - a simple `matrix` and a `circular_queue` that expands as necessary.

**log** - another log class

**parser** - a `.ini` parser is the first class of this namespace

**program** - classes related to a program, like `options`, a `exit` event and a `closer` classes , used with `async::dispatcher` allows asynchronous console programs.

**traits** - type traits and some very simple meta programming

**translator** - I could not resist trying to implement a `translator` structure

And some other namespaces.

# using it
As `tenacitas` is a header only "library", there is no need to build it, all you need is to add the directory `...tenacitas/src` to the include path of your compiler. In `gcc` it would be `-Isome-path/tenacitas/src`

# examples and tests
The examples and tests are, for now, the best source of documentation. If you want to build them, you can use `QtCreator`(I am using version 13.0.1) and open the `tenacitas/builder/qtcreator/tenacitas.pro` file, or create a project file you like the most.

# dependencies
`tenacitas` does not depend on any other software

# contributions
I would be most pleased to read you comments and suggestions, as long as they are presented in a polite way.

# disclaimer
This library is far from being ready to use in production, so if you want to use it you are on your own.

