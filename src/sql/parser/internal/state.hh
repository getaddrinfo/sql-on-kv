#ifndef _SQL_PARSER_INTERNAL_STATE_HH
#define _SQL_PARSER_INTERNAL_STATE_HH

#include "where/parser.hh"

namespace sql::parser::detail {
  // This will eventually be made into a concrete statement.
  // We need to be able to incrementally update this, since we can't
  // guarantee what order certain statements come in.
  class incomplete_parse_state__ {
    std::optional<std::unique_ptr<sql::parser::statement::where>> _where;

  public:
    void set_where();
    void set_order_by();

    bool has_where();
    bool has_order_by();
  };

}

#endif /* _SQL_PARSER_INTERNAL_STATE_HH */