#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

//#include <sql/entities/column.h>
#include <sql/entities/database.h>
#include <sql/entities/host.h>
#include <sql/entities/hosts.h>
#include <sql/entities/server.h>
//#include <sql/entities/table.h>
#include <sql/entities/internal/types.h>

using namespace capemisa::sql::entities;

int
main()
{
  try {
    hosts _hosts;

    ptr<host> _host = _hosts.add("host1", "172.152.92.27");
    _hosts.add("host2", "172.152.92.123");

    ptr<server> _server = _host->add_server("srv000");
    _host->add_server("srv001");

    ptr<database> _db = _server->add_database("db000");
    _server->add_database("db001");

    ptr<table> _employee = _db->add_table("employee");
    ptr<table> _dependent = _db->add_table("dependent");
    ptr<table> _project = _db->add_table("project");
    ptr<table> _employees_projects = _db->add_table("employees_projects");

    std::cout << _hosts << std::endl;

  } catch (std::exception& _ex) {
    std::cerr << "ERROR: " << _ex.what() << std::endl;
  }
}
