; @attribute	; attribute annotations (e.g. Python decorators)
; @attribute.builtin	; builtin annotations (e.g. `@property` in Python)
; @boolean	; boolean literals
; @character	; character literals
; @character.special	; special characters (e.g. wildcards)
; @comment	; line and block comments
; @comment.documentation	; comments documenting code
; @comment.error	; error-type comments (e.g. `ERROR`, `FIXME`, `DEPRECATED`)
; @comment.note	; note-type comments (e.g. `NOTE`, `INFO`, `XXX`)
; @comment.todo	; todo-type comments (e.g. `TODO`, `WIP`)
; @comment.warning	; warning-type comments (e.g. `WARNING`, `FIX`, `HACK`)
; @conceal	; captures that are only meant to be concealed
; @conditional	; keywords related to conditionals (e.g. `if` / `else`)
; @conditional.ternary	; ternary operator (e.g. `?` / `:`)
; @constant	; constant identifiers
; @constant.builtin	; built-in constant values
; @constant.macro	; constants defined by the preprocessor
; @constructor	; constructor calls and definitions
; @debug	; keywords related to debugging
; @define	; preprocessor definition directives
; @definition	; various definitions
; @definition.associated	; the associated type of a variable
; @definition.constant	; constants
; @definition.enum	; enumerations
; @definition.field	; fields or properties
; @definition.function	; functions
; @definition.import	; imported names
; @definition.macro	; preprocessor macros
; @definition.method	; methods
; @definition.namespace	; modules or namespaces
; @definition.parameter	; parameters
; @definition.type	; types or classes
; @definition.var	; variables
; @diff.delta	; changed text (for diff files)
; @diff.minus	; deleted text (for diff files)
; @diff.plus	; added text (for diff files)
; @error	; syntax/parser errors
; @exception	; keywords related to exceptions (e.g. `throw` / `catch`)
; @field	; object and struct fields
; @float	; floating-point number literals
; @function	; function definitions
; @function.builtin	; built-in functions
; @function.call	; function calls
; @function.macro	; preprocessor macros
; @function.method	; method definitions
; @function.method.call	; method calls
; @include	; keywords for including modules (e.g. `import` / `from` in Python)
; @keyword	; various keywords
; @keyword.conditional	; keywords related to conditionals (e.g. `if` / `else`)
; @keyword.conditional.ternary	; ternary operator (e.g. `?` / `:`)
; @keyword.coroutine	; keywords related to coroutines (e.g. `go` in Go, `async/await` in Python)
; @keyword.debug	; keywords related to debugging
; @keyword.directive	; various preprocessor directives & shebangs
; @keyword.directive.define	; preprocessor definition directives
; @keyword.exception	; keywords related to exceptions (e.g. `throw` / `catch`)
; @keyword.function	; keywords that define a function (e.g. `func` in Go, `def` in Python)
; @keyword.import	; keywords for including modules (e.g. `import` / `from` in Python)
; @keyword.modifier	; keywords modifying other constructs (e.g. `const`, `static`, `public`)
; @keyword.operator	; operators that are English words (e.g. `and` / `or`)
; @keyword.repeat	; keywords related to loops (e.g. `for` / `while`)
; @keyword.return	; keywords like `return` and `yield`
; @keyword.type	; keywords describing composite types (e.g. `struct`, `enum`)
; @label	; GOTO and other labels (e.g. `label:` in C)
; @markup.heading	; headings, titles (including markers)
; @markup.heading.1	; top-level heading
; @markup.heading.2	; section heading
; @markup.heading.3	; subsection heading
; @markup.heading.4	; and so on
; @markup.heading.5	; and so forth
; @markup.heading.6	; six levels ought to be enough for anybody
; @markup.italic	; italic text
; @markup.link	; text references, footnotes, citations, etc.
; @markup.link.label	; link, reference descriptions
; @markup.link.url	; URL-style links
; @markup.list	; list markers
; @markup.list.checked	; checked todo-style list markers
; @markup.list.unchecked	; unchecked todo-style list markers
; @markup.math	; math environments (e.g. `$ ... $` in LaTeX)
; @markup.quote	; block quotes
; @markup.raw	; literal or verbatim text (e.g. inline code)
; @markup.raw.block	; literal or verbatim text as a stand-alone block
; @markup.strikethrough	; struck-through text
; @markup.strong	; bold text
; @markup.underline	; underlined text (only for literal underline markup!)
; @method	; method definitions
; @method.call	; method calls
; @module	; modules or namespaces
; @module.builtin	; built-in modules or namespaces
; @namespace	; modules or namespaces
; @none	; completely disable the highlight
; @nospell	; for defining regions that should NOT be spellchecked
; @number	; numeric literals
; @number.float	; floating-point number literals
; @operator	; symbolic operators (e.g. `+` / `*`)
; @parameter	; parameters of a function
; @preproc	; various preprocessor directives & shebangs
; @property	; similar to `@field`
; @punctuation.bracket	; brackets (e.g. `()` / `{}` / `[]`)
; @punctuation.delimiter	; delimiters (e.g. `;` / `.` / `,`)
; @punctuation.special	; special symbols (e.g. `{}` in string interpolation)
; @reference	; identifier reference
; @repeat	; keywords related to loops (e.g. `for` / `while`)
; @scope	; scope block
; @spell	; for defining regions to be spellchecked
; @storageclass	; modifiers that affect storage in memory or life-time
; @string	; string literals
; @string.documentation	; string documenting code (e.g. Python docstrings)
; @string.escape	; escape sequences
; @string.regex	; regular expressions
; @string.regexp	; regular expressions
; @string.special	; other special strings (e.g. dates)
; @string.special.path	; filenames
; @string.special.symbol	; symbols or atoms
; @string.special.url	; URIs (e.g. hyperlinks)
; @symbol	; symbols or atoms
; @tag	; XML tag names
; @tag.attribute	; XML tag attributes
; @tag.builtin	; builtin tag names (e.g. HTML5 tags)
; @tag.delimiter	; XML tag delimiters
; @text	; non-structured text
; @text.danger	; danger/error notes
; @text.diff.add	; added text (for diff files)
; @text.diff.delete	; deleted text (for diff files)
; @text.emphasis	; text with emphasis
; @text.environment	; text environments of markup languages
; @text.environment.name	; text indicating the type of an environment
; @text.literal	; literal or verbatim text (e.g., inline code)
; @text.literal.block	; literal or verbatim text as a stand-alone block (use priority 90 for blocks with injections)
; @text.math	; math environments (e.g. `$ ... $` in LaTeX)
; @text.note	; info notes
; @text.quote	; text quotations
; @text.reference	; text references, footnotes, citations, etc.
; @text.strike	; strikethrough text
; @text.strong	; bold text
; @text.title	; text that is part of a title
; @text.todo	; todo notes
; @text.underline	; underlined text
; @text.uri	; URIs (e.g. hyperlinks)
; @text.warning	; warning notes
; @type	; type or class definitions and annotations
; @type.builtin	; built-in types
; @type.definition	; type definitions (e.g. `typedef` in C)
; @type.qualifier	; type qualifiers (e.g. `const`)
; @variable	; various variable names
; @variable.builtin	; built-in variable names (e.g. `this`)
; @variable.member	; object and struct fields
; @variable.parameter	; parameters of a function
; @variable.parameter.builtin	; special parameters (e.g. `_`, `it`)

