#include "chars.h"
#include <string.h>

static bool is_newline(int32_t c)
{
  const int32_t newline_chars[] = {
    CHAR_EOF,
    CHAR_NEWLINE,
    CHAR_CARRIAGE_RETURN,
  };
  const int length = sizeof(newline_chars) / sizeof(int32_t);
  for (int i = 0; i < length; i++) {
    if (c == newline_chars[i]) {
      return true;
    }
  }
  return false;
}

static bool is_space(int32_t c)
{
  const int32_t space_chars[] = {
    CHAR_SPACE,
    CHAR_FORM_FEED,
    CHAR_TAB,
    CHAR_VERTICAL_TAB,
    CHAR_NBSP,
  };
  const int length = sizeof(space_chars) / sizeof(int32_t);
  bool is_space_char = false;
  for (int i = 0; i < length; i++) {
    if (c == space_chars[i]) {
      is_space_char = true;
      break;
    }
  }
  return is_space_char;
}

static bool is_newline_or_space(int32_t c)
{
  return is_newline(c) || is_space(c);
}

static bool is_anything(int32_t c) { return true; }

static bool is_number(int32_t c)
{
  const int32_t upper = 48;
  const int32_t lower = 57;
  return c >= upper && c <= lower;
}

static bool is_abc_lower(int32_t c)
{
  const int32_t upper = 97;
  const int32_t lower = 122;
  return c >= upper && c <= lower;
}

static bool is_abc_upper(int32_t c)
{
  const int32_t upper = 65;
  const int32_t lower = 90;
  return c >= upper && c <= lower;
}

static bool is_abc(int32_t c)
{
  return is_abc_lower(c) || is_abc_upper(c);
}

static bool is_alphanumeric(int32_t c)
{
  return is_abc(c) || is_number(c);
}

static bool is_alphanumeric_dash_underscore(int32_t c)
{
  return is_abc(c) || is_number(c) || c == 45 || c == 95;
}

static bool is_word(int32_t c)
{
  return
    !(c >= 33 && c <= 47) &&
    !(c >= 58 && c <= 64) &&
    !(c >= 91 && c <= 96) &&
    !(c >= 123 && c <= 126) &&
    !is_space(c) &&
    !is_newline(c);
}

/*
33 … 47 // !"#$%&'()*+,-./
58 … 64 // :;<=>?@
91 … 96 // [\]^_`
123 … 126 // {|}~
*/

// When a space-like character directly precedes the text to format, and
// a space-like character or punctuation mark (`,`, `;`, `"`, `.`, `?`, or `!`) directly
// follows the text, and the text does not start or end with a space-like
// character, a constrained formatting pair can be used. A constrained pair
// uses a single opening mark and a single closing mark to enclose the text to
// be styled (e.g., *strong*).

static bool is_inline_markup_start_char(int32_t c)
{
  const int32_t valid_chars[] = {
    CHAR_HIGHLIGHT,             //
    CHAR_OPTION_START,          //
    CHAR_STRONG,                //
    CHAR_EMPHASIS,              //
    CHAR_MONOSPACE,             //
    CHAR_SUPERSCRIPT,           //
    CHAR_SUBSCRIPT,             //
    CHAR_INLINE_PASSTHROUGH,    //
    CHAR_CROSSREFERENCE_START,  //
    CHAR_ATTRIBUTE_REFERENCE_START,
    CHAR_ATTRIBUTE,
    CHAR_ESCAPE,
  };
  const int length = sizeof(valid_chars) / sizeof(int32_t);
  for (int i = 0; i < length; i++) {
    if (c == valid_chars[i]) {
      return true;
    }
  }
  return false;
}

static bool is_inline_markup_end_char(int32_t c)
{
  const int32_t valid_chars[] = {
    CHAR_HIGHLIGHT,             //
    CHAR_OPTION_END,            //
    CHAR_STRONG,                //
    CHAR_EMPHASIS,              //
    CHAR_MONOSPACE,             //
    CHAR_SUPERSCRIPT,           //
    CHAR_SUBSCRIPT,             //
    CHAR_INLINE_PASSTHROUGH,    //
    CHAR_CROSSREFERENCE_END,    //
    CHAR_ATTRIBUTE_REFERENCE_END,
    CHAR_ATTRIBUTE,
  };
  const int length = sizeof(valid_chars) / sizeof(int32_t);
  for (int i = 0; i < length; i++) {
    if (c == valid_chars[i]) {
      return true;
    }
  }
  return false;
}

static bool is_punctuation(int32_t c)
{
  const int32_t valid_chars[] = {
    CHAR_EM_DASH,
    '.',
    ',',
    ':',
    ';',
    '\'',
    '"',
    '?',
    '!',
    '(',
    ')',
  };
  const int length = sizeof(valid_chars) / sizeof(int32_t);
  for (int i = 0; i < length; i++) {
    if (c == valid_chars[i]) {
      return true;
    }
  }
  return false;
}
