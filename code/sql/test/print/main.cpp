#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

//#include <sql/entities/column.h>
#include <sql/entities/database.h>
#include <sql/entities/foreign_key.h>
#include <sql/entities/foreign_key_column.h>
#include <sql/entities/host.h>
#include <sql/entities/hosts.h>
#include <sql/generic/ptr.h>
#include <sql/entities/primary_key.h>
#include <sql/entities/server.h>
#include <sql/entities/table.h>
#include <sql/test/create_hosts.h>

using namespace capemisa::sql::entities;
using namespace capemisa::sql::test;

int
main()
{
  try {
    create_hosts _create_hosts;
    hosts _hosts(_create_hosts());
    std::cout << _hosts << std::endl;

  } catch (std::exception& _ex) {
    std::cerr << "ERROR: " << _ex.what() << std::endl;
  }
}
