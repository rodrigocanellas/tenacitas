# tenacitas.lib
C++ header only libraries

![alt text](https://github.com/rodrigocanellas/tenacitas.lib/doc/main/dependencies.png?raw=true)


@startuml
hide empty members
async ..> async
async ..> calendar
async ..> container
async ..> log
async ..> number
communication ..> async
communication ..> calendar
communication ..> communication
communication ..> log
communication ..> traits
container ..> log
container ..> number
conversions ..> conversions
crosswords ..> async
crosswords ..> container
crosswords ..> crosswords
crosswords ..> log
crosswords ..> math
crosswords ..> number
ipc ..> async
ipc ..> ipc
ipc ..> log
ipc ..> number
ipc ..> socket
log ..> log
log ..> number
log ..> traits
number ..> number
test ..> program
traits ..> traits
@enduml