; USE THIS ONLY

; @attribute	; attribute annotations (e.g. Python decorators, Rust lifetimes)
; @attribute.builtin	; builtin annotations (e.g. `@property` in Python)
; @boolean	; boolean literals
; @character	; character literals
; @character.special	; special characters (e.g. wildcards)
; @comment	; line and block comments
; @comment.documentation	; comments documenting code
; @comment.error	; error-type comments (e.g. `ERROR`, `FIXME`, `DEPRECATED`)
; @comment.note	; note-type comments (e.g. `NOTE`, `INFO`, `XXX`)
; @comment.todo	; todo-type comments (e.g. `TODO`, `WIP`)
; @comment.warning	; warning-type comments (e.g. `WARNING`, `FIX`, `HACK`)
; @conceal	; captures that are only meant to be concealed
; @constant	; constant identifiers
; @constant.builtin	; built-in constant values
; @constant.macro	; constants defined by the preprocessor
; @constructor	; constructor calls and definitions
; @diff.delta	; changed text (for diff files)
; @diff.minus	; deleted text (for diff files)
; @diff.plus	; added text (for diff files)
; @function	; function definitions
; @function.builtin	; built-in functions
; @function.call	; function calls
; @function.macro	; preprocessor macros
; @function.method	; method definitions
; @function.method.call	; method calls
; @keyword	; keywords not fitting into specific categories
; @keyword.conditional	; keywords related to conditionals (e.g. `if` / `else`)
; @keyword.conditional.ternary	; ternary operator (e.g. `?` / `:`)
; @keyword.coroutine	; keywords related to coroutines (e.g. `go` in Go, `async/await` in Python)
; @keyword.debug	; keywords related to debugging
; @keyword.directive	; various preprocessor directives & shebangs
; @keyword.directive.define	; preprocessor definition directives
; @keyword.exception	; keywords related to exceptions (e.g. `throw` / `catch`)
; @keyword.function	; keywords that define a function (e.g. `func` in Go, `def` in Python)
; @keyword.import	; keywords for including modules (e.g. `import` / `from` in Python)
; @keyword.modifier	; keywords modifying other constructs (e.g. `const`, `static`, `public`)
; @keyword.operator	; operators that are English words (e.g. `and` / `or`)
; @keyword.repeat	; keywords related to loops (e.g. `for` / `while`)
; @keyword.return	; keywords like `return` and `yield`
; @keyword.type	; keywords describing composite types (e.g. `struct`, `enum`)
; @label	; GOTO and other labels (e.g. `label:` in C), including heredoc labels
; @markup.heading	; headings, titles (including markers)
; @markup.heading.1	; top-level heading
; @markup.heading.2	; section heading
; @markup.heading.3	; subsection heading
; @markup.heading.4	; and so on
; @markup.heading.5	; and so forth
; @markup.heading.6	; six levels ought to be enough for anybody
; @markup.italic	; italic text
; @markup.link	; text references, footnotes, citations, etc.
; @markup.link.label	; link, reference descriptions
; @markup.link.url	; URL-style links
; @markup.list	; list markers
; @markup.list.checked	; checked todo-style list markers
; @markup.list.unchecked	; unchecked todo-style list markers
; @markup.math	; math environments (e.g. `$ ... $` in LaTeX)
; @markup.quote	; block quotes
; @markup.raw	; literal or verbatim text (e.g. inline code)
; @markup.raw.block	; literal or verbatim text as a stand-alone block
; @markup.strikethrough	; struck-through text
; @markup.strong	; bold text
; @markup.underline	; underlined text (only for literal underline markup!)
; @module	; modules or namespaces
; @module.builtin	; built-in modules or namespaces
; @none	; completely disable the highlight
; @nospell	; for defining regions that should NOT be spellchecked
; @number	; numeric literals
; @number.float	; floating-point number literals
; @operator	; symbolic operators (e.g. `+` / `*`)
; @property	; the key in key/value pairs
; @punctuation.bracket	; brackets (e.g. `()` / `{}` / `[]`)
; @punctuation.delimiter	; delimiters (e.g. `;` / `.` / `,`)
; @punctuation.special	; special symbols (e.g. `{}` in string interpolation)
; @spell	; for defining regions to be spellchecked
; @string	; string literals
; @string.documentation	; string documenting code (e.g. Python docstrings)
; @string.escape	; escape sequences
; @string.regexp	; regular expressions
; @string.special	; other special strings (e.g. dates)
; @string.special.path	; filenames
; @string.special.symbol	; symbols or atoms
; @string.special.url	; URIs (e.g. hyperlinks)
; @tag	; XML-style tag names (and similar)
; @tag.attribute	; XML-style tag attributes
; @tag.builtin	; builtin tag names (e.g. HTML5 tags)
; @tag.delimiter	; XML-style tag delimiters
; @type	; type or class definitions and annotations
; @type.builtin	; built-in types
; @type.definition	; identifiers in type definitions (e.g. `typedef <type> <identifier>` in C)
; @variable	; various variable names
; @variable.builtin	; built-in variable names (e.g. `this`)
; @variable.member	; object and struct fields
; @variable.parameter	; parameters of a function
; @variable.parameter.builtin	; special parameters (e.g. `_`, `it`)

