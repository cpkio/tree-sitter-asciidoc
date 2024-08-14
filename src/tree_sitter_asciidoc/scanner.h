#ifndef TREE_SITTER_ASCIIDOC_SCANNER_H
#define TREE_SITTER_ASCIIDOC_SCANNER_H

#define STACK_SIZE 12

#include <tree_sitter/parser.h>

typedef struct ADOCScanner ADOCScanner;

#define IM_SECTION1         1ULL << 63ULL // 10000000                                                                = Section
#define IM_SECTION2         1ULL << 62ULL // 01000000                                                                == Section
#define IM_SECTION3         1ULL << 61ULL // 00100000                                                                === Section
#define IM_SECTION4         1ULL << 60ULL // 00010000                                                                ==== Section
#define IM_SECTION5         1ULL << 59ULL // 00001000                                                                ===== Section
#define IM_SECTION6         1ULL << 58ULL // 00000100                                                                ====== Section
#define IM_ATTRIBUTE        1ULL << 57ULL // 00000010                                                                :my-attribute:
#define IM_OPTIONBLOCK      1ULL << 56ULL // 00000001                                                                [#id], [%header] etc

#define IM_OPTIONINLINE     1ULL << 55ULL //          10000000 00000000                                              []text; по-моему там особенность в том, что допускаются только короткие варианты: [.role#id%option]#some#
#define IM_MACROPARAMETER   1ULL << 54ULL //          01000000 00000000                                              text[]
#define IM_OPTIONSEPARATOR  1ULL << 53ULL //          00100000 00000000                                              `,` в [], <<,>> и возможно других контекстах
#define IM_TABLE1BLOCK      1ULL << 52ULL //          00010000 00000000                                              |===
#define IM_TABLE1CELL       1ULL << 51ULL //          00001000 00000000                                              `|`
#define IM_TABLE2BLOCK      1ULL << 50ULL //          00000100 00000000                                              !=== я пока не поддержваю ,=== и :=== как виды таблиц
#define IM_TABLE2CELL       1ULL << 49ULL //          00000010 00000000                                              `!`
#define IM_EXAMPLE1BLOCK    1ULL << 48ULL //          00000001 00000000                                              ====
#define IM_EXAMPLE2BLOCK    1ULL << 47ULL //          00000000 10000000                                              =====
#define IM_EXAMPLE3BLOCK    1ULL << 46ULL //          00000000 01000000                                              ======
#define IM_LISTINGBLOCK     1ULL << 45ULL //          00000000 00100000                                              ----
#define IM_LITERALBLOCK     1ULL << 44ULL //          00000000 00010000                                              ....
#define IM_OPENBLOCK        1ULL << 43ULL //          00000000 00001000                                              --
#define IM_SIDEBARBLOCK     1ULL << 42ULL //          00000000 00000100                                              ****
#define IM_PASSBLOCK        1ULL << 41ULL //          00000000 00000010                                              ++++
#define IM_QUOTEBLOCK       1ULL << 40ULL //          00000000 00000001                                              ____

#define IM_PASSTHROUGH      1ULL << 0ULL  //                                                                00000001
#define IM_HIGHLIGHT        1ULL << 1ULL  //                                                                00000010
#define IM_SUBSCRIPT        1ULL << 2ULL  //                                                                00000100
#define IM_SUPERSCRIPT      1ULL << 3ULL  //                                                                00001000
#define IM_MONOSPACE        1ULL << 4ULL  //                                                                00010000
#define IM_EMPHASIS         1ULL << 5ULL  //                                                                00100000
#define IM_STRONG           1ULL << 6ULL  //                                                                01000000
#define IM_COMMENT          1ULL << 7ULL  //                                                                10000000

#define IM_UNORDEREDLIST    1ULL << 08ULL //                            00000000 00000000 00000000 00000001
#define IM_ORDEREDLIST      1ULL << 09ULL //                            00000000 00000000 00000000 00000010
#define IM_DESCRIPTIONLIST  1ULL << 10ULL //                            00000000 00000000 00000000 00000100
#define IM_COMMENTBLOCK     1ULL << 11ULL //                            00000000 00000000 00000000 00001000
#define IM_CROSSREFERENCE   1ULL << 12ULL //                            00000000 00000000 00000000 00010000          <<#identifier,text>>
#define IM_XREFMACRO        1ULL << 13ULL //                            00000000 00000000 00000000 00100000          == также устанавливается (?) для inlcude::, image::
#define IM_CONDITIONAL      1ULL << 14ULL //                            00000000 00000000 00000000 01000000          ifdef, ifndef, ifeval, endif
#define IM_ANCHORMACRO      1ULL << 15ULL //                            00000000 00000000 00000000 10000000          anchor:text[] == [#anchortext]

