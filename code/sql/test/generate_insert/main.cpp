#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

#include <sql/entities/column.h>
#include <sql/entities/database.h>
#include <sql/entities/host.h>
#include <sql/entities/server.h>
#include <sql/entities/table.h>
#include <sql/entities/types.h>

using namespace capemisa::sql::entities;

int
main()
{
  host _host("host000", "172.310.67.012");
  ptr<server> _server = _host.add_server("srv000");
  ptr<database> _db0 = _server->add_database("db000");

  // employee
  ptr<table> _employee = _db0->add_table("employee");
  ptr<column> _id_employee;
  {
    _id_employee = _employee->add_column("id", column::type::INT_4);
    _id_employee->set_autoincrement(true);
    _employee->add_to_primary_key(_id_employee);
    _employee->add_column("name", column::type::VAR_SIZE_CHAR, 100);
    _employee->add_column("birthday", column::type::DATE);
  }

  // project
  ptr<table> _project = _db0->add_table("project");
  ptr<column> _id_project;
  {
    _id_project = _project->add_column("id", column::type::INT_4);
    _id_project->set_autoincrement(true);
    _project->add_to_primary_key(_id_project);

    _project->add_column("name", column::type::VAR_SIZE_CHAR, 100);
  }

  // dependent
  ptr<table> _dependent = _db0->add_table("dependent");
  ptr<column> _id_dependent;
  {
    _id_dependent = _dependent->add_column("id", column::type::INT_4);
    _id_dependent->set_autoincrement(true);
    _dependent->add_to_primary_key(_id_dependent);

    _dependent->add_column("name", column::type::VAR_SIZE_CHAR, 100);
    _dependent->add_column("birthday", column::type::DATE);
  }

  // employee - project
  ptr<table> _employee_project = _db0->add_table("employee-project");

  _dependent->add_foreign_key("employee", _employee->get_primary_key());

  _employee_project->add_foreign_key("employee", _employee->get_primary_key());
  _employee_project->add_foreign_key("project", _project->get_primary_key());

  _employee->add_foreign_key("boss", _employee->get_primary_key());

  std::cout << _host << std::endl;
}
