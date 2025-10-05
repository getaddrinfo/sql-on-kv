#ifndef _SQL_PARSER_INTERNAL_STATE_HH
#define _SQL_PARSER_INTERNAL_STATE_HH

namespace sql::parser::detail {
  // This will eventually be made into a concrete statement.
  // We need to be able to incrementally update this, since we can't
  // guarantee what order certain statements come in.
  class incomplete_parse_state__ {};

}

#endif /* _SQL_PARSER_INTERNAL_STATE_HH */