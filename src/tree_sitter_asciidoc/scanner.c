#include <tree_sitter/parser.h>
#include <stdio.h>
#include <string.h>

#include "chars.c"
#include "tokens.h"
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

static void adoc_scanner_destroy(ADOCScanner* scanner)
{
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
  bool* context = scanner->lookbehind->context;
  bool (*match_newline)(int32_t*, size_t) = &m_newline;
  bool (*match_newline_or_space)(int32_t*, size_t) = &m_newline_or_space;
  bool (*match_any)(int32_t*, size_t) = &m_any;
  bool (*match_emptyline)(int32_t*, size_t) = &m_emptyline;

  ///////////////
  // ПРОБЕЛЫ
  ///////////////
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

  ///////////////
  // CR
  ///////////////
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
        return true;
    }
  }

#include "colon_sign_parse.c"

  //////////////
  // КОММЕНТАРИИ
  //////////////
  if (
    scanner->lookahead == CHAR_COMMENT &&
    !(context[V_LITERAL]) &&
    !(context[V_LISTING]) &&
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
      return true;
    }
    if(
      match_before(scanner, STR_COMMENT_MARKER, strlen(STR_COMMENT_MARKER), match_newline)
    ) {
      lexer->result_symbol = T_COMMENT_MARKER;
      return true;
    }
  }

  /////////////////
  // PAGE BREAK
  /////////////////
  ///////////
  // CALLOUTS
  ///////////
  if (scanner->lookahead == CHAR_CROSSREFERENCE_START &&
    is_newline(scanner->lookbehind->stack[0]) &&
    (valid_symbols[T_PAGE_BREAK] || valid_symbols[T_CALLOUT_MARKER_TO])
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

#include "eq_sign_parse.c"          // '='

#include "dash_sign_parse.c"        // '-'

#include "dot_sign_parse.c"         // '.'

#include "asterisk_sign_parse.c"    // '*'

#include "plus_sign_parse.c"        // '+'

#include "underscore_sign_parse.c"  // '_'




  ////////////////////
  // TABLE SEPARATOR
  ////////////////////
  if (
    valid_symbols[T_BLOCK_TABLE_SEPARATOR1] &&
    scanner->lookahead == CHAR_CELL1 &&
    !(context[V_TABLE1]) &&
    is_newline(scanner->lookbehind->stack[0])
  ) {
    consume(scanner, strlen(STR_BLOCK_TABLE_SEPARATOR1));
    if(
      match_before(scanner, STR_BLOCK_TABLE_SEPARATOR1, strlen(STR_BLOCK_TABLE_SEPARATOR1), match_newline) &&
      is_newline(scanner->lookahead)
    ) {
      context[V_TABLE1] = true;
      scanner->advance(scanner);
      lexer->result_symbol = T_BLOCK_TABLE_SEPARATOR1;
      return true;
    }
  }

  if (
    (valid_symbols[T_BLOCK_TABLE_SEPARATOR1] || valid_symbols[T_TABLE_CELL_MARKER]) &&
    scanner->lookahead == CHAR_CELL1 &&
    context[V_TABLE1]
  ) {
    scanner->advance(scanner);
    lexer->mark_end(lexer);
    if ( scanner->lookahead == '=' ) {
      consume(scanner, 3);
      if (
        match_before(scanner, STR_BLOCK_TABLE_SEPARATOR1, strlen(STR_BLOCK_TABLE_SEPARATOR1), match_newline) &&
        is_newline(scanner->lookahead)
      ) {
        context[V_TABLE1] = false;
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



  //////////
  // ИНЛАЙНЫ
  //////////

  if (
    valid_symbols[T_MONOSPACE_MARKER_START] &&
    !(context[V_MONOSPACE]) &&
    !(context[V_XREF_LOCATOR]) &&
    !(context[V_MONOSPACE]) &&
    !(context[V_LITERAL]) &&
    !(context[V_LISTING]) &&
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
      context[V_MONOSPACE] = true;
      lexer->result_symbol = T_MONOSPACE_MARKER_START;
      return true;
    }
  }
  if(
    valid_symbols[T_MONOSPACE_MARKER_END] &&
    context[V_MONOSPACE] &&
    !(context[V_LITERAL]) &&
    !(context[V_LISTING]) &&
    scanner->lookahead == CHAR_MONOSPACE
  ) {
    if (
      is_space(scanner->lookbehind->stack[0])
    ) {
      return false;
    }
    scanner->advance(scanner);
    if (
      !is_word(scanner->lookahead)
    ) {
      context[V_MONOSPACE] = false;
      lexer->result_symbol = T_MONOSPACE_MARKER_END;
      return true;
    }
  }

  SET_PARSE(
    CHAR_SUPERSCRIPT,
    V_SUPERSCRIPT,
    T_SUPERSCRIPT_MARKER_START,
    T_SUPERSCRIPT_MARKER_END
  )
  SET_PARSE(
    CHAR_SUBSCRIPT,
    V_SUBSCRIPT,
    T_SUBSCRIPT_MARKER_START,
    T_SUBSCRIPT_MARKER_END
  )
  SET_PARSE(
    CHAR_HIGHLIGHT,
    V_HIGHLIGHT,
    T_HIGHLIGHT_MARKER_START,
    T_HIGHLIGHT_MARKER_END
  )

  //////////////////////////////////
  // ССЫЛКИ НА АТРИБУТЫ и СИМВОЛЫ {}
  //////////////////////////////////
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


  //////////
  // БЛОК ОПЦИЙ
  //////////
  if (
    valid_symbols[T_BLOCK_OPTION_MARKER_START] &&
    scanner->lookahead == CHAR_OPTION_START &&
    is_newline(scanner->lookbehind->stack[0])
  ) {
    scanner->advance(scanner);
    context[V_OPTIONBLOCK] = true;
    lexer->result_symbol = T_BLOCK_OPTION_MARKER_START;
    return true;
  }
  if (
    valid_symbols[T_BLOCK_OPTION_MARKER_END] &&
    scanner->lookahead == CHAR_OPTION_END &&
    context[V_OPTIONBLOCK]
  ) {
    scanner->advance(scanner);
    if( is_newline(scanner->lookahead) ) {
      context[V_OPTIONBLOCK] = false;
      scanner->advance(scanner);
      lexer->result_symbol = T_BLOCK_OPTION_MARKER_END;
      return true;
    }
  }

  ////////////////////////////
  // Блоки инлайн-опций [   ]
  ////////////////////////////
  if (
    valid_symbols[T_INLINE_OPTION_BLOCK_MARKER_START] &&
    scanner->lookahead == CHAR_OPTION_START
  ) {
    context[V_OPTIONINLINE] = true;
    scanner->advance(scanner);
    lexer->result_symbol = T_INLINE_OPTION_BLOCK_MARKER_START;
    return true;
  }

  if (
    valid_symbols[T_INLINE_OPTION_BLOCK_MARKER_END] &&
    context[V_OPTIONINLINE] &&
    scanner->lookahead == CHAR_OPTION_END
  ) {
    scanner->advance(scanner);
    context[V_OPTIONINLINE] = false;
    context[V_INCLUDE_LOCATOR] = false;
    context[V_XREF_LOCATOR] = false;
    lexer->result_symbol = T_INLINE_OPTION_BLOCK_MARKER_END;
    return true;
  }

  //////////////////////
  // Перекрестные ссылки
  //////////////////////
  if (
    valid_symbols[T_CROSSREFERENCE_MARKER_START] &&
    scanner->lookahead == CHAR_CROSSREFERENCE_START &&
    !(context[V_CROSSREFERENCE]) &&
    !(context[V_INCLUDE_LOCATOR]) &&
    !(context[V_XREF_LOCATOR]) &&
    (
      is_punctuation(scanner->lookbehind->stack[0]) ||
      is_space(scanner->lookbehind->stack[0]) ||
      is_newline(scanner->lookbehind->stack[0])
    )
  ) {
    scanner->advance(scanner);
    if(scanner->lookahead == CHAR_CROSSREFERENCE_START) {
      scanner->advance(scanner);
      context[V_CROSSREFERENCE] = true;
      context[V_CROSSREFERENCE_ID] = true;
      lexer->result_symbol = T_CROSSREFERENCE_MARKER_START;
      return true;
    }
  }

  if (
    valid_symbols[T_CROSSREFERENCE_MARKER_END] &&
    scanner->lookahead == CHAR_CROSSREFERENCE_END &&
    context[V_CROSSREFERENCE]
  ) {
    scanner->advance(scanner);
    if(scanner->lookahead == CHAR_CROSSREFERENCE_END) {
      context[V_CROSSREFERENCE_ID] = false;
      context[V_CROSSREFERENCE] = false;
      scanner->advance(scanner);
      lexer->result_symbol = T_CROSSREFERENCE_MARKER_END;
      return true;
    }
  }

  if (
    (valid_symbols[T_CROSSREFERENCE_MARKER_END] && valid_symbols[T_CROSSREFERENCE_SPLITTER]) &&
    scanner->lookahead == ',' &&
    context[V_CROSSREFERENCE_ID]
  ) {
    context[V_CROSSREFERENCE_ID] = false;
    scanner->advance(scanner);
    lexer->result_symbol = T_CROSSREFERENCE_SPLITTER;
    return true;
  }

  ///////////////
  // WORD
  ///////////////
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
      !(context[V_LITERAL]) &&
      !(context[V_LISTING]) &&
      match_before(scanner, STR_ADMONITION_NOTE, strlen(STR_ADMONITION_NOTE), match_emptyline)
    ) {
      lexer->result_symbol = T_ADMONITION_NOTE_MARKER;
      return true;
    }
    if (
      !(context[V_LITERAL]) &&
      !(context[V_LISTING]) &&
      match_before(scanner, STR_ADMONITION_TIP, strlen(STR_ADMONITION_TIP), match_emptyline)
    ) {
      lexer->result_symbol = T_ADMONITION_TIP_MARKER;
      return true;
    }
    if (
      !(context[V_LITERAL]) &&
      !(context[V_LISTING]) &&
      match_before(scanner, STR_ADMONITION_IMPORTANT, strlen(STR_ADMONITION_IMPORTANT), match_emptyline)
    ) {
      lexer->result_symbol = T_ADMONITION_IMPORTANT_MARKER;
      return true;
    }
    if (
      !(context[V_LITERAL]) &&
      !(context[V_LISTING]) &&
      match_before(scanner, STR_ADMONITION_CAUTION, strlen(STR_ADMONITION_CAUTION), match_emptyline)
    ) {
      lexer->result_symbol = T_ADMONITION_CAUTION_MARKER;
      return true;
    }
    if (
      !(context[V_LITERAL]) &&
      !(context[V_LISTING]) &&
      match_before(scanner, STR_ADMONITION_WARNING, strlen(STR_ADMONITION_WARNING), match_emptyline)
    ) {
      lexer->result_symbol = T_ADMONITION_WARNING_MARKER;
      return true;
    }
    if (
      !(context[V_LITERAL]) &&
      !(context[V_LISTING]) &&
      match_before(scanner, STR_XREF, strlen(STR_XREF), match_any)
    ) {
      lexer->result_symbol = T_XREF_MARKER;
      return true;
    }
    if (
      !(context[V_LITERAL]) &&
      !(context[V_LISTING]) &&
      match_before(scanner, STR_FOOTNOTE, strlen(STR_FOOTNOTE), match_any)
    ) {
      lexer->result_symbol = T_FOOTNOTE_MARKER;
      return true;
    }
    if (
      !(context[V_LITERAL]) &&
      !(context[V_LISTING]) &&
      match_before(scanner, STR_ICON, strlen(STR_ICON), match_any)
    ) {
      lexer->result_symbol = T_ICON_MARKER;
      return true;
    }
    if (
      !(context[V_LITERAL]) &&
      !(context[V_LISTING]) &&
      match_before(scanner, STR_LINK, strlen(STR_LINK), match_newline_or_space)
    ) {
      lexer->result_symbol = T_LINK_MARKER;
      return true;
    }
    if (
      !(context[V_LITERAL]) &&
      !(context[V_LISTING]) &&
      match_before(scanner, STR_MAILTO, strlen(STR_MAILTO), match_newline_or_space)
    ) {
      lexer->result_symbol = T_MAILTO_MARKER;
      return true;
    }
    if (
      !(context[V_LITERAL]) &&
      !(context[V_LISTING]) &&
      match_before(scanner, STR_IMAGE, strlen(STR_IMAGE), match_newline)
    ) {
      lexer->result_symbol = T_IMAGE_MARKER;
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