[
  (block_title)
  (page_break)
  (thematic_break)
]
@text.title @spell

(title0) @markup.heading.1
(title1) @markup.heading.2
(title2) @markup.heading.3
(title3) @markup.heading.4
(title4) @markup.heading.5
(title5) @markup.heading.6
(block_title) @markup.heading

[
  (attr_name)
  (ifdef_attribute)
] @attribute @nospell

[
  ; (tag_name)
  (attribute_reference)
]
@variable @nospell

[
  (block_comment)
  (comment)
] @comment

[
  (tag)
  (ifdef)
  (ifeval)
  (endif)
] @preproc @conditional @nospell


[
  (table_cell_marker1)
  (table_cell_marker2)
] @character

(hardbreak) @character

[
  (include_tag)
] @label @nospell

[
  (admonition_option_note)
  (admonition_note_label)
] @label.note
[
  (admonition_option_tip)
  (admonition_tip_label)
] @label.tip
[
  (admonition_option_important)
  (admonition_important_label)
] @label.important
[
  (admonition_option_caution)
  (admonition_caution_label)
] @label.caution
[
  (admonition_option_warning)
  (admonition_warning_label)
] @label.warning

[
  (table_column_span_factor)
  (table_row_span_factor)
  (table_span_operator)
] @attribute

