#ifndef _SQL_PARSER_STATEMENT_HH
#define _SQL_PARSER_STATEMENT_HH

#include <memory>
#include <string>
#include <vector>

#include "type.hh"

namespace sql::parser::statement {
  enum class Operator {
    Equals,
    Ne,
    And,
    Or
  };
  
  class where {};

  class condition : public where {
  private:
    std::unique_ptr<where> _lhs;
    Operator _operator;
    std::unique_ptr<where> _rhs;
  };

  class comparison : public where {
  private:
    const std::string _column;
    Operator _operator;
    const std::string _value;

  public:
    comparison(
      const std::string column,
      Operator op,
      const std::string value
    ) : _column(column), _operator(op), _value(value) {}

    const std::string& column() const;
    const Operator op() const;
    const std::string& value() const;
  };


  class column_def {
  private:
    const std::string _name;
    const SqlType _type;

  public:
    const std::string& name() const;
    const SqlType& type() const;
  };


  class select {
  private:
    const std::string _from;
    const std::vector<std::string> _columns;
    const std::optional<where> _condition;
  
  public:
    const std::string& from() const;
    const std::vector<std::string>& columns() const;
    const std::optional<where> condition() const;
  };

  class insert {
  private:
    const std::string _into;
    const std::vector<std::string> _columns;
    const std::vector<std::vector<std::string>> _values;
    const std::optional<where> _condition;

  public:
    const std::string& into() const;
    const std::vector<std::string>& columns() const;
    const std::vector<std::vector<std::string>>& values() const;
    const std::optional<where> condition() const;
  };

  class update {
  private:
    const std::string _table;
    const std::vector<std::string> _columns;
    const std::vector<std::string> _to;
    const std::optional<where> _condition;
  public:
    const std::string& table() const;
    const std::vector<std::string>& columns() const;
    const std::vector<std::string>& to() const;
    const std::optional<where> condition() const;
  };



  class delete_row {
  private:
    const std::string _table;
    const std::optional<where> _condition;
  public:
    const std::string& table() const;
    const std::optional<where> condition() const;
  };

  class create_table {
  private:
    const std::string _name;
    const std::vector<column_def> _columns;

  public:
    const std::string& name() const;
    const std::vector<column_def>& columns() const;
  };

  class drop_table {
  private:
    const std::string _table;

  public:
    const std::string& table() const;
  };
}

#endif /* _SQL_PARSER_STATEMENT_HH */