;From nvim-treesitter/nvim-treesitter

(title0) @text.title.0
(title1) @text.title.1
(title2) @text.title.2
(title3) @text.title.3
(title4) @text.title.4
(title5) @text.title.5

[
  (list_item_mark)
  (table_start)
  (table_end)
  (attr_mark)
  (attr_name)
] @punctuation.special

(comment) @comment
; (code) @document.code

(note) @text.note

[
  (tip)
  (important)
  (caution)
  (warning)
] @function.builtin

[
  (url)
  (audio_url)
  (xref_url)
] @text.uri

(emphasis) @text.emphasis
(strong) @text.strong
(footnote) @text.footnote
(highlight) @text.warning
(line_breaks) @punctuation.special
(page_breaks) @punctuation.special
; (code_language) @parameter

[
  ; (code)
  (kbd)
  (footnote)
  (passthrough)
  (audio)
  (video)
  (image)
  (xref)
] @function

[
  (monospace)
  (replacement)
  (kbd_content)
  (footnote_content)
  (passthrough_content)
  (audio_title)
  (attr_value)
] @text.literal

(include_directive) @function
(include_param) @parameter
[
    (antora_resource_version)
    (antora_resource_component)
    (antora_resource_module)
] @parameter

(antora_resource_family) @keyword
(antora_resource_file) @parameter

[
  (monospace)
] @nospell