#define IM_PASSMACRO        1ULL << 63ULL //                            00000000 00000000 00000000                   pass:
#define IM_ICONMACRO        1ULL << 63ULL //                            00000000 00000000 00000000                   icon:
#define IM_LINKMACRO        1ULL << 63ULL //                            00000000 00000000 00000000                   link:
#define IM_MAILTOMACRO      1ULL << 63ULL //                            00000000 00000000 00000000                   mailto:
#define IM_MATHMACRO        1ULL << 63ULL //                            00000000 00000000 00000000                   stem:, latexmath:
#define IM_FOOTNOTEMACRO    1ULL << 63ULL //                            00000000 00000000 00000000                   footnote:
#define IM_IMAGEMACRO       1ULL << 63ULL //                            00000000 00000000 00000000                   icon:
#define IM_AUDIOMACRO       1ULL << 63ULL //                            00000000 00000000 00000000                   audio::

#define IM_VIDEOMACRO       1ULL << 63ULL //                            00000000 00000000 00000000                   video::
#define IM_UNKNOWN15        1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN14        1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN13        1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN12        1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN11        1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN10        1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN9         1ULL << 63ULL //                            00000000 00000000 00000000

#define IM_UNKNOWN8         1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN7         1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN6         1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN5         1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN4         1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN3         1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN2         1ULL << 63ULL //                            00000000 00000000 00000000
#define IM_UNKNOWN1         1ULL << 63ULL //                            00000000 00000000 00000000

#define IM_MASK              (scanner->lookbehind->mask)
#define GET_BIT(bitIndex)   ((IM_MASK  &   (bitIndex)) > 0)
#define SET_BIT(bitIndex)    (IM_MASK  |=  (bitIndex))
#define TOGGLE_BIT(bitIndex) (IM_MASK  ^=  (bitIndex))
#define CLEAR_BIT(bitIndex)  (IM_MASK  &= ~(bitIndex))

#define NOT_BLOCK !( GET_BIT(IM_TABLE1BLOCK) || GET_BIT(IM_TABLE1BLOCK) || GET_BIT(IM_TABLE2BLOCK) || GET_BIT(IM_EXAMPLE1BLOCK) || GET_BIT(IM_EXAMPLE2BLOCK) || GET_BIT(IM_EXAMPLE3BLOCK) || GET_BIT(IM_LISTINGBLOCK) || GET_BIT(IM_LITERALBLOCK) || GET_BIT(IM_OPENBLOCK) || GET_BIT(IM_SIDEBARBLOCK) || GET_BIT(IM_PASSBLOCK) || GET_BIT(IM_QUOTEBLOCK) || GET_BIT(IM_COMMENTBLOCK) || GET_BIT(IM_COMMENT) )

#define NOT_PLAIN !( GET_BIT(IM_LISTINGBLOCK) || GET_BIT(IM_LITERALBLOCK) || GET_BIT(IM_PASSBLOCK) || GET_BIT(IM_COMMENTBLOCK) || GET_BIT(IM_COMMENT) )

typedef struct {
  const char* value;
  bool match;
} Match;

typedef struct {
  int32_t stack[STACK_SIZE];
  unsigned long long int mask;
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
    !GET_BIT(IN_RANGE) &&                                         \
    !GET_BIT(IM_XREFMACRO) &&                                     \
    !GET_BIT(IM_MONOSPACE) &&                                     \
    !GET_BIT(IM_LITERALBLOCK) &&                                  \
    !GET_BIT(IM_LISTINGBLOCK) &&                                  \
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
      SET_BIT(IN_RANGE);                                          \
      lexer->result_symbol = TOKEN_START;                         \
      return true;                                                \
    }                                                             \
  }                                                               \
  if(                                                             \
    valid_symbols[TOKEN_END] &&                                   \
    GET_BIT(IN_RANGE) &&                                          \
    !GET_BIT(IM_MONOSPACE) &&                                     \
    !GET_BIT(IM_LITERALBLOCK) &&                                  \
    !GET_BIT(IM_LISTINGBLOCK) &&                                  \
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
      CLEAR_BIT(IN_RANGE);                                        \
      lexer->result_symbol = TOKEN_END;                           \
      return true;                                                \
    }                                                             \
  }                                                               \

#endif /* ifndef TREE_SITTER_ASCIIDOC_SCANNER_H */