[
  (table_cell_multiplication_factor)
  (table_cell_multiplication_operator)
] @attribute

[
  (table_horizontal_alignment_operator)
  (table_vertical_alignment_operator)
] @attribute

[
  (table_cell_style)
  (cols)
  (float)
  (format)
  (frame)
  (grid)
  (separator)
  (stripes)
  (width)
] @label

[
  (option_block)
] @attribute @nospell

(callout_mark) @number

[
  (id)
] @text.reference

[
  (role)
  (parameter)
] @define @nospell

[
  (option_subs)
  (linktext)
] @text.literal

[
  (antora_resource_version)
  (antora_resource_component)
  (antora_resource_module)
  (antora_resource_family)
  (file_path)
  (cross_reference)
] @text.reference @nospell

[
  (leveloffset)
  (lines)
  (indent)
  (encoding)
  (optional)
] @parameter

[
  (include_directive)
  (xref_directive)
] @include @nospell

[
  (ifeval_expr)
] @conditional @nospell

[
  (monospace)
  (monospace_u)
  (monospace_marker_start)
  (monospace_marker_end)
] @text.quote @nospell

[
  (inline_passthrough)
  (inline_passthrough_u)
  (inline_passthrough_marker_start)
  (inline_passthrough_marker_end)
] @text.literal @nospell

[
  (emphasis)
  (emphasis_u)
] @text.emphasis

[
  (strong)
  (strong_u)
] @text.strong

[
  (highlight)
  (highlight_u)
] @text.highlight

[
  (superscript)
  (superscript_u)
] @text.underline

[
  (subscript)
  (subscript_u)
] @text.underline

[
  (escape)
  (replacement)
] @string.escape

[
  (list_asterisk_marker)
  (list_hyphen_marker)
  (list_numbered_marker)
  (list_box)
  (block_continuation)
  (encoded_symbol)
  (description_list_marker)
] @character @nospell

[
  (block_example_separator1)
  (block_example_separator2)
  (block_example_separator3)
  (block_listing_separator)
  (block_literal_separator)
  (block_open_separator)
  (block_sidebar_separator)
  (block_passthrough_separator)
  (block_quote_separator)
  (block_table_separator1)
  (block_table_separator2)
  (icon_macro)
  (footnote_macro)
  (link_macro)
] @string.special

; [
  ; (http)
  ; (email)
; ] @text.uri

[
  (attr_value)
  (paragraph)
] @spell

[
 (image_marker)
 (image_inline_marker)
] @image

[
  (ERROR)
] @error
