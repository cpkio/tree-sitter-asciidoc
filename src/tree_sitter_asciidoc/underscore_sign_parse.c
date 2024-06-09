// vim:sw=2
if (
  !(context[V_MONOSPACE]) &&
  !(context[V_LITERAL]) &&
  !(context[V_LISTING]) &&
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
    lexer->result_symbol = T_BLOCK_QUOTE_SEPARATOR;
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
    context[V_EMPHASIS] = true;
    lexer->result_symbol = T_EMPHASIS_MARKER_START;
    return true;
  }

  if (
      context[V_EMPHASIS] &&
      !is_space(scanner->lookbehind->stack[1])) {
    if (is_newline(scanner->lookahead) || is_space(scanner->lookahead) ||
        is_punctuation(scanner->lookahead) ||
        is_inline_markup_end_char(scanner->lookahead)) {
      context[V_PASSTHROUGH] = false;
      lexer->result_symbol = T_EMPHASIS_MARKER_END;
      return true;
    }
  }
}
