// vim:tw=0:sw=2:virtualedit=all
#include <tree_sitter/parser.h>
#include <stdio.h>
#include <string.h>

#include "chars.c"
#include "tokens.h"
#include "tree_sitter_asciidoc/chars.h"
#include "scanner.h"

static ADOCScanner* adoc_scanner_create()
{
  ADOCScanner* scanner = malloc(sizeof(ADOCScanner));

  scanner->advance = adoc_scanner_advance;
  scanner->skip = adoc_scanner_skip;

  scanner->lookbehind = calloc(sizeof(lookbehind_s), 1);

  scanner->serialize = adoc_scanner_serialize;
  scanner->deserialize = adoc_scanner_deserialize;
  scanner->scan = adoc_scanner_scan;

  return scanner;
}

static void adoc_scanner_destroy(ADOCScanner* scanner) {
  free(scanner->lookbehind);
  free(scanner);
}

static void shift(ADOCScanner* scanner, int32_t value) {
  int32_t* items = scanner->lookbehind->stack;
  memmove(&items[1], &items[0], (STACK_SIZE-1)*sizeof(int32_t));
  items[0] = value;
}

static void see(ADOCScanner* scanner) {
  int32_t* items = scanner->lookbehind->stack;
  printf(
        "[0]..[7] of `stack` | %c‧%c‧%c‧%c‧%c‧%c‧%c‧%c\n"
        "                    | %d‧%d‧%d‧%d‧%d‧%d‧%d‧%d\n\n",
            items[0],
            items[1],
            items[2],
            items[3],
            items[4],
            items[5],
            items[6],
            items[7],

            items[0],
            items[1],
            items[2],
            items[3],
            items[4],
            items[5],
            items[6],
            items[7]
  );
}

static bool match_before(ADOCScanner* scanner, char* value, size_t length, bool (*func_ptr)(int32_t*, size_t) ) {
    bool match = true;
    int32_t* items = scanner->lookbehind->stack;
    for (size_t i = 0; i < length && match; i++) {
        match = (value[length - i - 1] == items[i]);
    }
    bool match_end = func_ptr(items, length);
    return match && match_end;
}

static void adoc_scanner_advance(ADOCScanner* scanner)
{
  TSLexer* lexer = scanner->lexer;
  shift(scanner, lexer->lookahead);
  lexer->advance(lexer, false);
  scanner->lookahead = lexer->lookahead;
}

static void adoc_scanner_skip(ADOCScanner* scanner)
{
  TSLexer* lexer = scanner->lexer;
  shift(scanner, scanner->lookahead);
  lexer->advance(lexer, true);
  scanner->lookahead = lexer->lookahead;
}

static unsigned adoc_scanner_serialize(ADOCScanner* scanner, char* buffer)
{
  memcpy(buffer, scanner->lookbehind, sizeof(lookbehind_s));
  return sizeof(lookbehind_s);
}

static void adoc_scanner_deserialize(ADOCScanner* scanner, const char* buffer, unsigned length)
{
  if (buffer != NULL) {
    memcpy(scanner->lookbehind, (void*)buffer, sizeof(lookbehind_s));
  }
}

static void consume(ADOCScanner* scanner, int length) {
  if (length > 0) {
    for (size_t i = 0; i < length; i++) {
        scanner->advance(scanner);
    }
  } else {
    do {
      scanner->advance(scanner);
    } while ((scanner->lookbehind->stack[0]) ==
            (scanner->lookahead));
  }
}

static Match is_keyword(ADOCScanner* scanner, int position)
{
  const char* keywords[] = {
    STR_INCLUDE,
    STR_IMAGE,
    STR_IMAGE_INLINE,
    STR_XREF,
    STR_FOOTNOTE,
    STR_ANCHOR,
    STR_BTN,
    STR_KBD,
    STR_PASS,
    STR_ICON,
    STR_LINK,
    STR_MAILTO,
    STR_INDEXTERM,
    STR_STEM,
    STR_LATEXMATH,

    STR_ADMONITION_NOTE,
    STR_ADMONITION_TIP,
    STR_ADMONITION_IMPORTANT,
    STR_ADMONITION_CAUTION,
    STR_ADMONITION_WARNING,

    STR_IFDEF,
    STR_IFNDEF,
    STR_IFEVAL,
    STR_IFEND,
    STR_TAG,
    STR_TAGEND
  };
  const int length = sizeof(keywords) / sizeof(char*);
  int32_t* stack = scanner->lookbehind->stack;

  bool match = false;
  for (size_t i = 0; i < length && !match; i++) {
    const char* value = keywords[i];
    bool _match = true;

    for (size_t j = 0; j < position && j < strlen(value) && _match; j++) {
      _match = (value[j] == stack[position - j - 1]);
    }

    if (_match && value[position] == scanner->lookahead) {
      return (Match){ .match = true, .value = value };
    }
  }
  return (Match){ .match = false, .value = "NONE" };
}

bool m_newline(int32_t* value, size_t position) {
  return is_newline(*(value + position));
}
bool m_newline_or_space(int32_t* value, size_t position) {
  return is_newline(*(value + position)) &&
         is_space(*(value + position));
}
bool m_any(int32_t* value, size_t position) {
  return true;
}
bool m_emptyline(int32_t* value, size_t position) {
  return is_newline(*(value + position)) &&
         is_newline(*(value + position + 1));
}

