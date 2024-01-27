# tenacitas.lib
C++ header only libraries

## Not only C++ code
The `tenacitas.lib` also has the goal of suggesting a general structure for software development, which intends to make it easier to create highly decoupled artefacts.

The macro structured is presented by the UML like diagram below followed by a brief text explanation:

@startuml

hide empty members
product <|-- tst
product <|-- pgm
product <|-- exp
product <|-- lib
code <|-up- tst
code <|-up- pgm
code <|-up- exp
code <|-up- lib

code <|-- shu
code <|-- sta
code <|-- dom
code <|-- uix
code <|-- per
code <|-- asy
code <|-- net
code <|-- dev
code <|-- mat
code <|-- fmt
code <|-- log
code <|-- con
code <|-- tra
code <|-- sof

product ..> shu
product ..> sta
product ..> dom
product ..> uix
product ..> per
product ..> asy
product ..> net
product ..> dev
product ..> mat
product ..> fmt
product ..> log
product ..> con
product ..> tra
product ..> sof

bld ..> product

class docs

@enduml


*Code categories*

asy - asynchronous: code that supports asynchronous programming, like threads
bld - builder: files used to build libraries, tests, examples and programs
con - container: vectors, lists, stack...
dev - device: code that implements external devices, like robots
docs - documents: files that document code
dom - domain: data and functions that implement business logic
exp - example: applications that exemplifies use of any other category
fmt - formating: code responsible for formatting data
lib - library: other categories assembled
log - logger: code that logs execution messages
mat - mathematics: a special place for math data and functions
net - network: code for network programming
per - pesistence: writes and reads data to permanent memory
pgm - program: an executable
shu - shuffle (cipher/decipher)
sof - software: code that is related to software itself, like 'program_options'
sta - static: compile time code, like type traits and concepts
tra - translation: code used to translate text
tst - test: aplication used to test other categories
uix - final (human) user experience

## Library dependencies

The diagram below shows the dependencies within the library:

@startuml
hide empty members
asy ..> asy
asy ..> con
asy ..> fmt
asy ..> log
asy ..> sof
asy ..> sta
exp ..> asy
exp ..> exp
exp ..> fmt
exp ..> log
exp ..> sof
exp ..> sta
fmt ..> fmt
log ..> fmt
log ..> log
log ..> sta
sof ..> sof
sta ..> sta
tra ..> log
tra ..> tra
tst ..> asy
tst ..> con
tst ..> log
tst ..> mat
tst ..> sof
tst ..> sta
tst ..> tra
@enduml

## Other repository dependencies

This repository depends on https://github.com/rodrigocanellas/tenacitas.bld, which must be cloned in a directory side by side with `tenecitas.lib`


