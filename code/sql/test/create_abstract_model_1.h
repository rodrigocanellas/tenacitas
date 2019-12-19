#ifndef CAPEMISA_SQL_TEST_CREATE_ABSTRACT_MODEL_H
#define CAPEMISA_SQL_TEST_CREATE_ABSTRACT_MODEL_H

#include <sql/entities/hosts.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace test {

using namespace capemisa::sql::entities;
using namespace capemisa::sql::generic;

struct create_abstract_model_1
{
  ptr<hosts> operator()()
  {
    ptr<hosts> _hosts(make_ptr<hosts>());
    generic::ptr<host> _host = _hosts->add("host1", "172.152.92.27");

    generic::ptr<server> _server = _host->add_server("srv000");

    generic::ptr<database> _db = _server->add_database("db000");

    ptr<table> _c = _db->add_table("C");
    {
      ptr<primary_key> _pk = _c->get_primary_key();
      _pk->add_pk_column("id", column_type::int_4, false, true);
      _c->add_attribute("c1", column_type::small_real, false, false);
      _c->add_attribute("c2", column_type::var_size_text, 100, false, false);
    }

    ptr<table> _b = _db->add_table("B");
    {
      ptr<primary_key> _pk = _b->get_primary_key();
      _pk->add_pk_column("id", column_type::int_4, false, true);
      _b->add_attribute("b1", column_type::small_real, false, false);
      _b->add_attribute("b2", column_type::var_size_text, 100, false, false);
      ptr<foreign_key> _fk = _b->add_fk("FK_C");
      _fk->add_column("c_id", _c->get_primary_key()->find_pk_column("id"));
    }

    ptr<table> _f = _db->add_table("F");
    {
      ptr<primary_key> _pk = _f->get_primary_key();
      _pk->add_pk_column("id", column_type::int_4, false, true);
      _f->add_attribute("f1", column_type::small_real, false, false);
      ptr<foreign_key> _fk = _f->add_fk("FK_B");
      _fk->add_column("b_id", _b->get_primary_key()->find_pk_column("id"));
    }

    ptr<table> _h = _db->add_table("H");
    {
      ptr<primary_key> _pk = _h->get_primary_key();
      _pk->add_pk_column("id", column_type::int_4, false, true);
      _h->add_attribute("h1", column_type::small_real, false, false);
    }

    ptr<table> _j = _db->add_table("J");
    {
      ptr<primary_key> _pk = _j->get_primary_key();
      _pk->add_pk_column("id", column_type::int_4, false, true);
      _j->add_attribute("j1", column_type::small_real, false, false);
    }

    ptr<table> _e = _db->add_table("E");
    {
      ptr<primary_key> _pk = _e->get_primary_key();
      _pk->add_pk_column("id", column_type::int_4, false, true);
      _e->add_attribute("e1", column_type::small_real, false, false);
      ptr<foreign_key> _fk = _e->add_fk("FK_F");
      _fk->add_column("f_id", _f->get_primary_key()->find_pk_column("id"));
      _fk = _e->add_fk("FK_H");
      _fk->add_column("h_id", _h->get_primary_key()->find_pk_column("id"));
      _fk = _e->add_fk("FK_J");
      _fk->add_column("j_id", _j->get_primary_key()->find_pk_column("id"));
    }

    ptr<table> _g = _db->add_table("G");
    {
      ptr<primary_key> _pk = _g->get_primary_key();
      _pk->add_pk_column("id", column_type::int_4, false, true);
      _e->add_attribute("g1", column_type::small_real, false, false);
    }

    ptr<table> _d = _db->add_table("D");
    {
      ptr<primary_key> _pk = _d->get_primary_key();
      _pk->add_pk_column("id", column_type::int_4, false, true);
      _d->add_attribute("d1", column_type::small_real, false, false);
      ptr<foreign_key> _fk = _d->add_fk("FK_E");
      _fk->add_column("e_id", _e->get_primary_key()->find_pk_column("id"));
      _fk = _d->add_fk("FK_G");
      _fk->add_column("g_id", _g->get_primary_key()->find_pk_column("id"));
    }

    ptr<table> _a = _db->add_table("A");
    {
      ptr<primary_key> _pk = _a->get_primary_key();
      _pk->add_pk_column("id", column_type::int_4, false, true);
      _a->add_attribute("a1", column_type::small_real, false, false);
      ptr<foreign_key> _fk_d = _a->add_fk("FK_D");
      _fk_d->add_column("d_id", _d->get_primary_key()->find_pk_column("id"));
      ptr<foreign_key> _fk_b = _a->add_fk("FK_B");
      _fk_b->add_column("b_id", _b->get_primary_key()->find_pk_column("id"));
    }

    return _hosts;
  }
};
}
}
}

#endif // CREATE_ABSTRACT_MODEL_H
