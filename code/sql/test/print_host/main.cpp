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
#include <sql/entities/internal/types.h>
#include <sql/entities/primary_key.h>
#include <sql/entities/server.h>
#include <sql/entities/table.h>

using namespace capemisa::sql::entities;

void
create_employee(ptr<database> p_db)
{
  ptr<table> _employee = p_db->add_table("employee");

  ptr<primary_key> _pk = _employee->get_primary_key();

  _pk->add_pk_column("id", column::type::INT_4, true, true);

  _employee->add_attribute(
    "name", column::type::VAR_SIZE_CHAR, 100, false, false);
  _employee->add_attribute("birthday", column::type::DATE, false, false);

  ptr<foreign_key> _employee_boss_fk = _employee->add_fk("boss_fk");

  _employee_boss_fk->add_column("boss_id", _pk->find_pk_column("id"));
}

void
create_dependent(ptr<database> p_db)
{
  ptr<table> _dependent = p_db->add_table("dependent");
  _dependent->get_primary_key()->add_pk_column(
    "id", column::type::INT_4, true, true);

  _dependent->add_attribute(
    "name", column::type::VAR_SIZE_CHAR, 100, false, false);
  _dependent->add_attribute("birthday", column::type::DATE, false, false);

  ptr<table> _employee = p_db->find("employee");
  ptr<primary_key> _employee_pk = _employee->get_primary_key();

  ptr<foreign_key> _dependent_fk = _dependent->add_fk("employee_fk");
  _dependent_fk->add_column("responsible", _employee_pk->find_pk_column("id"));

  _dependent->get_primary_key()->add_fk_column(
    _dependent_fk->find_column("responsible"));
}

void
create_project(ptr<database> p_db)
{
  ptr<table> _project = p_db->add_table("project");
  _project->get_primary_key()->add_pk_column(
    "id", column::type::INT_4, true, true);
  _project->add_attribute(
    "name", column::type::VAR_SIZE_CHAR, 100, false, false);

  _project->get_primary_key()->add_pk_column(
    "id", column::type::INT_4, true, true);
}

void
create_employeess_projects(ptr<database> p_db)
{
  ptr<table> _employees_projects = p_db->add_table("employees_projects");

  ptr<table> _employee = p_db->find("employee");
  ptr<foreign_key> _employee_fk = _employees_projects->add_fk("employee_fk");
  _employee_fk->add_column("employee_id",
                           _employee->get_primary_key()->find_pk_column("id"));

  ptr<table> _project = p_db->find("project");
  ptr<foreign_key> _project_fk = _employees_projects->add_fk("project_fk");
  _project_fk->add_column("project_id",
                          _project->get_primary_key()->find_pk_column("id"));

  ptr<primary_key> _employees_projects_pk =
    _employees_projects->get_primary_key();

  _employees_projects_pk->add_fk_column(
    _employee_fk->find_column("employee_id"));

  _employees_projects_pk->add_fk_column(_project_fk->find_column("project_id"));
}

int
main()
{
  try {
    hosts _hosts;

    ptr<host> _host = _hosts.add("host1", "172.152.92.27");

    ptr<server> _server = _host->add_server("srv000");

    ptr<database> _db = _server->add_database("db000");

    create_employee(_db);
    create_dependent(_db);
    create_project(_db);
    create_employeess_projects(_db);

    std::cout << _hosts << std::endl;

  } catch (std::exception& _ex) {
    std::cerr << "ERROR: " << _ex.what() << std::endl;
  }
}