static bool adoc_scanner_scan(ADOCScanner* scanner)
{
  TSLexer* lexer = scanner->lexer;
  const bool* valid_symbols = scanner->valid_symbols;
  bool (*match_newline)(int32_t*, size_t) = &m_newline;
  bool (*match_newline_or_space)(int32_t*, size_t) = &m_newline_or_space;
  bool (*match_any)(int32_t*, size_t) = &m_any;
  bool (*match_emptyline)(int32_t*, size_t) = &m_emptyline;

  if (
    valid_symbols[T_WHITESPACE] &&
    is_space(scanner->lookahead)
  ) {
    while (is_space(scanner->lookahead)) {
      scanner->advance(scanner);
    }
    lexer->result_symbol = T_WHITESPACE;
    return true;
  }

  if (
    valid_symbols[T_EMPTYLINE] &&
    is_newline(scanner->lookahead) &&
    is_newline(scanner->lookbehind->stack[0])
  ) {
    if (scanner->lookahead == CHAR_CARRIAGE_RETURN) scanner->skip(scanner);
    if (scanner->lookahead == CHAR_NEWLINE) {
        scanner->advance(scanner);
        lexer->result_symbol = T_EMPTYLINE;
        return true;
    }
  }
  if (
    valid_symbols[T_NEWLINE] &&
    is_newline(scanner->lookahead) &&
    is_newline(scanner->lookbehind->stack[0])
  ) {
    if (scanner->lookahead == CHAR_CARRIAGE_RETURN) scanner->skip(scanner);
    if (scanner->lookahead == CHAR_NEWLINE) {
        scanner->advance(scanner);
        lexer->result_symbol = T_NEWLINE;
        return true;
    }
  }
  if (
    valid_symbols[T_NEWLINE] &&
    is_newline(scanner->lookahead) &&
    !is_newline(scanner->lookbehind->stack[0])
  ) {
    if (scanner->lookahead == CHAR_CARRIAGE_RETURN) scanner->skip(scanner);
    if (scanner->lookahead == CHAR_NEWLINE || scanner->lookahead == CHAR_EOF) {
        scanner->advance(scanner);
        lexer->result_symbol = T_NEWLINE;
        CLEAR_BIT(IM_COMMENT);
        CLEAR_BIT(IM_DESCRIPTIONLIST);
        return true;
    }
  }

  if (
    (valid_symbols[T_ATTRIBUTE_MARKER_START] || valid_symbols[T_ATTRIBUTE_MARKER_START_NEG]) &&
    NOT_BLOCK &&
    scanner->lookahead == CHAR_ATTRIBUTE &&
    is_newline(scanner->lookbehind->stack[0])
  ) {
    scanner->advance(scanner);
    if (scanner->lookahead == CHAR_ATTRIBUTENEGATION) {
      scanner->advance(scanner);
      lexer->result_symbol = T_ATTRIBUTE_MARKER_START_NEG;
      SET_BIT(IM_ATTRIBUTE);
      return true;
    }
    lexer->result_symbol = T_ATTRIBUTE_MARKER_START;
    SET_BIT(IM_ATTRIBUTE);
    return true;
  }

  if (
    valid_symbols[T_ATTRIBUTE_MARKER_END_NEG] &&
    GET_BIT(IM_ATTRIBUTE) &&
    scanner->lookahead == CHAR_ATTRIBUTENEGATION &&
    !is_space(scanner->lookbehind->stack[0])
  ) {
    scanner->advance(scanner);
    if (scanner->lookahead == CHAR_ATTRIBUTE) {
      scanner->advance(scanner);
      CLEAR_BIT(IM_ATTRIBUTE);
      lexer->result_symbol = T_ATTRIBUTE_MARKER_END_NEG;
      return true;
    }
  }

  if (
    valid_symbols[T_ATTRIBUTE_MARKER_END] &&
    GET_BIT(IM_ATTRIBUTE) &&
    scanner->lookahead == CHAR_ATTRIBUTE &&
    !is_space(scanner->lookbehind->stack[0])
  ) {
    scanner->advance(scanner);
    CLEAR_BIT(IM_ATTRIBUTE);
    lexer->result_symbol = T_ATTRIBUTE_MARKER_END;
    return true;
  }

  if (
    (valid_symbols[T_COMMENT_MARKER] || valid_symbols[T_BLOCK_COMMENT_SEPARATOR]) &&
    scanner->lookahead == CHAR_COMMENT &&
    !GET_BIT(IM_LITERALBLOCK) &&
    !GET_BIT(IM_LISTINGBLOCK) &&
    !GET_BIT(IM_PASSBLOCK) &&
    is_newline(scanner->lookbehind->stack[0])
  ) {
    consume(scanner, -1);
    if(
      match_before(scanner, STR_BLOCK_COMMENT_SEPARATOR, strlen(STR_BLOCK_COMMENT_SEPARATOR), match_newline) &&
      is_newline(scanner->lookahead)
    ) {
      do {
        scanner->advance(scanner);
      } while (
        !match_before(scanner, STR_BLOCK_COMMENT_SEPARATOR, strlen(STR_BLOCK_COMMENT_SEPARATOR), match_newline) && scanner->lookahead != CHAR_EOF
      );
      lexer->result_symbol = T_BLOCK_COMMENT_SEPARATOR;
      TOGGLE_BIT(IM_COMMENTBLOCK);
      return true;
    }
    if(
      match_before(scanner, STR_COMMENT_MARKER, strlen(STR_COMMENT_MARKER), match_newline)
    ) {
      lexer->result_symbol = T_COMMENT_MARKER;
      SET_BIT(IM_COMMENT);
      return true;
    }
  }

  if (
    (valid_symbols[T_PAGE_BREAK] || valid_symbols[T_CALLOUT_MARKER_TO]) &&
    scanner->lookahead == CHAR_CROSSREFERENCE_START &&
    is_newline(scanner->lookbehind->stack[0])
  ) {
    scanner->advance(scanner); // '<'

    if (scanner->lookahead == CHAR_CROSSREFERENCE_START) { // '<<'
      consume(scanner, strlen(STR_PAGE_BREAK) - 1);
      if(
        match_before(scanner, STR_PAGE_BREAK, strlen(STR_PAGE_BREAK), match_newline) &&
        is_newline(scanner->lookahead)
      ) {
        scanner->advance(scanner);
        lexer->result_symbol = T_PAGE_BREAK;
        return true;
      }
    }
    if (scanner->lookahead == '.') { // '<.'
      scanner->advance(scanner);
      if (scanner->lookahead == CHAR_CROSSREFERENCE_END) {
        scanner->advance(scanner);
        if (is_space(scanner->lookahead)) {
          lexer->result_symbol = T_CALLOUT_MARKER_TO;
          return true;
        }
      }
    }
    if (is_number(scanner->lookahead)) {
      while (is_number(scanner->lookahead)) {
        scanner->advance(scanner);
      };
      if (scanner->lookahead == CHAR_CROSSREFERENCE_END) {
        scanner->advance(scanner);
        if (is_space(scanner->lookahead)) {
          lexer->result_symbol = T_CALLOUT_MARKER_TO;
          return true;
        }
      }
    }
  }
  // FROM
  if (
    valid_symbols[T_CALLOUT_MARKER_FROM] &&
    scanner->lookahead == CHAR_CROSSREFERENCE_START &&
    is_space(scanner->lookbehind->stack[0])
  ) {
    scanner->advance(scanner); // '<'
    if (scanner->lookahead == '.') { // '<.'
      scanner->advance(scanner);
      if (scanner->lookahead == CHAR_CROSSREFERENCE_END) {
        scanner->advance(scanner);
        if (is_newline(scanner->lookahead)) {
          lexer->result_symbol = T_CALLOUT_MARKER_FROM;
          return true;
        }
      }
    }
    if (is_number(scanner->lookahead)) {
      while (is_number(scanner->lookahead)) {
        scanner->advance(scanner);
      };
      if (scanner->lookahead == CHAR_CROSSREFERENCE_END) {
        scanner->advance(scanner);
        if (is_newline(scanner->lookahead)) {
          lexer->result_symbol = T_CALLOUT_MARKER_FROM;
          return true;
        }
      }
    }
  }

  if (
    (
      valid_symbols[T_BLOCK_EXAMPLE_SEPARATOR1] ||
      valid_symbols[T_BLOCK_EXAMPLE_SEPARATOR2] ||
      valid_symbols[T_BLOCK_EXAMPLE_SEPARATOR3] ||
      valid_symbols[T_TITLE_MARKER0] ||
      valid_symbols[T_TITLE_MARKER1] ||
      valid_symbols[T_TITLE_MARKER2] ||
      valid_symbols[T_TITLE_MARKER3] ||
      valid_symbols[T_TITLE_MARKER4] ||
      valid_symbols[T_TITLE_MARKER5]
    ) &&
    scanner->lookahead == CHAR_SECTIONHEADER &&
    is_newline(scanner->lookbehind->stack[0])
  ) {
    do {
      scanner->advance(scanner);
    } while (scanner->lookbehind->stack[0] ==
            scanner->lookahead);
    if (scanner->lookahead == CHAR_SECTIONHEADER) {
      scanner->advance(scanner);
    }
    if (
      NOT_PLAIN &&
      match_before(scanner, STR_BLOCK_EXAMPLE_SEPARATOR3, strlen(STR_BLOCK_EXAMPLE_SEPARATOR3), match_newline) &&
      is_newline(scanner->lookahead)
    ) {
      TOGGLE_BIT(IM_EXAMPLE3BLOCK);
      lexer->result_symbol = T_BLOCK_EXAMPLE_SEPARATOR3;
      return true;
    }
    if (
      NOT_PLAIN &&
      match_before(scanner, STR_BLOCK_EXAMPLE_SEPARATOR2, strlen(STR_BLOCK_EXAMPLE_SEPARATOR2), match_newline) &&
      is_newline(scanner->lookahead)
    ) {
      TOGGLE_BIT(IM_EXAMPLE2BLOCK);
      lexer->result_symbol = T_BLOCK_EXAMPLE_SEPARATOR2;
      return true;
    }
    if (
      NOT_PLAIN &&
      match_before(scanner, STR_BLOCK_EXAMPLE_SEPARATOR1, strlen(STR_BLOCK_EXAMPLE_SEPARATOR1), match_newline) &&
      is_newline(scanner->lookahead)
    ) {
      TOGGLE_BIT(IM_EXAMPLE1BLOCK);
      lexer->result_symbol = T_BLOCK_EXAMPLE_SEPARATOR1;
      return true;
    }

    if (
      NOT_BLOCK &&
      match_before(scanner, STR_SECTION_HEADER5, strlen(STR_SECTION_HEADER5), match_newline) &&
      is_space(scanner->lookahead)
    ) {
      lexer->result_symbol = T_TITLE_MARKER5;
      SET_BIT(IM_SECTION6);
      return true;
    }
    if (
      NOT_BLOCK &&
      match_before(scanner, STR_SECTION_HEADER4, strlen(STR_SECTION_HEADER4), match_newline) &&
      is_space(scanner->lookahead)
    ) {
      lexer->result_symbol = T_TITLE_MARKER4;
      SET_BIT(IM_SECTION5);
      CLEAR_BIT(IM_SECTION6);
      return true;
    }
    if (
      NOT_BLOCK &&
      match_before(scanner, STR_SECTION_HEADER3, strlen(STR_SECTION_HEADER3), match_newline) &&
      is_space(scanner->lookahead)
    ) {
      lexer->result_symbol = T_TITLE_MARKER3;
      SET_BIT(IM_SECTION4);
      CLEAR_BIT(IM_SECTION5);
      CLEAR_BIT(IM_SECTION6);
      return true;
    }
    if (
      NOT_BLOCK &&
      match_before(scanner, STR_SECTION_HEADER2, strlen(STR_SECTION_HEADER2), match_newline) &&
      is_space(scanner->lookahead)
    ) {
      lexer->result_symbol = T_TITLE_MARKER2;
      SET_BIT(IM_SECTION3);
      CLEAR_BIT(IM_SECTION4);
      CLEAR_BIT(IM_SECTION5);
      CLEAR_BIT(IM_SECTION6);
      return true;
    }
    if (
      NOT_BLOCK &&
      match_before(scanner, STR_SECTION_HEADER1, strlen(STR_SECTION_HEADER1), match_newline) &&
      is_space(scanner->lookahead)
    ) {
      lexer->result_symbol = T_TITLE_MARKER1;
      SET_BIT(IM_SECTION2);
      CLEAR_BIT(IM_SECTION3);
      CLEAR_BIT(IM_SECTION4);
      CLEAR_BIT(IM_SECTION5);
      CLEAR_BIT(IM_SECTION6);
      return true;
    }
    if (
      NOT_BLOCK &&
      !( GET_BIT(IM_SECTION2) || GET_BIT(IM_SECTION3) || GET_BIT(IM_SECTION4) || GET_BIT(IM_SECTION5) || GET_BIT(IM_SECTION6) ) &&
      match_before(scanner, STR_SECTION_HEADER0, strlen(STR_SECTION_HEADER0), match_newline) &&
      is_space(scanner->lookahead)
    ) {
      SET_BIT(IM_SECTION1);
      CLEAR_BIT(IM_SECTION2);
      CLEAR_BIT(IM_SECTION3);
      CLEAR_BIT(IM_SECTION4);
      CLEAR_BIT(IM_SECTION5);
      CLEAR_BIT(IM_SECTION6);
      lexer->result_symbol = T_TITLE_MARKER0;
      return true;
    }
  }

  if (
    (
      valid_symbols[T_BLOCK_LISTING_SEPARATOR] ||
      valid_symbols[T_BLOCK_OPEN_SEPARATOR] ||
      valid_symbols[T_THEMATIC_BREAK]
    ) &&
    scanner->lookahead == '-' &&
    is_newline(scanner->lookbehind->stack[0])
  ) {
    consume(scanner, -1);
    if (
      match_before(scanner, STR_BLOCK_LISTING_SEPARATOR, strlen(STR_BLOCK_LISTING_SEPARATOR), match_newline) &&
      is_newline(scanner->lookahead)
    ) {
      scanner->advance(scanner);
      TOGGLE_BIT(IM_LISTINGBLOCK);
      lexer->result_symbol = T_BLOCK_LISTING_SEPARATOR;
      return true;
    }
    if (
      match_before(scanner, STR_BLOCK_OPEN_SEPARATOR, strlen(STR_BLOCK_OPEN_SEPARATOR), match_newline) &&
      is_newline(scanner->lookahead)
    ) {
      scanner->advance(scanner);
      TOGGLE_BIT(IM_OPENBLOCK);
      lexer->result_symbol = T_BLOCK_OPEN_SEPARATOR;
      return true;
    }

    if (
      match_before(scanner, STR_THEMATIC_BREAK1, strlen(STR_THEMATIC_BREAK1), match_newline) &&
      is_newline(scanner->lookahead)
    ) {
      lexer->result_symbol = T_THEMATIC_BREAK;
      return true;
    }
  }

  if (
    (valid_symbols[T_BLOCK_LITERAL_SEPARATOR] || valid_symbols[T_BLOCK_TITLE_MARKER]) &&
    scanner->lookahead == '.' && is_newline(scanner->lookbehind->stack[0])
  ) {
    consume(scanner, -1);

    if (
      match_before(scanner, STR_BLOCK_LITERAL_SEPARATOR,
                    strlen(STR_BLOCK_LITERAL_SEPARATOR), match_newline) &&
      is_newline(scanner->lookahead)
    ) {
      TOGGLE_BIT(IM_LITERALBLOCK);
      lexer->result_symbol = T_BLOCK_LITERAL_SEPARATOR;
      return true;
    }

    if (
      match_before(scanner, ".", strlen("."), match_newline) &&
      !is_space(scanner->lookahead)
    ) {
      lexer->result_symbol = T_BLOCK_TITLE_MARKER;
      return true;
    }

  }

  if (
    (valid_symbols[T_BLOCK_SIDEBAR_SEPARATOR]
    || valid_symbols[T_THEMATIC_BREAK]
    || valid_symbols[T_STRONG_MARKER_START]
    || valid_symbols[T_STRONG_MARKER_END]) &&
    !(GET_BIT(IM_MONOSPACE)) &&
    !(GET_BIT(IM_LITERALBLOCK)) &&
    !(GET_BIT(IM_LISTINGBLOCK)) &&
    scanner->lookahead == CHAR_STRONG
  ) {
    do {
      scanner->advance(scanner);
    } while ((scanner->lookbehind->stack[0]) ==
            (scanner->lookahead));
    if (scanner->lookahead == CHAR_STRONG) {
      scanner->advance(scanner);
    }

    if (match_before(scanner, STR_BLOCK_SIDEBAR_SEPARATOR,
                    strlen(STR_BLOCK_SIDEBAR_SEPARATOR), match_newline) &&
        is_newline(scanner->lookahead)) {
      lexer->result_symbol = T_BLOCK_SIDEBAR_SEPARATOR;
      return true;
    }

    if (match_before(scanner, STR_THEMATIC_BREAK2, strlen(STR_THEMATIC_BREAK2),
                    match_newline) &&
        is_newline(scanner->lookahead)) {
      lexer->result_symbol = T_THEMATIC_BREAK;
      return true;
    }

    if (
      !(GET_BIT(IM_MONOSPACE)) &&
      !(GET_BIT(IM_PASSTHROUGH)) &&
      !(GET_BIT(IM_XREFMACRO)) &&
      (is_newline(scanner->lookbehind->stack[1]) ||
        is_space(scanner->lookbehind->stack[1]) ||
        is_punctuation(scanner->lookbehind->stack[1]) ||
        is_inline_markup_start_char(scanner->lookbehind->stack[1])) &&
      !is_space(scanner->lookahead)
    ) {
      SET_BIT(IM_STRONG);
      lexer->result_symbol = T_STRONG_MARKER_START;
      return true;
    }

    if (GET_BIT(IM_STRONG) &&
        !is_space(scanner->lookbehind->stack[1])) {
      if (is_newline(scanner->lookahead) || is_space(scanner->lookahead) ||
          is_punctuation(scanner->lookahead) ||
          is_inline_markup_end_char(scanner->lookahead)) {
        CLEAR_BIT(IM_STRONG);
        lexer->result_symbol = T_STRONG_MARKER_END;
        return true;
      }
    }
  }

  if (
    (
      valid_symbols[T_BLOCK_PASSTHROUGH_SEPARATOR] ||
      valid_symbols[T_BLOCK_CONTINUATION] ||
      valid_symbols[T_HARDBREAK] ||
      valid_symbols[T_INLINE_PASSTHROUGH_MARKER_START] ||
      valid_symbols[T_INLINE_PASSTHROUGH_MARKER_END]
    ) &&
    !(GET_BIT(IM_LITERALBLOCK)) &&
    !(GET_BIT(IM_LISTINGBLOCK)) &&
    scanner->lookahead == CHAR_INLINE_PASSTHROUGH
  ) {
    do {
      scanner->advance(scanner);
    } while ((scanner->lookbehind->stack[0]) ==
            (scanner->lookahead));
    if (scanner->lookahead == CHAR_INLINE_PASSTHROUGH) {
      scanner->advance(scanner);
    }

    if (match_before(scanner, STR_BLOCK_PASSTHROUGH_SEPARATOR,
                    strlen(STR_BLOCK_PASSTHROUGH_SEPARATOR), match_newline) &&
        is_newline(scanner->lookahead)) {
      scanner->advance(scanner);
      lexer->result_symbol = T_BLOCK_PASSTHROUGH_SEPARATOR;
      return true;
    }

    if (is_newline(scanner->lookbehind->stack[1]) &&
        is_newline(scanner->lookahead)) {
      scanner->advance(scanner);
      lexer->result_symbol = T_BLOCK_CONTINUATION;
      return true;
    }

    if (is_space(scanner->lookbehind->stack[1]) &&
        is_newline(scanner->lookahead)) {
      lexer->result_symbol = T_HARDBREAK;
      return true;
    }

    if (!(GET_BIT(IM_PASSTHROUGH)) &&
        !(GET_BIT(IM_XREFMACRO)) &&
        (is_newline(scanner->lookbehind->stack[1]) ||
        is_space(scanner->lookbehind->stack[1]) ||
        is_punctuation(scanner->lookbehind->stack[1]) ||
        is_inline_markup_start_char(scanner->lookbehind->stack[1])) &&
        !is_space(scanner->lookahead)
    ) {
      SET_BIT(IM_PASSTHROUGH);
      lexer->result_symbol = T_INLINE_PASSTHROUGH_MARKER_START;
      return true;
    }

    if (GET_BIT(IM_PASSTHROUGH) &&
        !is_space(scanner->lookbehind->stack[1])
    ) {
      if (is_newline(scanner->lookahead) || is_space(scanner->lookahead) ||
          is_punctuation(scanner->lookahead) ||
          is_inline_markup_end_char(scanner->lookahead)) {
        CLEAR_BIT(IM_PASSTHROUGH);
        lexer->result_symbol = T_INLINE_PASSTHROUGH_MARKER_END;
        return true;
      }
    }
  }

  if (
    !(GET_BIT(IM_MONOSPACE)) &&
    !(GET_BIT(IM_LITERALBLOCK)) &&
    !(GET_BIT(IM_LISTINGBLOCK)) &&
    scanner->lookahead == CHAR_EMPHASIS
  ) {
    do {
      scanner->advance(scanner);
    } while ((scanner->lookbehind->stack[0]) ==
            (scanner->lookahead));
    if (scanner->lookahead == CHAR_EMPHASIS) {
      scanner->advance(scanner);
    }

    if (match_before(scanner, STR_BLOCK_QUOTE_SEPARATOR,
                    strlen(STR_BLOCK_QUOTE_SEPARATOR), match_newline) &&
        is_newline(scanner->lookahead)) {
      scanner->advance(scanner);
      TOGGLE_BIT(IM_QUOTEBLOCK);
      lexer->result_symbol = T_BLOCK_QUOTE_SEPARATOR;
      return true;
    }

    if (
      !(GET_BIT(IM_MONOSPACE)) &&
      !(GET_BIT(IM_PASSTHROUGH)) &&
      !(GET_BIT(IM_XREFMACRO)) &&
      (is_newline(scanner->lookbehind->stack[1]) ||
        is_space(scanner->lookbehind->stack[1]) ||
        is_punctuation(scanner->lookbehind->stack[1]) ||
        is_inline_markup_start_char(scanner->lookbehind->stack[1])) &&
      !is_space(scanner->lookahead)
    ) {
      SET_BIT(IM_EMPHASIS);
      lexer->result_symbol = T_EMPHASIS_MARKER_START;
      return true;
    }

    if (
        GET_BIT(IM_EMPHASIS) &&
        !is_space(scanner->lookbehind->stack[1])
    ) {
      if (is_newline(scanner->lookahead) || is_space(scanner->lookahead) ||
          is_punctuation(scanner->lookahead) ||
          is_inline_markup_end_char(scanner->lookahead)) {
        CLEAR_BIT(IM_EMPHASIS);
        lexer->result_symbol = T_EMPHASIS_MARKER_END;
        return true;
      }
    }
  }


  if (
    valid_symbols[T_BLOCK_TABLE_SEPARATOR1] &&
    scanner->lookahead == CHAR_CELL1 &&
    !GET_BIT(IM_TABLE1BLOCK) &&
    is_newline(scanner->lookbehind->stack[0])
  ) {
    consume(scanner, strlen(STR_BLOCK_TABLE_SEPARATOR1));
    if(
      match_before(scanner, STR_BLOCK_TABLE_SEPARATOR1, strlen(STR_BLOCK_TABLE_SEPARATOR1), match_newline) &&
      is_newline(scanner->lookahead)
    ) {
      SET_BIT(IM_TABLE1BLOCK);
      scanner->advance(scanner);
      lexer->result_symbol = T_BLOCK_TABLE_SEPARATOR1;
      return true;
    }
  }

  if (
    (valid_symbols[T_BLOCK_TABLE_SEPARATOR1] || valid_symbols[T_TABLE_CELL_MARKER]) &&
    scanner->lookahead == CHAR_CELL1 &&
    GET_BIT(IM_TABLE1BLOCK)
  ) {
    scanner->advance(scanner);
    lexer->mark_end(lexer);
    if ( scanner->lookahead == '=' ) {
      consume(scanner, 3);
      if (
        match_before(scanner, STR_BLOCK_TABLE_SEPARATOR1, strlen(STR_BLOCK_TABLE_SEPARATOR1), match_newline) &&
        is_newline(scanner->lookahead)
      ) {
        CLEAR_BIT(IM_TABLE1BLOCK);
        scanner->advance(scanner);
        lexer->mark_end(lexer);
        lexer->result_symbol = T_BLOCK_TABLE_SEPARATOR1;
        return true;
      }
    } else {
      lexer->result_symbol = T_TABLE_CELL_MARKER;
      return true;
    }
  }


  if (
    valid_symbols[T_MONOSPACE_MARKER_START] &&
    !GET_BIT(IM_MONOSPACE) &&
    !GET_BIT(IM_XREFMACRO) &&
    !GET_BIT(IM_MONOSPACE) &&
    !GET_BIT(IM_LITERALBLOCK) &&
    !GET_BIT(IM_LISTINGBLOCK) &&
    scanner->lookahead == CHAR_MONOSPACE
  ) {
    if (
      is_newline(scanner->lookbehind->stack[0]) ||
      is_space(scanner->lookbehind->stack[0]) ||
      is_punctuation(scanner->lookbehind->stack[0]) ||
      is_inline_markup_start_char(scanner->lookbehind->stack[0])
    ) {
      scanner->advance(scanner);
      if(
        is_space(scanner->lookahead)
      ) {
        return false;
      }
      SET_BIT(IM_MONOSPACE);
      lexer->result_symbol = T_MONOSPACE_MARKER_START;
      return true;
    }
  }
  if(
    valid_symbols[T_MONOSPACE_MARKER_END] &&
    GET_BIT(IM_MONOSPACE) &&
    !GET_BIT(IM_LITERALBLOCK) &&
    !GET_BIT(IM_LISTINGBLOCK) &&
    scanner->lookahead == CHAR_MONOSPACE
  ) {
    if (
      is_space(scanner->lookbehind->stack[0])
    ) {
      return false;
    }
    scanner->advance(scanner);
    if (
      // !is_word(scanner->lookahead)
      is_newline_or_space(scanner->lookahead) ||
      is_punctuation(scanner->lookahead)
    ) {
      CLEAR_BIT(IM_MONOSPACE);
      lexer->result_symbol = T_MONOSPACE_MARKER_END;
      return true;
    }
  }

  SET_PARSE(
    CHAR_SUPERSCRIPT,
    IM_SUPERSCRIPT,
    T_SUPERSCRIPT_MARKER_START,
    T_SUPERSCRIPT_MARKER_END
  )
  SET_PARSE(
    CHAR_SUBSCRIPT,
    IM_SUBSCRIPT,
    T_SUBSCRIPT_MARKER_START,
    T_SUBSCRIPT_MARKER_END
  )
  SET_PARSE(
    CHAR_HIGHLIGHT,
    IM_HIGHLIGHT,
    T_HIGHLIGHT_MARKER_START,
    T_HIGHLIGHT_MARKER_END
  )

  if (
    valid_symbols[T_ATTRIBUTE_REFERENCE_START_MARKER] &&
    scanner->lookahead == CHAR_ATTRIBUTE_REFERENCE_START
  ) {
    scanner->advance(scanner);
    lexer->result_symbol = T_ATTRIBUTE_REFERENCE_START_MARKER;
    return true;
  }
  if (
    valid_symbols[T_ATTRIBUTE_REFERENCE_END_MARKER] &&
    scanner->lookahead == CHAR_ATTRIBUTE_REFERENCE_END
  ) {
    scanner->advance(scanner);
    lexer->result_symbol = T_ATTRIBUTE_REFERENCE_END_MARKER;
    return true;
  }


  if (
    valid_symbols[T_BLOCK_OPTION_MARKER_START] &&
    scanner->lookahead == CHAR_OPTION_START &&
    is_newline(scanner->lookbehind->stack[0])
  ) {
    scanner->advance(scanner);
    SET_BIT(IM_OPTIONBLOCK);
    lexer->result_symbol = T_BLOCK_OPTION_MARKER_START;
    return true;
  }
  if (
    valid_symbols[T_BLOCK_OPTION_MARKER_END] &&
    scanner->lookahead == CHAR_OPTION_END &&
    GET_BIT(IM_OPTIONBLOCK)
  ) {
    scanner->advance(scanner);
    if( is_newline(scanner->lookahead) ) {
      CLEAR_BIT(IM_OPTIONBLOCK);
      scanner->advance(scanner);
      lexer->result_symbol = T_BLOCK_OPTION_MARKER_END;
      return true;
    }
  }

  if (
    valid_symbols[T_INLINE_OPTION_BLOCK_MARKER_START] &&
    scanner->lookahead == CHAR_OPTION_START
  ) {
    SET_BIT(IM_OPTIONINLINE);
    scanner->advance(scanner);
    lexer->result_symbol = T_INLINE_OPTION_BLOCK_MARKER_START;
    return true;
  }

  if (
    valid_symbols[T_INLINE_OPTION_BLOCK_MARKER_END] &&
    GET_BIT(IM_OPTIONINLINE) &&
    scanner->lookahead == CHAR_OPTION_END
  ) {
    scanner->advance(scanner);
    CLEAR_BIT(IM_OPTIONINLINE);
    if GET_BIT(IM_XREFMACRO) { CLEAR_BIT(IM_XREFMACRO); }
    lexer->result_symbol = T_INLINE_OPTION_BLOCK_MARKER_END;
    return true;
  }

  if (
    valid_symbols[T_CROSSREFERENCE_MARKER_START] &&
    !GET_BIT(IM_LITERALBLOCK) &&
    !GET_BIT(IM_LISTINGBLOCK)
  ) {
    if (scanner->lookahead == CHAR_CROSSREFERENCE_START) {
      consume(scanner, 2);
    }
    if (
      (scanner->lookahead != CHAR_CROSSREFERENCE_START) &&
      match_before(scanner, STR_CROSSREFERENCE_START, strlen(STR_CROSSREFERENCE_START), match_newline_or_space)
    ) {
      SET_BIT(IM_CROSSREFERENCE);
      lexer->result_symbol = T_CROSSREFERENCE_MARKER_START;
      return true;
    }
  }

  if (
    valid_symbols[T_CROSSREFERENCE_MARKER_END] &&
    !GET_BIT(IM_LITERALBLOCK) &&
    !GET_BIT(IM_LISTINGBLOCK)
  ) {
    if (scanner->lookahead == CHAR_CROSSREFERENCE_END) {
      consume(scanner, 2);
    }
    if (
      (scanner->lookahead != CHAR_CROSSREFERENCE_END) &&
      match_before(scanner, STR_CROSSREFERENCE_END, strlen(STR_CROSSREFERENCE_END), match_any)
    ) {
      CLEAR_BIT(IM_CROSSREFERENCE);
      lexer->result_symbol = T_CROSSREFERENCE_MARKER_END;
      return true;
    }
  }
  if (
    (valid_symbols[T_CROSSREFERENCE_MARKER_END] && valid_symbols[T_CROSSREFERENCE_SPLITTER]) &&
    GET_BIT(IM_CROSSREFERENCE) &&
    scanner->lookahead == CHAR_COMMA
  ) {
    scanner->advance(scanner);
    lexer->result_symbol = T_CROSSREFERENCE_SPLITTER;
    return true;
  }

  if (
    (
      valid_symbols[T_TEXT]                          ||
      valid_symbols[T_ADMONITION_NOTE_MARKER]        ||
      valid_symbols[T_ADMONITION_TIP_MARKER]         ||
      valid_symbols[T_ADMONITION_IMPORTANT_MARKER]   ||
      valid_symbols[T_ADMONITION_CAUTION_MARKER]     ||
      valid_symbols[T_ADMONITION_WARNING_MARKER]     ||
      valid_symbols[T_XREF_MARKER]                   ||
      valid_symbols[T_FOOTNOTE_MARKER]               ||
      valid_symbols[T_ICON_MARKER]                   ||
      valid_symbols[T_LINK_MARKER]                   ||
      valid_symbols[T_MAILTO_MARKER]                 ||

      valid_symbols[T_PASS_MARKER]                   ||
      valid_symbols[T_STEM_MARKER]                   ||
      valid_symbols[T_LATEXMATH_MARKER]              ||

      valid_symbols[T_INCLUDE_MARKER]                ||
      valid_symbols[T_IMAGE_MARKER]                  ||

      valid_symbols[T_IMAGE_INLINE_MARKER]           ||
      valid_symbols[T_TAG_MARKER]                    ||
      valid_symbols[T_TAG_END_MARKER]                ||
      valid_symbols[T_IFDEF_MARKER]                  ||
      valid_symbols[T_IFEVAL_MARKER]                 ||
      valid_symbols[T_ENDIF_MARKER]
    ) &&
    is_word(scanner->lookahead) &&
    is_keyword(scanner, 0).match
  ) {
    unsigned char position = 0;
    do {
      scanner->advance(scanner);
      position++;
    } while(is_keyword(scanner, position).match);
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_ADMONITION_NOTE, strlen(STR_ADMONITION_NOTE), match_emptyline)
    ) {
      lexer->result_symbol = T_ADMONITION_NOTE_MARKER;
      return true;
    }
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_ADMONITION_TIP, strlen(STR_ADMONITION_TIP), match_emptyline)
    ) {
      lexer->result_symbol = T_ADMONITION_TIP_MARKER;
      return true;
    }
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_ADMONITION_IMPORTANT, strlen(STR_ADMONITION_IMPORTANT), match_emptyline)
    ) {
      lexer->result_symbol = T_ADMONITION_IMPORTANT_MARKER;
      return true;
    }
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_ADMONITION_CAUTION, strlen(STR_ADMONITION_CAUTION), match_emptyline)
    ) {
      lexer->result_symbol = T_ADMONITION_CAUTION_MARKER;
      return true;
    }
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_ADMONITION_WARNING, strlen(STR_ADMONITION_WARNING), match_emptyline)
    ) {
      lexer->result_symbol = T_ADMONITION_WARNING_MARKER;
      return true;
    }
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_XREF, strlen(STR_XREF), match_any)
    ) {
      lexer->result_symbol = T_XREF_MARKER;
      return true;
    }
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_FOOTNOTE, strlen(STR_FOOTNOTE), match_any)
    ) {
      lexer->result_symbol = T_FOOTNOTE_MARKER;
      return true;
    }
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_ICON, strlen(STR_ICON), match_any)
    ) {
      lexer->result_symbol = T_ICON_MARKER;
      return true;
    }
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_LINK, strlen(STR_LINK), match_newline_or_space)
    ) {
      lexer->result_symbol = T_LINK_MARKER;
      return true;
    }
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_MAILTO, strlen(STR_MAILTO), match_newline_or_space)
    ) {
      lexer->result_symbol = T_MAILTO_MARKER;
      return true;
    }
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_IMAGE, strlen(STR_IMAGE), match_newline)
    ) {
      lexer->result_symbol = T_IMAGE_MARKER;
      return true;
    }
    if (
      !GET_BIT(IM_LITERALBLOCK) &&
      !GET_BIT(IM_LISTINGBLOCK) &&
      match_before(scanner, STR_IMAGE_INLINE, strlen(STR_IMAGE_INLINE), match_any)
    ) {
      lexer->result_symbol = T_IMAGE_INLINE_MARKER;
      return true;
    }
    if (match_before(scanner, STR_INCLUDE, strlen(STR_INCLUDE), match_newline)) {
      lexer->result_symbol = T_INCLUDE_MARKER;
      return true;
    }
    if (match_before(scanner, STR_IFDEF, strlen(STR_IFDEF), match_newline)) {
      lexer->result_symbol = T_IFDEF_MARKER;
      return true;
    }
    if (match_before(scanner, STR_IFNDEF, strlen(STR_IFNDEF), match_newline)) {
      lexer->result_symbol = T_IFDEF_MARKER;
      return true;
    }
    if (match_before(scanner, STR_IFEVAL, strlen(STR_IFEVAL), match_newline)) {
      lexer->result_symbol = T_IFEVAL_MARKER;
      return true;
    }
    if (match_before(scanner, STR_IFEND, strlen(STR_IFEND), match_newline)) {
      lexer->result_symbol = T_ENDIF_MARKER;
      return true;
    }
    if (match_before(scanner, STR_TAG, strlen(STR_TAG), match_any)) {
      lexer->result_symbol = T_TAG_MARKER;
      return true;
    }
    if (match_before(scanner, STR_TAGEND, strlen(STR_TAGEND), match_any)) {
      lexer->result_symbol = T_TAG_END_MARKER;
      return true;
    }
    if (match_before(scanner, STR_PASS, strlen(STR_PASS), match_newline_or_space)) {
      lexer->result_symbol = T_PASS_MARKER;
      return true;
    }
    if (match_before(scanner, STR_STEM, strlen(STR_STEM), match_newline_or_space)) {
      lexer->result_symbol = T_STEM_MARKER;
      return true;
    }
    if (match_before(scanner, STR_LATEXMATH, strlen(STR_LATEXMATH), match_newline_or_space)) {
      lexer->result_symbol = T_LATEXMATH_MARKER;
      return true;
    }
    lexer->result_symbol = T_TEXT;
    return true;
  }

  if (
    valid_symbols[T_TEXT] &&
    is_word(scanner->lookahead) &&
    !is_keyword(scanner, 0).match
  ) {
    while( is_word(scanner->lookahead) && !is_keyword(scanner, 0).match ) scanner->advance(scanner);
    lexer->result_symbol = T_TEXT;
    return true;
  }

  return false;
}
