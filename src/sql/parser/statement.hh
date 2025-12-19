#ifndef _SQL_PARSER_STATEMENT_HH
#define _SQL_PARSER_STATEMENT_HH

#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "type.hh"

#include "absl/log/check.h"

/**
 * Holds all the classes and types relevant to statements and
 * their structure.
 */
namespace sql::parser::statement {

  /**
   * Represents a logical operator applied within a comparison statement.
   */
  enum class Operator {
    /**
     * = (e.g., `field = "value")
     */
    Equals,
    
    /**
     * != (e.g., `field != "value")
     */
    Ne,

    /**
     * AND (e.g., `field = "value" AND field != "value"`)
     */
    And,

    /**
     * OR (e.g., `field = "value" OR field != "value"`)
     */
    Or
  };

  const std::string& operator_name(const Operator op);
  const size_t operator_precedence(const Operator op);
  
  /**
   * A base class to hold `condition` and `comparison` in a tree
   * for a WHERE clause.
   * 
   * Must be `dynamic_cast`ed to `condition` or `where`.
   */
  class Where {
  public:
    virtual ~Where() {}
  };

  /**
   * Represents a condition that contains subconditions, or comparisons as
   * left and right children, and an operator that compares the left and right side.
   */
  class Condition : public Where {
  private:
    /**
     * The left hand side of this condition.
     */
    std::shared_ptr<Where> _lhs;

    /**
     * The operator for this condition.
     */
    Operator _operator;

    /**
     * The right hand side of this condition.
     */
    std::shared_ptr<Where> _rhs;

  public:
    Condition(
      std::shared_ptr<Where> lhs,
      Operator op,
      std::shared_ptr<Where> rhs
    ) : _lhs(lhs), _operator(op), _rhs(rhs) {}

    std::shared_ptr<Where> lhs() const;
    std::shared_ptr<Where> rhs() const;
    const Operator op() const;
  };

  class Comparison : public Where {
  private:
    /**
     * The column that the comparison is applied to.
     */
    const std::string _column;

    /**
     * The operator being used to compare with.
     */
    Operator _operator;

    /**
     * The value to compare against.
     */
    const std::string _value;

  public:
    Comparison(
      const std::string column,
      Operator op,
      const std::string value
    ) : _column(column), _operator(op), _value(value) {}

    /**
     * Gets a const reference to the column name.
     */
    const std::string& column() const;

    /**
     * Gets the operator that this comparison is being
     * performed with.
     */
    const Operator op() const;
    
    /**
     * Gets a const reference to the value being compared against.
     */
    const std::string& value() const;
  };


  /**
   * Represents a definition of a column: its type, and its name (and eventually
   * any constraints or dependencies).
   */
  class ColumnDef {
  private:
    /**
     * The name of the column.
     */
    const std::string _name;

    /**
     * The type that this column holds.
     */
    const SqlType _type;

  public:
    /** 
     * Gets a const reference to the name of the column. 
     */
    const std::string& name() const;

    /**
     * Gets a const reference to the type of the column.
     */
    const SqlType& type() const;
  };


  /**
   * Represents a `SELECT` statement.
   */
  class Select {
  private:
    /**
     * The name of the table to select from.
     */
    const std::string _from;

    /**
     * The columns to select.
     */
    const std::vector<std::string> _columns;

    /**
     * The condition to filter upon.
     */
    const std::optional<std::shared_ptr<Where>> _condition;
  
  public:
    /**
     * Gets a const reference to the name of the table 
     * to select from.
     */
    const std::string& from() const;

    /**
     * Gets a const reference to the vector of names of columns
     * to select.
     */
    const std::vector<std::string>& columns() const;

    /**
     * Gets a const reference to the where clause on this query.
     */
    const std::optional<std::shared_ptr<Where>>& condition() const;
  };


  /**
   * Represents an `INSERT INTO` query.
   */
  class Insert {
  private:
    /**
     * The table to insert into.
     */
    const std::string _into;

    /**
     * The columns to insert into.
     */
    const std::vector<std::string> _columns;
    
    /**
     * The vector of values to insert into.
     * 
     * The inner vector gets its keys from the `_columns` vector. Think of 
     * `_columns` as a table heading, and `_values` as rows and columns in that table.
     * 
     * It must hold that `_values[n].size()` == `_columns.size()` for all `n` in `0 .. _columns.size()`.
     * TODO: Add a `CHECK` to guarantee this.
     */
    const std::vector<std::vector<std::string>> _values;

  public:
    /**
     * Gets a const reference to the name of the table to insert into.
     */
    const std::string& into() const;

    /**
     * Gets a const reference to the vector of column names to insert into.
     */
    const std::vector<std::string>& columns() const;

    /**
     * Gets a const reference to the vector of values to insert.
     */
    const std::vector<std::vector<std::string>>& values() const;
  };

  /**
   * Represents an `UPDATE` query.
   */
  class Update {
  private:
    /**
     * The name of the table to insert into.
     */
    const std::string _table;

    /**
     * The name of the colums to insert into.
     */
    const std::vector<std::string> _columns;

    /**
     * The values to update the rows in the table to.
     * 
     * TODO: This needs to be more complex, e.g., `a = a + 1` is valid and this can't represent that.
     */
    const std::vector<std::string> _to;

    /**
     * An optional condition to filter using.
     */
    const std::optional<std::shared_ptr<Where>> _condition;
  public:

    /**
     * Gets a const reference to the name of the table to update.
     */
    const std::string& table() const;

    /**
     * Gets a const reference to the vector of column names to update.
     */
    const std::vector<std::string>& columns() const;

    /**
     * Gets a const reference to the vector of values to change the columns to.
     */
    const std::vector<std::string>& to() const;

    /**
     * Gets a const reference to the optional condition that is used to filter
     * this update statement.
     */
    const std::optional<std::shared_ptr<Where>>& condition() const;
  };



  /**
   * Represents a `DELETE` query.
   */
  class DeleteRow {
  private:
    /**
     * The name of the table to delete from.
     */
    const std::string _table;

    /**
     * The condition to delete using.
     */
    const std::optional<std::shared_ptr<Where>> _condition;
  public:
    /**
     * Gets a const reference to the name of the table to delete from.
     */
    const std::string& table() const;

    /**
     * Gets a const reference to the WHERE clause.
     */
    const std::optional<std::shared_ptr<Where>>& condition() const;
  };

  /**
   * Represents the `CREATE TABLE ... (...)` command. 
   */
  class CreateTable {
  private:
    /**
     * The name of the table to create.
     */
    const std::string _name;

    /**
     * The definitions of columns to create the table with.
     */
    const std::vector<ColumnDef> _columns;

  public:
    /**
     * Gets a const reference to the name of the table to create.
     */
    const std::string& name() const;

    /**
     * Gets a const reference to the columns to create the table with.
     */
    const std::vector<ColumnDef>& columns() const;
  };

  /**
   * Represents the `DROP TABLE` command.
   */
  class DropTable {
  private:
    /** 
     * The name of the table to drop
     */
    const std::string _table;

  public:
    /**
     * Gets a const reference to the name of the table to drop.
     */
    const std::string& table() const;
  };
}

#endif /* _SQL_PARSER_STATEMENT_HH */