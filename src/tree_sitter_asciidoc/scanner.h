#ifndef TREE_SITTER_ASCIIDOC_SCANNER_H
#define TREE_SITTER_ASCIIDOC_SCANNER_H

#define STACK_SIZE 12

#include <tree_sitter/parser.h>

typedef struct ADOCScanner ADOCScanner;

enum Contexts {
  V_MONOSPACE,                      // ``
  V_EMPHASIS,                       // __
  V_STRONG,                         // **
  V_SUPERSCRIPT,                    // ^^
  V_SUBSCRIPT,                      // ~~
  V_HIGHLIGHT,                      // ##
  V_PASSTHROUGH,                    // ++
  V_ATTRIBUTE,                      // ::
  V_INCLUDE_LOCATOR,                //
  V_XREF_LOCATOR,                   //
  V_OPTIONBLOCK,                    // []
  V_OPTIONINLINE,                   // []
  V_CROSSREFERENCE,                 // <<>>
  V_CROSSREFERENCE_ID,              // REMOVE?
  V_TABLE1,                         // |===
  V_TABLE2,                         // !===
  V_CALLOUT,                        // <.> TODO
  V_LITERAL,                        //
  V_LISTING,                        //
  V_BLOCK,                          //
  CONTEXT_MAX_VALUE
};

typedef struct {
  const char* value;
  bool match;
} Match;

typedef struct {
  int32_t stack[STACK_SIZE];
  bool context[CONTEXT_MAX_VALUE];
} lookbehind_s;

struct ADOCScanner {
  TSLexer* lexer;
  const bool* valid_symbols;

  int32_t lookahead;
  lookbehind_s* lookbehind;

  void (*advance)(ADOCScanner* scanner);
  void (*skip)(ADOCScanner* scanner);

  unsigned (*serialize)(ADOCScanner* scanner, char* buffer);
  void (*deserialize)(ADOCScanner* scanner, const char* buffer, unsigned length);
  bool (*scan)(ADOCScanner* scanner);
  bool (*eof)(TSLexer const* TSLexer);
};

static ADOCScanner* adoc_scanner_create();
static void adoc_scanner_destroy(ADOCScanner* scanner);

static void shift(ADOCScanner* scanner, int32_t value);
static bool match_before(ADOCScanner* scanner, char* value, size_t length, bool (*func_ptr)(int32_t*, size_t) );

static void adoc_scanner_advance(ADOCScanner* scanner);
static void adoc_scanner_skip(ADOCScanner* scanner);

static unsigned adoc_scanner_serialize(ADOCScanner* scanner, char* buffer);
static void adoc_scanner_deserialize(ADOCScanner* scanner, const char* buffer, unsigned length);

static void consume(ADOCScanner* scanner, int length);
static Match is_keyword(ADOCScanner* scanner, int length);
static bool adoc_scanner_scan(ADOCScanner* scanner);

#define SET_PARSE(CHR,IN_RANGE,TOKEN_START,TOKEN_END)             \
  if (                                                            \
    valid_symbols[TOKEN_START] &&                                 \
    !(context[IN_RANGE]) &&                                       \
    !(context[V_XREF_LOCATOR]) &&                                 \
    !(context[V_MONOSPACE]) &&                                    \
    !(context[V_LITERAL]) &&                                      \
    !(context[V_LISTING]) &&                                      \
    scanner->lookahead == CHR                                     \
  ) {                                                             \
    if (                                                          \
      is_newline(scanner->lookbehind->stack[0]) ||                \
      is_space(scanner->lookbehind->stack[0]) ||                  \
      is_punctuation(scanner->lookbehind->stack[0]) ||            \
      is_inline_markup_start_char(scanner->lookbehind->stack[0])  \
    ) {                                                           \
      scanner->advance(scanner);                                  \
      if(                                                         \
        is_space(scanner->lookahead)                              \
      ) {                                                         \
        return false;                                             \
      }                                                           \
      context[IN_RANGE] = true;                                   \
      lexer->result_symbol = TOKEN_START;                         \
      return true;                                                \
    }                                                             \
  }                                                               \
  if(                                                             \
    valid_symbols[TOKEN_END] &&                                   \
    context[IN_RANGE] &&                                          \
    !(context[V_MONOSPACE]) &&                                    \
    !(context[V_LITERAL]) &&                                      \
    !(context[V_LISTING]) &&                                      \
    scanner->lookahead == CHR                                     \
  ) {                                                             \
    if (                                                          \
      is_space(scanner->lookbehind->stack[0])                     \
    ) {                                                           \
      return false;                                               \
    }                                                             \
    scanner->advance(scanner);                                    \
    if (                                                          \
      !is_word(scanner->lookahead)                                \
    ) {                                                           \
      context[IN_RANGE] = false;                                  \
      lexer->result_symbol = TOKEN_END;                           \
      return true;                                                \
    }                                                             \
  }                                                               \

#endif /* ifndef TREE_SITTER_ASCIIDOC_SCANNER_H */
