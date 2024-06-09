// vim:sw=2
/*

[x] BLOCK PASSTHROUGH

[x] BLOCK CONTINUATION

[x] HARDBREAK

[x] INLINE PASSTHROUGH

*/

if (
  (
    valid_symbols[T_BLOCK_PASSTHROUGH_SEPARATOR] ||
    valid_symbols[T_BLOCK_CONTINUATION] ||
    valid_symbols[T_HARDBREAK] ||
    valid_symbols[T_INLINE_PASSTHROUGH_MARKER_START] ||
    valid_symbols[T_INLINE_PASSTHROUGH_MARKER_END]
  ) &&
  !(context[V_LITERAL]) &&
  !(context[V_LISTING]) &&
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
    // scanner->skip(scanner);
    lexer->result_symbol = T_HARDBREAK;
    return true;
  }

  if (!(context[V_PASSTHROUGH]) &&
      !(context[V_XREF_LOCATOR]) &&
      (is_newline(scanner->lookbehind->stack[1]) ||
       is_space(scanner->lookbehind->stack[1]) ||
       is_punctuation(scanner->lookbehind->stack[1]) ||
       is_inline_markup_start_char(scanner->lookbehind->stack[1])) &&
      !is_space(scanner->lookahead)
  ) {
    context[V_PASSTHROUGH] = true;
    lexer->result_symbol = T_INLINE_PASSTHROUGH_MARKER_START;
    return true;
  }

  if (context[V_PASSTHROUGH] &&
      !is_space(scanner->lookbehind->stack[1])) {
    if (is_newline(scanner->lookahead) || is_space(scanner->lookahead) ||
        is_punctuation(scanner->lookahead) ||
        is_inline_markup_end_char(scanner->lookahead)) {
      context[V_PASSTHROUGH] = false;
      lexer->result_symbol = T_INLINE_PASSTHROUGH_MARKER_END;
      return true;
    }
  }
}
