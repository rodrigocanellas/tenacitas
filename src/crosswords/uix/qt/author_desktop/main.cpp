
#include "mainwindow.h"

#include <QApplication>
#include <QString>

//#include "grid_solved.h"
#include "tenacitas/src/crosswords/asy/dispatcher.h"
#include "tenacitas/src/crosswords/bus/grid_creator.h"
#include "tenacitas/src/log/log.h"
// #include "tnctmoduser.h"



int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  lib::src::log::set_debug_level();

  // crosswords::dat::is_user_created is_user_created;

  // auto maybe{is_user_created()};

  // std::shared_ptr<lib::user> user;

  // if (!maybe) {
  //   // create user database
  // } else {
  //   user = std::make_shared<lib::user>(maybe.value());
  // }

  crosswords::asy::dispatcher::ptr _dispatcher{
      crosswords::asy::dispatcher::create()};

  crosswords::dat::grid_creator _grid_creator(_dispatcher);
  //  GridSolved _gridSolved(_dispatcher);
  MainWindow w(_dispatcher);
  w.show();
  return a.exec();
}
