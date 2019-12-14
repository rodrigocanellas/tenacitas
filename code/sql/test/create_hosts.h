#ifndef CAPEMISA_SQL_TEST_CREATE_HOSTS_H
#define CAPEMISA_SQL_TEST_CREATE_HOSTS_H

#include <sql/entities/hosts.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace test {

using namespace capemisa::sql::entities;

struct create_hosts
{

  hosts operator()()
  {
    hosts _hosts;

    generic::ptr<host> _host = _hosts.add("host1", "172.152.92.27");

    generic::ptr<server> _server = _host->add_server("srv000");

    generic::ptr<database> _db = _server->add_database("db000");

    create_employee(_db);
    create_dependent(_db);
    create_project(_db);
    create_employeess_projects(_db);
    return _hosts;
  }

private:
  void create_employee(generic::ptr<database> p_db)
  {
    generic::ptr<table> _employee = p_db->add_table("employee");

    generic::ptr<primary_key> _pk = _employee->get_primary_key();

    _pk->add_pk_column("id", column_type::int_4, true, true);

    _employee->add_attribute(
      "name", column_type::var_size_text, 100, false, false);
    _employee->add_attribute("birthday", column_type::date, false, false);

    generic::ptr<foreign_key> _employee_boss_fk =
      _employee->add_fk("boss_fk", true);

    _employee_boss_fk->add_column("boss_id", _pk->find_pk_column("id"));
  }

  void create_dependent(generic::ptr<database> p_db)
  {
    generic::ptr<table> _dependent = p_db->add_table("dependent");
    _dependent->get_primary_key()->add_pk_column(
      "id", column_type::int_4, true, true);

    _dependent->add_attribute(
      "name", column_type::var_size_text, 100, false, false);
    _dependent->add_attribute("birthday", column_type::date, false, false);

    generic::ptr<table> _employee = p_db->find("employee");
    generic::ptr<primary_key> _employee_pk = _employee->get_primary_key();

    generic::ptr<foreign_key> _dependent_fk = _dependent->add_fk("employee_fk");
    _dependent_fk->add_column("responsible",
                              _employee_pk->find_pk_column("id"));

    _dependent->get_primary_key()->add_fk_column(
      _dependent_fk->find_column("responsible"));
  }

  void create_project(generic::ptr<database> p_db)
  {
    generic::ptr<table> _project = p_db->add_table("project");
    _project->get_primary_key()->add_pk_column(
      "id", column_type::int_4, true, true);
    _project->add_attribute(
      "name", column_type::var_size_text, 100, false, false);

    _project->get_primary_key()->add_pk_column(
      "id", column_type::int_4, true, true);
  }

  void create_employeess_projects(generic::ptr<database> p_db)
  {
    generic::ptr<table> _employees_projects =
      p_db->add_table("employees_projects");

    generic::ptr<table> _employee = p_db->find("employee");
    generic::ptr<foreign_key> _employee_fk =
      _employees_projects->add_fk("employee_fk");
    _employee_fk->add_column(
      "employee_id", _employee->get_primary_key()->find_pk_column("id"));

    generic::ptr<table> _project = p_db->find("project");
    generic::ptr<foreign_key> _project_fk =
      _employees_projects->add_fk("project_fk");
    _project_fk->add_column("project_id",
                            _project->get_primary_key()->find_pk_column("id"));

    generic::ptr<primary_key> _employees_projects_pk =
      _employees_projects->get_primary_key();

    _employees_projects_pk->add_fk_column(
      _employee_fk->find_column("employee_id"));

    _employees_projects_pk->add_fk_column(
      _project_fk->find_column("project_id"));
  }
};
}
}
}

#endif // CREATE_HOSTS_H
