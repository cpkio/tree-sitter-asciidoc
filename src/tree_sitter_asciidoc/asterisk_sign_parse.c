// vim:sw=2
if (
  !(context[V_MONOSPACE]) &&
  !(context[V_LITERAL]) &&
  !(context[V_LISTING]) &&
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
    !(context[V_MONOSPACE]) &&
    !(context[V_PASSTHROUGH]) &&
    !(context[V_XREF_LOCATOR]) &&
    (is_newline(scanner->lookbehind->stack[1]) ||
      is_space(scanner->lookbehind->stack[1]) ||
      is_punctuation(scanner->lookbehind->stack[1]) ||
      is_inline_markup_start_char(scanner->lookbehind->stack[1])) &&
    !is_space(scanner->lookahead)
  ) {
    context[V_STRONG] = true;
    lexer->result_symbol = T_STRONG_MARKER_START;
    return true;
  }

  if (context[V_STRONG] &&
      !is_space(scanner->lookbehind->stack[1])) {
    if (is_newline(scanner->lookahead) || is_space(scanner->lookahead) ||
        is_punctuation(scanner->lookahead) ||
        is_inline_markup_end_char(scanner->lookahead)) {
      context[V_STRONG] = false;
      lexer->result_symbol = T_STRONG_MARKER_END;
      return true;
    }
  }
}
