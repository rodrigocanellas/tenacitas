#ifndef GENERATOR_DEFINITION_WINDOW_FACTORY_H
#define GENERATOR_DEFINITION_WINDOW_FACTORY_H

#include <cstdint>

#include <QMainWindow>
#include <QWidget>

#include <sql/applications/insert_generator/fixed_value_generator.h>
#include <sql/applications/insert_generator/number_value_generator_definition.h>
#include <sql/applications/insert_generator/table_insert_generator.h>
#include <sql/applications/insert_generator/text_value_generator_definition.h>
#include <sql/business/fixed_value_generator.h>
#include <sql/business/number_value_generator.h>
#include <sql/business/text_value_generator.h>
#include <sql/generic/name.h>

using namespace capemisa::sql::business;
using namespace capemisa::sql::entities;
using namespace capemisa::sql::generic;

struct generator_definition_window_factory
{

  template<typename t_column>
  QDialog* create(const name& p_name,
                  TableInsertGenerator* p_TableInsertGenerator)
  {
    if (p_name == number_value_generator<int8_t, t_column>::id) {
      if (m_NumberValueGeneratorDefinition == nullptr) {
        m_NumberValueGeneratorDefinition = new NumberValueGeneratorDefinition(
          p_TableInsertGenerator, (QWidget*)p_TableInsertGenerator);
      }
      return m_NumberValueGeneratorDefinition;
    }
    if (p_name == text_value_generator<t_column>::id) {
      if (m_TextValueGeneratorDefinition == nullptr) {
        m_TextValueGeneratorDefinition = new TextValueGeneratorDefinition(
          p_TableInsertGenerator, (QWidget*)p_TableInsertGenerator);
      }
      return m_TextValueGeneratorDefinition;
    }
    if (p_name == fixed_value_generator::id) {
      if (m_FixedValueGenerator == nullptr) {
        m_FixedValueGenerator = new FixedValueGenerator(
          p_TableInsertGenerator, (QWidget*)p_TableInsertGenerator);
      }
      return m_FixedValueGenerator;
    }
    return nullptr;
  }

  ~generator_definition_window_factory()
  {
    if (m_NumberValueGeneratorDefinition != nullptr) {
      delete m_NumberValueGeneratorDefinition;
    }
  }

private:
  NumberValueGeneratorDefinition* m_NumberValueGeneratorDefinition = nullptr;
  TextValueGeneratorDefinition* m_TextValueGeneratorDefinition = nullptr;
  FixedValueGenerator* m_FixedValueGenerator = nullptr;
};

#endif // GENERATOR_DEFINITION_WINDOW_FACTORY_H