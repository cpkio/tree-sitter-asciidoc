// vim:sw=2



  if (scanner->lookahead == CHAR_ATTRIBUTE &&
      is_newline(scanner->lookbehind->stack[0])) {
    scanner->advance(scanner);
    if (scanner->lookahead == CHAR_ATTRIBUTENEGATION) {
      scanner->advance(scanner);
      lexer->result_symbol = T_ATTRIBUTE_MARKER_START_NEG;
      context[V_ATTRIBUTE] = true;
      return true;
    }
    lexer->result_symbol = T_ATTRIBUTE_MARKER_START;
    context[V_ATTRIBUTE] = true;
    return true;
  }

  if (context[V_ATTRIBUTE] &&
      scanner->lookahead == CHAR_ATTRIBUTENEGATION &&
      !is_space(scanner->lookbehind->stack[0])) {
    scanner->advance(scanner);
    if (scanner->lookahead == CHAR_ATTRIBUTE) {
      scanner->advance(scanner);
      context[V_ATTRIBUTE] = false;
      lexer->result_symbol = T_ATTRIBUTE_MARKER_END_NEG;
      return true;
    }
  }

  if (context[V_ATTRIBUTE] && scanner->lookahead == CHAR_ATTRIBUTE &&
      !is_space(scanner->lookbehind->stack[0])) {
    scanner->advance(scanner);
    context[V_ATTRIBUTE] = false;
    lexer->result_symbol = T_ATTRIBUTE_MARKER_END;
    return true;
  }
