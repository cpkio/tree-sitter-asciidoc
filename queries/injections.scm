; Source blocks: [source,LANG] directly above a listing (----) or
; literal (....) block. A block title can be above or below the
; attribute line. Callouts, comments and include directives in the block
; are named nodes, so the editor removes them from the injected text.
((option_block
  (block_type) @_style
  .
  (attribute) @injection.language)
  .
  (block_title)?
  .
  [
    (block_listing)
    (block_literal)
  ] @injection.content
  (#eq? @_style "source"))
