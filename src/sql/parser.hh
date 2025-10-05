#ifndef _SQL_PARSER_HH
#define _SQL_PARSER_HH

#include "sql/lexer.hh"

#include <variant>

namespace sql::parser {

  namespace ty {
    struct Text {
      uint32_t max_length;
    };

    struct Char {
      uint32_t max_length;
    };

    struct VarChar {
      uint32_t max_length;
    };

    struct Uint8 {};
    struct Uint16 {};
    struct Uint32 {};
    struct Uint64{};
    struct Int8 {};
    struct Int16 {};
    struct Int32 {};
    struct Int64 {};
    struct Bool {};
    struct Bit {};
  };

  using SqlType = std::variant<
    ty::Text,
    ty::Char,
    ty::VarChar,
    ty::Uint8,
    ty::Uint16,
    ty::Uint32,
    ty::Uint64,
    ty::Int8,
    ty::Int16,
    ty::Int32,
    ty::Int64
  >;

  namespace statement {
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

      // const std::string& column() const;
      // const Operator op() const;
      // const std::string& value() const;
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

  // TODO: Each member of this should be a `std::unique_ptr`
  // since stack allocating these objects could get large.
  //
  // TODO: Should this really be done using polymorphism?
  // 
  // We still need to do the discrimination upon the base class
  // using either a `type()` method, or 
  // `dynamic_cast<Desired*>(ptr) != nullptr`.
  using ParseResult = std::variant<
    statement::select,
    statement::update,
    statement::insert,
    statement::delete_row,
    statement::create_table,
    statement::drop_table
  >;

  ParseResult parse(const std::vector<sql::lexer::Token>& tokens);

  namespace detail {
    // This will eventually be made into a concrete statement.
    // We need to be able to incrementally update this, since we can't
    // guarantee what order certain statements come in.
    class incomplete_parse_state__ {};

    class token_reader__ {
    private:
      const std::vector<sql::lexer::Token>& _tokens;
      size_t _index;

    public:
      token_reader__(
        const std::vector<sql::lexer::Token>& tokens
      ) : _tokens(tokens), _index(0) {}

      bool empty() const;

      const sql::lexer::Token& token() const;
      void next();
      void next_non_space();
      
      void back();

      void must(sql::lexer::TokenType ty);
      void must_one_of(
        std::initializer_list<sql::lexer::TokenType> tys
      );
      
      bool is(sql::lexer::TokenType ty);
      bool is_one_of(
        std::initializer_list<sql::lexer::TokenType> tys
      );
    };    
    
    void trim_left(token_reader__& reader);

    statement::select parse_select(token_reader__& reader);
    statement::where parse_where(token_reader__& reader);


    // statement::update parse_update(token_reader__& reader);
    // statement::insert parse_insert(token_reader__& reader);
    // statement::delete_row parse_delete(token_reader__& reader);
    // statement::drop_table parse_drop(token_reader__& reader);
    // statement::create_table parse_crete(token_reader__& reader);
  }
}

#endif /* _SQL_PARSER_HH */