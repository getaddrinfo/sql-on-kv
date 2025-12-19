#ifndef _KV_INTERNAL_TABLE_DEFINITION_HH
#define _KV_INTERNAL_TABLE_DEFINITION_HH

#include "sql/parser/type.hh"

#include "lib/flags.hh"

#include <string>
#include <vector>

namespace kv::detail::table::definition {
  enum class Flags : uint32_t {
    PrimaryKey,
    Unique
  };

  // TODO: These
  class constraint {};

  /**
   * Represents the definition of a column within a database.
   */
  class column { 
  private:
    const std::string _name;
    const sql::parser::SqlType _type;
    const std::vector<constraint> _constraints;
    const lib::flags<Flags> flags;

  public:
    const std::string& name() const;
    const sql::parser::SqlType& type() const;
    const std::vector<constraint>& constriants() const;
    const lib::flags<Flags> flags() const;
  };


  /**
   * Represents the definition of a table within a database.
   */
  class definition {
  private:
    const std::string _name;
    const std::vector<column> _columns;

  public:
    const std::string& name() const;
    const std::vector<column>& columns() const;
    const std::vector<column> primary_key() const;
    const std::vector<column> uniques() const;
    const std::vector<uint8_t> serialize() const;
  };

  
}

#endif /* _KV_INTERNAL_TABLE_DEFINITION_HH */