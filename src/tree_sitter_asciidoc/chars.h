#ifndef TREE_SITTER_ASCIIDOC_CHARS_H
#define TREE_SITTER_ASCIIDOC_CHARS_H

#include <tree_sitter/parser.h>
#include "scanner.h"

#define CHAR_EOF 0
#define CHAR_NEWLINE 10 // LF
#define CHAR_CARRIAGE_RETURN 13 // CR
#define CHAR_NBSP 160

#define CHAR_HYPHEN 8208
#define CHAR_HYPHEN_NONBREAKING 8209
#define CHAR_FIGURE_DASH 8210
#define CHAR_EN_DASH 8211
#define CHAR_EM_DASH 8212
#define CHAR_QUOTATION_DASH 8213

#define CHAR_LEFT_SINGLE_QUOTE 8216
#define CHAR_RIGHT_SINGLE_QUOTE 8217
#define CHAR_SINGLE_LOW_QUOTE 8218
#define CHAR_SINGLE_HIGH_REVERSE_QUOTE 8219
#define CHAR_LEFT_DOUBLE_QUOTE 8220
#define CHAR_RIGHT_DOUBLE_QUOTE 8221
#define CHAR_DOUBLE_LOW_QUOTE 8222
#define CHAR_DOUBLE_HIGH_REVERSE_QUOTE 8223
#define CHAR_ELLIPSIS 8230

#define CHAR_SPACE ' '
#define CHAR_FORM_FEED '\f'
#define CHAR_TAB '\t'
#define CHAR_VERTICAL_TAB '\v'

#define CHAR_EN_QUAD 8192
#define CHAR_EM_QUAD 8193
#define CHAR_EN_SPACE 8194
#define CHAR_EM_SPACE 8195
#define CHAR_THREE_PER_EM_SPACE 8196
#define CHAR_FOUR_PER_EM_SPACE 8197
#define CHAR_SIX_PER_EM_SPACE 8198
#define CHAR_FIGURE_SPACE 8199
#define CHAR_PUNCTUATION_SPACE 8200
#define CHAR_THIN_SPACE 8201
#define CHAR_HAIR_SPACE 8202
#define CHAR_COMMA ','
#define CHAR_LDQUO 171
#define CHAR_RDQUO 187

#define CHAR_HIGHLIGHT '#'
#define CHAR_OPTION_START '['
#define CHAR_OPTION_END ']'
#define CHAR_STRONG '*'
#define CHAR_EMPHASIS '_'
#define CHAR_MONOSPACE '`'
#define CHAR_SUPERSCRIPT '^'
#define CHAR_SUBSCRIPT '~'
#define CHAR_INLINE_PASSTHROUGH '+'
#define CHAR_MACRO_SEPARATOR ':'
#define CHAR_CROSSREFERENCE_START '<'
#define CHAR_CROSSREFERENCE_END '>'
#define CHAR_ATTRIBUTE_REFERENCE_START '{'
#define CHAR_ATTRIBUTE_REFERENCE_END '}'

#define CHAR_ATTRIBUTE ':'
#define CHAR_ATTRIBUTENEGATION '!'
#define CHAR_SECTIONHEADER '='
#define CHAR_COMMENT '/'
#define CHAR_ESCAPE '\\'

#define CHAR_CELL1 '|'
#define CHAR_CELL2 '!'

static bool is_newline(int32_t c);
static bool is_space(int32_t c);
static bool is_space_extended(int32_t c);
static bool is_space_consume(ADOCScanner* scanner);
static bool is_newline_or_space(int32_t c);
static bool is_newline_or_space_extended(int32_t c);
static bool is_anything(int32_t c);

static bool is_number(int32_t c);
static bool is_abc_lower(int32_t c);
static bool is_abc_upper(int32_t c);
static bool is_abc(int32_t c);
static bool is_alphanumeric(int32_t c);
static bool is_alphanumeric_dash_underscore(int32_t c);
static bool is_word(int32_t c);

static bool is_inline_markup_start_char(int32_t c);
static bool is_inline_markup_end_char(int32_t c);
static bool is_punctuation(int32_t c);

#endif // TREE_SITTER_ASCIIDOC_CHARS_H
