archive/bootstrap-rs/heroes/src/library/source.hero 1  8 error[old_cell_symbol];
examples/collatz/main.hero 0 
examples/ctime/main.hero 0 
examples/curl/main.hero 0 
examples/dotproduct/main.hero 0 
examples/firsthit/main.hero 0 
examples/gallery/08-ffi.hero 0 
examples/gallery/13-lease.hero 0 
examples/histogram/main.hero 0 
examples/ledger/db/sqlite.hero 0 
examples/mandelbrot/main.hero 0 
examples/matmul/main.hero 0 
examples/montecarlo/main.hero 0 
examples/nbody/main.hero 0 
examples/nqueens/main.hero 0 
examples/raylib/main.hero 1  1 error[ffi_package];
examples/sdl/main.hero 1  1 error[ffi_package];
examples/spectral/main.hero 0 
examples/sqlite/main.hero 0 
examples/tally/main.hero 0 
examples/threads/main.hero 0 
examples/wordbands/main.hero 0 
selfhost/cli/files.hero 1  1 error[unknown_module];
selfhost/cli/io.hero 0 
selfhost/cli/link.hero 1  13 error[unknown_module];
selfhost/cli/process.hero 1  2 error[unknown_module];
selfhost/cli/runtime_places.hero 1  2 error[unknown_module];
selfhost/cli/symbolized.hero 1  1 error[unknown_module];
selfhost/emit/literal.hero 1  4 error[unknown_module];
selfhost/module/reading.hero 1  7 error[unknown_module];
selfhost/mutate/child.hero 1  1 error[unknown_module];
selfhost/mutate/cut.hero 1  13 error[unknown_module];
selfhost/mutate/recovery.hero 1  7 error[unknown_module];
tests/golden/check/end-lease-takes-only-a-lease.hero 1  1 error[end_lease_of_no_lease]; 1 error[marker_mismatch];
tests/golden/check/ffi-a-construction-gives-more-arguments-than-fields.hero 1  1 error[wrong_label];
tests/golden/check/ffi-a-construction-leaves-a-label-out.hero 1  1 error[missing_label];
tests/golden/check/ffi-a-construction-misspells-a-label.hero 1  1 error[wrong_label];
tests/golden/check/ffi-a-construction-names-a-field-twice.hero 1  1 error[wrong_label];
tests/golden/check/ffi-a-construction-names-its-labels-out-of-order.hero 1  1 error[wrong_label];
tests/golden/check/ffi-a-group-head-names-not-locates.hero 1  1 error[machine_locked_path]; 2 error[unwritable_name];
tests/golden/check/ffi-a-lend-parked-in-a-group-record.hero 1  1 error[cstr_escapes]; 1 error[lease_escapes];
tests/golden/check/ffi-a-lend-reaches-only-a-lent-parameter.hero 1  1 error[field_lend_uncounted]; 2 error[lend_kept]; 1 error[lend_needs_a_header];
tests/golden/check/ffi-a-lent-field-needs-a-place.hero 1  1 error[bad_operand]; 1 error[field_lend_escapes]; 1 error[field_lend_needs_a_header]; 1 error[field_lend_needs_a_place];
tests/golden/check/ffi-callback-carries-a-container.hero 1  1 error[ffi_type];
tests/golden/check/ffi-callback-position.hero 1  1 error[ffi_constant_type]; 1 error[ffi_field_type]; 1 error[ffi_parameter_type]; 1 error[ffi_type];
tests/golden/check/ffi-constant-type.hero 1  2 error[ffi_constant_type];
tests/golden/check/ffi-consumes-a-borrowed-handle.hero 1  1 error[consumed_borrowed_handle];
tests/golden/check/ffi-counted-by-shape.hero 1  3 error[counted_by_shape]; 1 error[field_lend_uncounted];
tests/golden/check/ffi-counted-by-states-its-extent.hero 1  5 error[counted_by_shape]; 1 error[ffi_type];
tests/golden/check/ffi-cstr-has-a-route.hero 1  1 error[bad_operand];
tests/golden/check/ffi-fixed-array.hero 1  1 error[ffi_field_type]; 1 error[fixed_array_length]; 1 error[fixed_index_out_of_range]; 1 error[fixed_outside_a_group];
tests/golden/check/ffi-handle-refusals.hero 1  2 error[duplicate_tag]; 2 error[handle_map_key];
tests/golden/check/ffi-lent-is-read-on-a-lend-and-nowhere-else.hero 1  3 error[lent_shape];
tests/golden/check/ffi-nullptr-is-a-cstr.hero 1  2 error[type_mismatch];
tests/golden/check/ffi-parameter-width.hero 1  1 error[type_mismatch];
tests/golden/check/ffi-partial-record.hero 1  6 error[ffi_partial_operation];
tests/golden/check/ffi-record-field.hero 1  6 error[ffi_field_type];
tests/golden/check/ffi-transfers-a-borrowed-handle.hero 1  1 error[consumed_borrowed_handle];
tests/golden/check/ffi-type.hero 1  2 error[ffi_type];
tests/golden/check/fixedbugs-092-counted-by-is-read-on-a-record-lent-whole.hero 1  4 error[counted_by_shape];
tests/golden/check/fixedbugs-130-a-block-below-a-line-that-failed-past-its-declaration.hero 1  10 error[expected_declaration]; 2 error[expected_end_of_line]; 1 error[expected_extern_header]; 5 error[expected_extern_signature]; 1 error[expected_module_name]; 3 error[extern_has_body]; 2 error[reserved_word]; 2 error[use_wants_a_name];
tests/golden/check/fixedbugs-130-a-bracket-left-open-goes-on-where-a-bracket-can-hold-the-line.hero 1  2 error[expected_parameter]; 1 error[missing_body]; 2 error[unclosed_bracket];
tests/golden/check/fixedbugs-130-a-declaration-past-a-bracket-never-closed.hero 1  1 error[expected_declaration]; 6 error[expected_end_of_line]; 1 error[expected_field_type]; 8 error[unclosed_bracket];
tests/golden/check/fixedbugs-130-a-function-among-a-records-fields.hero 1  3 error[expected_end_of_line]; 6 error[expected_expression]; 4 error[expected_field]; 1 error[expected_field_type]; 5 error[reserved_word];
tests/golden/check/fixedbugs-130-a-head-with-no-name-before-braces-says-where-its-block-goes.hero 1  1 error[expected_extern_header]; 6 error[expected_name]; 1 error[expected_test_name]; 1 error[reserved_word];
tests/golden/check/fixedbugs-130-a-member-line-holds-one-member.hero 1  18 error[expected_end_of_line]; 1 error[reserved_word];
tests/golden/check/fixedbugs-130-an-empty-declaration-above-a-refused-line.hero 1  1 error[empty_record]; 1 error[empty_variant]; 1 error[expected_extern_block]; 3 error[missing_body]; 4 error[reserved_word]; 2 error[unexpected_character];
tests/golden/check/fixedbugs-130-an-extern-group-whose-header-failed.hero 1  1 error[char_literal]; 3 error[expected_end_of_line]; 1 error[expected_extern_header]; 1 error[unterminated_string];
tests/golden/check/fixedbugs-130-an-extern-member-left-open-ends-at-the-next-member.hero 1  1 error[expected_extern_signature]; 1 error[unclosed_bracket];
tests/golden/check/fixedbugs-130-commas-in-braces-are-told-as-without-them.hero 1  17 error[expected_end_of_line]; 1 error[expected_extern_signature]; 1 error[missing_body]; 2 error[reserved_word];
tests/golden/check/fixedbugs-130-members-in-braces-are-read.hero 1  2 error[empty_record]; 10 error[expected_end_of_line]; 4 error[expected_field_type]; 1 error[expected_parameter_type];
tests/golden/check/fixedbugs-131-a-brace-alone-below-a-member-head.hero 1  3 error[empty_record]; 1 error[empty_variant]; 1 error[expected_case]; 1 error[expected_end_of_line]; 1 error[expected_extern_block]; 1 error[expected_field_type]; 1 error[expected_parameter_type];
tests/golden/check/fixedbugs-131-a-brace-habit-never-closed-is-told-once.hero 1  7 error[expected_end_of_line]; 1 error[expected_field_type]; 2 error[missing_body]; 1 error[missing_match_arms];
tests/golden/check/fixedbugs-131-a-margin-in-tabs-is-one-message.hero 1  11 error[tab_in_indentation];
tests/golden/check/fixedbugs-131-a-record-or-variant-head-that-goes-on-over-its-members.hero 1  1 error[empty_record]; 1 error[empty_variant]; 6 error[expected_end_of_line]; 1 error[expected_extern_block]; 2 error[expected_field_type];
tests/golden/check/fixedbugs-131-the-brace-habit-is-one-mistake.hero 1  5 error[expected_end_of_line]; 1 error[expected_expression]; 4 error[missing_body];
tests/golden/check/fixedbugs-131-two-spaces-a-level-is-one-message.hero 1 
tests/golden/check/fixedbugs-132-a-comma-after-every-member-is-one-message.hero 1  5 error[expected_end_of_line]; 2 error[expected_extern_signature];
tests/golden/check/fixedbugs-133-a-group-member-opened-by-a-foreign-word.hero 1  1 error[expected_extern_block]; 1 error[expected_extern_signature]; 1 error[expected_field_type]; 6 error[reserved_word];
tests/golden/check/fixedbugs-133-a-refused-word-heading-a-declaration-over-no-body.hero 1  2 error[empty_record]; 1 error[empty_variant]; 1 error[expected_extern_block]; 9 error[missing_body]; 14 error[reserved_word]; 1 error[unexpected_character];
tests/golden/check/fixedbugs-133-a-swap-certain-only-where-its-word-is-right.hero 1  10 error[reserved_word];
tests/golden/check/fixedbugs-135-l6-int-in-an-extern-group.hero 1  6 error[reserved_word];
tests/golden/check/fixedbugs-135-p8-a-result-after-a-colon-over-no-body.hero 1  5 error[expected_end_of_line]; 3 error[missing_body];
tests/golden/check/fixedbugs-138-a-mark-the-library-does-not-write.hero 1  1 error[contract_differs];
tests/golden/check/fixedbugs-138-a-mark-the-library-writes-left-out.hero 1  1 error[contract_differs];
tests/golden/check/fixedbugs-138-a-result-mark-the-library-does-not-write.hero 1  1 error[contract_differs];
tests/golden/check/fixedbugs-138-an-at-the-library-writes-left-out.hero 1  1 error[contract_differs];
tests/golden/check/fixedbugs-140-a-handle-key-a-thousand-deep-is-refused.hero 1  1 error[handle_map_key];
tests/golden/check/fixedbugs-140-a-handle-key-in-variants-a-thousand-deep-is-refused.hero 1  1 error[handle_map_key];
tests/golden/check/fixedbugs-140-a-partial-a-thousand-deep-is-refused.hero 1  1 error[ffi_partial_operation];
tests/golden/check/fixedbugs-140-a-partial-in-variants-a-thousand-deep-is-refused.hero 1  1 error[ffi_partial_operation];
tests/golden/check/fixedbugs-146-a-file-of-foreign-heads-is-placed-once.hero 1  2 error[missing_body]; 13 error[reserved_word];
tests/golden/check/fixedbugs-172-a-callbacks-types-are-the-headers-own.hero 1  2 error[reserved_word];
tests/golden/check/fixedbugs-172-a-constant-may-be-wider.hero 1  2 error[reserved_word];
tests/golden/check/fixedbugs-172-a-parameter-and-a-field-are-the-headers-own-width.hero 1  4 error[reserved_word];
tests/golden/check/fixedbugs-172-a-result-the-header-gives-64-bits.hero 1  2 error[reserved_word];
tests/golden/check/fixedbugs-187-the-words-rest-zero-are-kept-in-place.hero 1  6 error[rest_outside_a_group];
tests/golden/check/fixedbugs-198-a-heads-junk-and-a-closer-past-it-are-told.hero 1  3 error[empty_record]; 2 error[empty_variant]; 13 error[expected_end_of_line]; 1 error[expected_extern_block]; 3 error[missing_body];
tests/golden/check/fixedbugs-216-a-backslash-in-a-header-name.hero 1  8 error[escape_in_header_name];
tests/golden/check/fixedbugs-216-a-control-character-in-a-name.hero 1  2 error[unshowable_name]; 3 error[unwritable_name];
tests/golden/check/fixedbugs-216-a-header-name-c-leaves-undefined.hero 1  2 error[undefined_header_name]; 3 error[unwritable_name];
tests/golden/check/fixedbugs-216-a-name-c-cannot-carry.hero 1  8 error[unwritable_name];
tests/golden/check/fixedbugs-216-a-package-names-one-package.hero 1  3 error[machine_locked_path]; 1 error[option_like_name]; 4 error[unwritable_name];
tests/golden/check/fixedbugs-216-c-spelling-carried-into-a-header.hero 1  2 error[unwritable_name];
tests/golden/check/fixedbugs-222-a-library-name-beginning-with-a-dash.hero 1  4 error[option_like_name];
tests/golden/check/fixedbugs-225-a-climb-to-the-root-is-a-known-cost.hero 1  1 error[machine_locked_path];
tests/golden/check/fixedbugs-233-a-librarys-route-names-the-compilers-own-flag.hero 1  1 error[unwritable_name];
tests/golden/check/fixedbugs-234-a-name-windows-reads-as-another-file.hero 1  6 error[machine_locked_path]; 6 error[unwritable_name];
tests/golden/check/fixedbugs-235-a-name-no-windows-file-can-hold.hero 1  8 error[unwritable_name];
tests/golden/check/fixedbugs-250-an-escape-in-a-header-name-is-told-once-by-the-head.hero 1  3 error[escape_in_header_name];
tests/golden/check/fixedbugs-251-a-control-character-in-a-group-head-is-the-head-s-to-tell.hero 1  2 error[unwritable_name];
tests/golden/check/fixedbugs-251-an-escape-by-code-is-a-string-s-alone.hero 1  1 error[unknown_escape]; 1 error[unshowable_name]; 1 error[unwritable_name];
tests/golden/check/fixedbugs-252-a-library-name-no-windows-linker-reads-as-one-file.hero 1  2 error[package_comparison]; 9 error[unwritable_name];
tests/golden/check/fixedbugs-253-a-device-named-by-a-superscript-digit-or-the-console.hero 1  10 error[unwritable_name];
tests/golden/check/fixedbugs-255-a-nul-in-a-group-head-s-name-is-the-head-s-to-tell.hero 1  5 error[unwritable_name];
tests/golden/check/fixedbugs-264-built-ins-found-by-name-along-a-chain-two-thousand-long.hero 1  1 error[builtin_as_value]; 1 error[cstr_escapes]; 4 error[needs_label]; 2 error[wrong_label];
tests/golden/check/fixedbugs-396-a-buffer-is-declared-whole.hero 1  1 error[counted_by_shape]; 4 error[ffi_parameter_type]; 1 error[ffi_type];
tests/golden/check/fixedbugs-397-a-handle-is-never-built.hero 1  8 error[handle_built];
tests/golden/check/fixedbugs-398-a-function-with-an-at-parameter-has-no-value.hero 1  12 error[mutable_parameter_as_value];
tests/golden/check/fixedbugs-401-a-binding-called-zero-hides-no-mistake-at-the-words.hero 1  1 error[rest_is_a_field]; 2 error[rest_not_a_construction]; 2 error[rest_outside_a_group]; 5 error[unused_binding];
tests/golden/check/fixedbugs-401-a-constant-called-zero.hero 1  1 error[rest_not_a_construction]; 1 error[rest_outside_a_group];
tests/golden/check/fixedbugs-405-a-nested-fixed-array-outside-a-group-is-told-once.hero 1  1 error[ffi_field_type]; 2 error[ffi_type]; 14 error[fixed_outside_a_group];
tests/golden/check/fixedbugs-409-every-handle-read-after-its-end-is-told-in-order.hero 1  20 error[handle_used_after_end];
tests/golden/check/fixedbugs-412-an-at-argument-to-a-construction.hero 1  12 error[marker_mismatch];
tests/golden/check/fixedbugs-423-a-fixed-array-built-from-outside-a-group-is-told-once.hero 1  1 error[ffi_type]; 1 error[fixed_array_length]; 15 error[fixed_outside_a_group]; 4 error[type_mismatch];
tests/golden/check/fixedbugs-431-an-at-argument-a-call-takes-none-for.hero 1  9 error[marker_mismatch];
tests/golden/check/fixedbugs-432-a-use-of-a-refused-fixed-array-is-judged-as-its-growable.hero 1  2 error[bad_operand]; 4 error[fixed_outside_a_group];
tests/golden/check/fixedbugs-439-an-at-past-the-last-parameter.hero 1  1 error[not_callable]; 5 error[wrong_arity];
tests/golden/check/fixedbugs-440-a-strings-byte-is-no-place-for-an-at-argument.hero 1  6 error[str_element_write];
tests/golden/check/fixedbugs-456-a-broken-type-beside-a-head-is-told-once.hero 1  3 error[expected_function_type];
tests/golden/check/fixedbugs-499-three-hundred-handles-ended-each-on-its-own-local.hero 1  7 error[handle_used_after_end];
tests/golden/check/fixedbugs-511-handles-ended-then-three-hundred-ifs-meet-what-changed.hero 1  6 error[handle_used_after_end];
tests/golden/check/fixedbugs-530-three-hundred-ifs-each-ending-a-handle-copy-what-they-change.hero 1  4 error[handle_used_after_end];
tests/golden/check/fixedbugs-532-a-type-is-named-with-no-article-chosen-by-hand.hero 1  1 error[cell_cannot_end]; 1 error[hole_not_renderable]; 1 error[nested_read]; 2 error[not_callable]; 1 error[not_iterable]; 1 error[not_matchable]; 1 error[owned_needs_cstr]; 1 error[unmarked_handle_producer]; 1 error[unread_releaser];
tests/golden/check/fixedbugs-629-a-handle-ended-through-the-dot-is-judged-as-the-named-call.hero 1  4 error[consumed_borrowed_handle];
tests/golden/check/fixedbugs-629-a-handle-handed-away-through-the-dot-is-judged-as-the-named-call.hero 1  2 error[consumed_borrowed_handle];
tests/golden/check/fixedbugs-a-crossed-free-between-two-void-families.hero 1  2 error[type_mismatch];
tests/golden/check/fixedbugs-a-handle-given-back-after-its-address-was-handed-out-again.hero 1  2 error[handle_used_after_end];
tests/golden/check/fixedbugs-a-handle-reached-through-a-field.hero 1  4 error[unmarked_handle_producer];
tests/golden/check/fixedbugs-a-handle-read-after-the-call-that-ended-it.hero 1  2 error[handle_used_after_end];
tests/golden/check/fixedbugs-a-lend-bound-instead-of-passed.hero 1  1 error[cstr_escapes];
tests/golden/check/fixedbugs-a-lend-c-keeps-past-the-frame.hero 1  2 error[lend_kept];
tests/golden/check/fixedbugs-a-lend-laundered-through-a-helper.hero 1  2 error[cstr_out_of_heroes]; 1 error[lend_needs_a_header];
tests/golden/check/fixedbugs-a-lend-parked-in-a-record.hero 1  1 error[cstr_in_a_record];
tests/golden/check/fixedbugs-a-lend-that-leaves-its-frame.hero 1  1 error[cstr_escapes]; 1 error[cstr_out_of_heroes];
tests/golden/check/fixedbugs-a-nesting-bound-was-a-silence.hero 1  2 error[ffi_partial_operation]; 2 error[float_map_key];
tests/golden/check/fixedbugs-a-record-rewritten-under-a-held-address.hero 1  1 error[lend_kept];
tests/golden/check/fixedbugs-a-releaser-that-ends-nothing.hero 1  2 error[unread_releaser];
tests/golden/check/fixedbugs-at-marker-on-a-builtin.hero 1  4 error[marker_mismatch];
tests/golden/check/fixedbugs-owned-asks-the-result-too.hero 1  3 error[owned_needs_cstr];
tests/golden/check/fixedbugs-owned-on-a-by-value-parameter.hero 1  1 error[owned_needs_cstr];
tests/golden/check/fixedbugs-unit-as-an-extern-parameter.hero 1  1 error[ffi_parameter_type];
tests/golden/check/handle-a-consumed-part-is-dead-for-its-reads-and-the-whole-crossing.hero 1  5 error[handle_used_after_end];
tests/golden/check/handle-ended-on-every-path-is-refused-at-the-read.hero 1  6 error[handle_used_after_end];
tests/golden/check/inferred-cell-born-of-nullptr-is-told-at-its-birth.hero 1  1 error[cannot_infer];
tests/golden/check/lease-cell-written-by-another-hand.hero 1  2 error[lease_written];
tests/golden/check/lease-copied-out-of-its-cell.hero 1  2 error[lease_escapes];
tests/golden/check/marks-in-any-order.hero 1  2 error[one_mark_per_position];
tests/golden/check/never-written-parameter-what-writes-it.hero 1  1 error[discarded_value];
tests/golden/check/out-cell-cannot-end.hero 1  2 error[cell_cannot_end];
tests/golden/check/owned-freer-called.hero 1  1 error[owned_freer_called];
tests/golden/check/owned-freer-must-be-declarable.hero 1  1 error[freer_arity]; 2 error[unknown_freer];
tests/golden/check/owned-names-its-freer.hero 1  2 error[expected_freer];
tests/golden/check/repeated-mark.hero 1  6 error[repeated_mark];
tests/golden/check/rest-zero-comes-last.hero 1  1 error[rest_not_last];
tests/golden/check/rest-zero-ends-only-a-construction-of-a-groups-record.hero 1  1 error[rest_is_a_field]; 3 error[rest_not_a_construction]; 1 error[rest_outside_a_group];
tests/golden/check/success-clause-misuse.hero 1  1 error[one_mark_per_position]; 2 error[success_value_type]; 2 error[success_without_end]; 1 error[when_on_void];
tests/golden/check/success-clause-placement.hero 1  1 error[expected_releaser]; 4 error[expected_success_value]; 1 error[success_needs_result]; 1 error[success_on_parameter];
tests/golden/check/success-value-out-of-range.hero 1  5 error[success_value_range];
tests/golden/check/transfer-into-a-sibling-that-does-not-survive.hero 1  3 error[transfer_without_receiver];
tests/golden/check/transfer-without-receiver.hero 1  1 error[transfer_without_receiver];
tests/golden/check/unmarked-handle-producer.hero 1  2 error[unmarked_handle_producer];
tests/golden/check/unread-mark.hero 1  7 error[unread_mark];
tests/golden/check/unread-releaser-in-a-set.hero 1  1 error[unread_releaser];
tests/golden/check/unread-releaser-names-its-word.hero 1  3 error[unread_releaser];
tests/golden/emit/ffi-lent-emits-nothing.hero 0 
tests/golden/fixedbugs/a-header-field-that-is-not-const.hero 0 
tests/golden/fixedbugs/ffi-a-field-the-header-has-as-an-array.hero 1  1 error[ffi_field_type];
tests/golden/fixedbugs/ffi-a-freer-that-cannot-free-a-string.hero 1  1 error[ffi_freer_type];
tests/golden/fixedbugs/ffi-a-lent-field-c-would-write.hero 1  1 error[field_lend_written];
tests/golden/fixedbugs/ffi-a-lent-field-extent-overstated.hero 1  3 error[field_lend_extent];
tests/golden/fixedbugs/ffi-a-null-function-pointer-says-so.hero 0 
tests/golden/fixedbugs/ffi-a-stated-extent-past-the-place.hero 1  3 error[field_lend_extent]; 1 error[record_lend_extent];
tests/golden/fixedbugs/ffi-call-arity.hero 1  1 error[ffi_call_shape];
tests/golden/fixedbugs/ffi-call-format.hero 1  1 error[ffi_call_shape];
tests/golden/fixedbugs/ffi-callback-const-pointee.hero 1  1 error[ffi_callback_type];
tests/golden/fixedbugs/ffi-callback-signature-the-header-refutes.hero 1  1 error[ffi_callback_type];
tests/golden/fixedbugs/ffi-callback-typedef-reaches-the-probe.hero 0 
tests/golden/fixedbugs/ffi-const-pointer.hero 0 
tests/golden/fixedbugs/ffi-constant-type.hero 1  1 error[ffi_constant_type];
tests/golden/fixedbugs/ffi-cstr-is-not-any-pointer.hero 1  1 error[ffi_return_type];
tests/golden/fixedbugs/ffi-f32-against-a-float-header-is-correct.hero 0 
tests/golden/fixedbugs/ffi-float-parameters.hero 1  4 error[ffi_parameter_type];
tests/golden/fixedbugs/ffi-int-narrowing-below-32.hero 1  1 error[ffi_parameter_type];
tests/golden/fixedbugs/ffi-missing-link.hero 0 
tests/golden/fixedbugs/ffi-not-constant.hero 1  1 error[ffi_not_constant];
tests/golden/fixedbugs/ffi-out-cell-on-a-handle-the-header-takes-by-value.hero 1  1 error[ffi_parameter_type];
tests/golden/fixedbugs/ffi-out-cell-on-a-ptr-the-header-takes-by-value.hero 1  1 error[ffi_parameter_type];
tests/golden/fixedbugs/ffi-out-parameter-guard.hero 0 
tests/golden/fixedbugs/ffi-owned-on-a-cell-a-typedef-spells-const.hero 1  1 error[ffi_owned_const_cell];
tests/golden/fixedbugs/ffi-owned-on-a-cell-the-header-spells-const.hero 1  1 error[ffi_owned_const_cell];
tests/golden/fixedbugs/ffi-parameter-width.hero 1  1 error[ffi_parameter_type];
tests/golden/fixedbugs/ffi-pointee-opaque.hero 1  1 error[ffi_parameter_type];
tests/golden/fixedbugs/ffi-pointee-sign.hero 1  1 error[ffi_parameter_type];
tests/golden/fixedbugs/ffi-pointee-void.hero 1  1 error[ffi_parameter_type];
tests/golden/fixedbugs/ffi-pointee-width.hero 1  1 error[ffi_parameter_type];
tests/golden/fixedbugs/ffi-pointer-return.hero 1  1 error[ffi_return_type];
tests/golden/fixedbugs/ffi-return-type.hero 1  1 error[ffi_return_type];
tests/golden/fixedbugs/ffi-struct-result-unchecked.hero 1  1 error[ffi_return_type];
tests/golden/fixedbugs/ffi-struct-return.hero 1  1 error[ffi_return_type];
tests/golden/fixedbugs/ffi-unknown-name.hero 1  1 error[ffi_unknown_name];
tests/golden/fixedbugs/ffi-word-width.hero 1  2 error[ffi_parameter_type];
tests/golden/fixedbugs/fixedbugs-396-a-buffer-of-another-element.hero 1  4 error[ffi_parameter_type];
tests/golden/fixedbugs/fixedbugs-563-another-module-s-record-is-read-without-its-module-s-groups/holding.hero 0 
tests/golden/fixedbugs/fixedbugs-a-misspelled-handle-tag-stays-refused.hero 1  1 error[ffi_unknown_name];
tests/golden/fixedbugs/fixedbugs-a-tag-with-two-kinds-names-the-right-record.hero 1  1 error[ffi_unknown_tag];
tests/golden/full/fixedbugs-399-a-fixed-array-outside-a-group-is-told-the-repair-that-works.hero 1  5 error[fixed_outside_a_group];
tests/golden/full/fixedbugs-401-what-a-binding-called-zero-beside-the-words-is-told.hero 1  2 error[rest_outside_a_group]; 1 error[unused_binding];
tests/golden/full/fixedbugs-408-a-fixed-array-built-into-a-group-record-is-told-rest-zero.hero 1  8 error[fixed_outside_a_group]; 1 error[type_mismatch];
tests/golden/full/fixedbugs-412-what-an-at-in-a-construction-is-told.hero 1  2 error[marker_mismatch];
tests/golden/full/fixedbugs-424-a-fixed-array-at-a-function-s-result-is-told-results-hold-none.hero 1  2 error[fixed_outside_a_group];
tests/golden/ir/owned-cell.hero 1  1 error[ffi_unknown_name];
tests/golden/ir/owned-release.hero 0 
tests/golden/ir/regression-extern-parameter-types.hero 1  1 error[ffi_parameter_type];
tests/golden/lldb/formatter-steps-aside.hero 0 
tests/golden/permissive/fixedbugs-445-the-control-arm-refuses-a-control-byte-no-windows-file-holds.hero 1  2 error[unshowable_name]; 4 error[unwritable_name];
tests/golden/permissive/fixedbugs-445-the-control-arm-refuses-a-header-no-windows-file-holds.hero 1  1 error[undefined_header_name]; 5 error[unwritable_name];
tests/golden/permissive/fixedbugs-446-the-control-arm-refuses-a-name-ending-in-a-colon-and-keeps-a-stream.hero 1  3 error[machine_locked_path]; 5 error[unwritable_name];
tests/golden/permissive/fixedbugs-446-the-control-arm-refuses-a-rooted-library-no-linker-reads-as-written.hero 1  3 error[machine_locked_path]; 6 error[unwritable_name];
tests/golden/run/a-panic-with-a-live-lease-says-one-thing.hero 0 
tests/golden/run/abort-a-fixed-array-element-written-past-its-length.hero 0 
tests/golden/run/abort-handle-borrows-that-gives-away.hero 0 
tests/golden/run/abort-handle-given-back-twice.hero 0 
tests/golden/run/abort-handle-never-given-back.hero 0 
tests/golden/run/abort-lease-never-ended.hero 0 
tests/golden/run/abort-null-cstr-into-c.hero 0 
tests/golden/run/c-frees-a-lease-and-the-runtime-names-it.hero 0 
tests/golden/run/c-frees-a-lease-on-a-later-call.hero 0 
tests/golden/run/c-frees-a-lease-through-a-callback.hero 0 
tests/golden/run/dead-address-callback-hand-back-clears-the-mark.hero 0 
tests/golden/run/dead-address-copy-in-a-growing-array.hero 0 
tests/golden/run/dead-address-copy-in-a-record-field.hero 0 
tests/golden/run/dead-address-copy-inside-a-fallible-value.hero 0 
tests/golden/run/dead-address-copy-made-before-the-end-reaches-c.hero 0 
tests/golden/run/dead-address-copy-reaching-a-callback-called-by-name.hero 0 
tests/golden/run/dead-address-remembered-through-a-million-later-ends.hero 0 
tests/golden/run/dead-handle-a-place-the-same-call-writes-is-not-poisoned.hero 0 
tests/golden/run/dead-handle-at-a-consuming-position-after-its-life-ended.hero 0 
tests/golden/run/dead-handle-compared-after-its-address-was-handed-out-again.hero 0 
tests/golden/run/dead-handle-field-of-a-cell-ended-through-a-helper.hero 0 
tests/golden/run/dead-handle-given-back-after-its-address-was-handed-out-again.hero 0 
tests/golden/run/dead-handle-in-a-callback-result-record-names-its-field.hero 0 
tests/golden/run/dead-handle-read-after-consume-through-one-name.hero 0 
tests/golden/run/dead-handle-read-by-c-at-a-field-past-the-first.hero 0 
tests/golden/run/dead-handle-read-by-c-at-the-last-word-of-the-dead-region.hero 0 
tests/golden/run/dead-handle-read-by-c-through-a-cell-it-should-only-write.hero 0 
tests/golden/run/dead-handle-returned-by-a-callback-is-checked.hero 0 
tests/golden/run/dead-handle-reused-as-a-retains-out-cell.hero 0 
tests/golden/run/dead-handle-through-a-helper-the-checker-cannot-see-into.hero 0 
tests/golden/run/dead-handle-two-releases-through-one-name-with-retains.hero 0 
tests/golden/run/dead-handle-written-by-c-through-a-cell-it-should-only-write.hero 0 
tests/golden/run/ffi-a-byte-field-crosses-to-c.hero 0 
tests/golden/run/ffi-a-c-array-member.hero 1  1 error[ffi_package];
tests/golden/run/ffi-a-char-array-member.hero 0 
tests/golden/run/ffi-a-char-field-becomes-text.hero 0 
tests/golden/run/ffi-a-construction-names-an-anonymous-struct-whole.hero 0 
tests/golden/run/ffi-a-construction-names-one-arm-of-each-union.hero 0 
tests/golden/run/ffi-a-construction-names-one-member-of-a-union-type.hero 0 
tests/golden/run/ffi-a-construction-names-the-integer-arm-and-compares.hero 0 
tests/golden/run/ffi-a-construction-polls-an-sdl3-event.hero 1  1 error[ffi_package];
tests/golden/run/ffi-a-filled-record-is-asked-for.hero 0 
tests/golden/run/ffi-a-lent-field-counted-by-a-constant.hero 0 
tests/golden/run/ffi-a-lent-field-reads-through-const.hero 0 
tests/golden/run/ffi-a-typedef-the-note-names-binds-by-its-own-name.hero 0 
tests/golden/run/ffi-acquires-and-consumes.hero 0 
tests/golden/run/ffi-borrows-owes-nothing.hero 0 
tests/golden/run/ffi-callback-c-calls-back.hero 0 
tests/golden/run/ffi-constant.hero 0 
tests/golden/run/ffi-counted-by-a-stated-extent.hero 0 
tests/golden/run/ffi-cstr.hero 0 
tests/golden/run/ffi-lent-on-an-out-parameter.hero 0 
tests/golden/run/ffi-libm.hero 0 
tests/golden/run/ffi-out-cell-on-a-handle-the-header-writes-back.hero 0 
tests/golden/run/ffi-owned-cell-is-freed.hero 0 
tests/golden/run/ffi-pointee-at-the-headers-width.hero 0 
tests/golden/run/ffi-read-back-a-cstr.hero 0 
tests/golden/run/ffi-rest-zero-a-nested-group-record.hero 0 
tests/golden/run/ffi-rest-zero-a-partial-record.hero 0 
tests/golden/run/ffi-rest-zero-a-record-of-long-arrays.hero 0 
tests/golden/run/ffi-rest-zero-a-union-none-of-whose-members-is-named.hero 0 
tests/golden/run/ffi-rest-zero-a-union-with-one-member-named.hero 0 
tests/golden/run/ffi-variadic-at-a-fixed-arity.hero 0 
tests/golden/run/fixedbugs-092-a-count-in-a-cell-past-the-record-aborts.hero 0 
tests/golden/run/fixedbugs-092-a-record-lent-whole-with-its-count-runs.hero 0 
tests/golden/run/fixedbugs-092-getsockopt-reads-its-length-through-a-cell.hero 0 
tests/golden/run/fixedbugs-094-a-constant-the-header-spells-as-a-brace-list.hero 0 
tests/golden/run/fixedbugs-094-a-constant-the-header-spells-as-an-expression-still-builds.hero 0 
tests/golden/run/fixedbugs-094-a-nested-brace-list-builds-a-record-of-records.hero 0 
tests/golden/run/fixedbugs-094-a-partial-records-constant-keeps-the-bytes-it-does-not-name.hero 0 
tests/golden/run/fixedbugs-094-a-union-records-constant-is-built-by-one-member.hero 0 
tests/golden/run/fixedbugs-094-an-empty-brace-list-and-a-zero-build-a-zero-record.hero 0 
tests/golden/run/fixedbugs-094-designated-initialisers-leave-the-rest-zero.hero 0 
tests/golden/run/fixedbugs-094-the-systems-mutex-initialiser-builds-a-record.hero 0 
tests/golden/run/fixedbugs-140-extern-records-a-thousand-deep-build.hero 0 
tests/golden/run/fixedbugs-143-a-macro-reached-through-a-function-of-the-programs-own.hero 0 
tests/golden/run/fixedbugs-143-statement-macros-through-functions-of-the-programs-own.hero 0 
tests/golden/run/fixedbugs-143-system-macros-through-functions-of-the-programs-own.hero 0 
tests/golden/run/fixedbugs-144-every-wider-result-builds.hero 0 
tests/golden/run/fixedbugs-150-a-union-read-through-two-members-builds-silently.hero 0 
tests/golden/run/fixedbugs-151-a-flagged-record-over-a-thousand-deep-nest-builds.hero 0 
tests/golden/run/fixedbugs-151-a-union-compared-sixteen-deep-by-an-arm-as-wide-builds.hero 0 
tests/golden/run/fixedbugs-151-a-union-holding-an-anonymous-struct-bound-by-its-wide-arm.hero 0 
tests/golden/run/fixedbugs-151-a-union-in-a-variant-case-compared-by-an-arm-as-wide-builds.hero 0 
tests/golden/run/fixedbugs-151-an-arm-as-wide-as-a-packed-union-compared.hero 0 
tests/golden/run/fixedbugs-151-arms-a-macro-reaches-as-wide-as-their-named-unions-compared.hero 0 
tests/golden/run/fixedbugs-151-arms-as-wide-as-their-unions-compared.hero 0 
tests/golden/run/fixedbugs-151-members-with-no-bytes-left-out.hero 0 
tests/golden/run/fixedbugs-151-one-arm-of-an-anonymous-union-built-and-read.hero 0 
tests/golden/run/fixedbugs-152-an-object-through-a-function-of-the-programs-own.hero 0 
tests/golden/run/fixedbugs-156-records-beside-bit-fields-build.hero 0 
tests/golden/run/fixedbugs-157-a-handle-and-a-record-reach-a-tagged-union-by-typedefs.hero 0 
tests/golden/run/fixedbugs-157-a-union-named-by-its-tag-reached-by-the-note-s-routes.hero 0 
tests/golden/run/fixedbugs-163-a-plain-char-pointee-bound-as-i8.hero 0 
tests/golden/run/fixedbugs-168-a-whole-record-beside-a-same-named-header.hero 0 
tests/golden/run/fixedbugs-171-a-function-pointer-the-header-holds-called-while-null.hero 0 
tests/golden/run/fixedbugs-171-a-loaders-pointers-set-then-called.hero 0 
tests/golden/run/fixedbugs-171-a-pointer-behind-a-dereferencing-macro-called-while-null.hero 0 
tests/golden/run/fixedbugs-171-a-pointer-in-a-struct-behind-a-macro-called-while-null.hero 0 
tests/golden/run/fixedbugs-171-a-pointer-with-no-parameters-called-while-null.hero 0 
tests/golden/run/fixedbugs-173-a-regular-expression-gets-the-bytes-its-author-wrote.hero 0 
tests/golden/run/fixedbugs-185-a-char-array-field-before-a-blank-line-keeps-its-line.hero 0 
tests/golden/run/fixedbugs-185-a-float-array-field-before-a-blank-line-keeps-its-line.hero 0 
tests/golden/run/fixedbugs-185-a-record-array-field-on-the-last-line-keeps-its-line.hero 0 
tests/golden/run/fixedbugs-2036-a-header-s-own-printf-of-a-format-held-in-a-variable-builds.hero 0 
tests/golden/run/fixedbugs-2036-a-header-s-own-printf-of-an-empty-format-given-an-argument-builds.hero 0 
tests/golden/run/fixedbugs-2036-a-header-s-own-printf-with-an-argument-past-its-format-builds.hero 0 
tests/golden/run/fixedbugs-2036-a-header-s-own-snprintf-cut-at-its-size-builds.hero 0 
tests/golden/run/fixedbugs-2044-a-header-that-declares-int64-t-again-as-c-does-builds.hero 0 
tests/golden/run/fixedbugs-2056-a-bound-macro-never-rewrites-the-math-header-read-after-the-groups.hero 0 
tests/golden/run/fixedbugs-2056-a-header-s-own-macro-of-a-name-it-binds-stands.hero 0 
tests/golden/run/fixedbugs-2056-a-macro-two-groups-headers-define-alike-is-the-header-s-own.hero 0 
tests/golden/run/fixedbugs-2059-a-bound-macro-keeps-the-math-name-it-expands-to.hero 0 
tests/golden/run/fixedbugs-2059-a-chain-of-two-macros-to-a-math-name-is-kept.hero 0 
tests/golden/run/fixedbugs-2059-a-function-like-bound-macro-to-a-math-name-is-kept.hero 0 
tests/golden/run/fixedbugs-227-the-shown-read-names-every-byte.hero 0 
tests/golden/run/fixedbugs-245-a-character-carries-the-nul.hero 0 
tests/golden/run/fixedbugs-245-a-concatenation-carries-the-nul.hero 0 
tests/golden/run/fixedbugs-245-a-f-literal-carries-the-nul.hero 0 
tests/golden/run/fixedbugs-245-a-join-part-carries-the-nul.hero 0 
tests/golden/run/fixedbugs-245-a-join-separator-carries-the-nul.hero 0 
tests/golden/run/fixedbugs-245-a-lease-of-a-str-holding-a-nul-stops.hero 0 
tests/golden/run/fixedbugs-245-a-lend-of-a-str-holding-a-nul-stops.hero 0 
tests/golden/run/fixedbugs-245-a-path-holding-a-nul-names-no-file.hero 0 
tests/golden/run/fixedbugs-245-a-repeat-carries-the-nul.hero 0 
tests/golden/run/fixedbugs-245-a-slice-across-carries-the-nul.hero 0 
tests/golden/run/fixedbugs-245-a-str-c-copies-holding-a-nul-stops.hero 0 
tests/golden/run/fixedbugs-245-a-str-c-lays-out-holding-a-nul-stops.hero 0 
tests/golden/run/fixedbugs-245-a-str-made-without-its-nul-is-lent.hero 0 
tests/golden/run/fixedbugs-314-a-map-released-twice-is-named.hero 0 
tests/golden/run/fixedbugs-314-a-str-released-twice-is-named-at-every-size.hero 0 
tests/golden/run/fixedbugs-314-an-array-released-twice-is-named.hero 0 
tests/golden/run/fixedbugs-322-a-sanitiser-names-the-hero-line-at-every-level.hero 0 
tests/golden/run/fixedbugs-353-an-argument-holding-a-lone-surrogate-is-not-text.hero 0 
tests/golden/run/fixedbugs-354-a-byte-array-becomes-every-byte-it-holds.hero 0 
tests/golden/run/fixedbugs-361-a-bound-name-the-guard-holds-keeps-its-header-s-macro.hero 0 
tests/golden/run/fixedbugs-361-a-header-s-macros-never-reach-the-program-s-own-c.hero 0 
tests/golden/run/fixedbugs-361-a-macro-from-a-header-the-group-includes-is-kept-off-too.hero 0 
tests/golden/run/fixedbugs-382-a-constant-of-functions-keeps-its-thread-guard.hero 0 
tests/golden/run/fixedbugs-382-two-spawned-threads-read-one-static-constant.hero 0 
tests/golden/run/fixedbugs-395-a-count-in-c-s-own-unit-runs.hero 0 
tests/golden/run/fixedbugs-395-a-count-read-at-run-time-past-c-s-unit-aborts.hero 0 
tests/golden/run/fixedbugs-396-a-buffer-c-fills.hero 0 
tests/golden/run/fixedbugs-396-a-buffer-c-overruns-aborts.hero 0 
tests/golden/run/fixedbugs-396-a-count-read-from-an-element-keeps-its-sign.hero 0 
tests/golden/run/fixedbugs-396-a-field-lent-as-one-cell-is-the-binding-s-word.hero 0 
tests/golden/run/fixedbugs-396-a-local-is-named-by-the-call-that-overruns-it.hero 0 
tests/golden/run/fixedbugs-396-a-local-lent-in-place-is-guarded.hero 0 
tests/golden/run/fixedbugs-396-a-local-lent-through-a-wrapper-aborts.hero 0 
tests/golden/run/fixedbugs-396-a-one-cell-local-c-overruns-aborts.hero 0 
tests/golden/run/fixedbugs-396-an-extent-above-a-page-aborts.hero 0 
tests/golden/run/fixedbugs-396-bytes-a-canary-would-hold-abort.hero 0 
tests/golden/run/fixedbugs-396-posix-fills-its-buffers.hero 0 
tests/golden/run/fixedbugs-396-sha256-final-fills-its-digest.hero 0 
tests/golden/run/fixedbugs-396-sha256-final-into-one-cell-aborts.hero 0 
tests/golden/run/fixedbugs-396-sha256-final-past-sixteen-aborts.hero 0 
tests/golden/run/fixedbugs-396-two-threads-lend-at-once.hero 0 
tests/golden/run/fixedbugs-400-a-case-holding-a-handle-compared-only-in-a-test-runs.hero 0 
tests/golden/run/fixedbugs-400-a-handle-compared-only-in-a-test-runs.hero 0 
tests/golden/run/fixedbugs-400-a-record-holding-a-handle-compared-only-in-a-test-runs.hero 0 
tests/golden/run/fixedbugs-400-an-array-of-handles-compared-only-in-a-test-runs.hero 0 
tests/golden/run/fixedbugs-401-a-binding-called-zero-beside-the-words.hero 0 
tests/golden/run/fixedbugs-411-read-only-pointers-take-every-lend.hero 0 
tests/golden/run/fixedbugs-413-a-record-c-knows-by-its-address.hero 0 
tests/golden/run/fixedbugs-413-libuv-keeps-every-handles-address.hero 1  1 error[ffi_package];
tests/golden/run/fixedbugs-413-zlib-knows-a-stream-by-its-address.hero 0 
tests/golden/run/fixedbugs-416-handles-nothing-reaches-run.hero 0 
tests/golden/run/fixedbugs-417-every-function-value-is-named-before-c-is-written.hero 0 
tests/golden/run/fixedbugs-419-a-handle-over-a-union-compares-as-an-address.hero 0 
tests/golden/run/fixedbugs-425-a-program-ended-by-its-watchdog-ends-the-one-it-runs.hero 0 
tests/golden/run/fixedbugs-427-a-cstr-to-const-unsigned-bytes-builds-with-no-warning.hero 0 
tests/golden/run/fixedbugs-436-a-child-that-stops-is-waited-on-and-not-killed.hero 0 
tests/golden/run/fixedbugs-437-a-program-whose-runner-is-killed-dies-with-it.hero 0 
tests/golden/run/fixedbugs-438-a-link-is-written-through-and-stays-a-link.hero 0 
tests/golden/run/fixedbugs-438-a-rewrite-keeps-what-a-file-is-besides-its-bytes.hero 0 
tests/golden/run/fixedbugs-438-a-write-cut-short-leaves-the-old-text.hero 0 
tests/golden/run/fixedbugs-440-a-bytes-element-stays-a-place.hero 0 
tests/golden/run/fixedbugs-451-a-buffer-c-writes-after-its-call-returns-is-named.hero 0 
tests/golden/run/fixedbugs-451-a-buffer-past-a-mebibyte-c-writes-after-its-call-returns-is-named.hero 0 
tests/golden/run/fixedbugs-451-a-local-c-reads-after-its-function-returns-is-seen-by-the-sanitiser.hero 0 
tests/golden/run/fixedbugs-451-a-local-c-writes-after-its-function-returns-is-named.hero 0 
tests/golden/run/fixedbugs-451-a-local-c-writes-after-its-function-returns-on-a-thread-is-named.hero 0 
tests/golden/run/fixedbugs-451-a-local-c-writes-while-a-later-lend-holds-its-region-is-seen-by-the-sanitiser.hero 0 
tests/golden/run/fixedbugs-461-a-rewrite-keeps-the-owner-the-acl-and-the-streams.hero 0 
tests/golden/run/fixedbugs-461-the-publish-route-keeps-the-owner-the-acl-and-the-streams.hero 0 
tests/golden/run/fixedbugs-462-a-map-read-after-its-last-release-stops-at-the-read.hero 0 
tests/golden/run/fixedbugs-462-a-str-read-after-its-last-release-stops-at-the-read.hero 0 
tests/golden/run/fixedbugs-462-an-array-read-after-its-last-release-stops-at-the-read.hero 0 
tests/golden/run/fixedbugs-465-a-program-whose-runner-is-killed-dies-with-it-on-darwin.hero 0 
tests/golden/run/fixedbugs-484-a-file-s-key-made-in-c-is-the-key-of-its-shown-text.hero 0 
tests/golden/run/fixedbugs-549-rest-zero-on-a-group-record-a-thousand-deep-builds.hero 0 
tests/golden/run/fixedbugs-555-two-static-definitions-of-one-name-stay-each-module-s.hero 1  1 error[ffi_one_file];
tests/golden/run/fixedbugs-559-a-macro-of-another-module-does-not-lay-out-a-record-in-the-other-order.hero 0 
tests/golden/run/fixedbugs-559-a-macro-of-another-module-does-not-lay-out-a-record.hero 0 
tests/golden/run/fixedbugs-559-a-record-beside-another-module-s-header-builds.hero 1  1 error[ffi_one_file];
tests/golden/run/fixedbugs-563-a-header-of-the-program-s-own-keeps-the-order-written.hero 0 
tests/golden/run/fixedbugs-563-a-record-only-group-is-included-where-it-is-written.hero 0 
tests/golden/run/fixedbugs-563-two-defaults-mean-what-their-written-order-says-the-other-way.hero 0 
tests/golden/run/fixedbugs-563-two-defaults-mean-what-their-written-order-says.hero 0 
tests/golden/run/fixedbugs-563-two-headers-are-read-in-the-order-their-groups-are-written.hero 0 
tests/golden/run/fixedbugs-568-a-group-constant-reads-the-library-s-limit-after-the-guard.hero 0 
tests/golden/run/fixedbugs-568-a-group-s-narrow-limits-never-reach-a-division.hero 0 
tests/golden/run/fixedbugs-568-the-first-group-s-header-is-read-before-the-c-library.hero 0 
tests/golden/run/fixedbugs-569-a-header-s-own-unused-variable-prints-no-clang-words.hero 0 
tests/golden/run/fixedbugs-569-a-library-s-own-sign-conversion-is-not-the-program-s.hero 0 
tests/golden/run/fixedbugs-570-a-constant-s-pragma-leaves-a-correct-program-running.hero 0 
tests/golden/run/fixedbugs-570-a-header-s-pragma-leaves-a-correct-binding-building.hero 0 
tests/golden/run/fixedbugs-571-a-deprecated-constant-read-prints-no-clang-words.hero 0 
tests/golden/run/fixedbugs-571-a-deprecated-function-bound-and-not-called-prints-no-clang-words.hero 0 
tests/golden/run/fixedbugs-571-a-deprecated-libc-function-through-a-switch-header-prints-no-clang-words.hero 0 
tests/golden/run/fixedbugs-571-a-deprecated-record-bound-prints-no-clang-words.hero 0 
tests/golden/run/fixedbugs-573-a-launch-list-left-standing-is-told-as-the-program-s.hero 0 
tests/golden/run/fixedbugs-573-a-listing-left-standing-is-told-as-the-program-s.hero 0 
tests/golden/run/fixedbugs-577-a-group-s-constant-in-a-step-aborts-where-it-is-read.hero 0 
tests/golden/run/fixedbugs-582-a-header-s-clang-section-leaves-the-program-s-functions-in-their-own.hero 0 
tests/golden/run/fixedbugs-582-a-header-s-float-control-pop-past-its-own-takes-the-close-s-push.hero 0 
tests/golden/run/fixedbugs-582-a-header-s-float-control-precise-off-leaves-the-program-s-sums-in-order.hero 0 
tests/golden/run/fixedbugs-582-a-header-s-fp-contract-fast-leaves-the-program-s-arithmetic-unfused.hero 0 
tests/golden/run/fixedbugs-582-a-header-s-options-align-packed-leaves-the-records-their-layout.hero 0 
tests/golden/run/fixedbugs-582-a-header-s-own-unclosed-float-control-push-is-the-one-popped.hero 0 
tests/golden/run/fixedbugs-582-a-header-s-pack-2-with-no-push-leaves-the-records-their-layout.hero 0 
tests/golden/run/fixedbugs-582-a-header-s-unclosed-pack-push-leaves-two-modules-one-record-layout.hero 0 
tests/golden/run/fixedbugs-582-a-pack-pop-of-another-label-leaves-the-records-their-layout.hero 0 
tests/golden/run/fixedbugs-582-a-pack-push-in-a-header-the-group-s-header-includes.hero 0 
tests/golden/run/fixedbugs-582-a-record-bound-from-a-packing-header-keeps-the-header-s-layout.hero 0 
tests/golden/run/fixedbugs-584-a-deprecated-enumerator-read-builds-silent.hero 0 
tests/golden/run/fixedbugs-584-a-deprecated-function-bound-and-not-called-builds-silent.hero 0 
tests/golden/run/fixedbugs-584-a-deprecated-function-called-builds-silent.hero 0 
tests/golden/run/fixedbugs-584-a-deprecated-record-as-a-local-builds-silent.hero 0 
tests/golden/run/fixedbugs-584-a-deprecated-record-in-a-signature-builds-silent.hero 0 
tests/golden/run/fixedbugs-584-a-function-its-header-marks-with-the-warning-attribute-builds-silent.hero 0 
tests/golden/run/fixedbugs-584-sprintf-with-fortification-off-builds-silent.hero 0 
tests/golden/run/fixedbugs-587-a-macro-its-header-marks-with-pragma-clang-deprecated-builds-silent.hero 0 
tests/golden/run/fixedbugs-589-a-call-of-mktemp-links-silent-on-linux.hero 0 
tests/golden/run/fixedbugs-590-a-fp-eval-method-in-a-header-the-group-s-header-includes.hero 0 
tests/golden/run/fixedbugs-590-a-header-s-fp-eval-method-extended-leaves-the-program-s-doubles.hero 0 
tests/golden/run/fixedbugs-590-a-header-s-fp-eval-method-leaves-the-unit-s-math-h-its-own.hero 0 
tests/golden/run/fixedbugs-643-an-inline-builtin-two-modules-bind-is-no-definition.hero 0 
tests/golden/run/fixedbugs-a-borrowed-handle-is-written-into-a-fixed-array.hero 0 
tests/golden/run/fixedbugs-a-c-function-crosses-as-a-callback.hero 0 
tests/golden/run/fixedbugs-a-c-handler-that-recovers-leaves-no-line.hero 0 
tests/golden/run/fixedbugs-a-callback-handed-the-address-a-release-freed-begins-a-new-life.hero 0 
tests/golden/run/fixedbugs-a-discarded-acquisition-is-owed-at-exit.hero 0 
tests/golden/run/fixedbugs-a-double-release-before-any-acquisition-is-stopped-before-c.hero 0 
tests/golden/run/fixedbugs-a-fixed-array-element-is-written-at-a-computed-index.hero 0 
tests/golden/run/fixedbugs-a-fixed-array-element-is-written-at-any-depth.hero 0 
tests/golden/run/fixedbugs-a-fixed-array-element-is-written-in-place.hero 0 
tests/golden/run/fixedbugs-a-fixed-array-element-of-every-field-kind-is-written.hero 0 
tests/golden/run/fixedbugs-a-freed-object-read-through-a-stale-copy-is-not-a-null-handle.hero 0 
tests/golden/run/fixedbugs-a-group-record-in-every-container.hero 1  1 error[ffi_package];
tests/golden/run/fixedbugs-a-handle-compares-as-an-address.hero 0 
tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero 0 
tests/golden/run/fixedbugs-a-nested-record-and-a-typed-pointer.hero 1  1 error[ffi_package];
tests/golden/run/fixedbugs-a-real-deallocator-given-the-same-handle-twice.hero 0 
tests/golden/run/fixedbugs-a-records-only-group-loses-its-header.hero 1  1 error[ffi_package];
tests/golden/run/fixedbugs-a-reference-beside-an-end-in-one-call-is-checked-first.hero 0 
tests/golden/run/fixedbugs-a-reference-that-names-another-releaser-is-refused.hero 0 
tests/golden/run/fixedbugs-a-release-after-a-transfer-is-a-stray.hero 0 
tests/golden/run/fixedbugs-a-release-the-mark-did-not-name-is-stopped-before-c.hero 0 
tests/golden/run/fixedbugs-a-tag-names-a-handle-and-a-record.hero 0 
tests/golden/run/fixedbugs-a-tag-that-needs-struct.hero 0 
tests/golden/run/fixedbugs-a-transfer-into-a-value-that-ends-it-wrongly-is-stopped-before-c.hero 0 
tests/golden/run/fixedbugs-a-transfer-that-failed-and-nobody-ended-is-owed-at-exit.hero 0 
tests/golden/run/fixedbugs-an-acquired-handle-is-written-into-a-fixed-array.hero 0 
tests/golden/run/fixedbugs-an-at-argument-through-a-fixed-array-element-is-written-back.hero 0 
tests/golden/run/fixedbugs-an-ignored-signal-stays-ignored.hero 0 
tests/golden/run/fixedbugs-c-aborts-with-no-lease-live-and-the-runtime-says-it-did-not.hero 0 
tests/golden/run/fixedbugs-c-gives-a-pointer-back-twice-and-the-runtime-says-so.hero 0 
tests/golden/run/fixedbugs-c-traps-with-a-lease-live-on-every-architecture.hero 0 
tests/golden/run/fixedbugs-c-writes-over-a-leases-header.hero 0 
tests/golden/run/fixedbugs-c-writes-over-a-strings-header.hero 0 
tests/golden/run/fixedbugs-getaddrinfo-is-bindable.hero 0 
tests/golden/run/fixedbugs-one-call-taking-a-handle-at-two-consuming-positions-says-so.hero 0 
tests/golden/run/fixedbugs-read-file-on-bytes-that-are-not-text.hero 0 
tests/golden/run/fixedbugs-sibling-fields-of-one-record-type.hero 0 
tests/golden/run/fixedbugs-the-unacquired-sibling-leaks-loudly.hero 0 
tests/golden/run/fixedbugs-two-allocator-families-over-one-void.hero 0 
tests/golden/run/fixedbugs-two-marks-on-one-call-are-two-obligations.hero 0 
tests/golden/run/fixedbugs-two-threads-over-one-allocator-give-back-their-own-handles.hero 0 
tests/golden/run/fixedbugs555/static_long.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs559/holder.hero 1  1 error[unknown_module];
tests/golden/run/fixedbugs559/long_twice.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs559/points.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs559/wide.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs560/cap_ten.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs560/int_twice.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs560/left_shim.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs560/limit_half.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs560/long_twice.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs560/macro_dbl.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs560/right_shim.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs563/narrow.hero 1  1 error[unknown_module];
tests/golden/run/fixedbugs563/wide.hero 1  1 error[ffi_missing_header];
tests/golden/run/fixedbugs643/other.hero 1  1 error[ffi_missing_header];
tests/golden/run/handle-a-copy-made-before-the-end-is-the-runtime-s.hero 0 
tests/golden/run/handle-a-discarded-reference-is-still-owed.hero 0 
tests/golden/run/handle-a-helper-that-ends-its-parameter-is-the-runtime-s.hero 0 
tests/golden/run/handle-a-reference-through-an-out-cell.hero 0 
tests/golden/run/handle-a-shim-consumes-a-handle-once-for-two-positions.hero 0 
tests/golden/run/handle-alive-after-a-loop-that-ends-on-break.hero 0 
tests/golden/run/handle-alive-after-an-end-in-one-arm.hero 0 
tests/golden/run/handle-alive-after-an-end-then-a-rebinding.hero 0 
tests/golden/run/handle-alive-through-a-reference-released-twice.hero 0 
tests/golden/run/handle-alive-under-when-after-a-failed-end.hero 0 
tests/golden/run/handle-end-only-on-success-keeps-the-connection-owed.hero 0 
tests/golden/run/handle-reference-joins-the-life-it-finds.hero 0 
tests/golden/run/handle-set-a-call-that-also-acquires-pays-its-own-mark.hero 0 
tests/golden/run/handle-set-any-releaser-named-ends-the-life.hero 0 
tests/golden/run/handle-success-value-at-the-width-of-the-result.hero 0 
tests/golden/run/handle-transfer-into-a-null-result-is-not-made.hero 0 
tests/golden/run/handle-transfer-only-on-success-keeps-the-obligation.hero 0 
tests/golden/run/handle-two-references-at-two-consuming-positions.hero 0 
tests/golden/run/lease-c-keeps-the-pointer.hero 0 
tests/golden/run/lease-tail-points-into-the-bytes.hero 0 
tests/golden/run/lend-inside-a-loop.hero 0 
tests/golden/run/limit-a-copy-given-back-after-c-reused-its-address-is-not-caught.hero 0 
tests/golden/run/limit-a-copy-read-after-a-million-later-ends-is-not-caught.hero 0 
tests/golden/run/limit-a-copy-read-after-c-reused-its-address-is-not-caught.hero 0 
tests/golden/run/limit-a-handle-c-kept-and-hands-a-callback-after-the-release-is-not-caught.hero 0 
tests/golden/run/limit-a-stale-copy-faulting-in-c-names-the-copy-the-program-kept.hero 0 
tests/golden/surface-fixtures/brackets180/shape108.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/brackets180/shape109.hero 1  1 error[expected_params_close];
tests/golden/surface-fixtures/brackets180/shape110.hero 1  1 error[expected_extent]; 1 error[expected_params_close];
tests/golden/surface-fixtures/brackets180/shape111.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/brackets180/shape112.hero 1  1 error[expected_params_close];
tests/golden/surface-fixtures/brackets180/shape113.hero 1  1 error[line_end_before_continuation];
tests/golden/surface-fixtures/brackets180/shape114.hero 1  2 error[unread_releaser];
tests/golden/surface-fixtures/brackets180/shape115.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/comments099/inside.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/comments099/tight.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/comments100/kepthead.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/comments100/members.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/comments101/blankunderhead.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/comments101/openers.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/comments101/remarkdochead.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/comments101/signature.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/comments101/trailinghead.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/comments101/unitinside.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/comments101/unitresult.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/contract/alt.hero 0 
tests/golden/surface-fixtures/contract/main.hero 1  1 error[contract_differs];
tests/golden/surface-fixtures/deepthread/main.hero 0 
tests/golden/surface-fixtures/externroute/bind.hero 0 
tests/golden/surface-fixtures/externsignature/bind.hero 1  1 error[ffi_return_type];
tests/golden/surface-fixtures/externsignature/bindright.hero 0 
tests/golden/surface-fixtures/groupremark096/beside.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/groupremark096/middle.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/groupremark096/samehead.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/handlearming/main.hero 1  1 error[unmarked_handle_producer];
tests/golden/surface-fixtures/handlearming/slots.hero 0 
tests/golden/surface-fixtures/nullread/main.hero 0 
tests/golden/surface-fixtures/onefile453/narrow.hero 0 
tests/golden/surface-fixtures/onefile453/wide.hero 0 
tests/golden/surface-fixtures/onefile453macro/hundred.hero 0 
tests/golden/surface-fixtures/onefile453macro/narrow.hero 0 
tests/golden/surface-fixtures/onefile453type/num.hero 0 
tests/golden/surface-fixtures/onefile453type/wide.hero 0 
tests/golden/surface-fixtures/paramcomment095/following.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/paramcomment095/last.hero 1  1 error[ffi_missing_header];
tests/golden/surface-fixtures/recursion521/hidden.hero 0 
tests/golden/surface-fixtures/releaser085/bio.hero 1  1 error[unread_releaser];
tests/golden/surface-fixtures/releaser085/main.hero 1  1 error[unread_releaser];
tests/golden/surface-fixtures/releaser108/commented.hero 0 
tests/golden/surface-fixtures/releaser108/contract.hero 0 
tests/golden/surface-fixtures/releaser108/sets.hero 0 
tests/golden/surface-fixtures/structtag/net.hero 0 
tests/golden/surface-fixtures/twoarity/as_int.hero 0 
tests/golden/surface-fixtures/twoarity/as_text.hero 0 
tests/golden/surface-fixtures/warning248/main.hero 0 
tests/golden/unsupported/ffi-a-char-field-declared-u8-is-told-i8.hero 1  2 error[ffi_field_type];
tests/golden/unsupported/ffi-a-construction-across-lines-is-told-at-its-head.hero 1  1 error[missing_fields];
tests/golden/unsupported/ffi-a-construction-builds-where-its-record-cannot-be-compared.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/ffi-a-construction-leaves-out-a-field-outside-any-union.hero 1  1 error[missing_fields];
tests/golden/unsupported/ffi-a-construction-leaves-out-two-fields-told-once.hero 1  1 error[missing_fields];
tests/golden/unsupported/ffi-a-construction-names-an-arm-and-an-anonymous-struct.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/ffi-a-construction-names-half-an-anonymous-struct.hero 1  1 error[missing_fields];
tests/golden/unsupported/ffi-a-construction-names-no-member-of-a-union.hero 1  1 error[missing_fields];
tests/golden/unsupported/ffi-a-construction-names-two-members-of-a-union-type.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/ffi-a-construction-of-a-partial-record-leaves-out-a-field.hero 1  1 error[missing_fields];
tests/golden/unsupported/ffi-a-construction-of-a-plain-struct-leaves-out-a-field.hero 1  1 error[missing_fields];
tests/golden/unsupported/ffi-a-construction-refused-beside-one-that-builds.hero 1  1 error[missing_fields];
tests/golden/unsupported/ffi-a-cstr-field-blames-the-field.hero 1  1 error[ffi_field_type];
tests/golden/unsupported/ffi-a-field-named-after-a-header-macro.hero 1  2 error[ffi_unknown_field];
tests/golden/unsupported/ffi-a-header-declaring-a-function-the-unit-writes-is-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/ffi-a-header-declaring-main-is-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/ffi-a-header-defining-a-static-main-is-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/ffi-a-header-defining-main-as-an-object-is-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/ffi-a-header-defining-the-unit-s-own-macro-is-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/ffi-a-header-misusing-a-runtime-macro-is-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/ffi-a-header-poisoning-a-type-of-c-s-own-is-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/ffi-a-header-s-call-of-the-runtime-over-two-lines-is-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/ffi-a-header-s-macro-meeting-the-name-of-a-field-is-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/ffi-a-header-s-macro-meeting-the-name-of-a-temporary-is-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/ffi-a-poisoning-header-after-one-needing-another-s-typedef-is-told-as-its-own.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/ffi-const-member.hero 1  1 error[ffi_const_field];
tests/golden/unsupported/ffi-incomplete-record.hero 1  1 error[ffi_package];
tests/golden/unsupported/ffi-records-only-group-cannot-claim-its-package.hero 1  1 error[ffi_package];
tests/golden/unsupported/ffi-writable-parameter.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixed-array-flow.hero 1 
tests/golden/unsupported/fixedbugs-092-a-count-past-a-record-lent-whole-is-refused.hero 1  9 error[record_lend_extent];
tests/golden/unsupported/fixedbugs-092-a-record-lent-whole-to-a-void-pointer-names-its-count.hero 1  5 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-092-read-into-a-record-names-its-count.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-094-a-field-the-struct-does-not-have-is-told.hero 1  1 error[ffi_constant_type];
tests/golden/unsupported/fixedbugs-094-a-number-c-would-change-is-told.hero 1  2 error[ffi_constant_type];
tests/golden/unsupported/fixedbugs-094-a-value-of-another-kind-than-its-field-is-told.hero 1  1 error[ffi_constant_type];
tests/golden/unsupported/fixedbugs-094-a-value-spelled-in-a-header-the-group-includes.hero 1  1 error[ffi_constant_type];
tests/golden/unsupported/fixedbugs-094-an-element-too-many-is-told-on-the-constant.hero 1  1 error[ffi_constant_type];
tests/golden/unsupported/fixedbugs-094-an-expression-of-another-type-is-told-by-its-type.hero 1  2 error[ffi_constant_type];
tests/golden/unsupported/fixedbugs-140-a-union-compared-sixteen-deep-is-refused.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/fixedbugs-140-a-union-in-a-variant-case-is-refused.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/fixedbugs-143-a-function-like-macro-declared-as-a-constant.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-143-a-function-like-macro-declared-with-no-parameter.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-143-a-function-like-macro-of-the-programs-own-header.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-143-a-function-that-is-also-a-macro-keeps-the-first-round.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-143-a-macro-declared-at-a-wrong-width-drafts-no-width.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-143-a-macro-of-the-programs-own-header-at-a-wrong-width.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-143-a-misspelled-macro-stays-an-unknown-name.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-143-a-system-macro-is-named-as-a-macro.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-143-an-object-like-macro-is-a-value.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-143-statement-macros-of-the-programs-own-header.hero 1  3 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-143-the-fd-set-macros.hero 1  3 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-143-the-programs-own-function-holds-the-declaration-to-its-width.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-144-what-a-bool-result-refuses.hero 1  14 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-144-what-a-u16-result-refuses.hero 1  12 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-144-what-a-u32-result-refuses.hero 1  10 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-144-what-a-u64-result-refuses.hero 1  9 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-144-what-a-u8-result-refuses.hero 1  13 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-144-what-an-f32-result-refuses.hero 1  14 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-144-what-an-f64-result-refuses.hero 1  12 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-144-what-an-i16-result-refuses.hero 1  10 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-144-what-an-i32-result-refuses.hero 1  7 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-144-what-an-i64-result-refuses.hero 1  4 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-144-what-an-i8-result-refuses.hero 1  12 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-145-a-misspelled-tag.hero 1  1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-145-a-result-spells-the-typedef.hero 1  1 error[ffi_return_type]; 1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-145-a-struct-the-header-keeps-opaque-is-a-handle.hero 1  1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-145-a-typedef-of-a-tagged-struct-names-its-tag.hero 1  1 error[ffi_parameter_type]; 1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-145-a-typedef-of-an-anonymous-struct-is-not-a-tag.hero 1  1 error[ffi_parameter_type]; 1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-145-a-typedef-of-an-anonymous-union-is-not-a-tag.hero 1  1 error[ffi_parameter_type]; 1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields.hero 1  1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-145-a-typedef-of-void-bound-with-fields.hero 1  1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-145-a-typedef-with-nothing-else-said-of-it.hero 1  1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-151-a-field-left-out-after-an-anonymous-union.hero 1  1 error[ffi_incomplete_record];
tests/golden/unsupported/fixedbugs-151-a-field-left-out-beside-a-union-in-an-anonymous-struct.hero 1  1 error[ffi_incomplete_record];
tests/golden/unsupported/fixedbugs-151-a-field-left-out-between-two-it-names.hero 1  1 error[ffi_incomplete_record];
tests/golden/unsupported/fixedbugs-151-a-float-arm-of-an-anonymous-union-compared.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/fixedbugs-151-a-misspelt-field-is-told-before-the-union-s-layout.hero 1  1 error[ffi_unknown_field];
tests/golden/unsupported/fixedbugs-151-a-record-arm-a-macro-reaches-compared.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/fixedbugs-151-a-union-type-compared-by-an-arm-narrower-than-it.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/fixedbugs-151-an-anonymous-union-built-from-two-arms.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/fixedbugs-151-an-arm-narrower-than-its-anonymous-union-compared.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/fixedbugs-151-an-arm-short-of-an-aligned-union-compared.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/fixedbugs-151-an-array-arm-short-of-its-union-s-width-compared.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/fixedbugs-151-the-first-field-left-out-before-an-anonymous-union.hero 1  1 error[ffi_incomplete_record];
tests/golden/unsupported/fixedbugs-151-two-arms-of-a-named-union-reached-through-macros-built.hero 1  1 error[ffi_union_field];
tests/golden/unsupported/fixedbugs-152-a-field-the-header-lacks-in-a-partial-record.hero 1  1 error[ffi_unknown_field];
tests/golden/unsupported/fixedbugs-152-a-field-the-header-lacks-in-a-struct-by-its-tag.hero 1  1 error[ffi_unknown_field];
tests/golden/unsupported/fixedbugs-152-a-field-the-header-lacks-in-a-struct-named-for-its-tag.hero 1  1 error[ffi_unknown_field];
tests/golden/unsupported/fixedbugs-152-a-field-the-header-lacks-in-a-struct-with-no-typedef.hero 1  1 error[ffi_unknown_field];
tests/golden/unsupported/fixedbugs-152-a-field-the-header-lacks-in-a-typedef-of-a-tagged-struct.hero 1  1 error[ffi_unknown_field];
tests/golden/unsupported/fixedbugs-152-a-field-the-header-lacks-in-a-union.hero 1  1 error[ffi_unknown_field];
tests/golden/unsupported/fixedbugs-152-a-field-the-header-lacks-in-an-anonymous-struct.hero 1  1 error[ffi_unknown_field];
tests/golden/unsupported/fixedbugs-152-a-function-declared-with-too-few-parameters.hero 1  1 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-152-a-pointer-object-declared-as-a-function.hero 1  1 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-152-a-struct-object-declared-as-a-function.hero 1  2 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-152-a-type-declared-as-a-function.hero 1  2 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-152-a-value-declared-as-a-function.hero 1  2 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-152-an-object-beside-a-misspelled-name.hero 1  1 error[ffi_call_shape]; 1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-152-an-object-declared-as-a-function.hero 1  1 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-152-an-object-declared-with-parameters.hero 1  1 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-152-an-object-never-called.hero 1  1 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-156-a-bit-field-as-wide-as-its-type.hero 1  1 error[ffi_field_type];
tests/golden/unsupported/fixedbugs-156-a-bit-field-at-the-end-of-a-struct.hero 1  1 error[ffi_field_type];
tests/golden/unsupported/fixedbugs-156-a-bit-field-left-out-is-left-to-partial.hero 1  1 error[ffi_incomplete_record];
tests/golden/unsupported/fixedbugs-156-a-bit-field-named-by-a-record.hero 1  2 error[ffi_field_type];
tests/golden/unsupported/fixedbugs-156-a-bit-field-named-in-a-partial-record.hero 1  1 error[ffi_field_type];
tests/golden/unsupported/fixedbugs-156-a-tagged-struct-shaped-as-curl-s-hsts-entry.hero 1  1 error[ffi_field_type];
tests/golden/unsupported/fixedbugs-156-bit-fields-inside-an-anonymous-struct.hero 1  1 error[ffi_field_type]; 1 error[ffi_incomplete_record];
tests/golden/unsupported/fixedbugs-156-curl-s-hsts-entry.hero 1  1 error[ffi_field_type];
tests/golden/unsupported/fixedbugs-157-a-handle-over-a-union-s-tag.hero 1  1 error[ffi_tag_is_a_union];
tests/golden/unsupported/fixedbugs-157-a-handle-over-an-enum-s-tag.hero 1  1 error[ffi_tag_is_a_union];
tests/golden/unsupported/fixedbugs-157-a-record-named-for-a-union-s-tag.hero 1  1 error[ffi_tag_is_a_union];
tests/golden/unsupported/fixedbugs-157-a-record-over-an-enum-s-tag.hero 1  1 error[ffi_tag_is_a_union];
tests/golden/unsupported/fixedbugs-157-a-union-bound-by-its-tag.hero 1  1 error[ffi_tag_is_a_union];
tests/golden/unsupported/fixedbugs-157-a-union-s-tag-that-is-also-a-typedef.hero 1  1 error[ffi_tag_is_a_union];
tests/golden/unsupported/fixedbugs-158-a-groups-header-through-another-groups-header.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-a-groups-header-written-with-a-directory.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-a-header-reached-through-a-second-header.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-a-header-reached-through-three-headers.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-a-header-the-group-names-itself-keeps-its-words.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-a-header-the-groups-header-includes-in-angle-brackets.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-a-header-the-groups-header-includes-in-quotes.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-a-missing-include-inside-a-taken-if.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-a-note-in-another-header-between.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-a-warning-above-the-missing-include.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-a-warning-in-a-sibling-header.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-a-warning-in-each-header-of-the-stack.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-of-two-groups-the-one-whose-header-includes-it.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-158-two-missing-includes-the-first-is-told.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-163-a-plain-char-pointee-bound-as-u8-is-refused.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-168-a-record-leaving-out-a-member-beside-a-same-named-header.hero 1  1 error[ffi_incomplete_record];
tests/golden/unsupported/fixedbugs-171-a-pointer-of-another-signature.hero 1  1 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-185-r2-a-function-named-as-a-tag-is-told-as-misspelled.hero 1  1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-185-r2-a-typedef-of-a-tagged-struct-named-as-a-tag.hero 1  1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-186-a-constant-the-header-gives-another-type.hero 1  2 error[ffi_constant_type];
tests/golden/unsupported/fixedbugs-186-a-pointer-result-offers-the-groups-handle.hero 1  1 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-186-a-result-the-header-gives-64-bits.hero 1  2 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-186-a-result-the-header-gives-nothing.hero 1  2 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-191-a-partial-record-of-an-opaque-typedef.hero 1  1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-191-a-void-typedef-record-with-fields.hero 1  1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-191-an-opaque-typedef-record-a-function-is-handed.hero 1  1 error[ffi_unknown_tag];
tests/golden/unsupported/fixedbugs-2034-a-header-s-own-printf-with-too-few-arguments-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2034-a-header-s-own-printf-wrapper-with-a-format-attribute-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2036-a-header-s-printf-of-a-bool-for-a-character-is-a-known-cost.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2036-a-header-s-printf-of-a-format-holding-a-nul-is-a-known-cost.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2036-a-header-s-printf-of-a-wide-literal-read-as-bytes-is-a-known-cost.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2036-a-header-s-printf-with-a-space-flag-beside-a-plus-is-a-known-cost.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2036-a-header-s-printf-with-a-zero-flag-beside-a-minus-is-a-known-cost.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2044-a-header-that-declares-int32-t-another-way-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2044-a-header-that-declares-int64-t-as-a-variable-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2044-a-header-that-declares-intptr-t-as-a-function-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2044-a-header-that-declares-uint8-t-another-way-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2044-a-header-that-misspells-a-c-integer-type-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2045-a-header-that-declares-a-name-of-hero-os-h-another-way-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2045-a-header-that-declares-a-runtime-function-another-way-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2045-a-header-that-declares-the-runtime-s-string-type-again-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2045-a-header-that-defines-a-runtime-function-again-as-static-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2045-a-header-the-group-s-header-includes-declares-a-runtime-name.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2049-a-header-that-calls-a-runtime-function-inside-another-call-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2049-a-header-that-calls-a-runtime-function-wrongly-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2049-a-header-that-declares-a-function-s-c-name-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2049-a-header-that-declares-a-name-of-math-h-another-way-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2049-a-header-that-declares-main-s-c-name-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2049-a-header-that-defines-a-name-of-math-h-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2049-a-header-that-misspells-the-runtime-s-failure-type-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2049-a-header-that-misspells-the-runtime-s-string-type-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2051-a-header-that-declares-a-c-library-function-as-a-record-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2051-a-header-that-declares-a-c-library-function-as-a-variable-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2052-a-header-that-calls-a-function-nobody-declared-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2052-a-header-that-calls-a-library-function-it-never-includes-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2052-a-header-that-calls-a-misspelled-runtime-function-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2053-a-header-that-defines-a-called-runtime-function-again-as-static-is-refused-on-windows.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2053-a-header-that-defines-an-uncalled-runtime-function-again-as-static-is-refused-on-windows.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-2056-a-function-like-macro-of-a-later-group-s-header.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-2056-a-function-like-macro-of-an-earlier-group-s-header.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-2056-a-later-group-s-header-defines-a-bound-constant-again.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-2056-a-later-group-s-header-undefines-a-bound-macro.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-2056-a-macro-of-a-later-group-s-header-takes-over-a-bound-function.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-2056-a-macro-of-an-earlier-group-s-header-takes-over-a-bound-function.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-2056-a-name-the-guard-does-not-keep-is-taken-over-too.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-2059-a-bound-macro-reaching-a-name-the-unit-writes-is-refused.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-2059-the-math-name-bound-by-another-group-too-is-refused.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-226-a-package-not-installed-is-told-at-its-string.hero 1  1 error[ffi_package];
tests/golden/unsupported/fixedbugs-232-a-header-the-groups-header-includes-named-with-a-quote.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-232-a-missing-header-named-with-a-noncharacter.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-232-a-missing-header-named-with-a-private-use-code-point.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-232-a-missing-header-named-with-an-unassigned-code-point.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs-237-a-package-name-holding-a-backtick.hero 1  1 error[ffi_package];
tests/golden/unsupported/fixedbugs-337-a-construction-the-layout-check-refuses-past-a-byte-that-is-not-utf-8.hero 1  1 error[missing_fields];
tests/golden/unsupported/fixedbugs-337-an-extern-the-header-refutes-past-a-byte-that-is-not-utf-8.hero 1  1 error[ffi_return_type];
tests/golden/unsupported/fixedbugs-360-a-header-only-the-builds-flags-refuse.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-360-a-header-that-does-not-compile-is-told-before-what-it-refutes.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-360-a-header-that-does-not-compile.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-360-a-header-the-groups-header-includes.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-360-a-header-whose-error-directive-fires.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-360-of-two-groups-the-one-whose-header-does-not-compile.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-395-a-count-in-a-type-the-header-hides-is-refused.hero 1  3 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-395-a-field-is-counted-in-what-c-points-at.hero 1  13 error[field_lend_extent];
tests/golden/unsupported/fixedbugs-400-a-handle-over-a-union-s-tag-compared-only-in-a-test.hero 1  1 error[ffi_tag_is_a_union];
tests/golden/unsupported/fixedbugs-400-a-handle-over-an-enum-s-tag-compared-only-in-a-test.hero 1  1 error[ffi_tag_is_a_union];
tests/golden/unsupported/fixedbugs-400-a-misspelled-handle-tag-compared-only-in-a-test.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-411-a-const-pointer-to-char-is-refused.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-a-count-the-header-disagrees-with-asks-no-parameter.hero 1  1 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-411-a-function-pointer-s-out-parameter-is-held-to-its-width.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-411-a-function-pointer-the-header-declares-is-asked.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-a-lease-lent-to-unsigned-char-is-refused.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-a-literal-lent-to-unsigned-char-is-refused.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-a-restrict-pointer-is-refused-and-told-without-restrict.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-a-str-lent-to-unsigned-char-is-refused.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-a-typedef-of-an-unsigned-char-pointer-is-refused.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-an-atomic-pointer-to-unsigned-char-is-refused.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-an-unsigned-char-array-parameter-is-refused.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-char-is-refused-by-the-header-s-answer.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-signed-char-is-refused.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-uint8-t-is-refused.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-411-void-is-refused.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-416-a-handle-over-a-union-s-tag-bound-only-in-a-test.hero 1  1 error[ffi_tag_is_a_union];
tests/golden/unsupported/fixedbugs-416-a-misspelled-handle-tag-bound-only-in-a-test.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-416-an-unused-handle-over-a-union-s-tag.hero 1  1 error[ffi_tag_is_a_union];
tests/golden/unsupported/fixedbugs-416-an-unused-handle-over-an-enum-s-tag.hero 1  1 error[ffi_tag_is_a_union];
tests/golden/unsupported/fixedbugs-416-an-unused-misspelled-handle-tag.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-426-a-cstr-lent-to-a-function-with-no-prototype-is-refused.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-426-a-function-declared-void-is-told-its-count.hero 1  1 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-426-a-function-pointer-with-no-prototype-is-refused.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-426-a-number-to-a-function-with-no-prototype-is-refused.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-427-a-cstr-to-a-const-int-is-refused.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-427-a-cstr-to-a-const-struct-is-refused.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-428-the-written-parameter-of-two-is-told-on-its-name.hero 1  1 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-428-two-writable-parameters-are-told-together.hero 1  2 error[ffi_writable_parameter];
tests/golden/unsupported/fixedbugs-517-a-field-is-told-the-header-s-type-width-and-sign.hero 1  3 error[ffi_field_type];
tests/golden/unsupported/fixedbugs-538-two-headers-refused-together-are-both-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-538-two-headers-refused-together-in-the-other-order.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-550-a-file-another-group-s-header-includes-in-the-other-order.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-550-a-file-another-group-s-header-includes-is-told-through-it.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-555-a-name-no-group-binds-declared-two-ways-is-told.hero 1  1 error[ffi_declared_two_ways];
tests/golden/unsupported/fixedbugs-555-one-c-function-two-groups-bind-two-ways-is-told.hero 1  1 error[ffi_declared_two_ways];
tests/golden/unsupported/fixedbugs-556-a-definition-two-modules-units-compile-is-told.hero 1  1 error[ffi_defined_twice];
tests/golden/unsupported/fixedbugs-556-a-tentative-definition-two-modules-units-compile-is-told.hero 1  1 error[ffi_defined_twice];
tests/golden/unsupported/fixedbugs-556-two-definitions-of-one-name-are-told.hero 1  1 error[ffi_defined_twice];
tests/golden/unsupported/fixedbugs-557-a-header-that-compiles-alone-and-not-after-another-is-told.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-559-a-pointee-is-judged-by-its-own-module-s-header.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-559-a-record-is-laid-out-by-its-own-module-s-header.hero 1  1 error[ffi_field_type];
tests/golden/unsupported/fixedbugs-559-a-record-laid-out-alone-is-told-the-same-in-the-other-order.hero 1  1 error[ffi_field_type];
tests/golden/unsupported/fixedbugs-559-a-wider-pointee-is-refused-at-build-not-at-run.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-562-a-header-only-inline-is-told-what-c-wants.hero 0 
tests/golden/unsupported/fixedbugs-563-a-header-that-needs-one-of-c-s-own-is-told-which.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-563-a-header-written-above-the-one-it-needs-is-told-the-move.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-563-a-macro-two-headers-define-two-ways-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-563-a-macro-two-headers-define-unconditionally-is-refused-in-either-order.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-563-a-move-that-changes-a-bound-function-says-so.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-563-a-name-a-header-declares-after-another-is-told-the-move.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-563-a-name-a-header-declares-after-one-of-c-s-own-is-told-which.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-563-a-name-a-header-declares-alone-is-told-the-move.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-568-a-name-a-header-declares-only-under-a-switch-is-told-so.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-568-a-name-hidden-under-x-open-is-told-that-switch.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-568-a-wrong-binding-through-a-switch-header-is-refused.hero 1  1 error[ffi_call_shape];
tests/golden/unsupported/fixedbugs-569-a-header-of-the-program-s-own-with-a-mismatched-format-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-569-a-header-of-the-program-s-own-with-a-missing-return-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-569-a-header-of-the-program-s-own-with-an-uninitialised-read-is-refused.hero 1  1 error[ffi_header_refused];
tests/golden/unsupported/fixedbugs-569-a-wrong-sign-through-a-library-s-header-is-the-binding-s.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-570-a-callback-of-another-type-is-refused-through-the-pragma.hero 1  1 error[ffi_callback_type];
tests/golden/unsupported/fixedbugs-570-a-constant-s-pragma-turns-off-no-check.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-570-a-constant-s-pragma-turns-off-no-lend-check.hero 1  1 error[field_lend_written];
tests/golden/unsupported/fixedbugs-570-a-downgrade-to-a-warning-turns-off-no-check.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-570-a-field-macro-holding-a-pragma-is-refused.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-570-a-function-like-macro-holding-a-pragma-is-refused.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-570-a-handle-of-another-tag-is-refused-through-the-pragma.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-570-a-header-s-ignored-pragma-turns-off-no-sign-check.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-570-a-header-s-pragma-turns-off-no-lend-check.hero 1  1 error[field_lend_written];
tests/golden/unsupported/fixedbugs-570-a-macro-naming-the-bound-function-holds-a-pragma.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-570-a-push-with-no-pop-turns-off-no-check.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-570-a-tag-macro-holding-a-pragma-is-refused.hero 1  1 error[ffi_macro_name];
tests/golden/unsupported/fixedbugs-570-an-integer-for-a-library-s-pointer-is-refused-through-the-pragma.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-570-an-integer-for-a-pointer-is-refused-through-the-pragma.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-570-an-undeclared-function-is-refused-through-the-pragma.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-570-ignoring-wconversion-turns-off-no-check.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-570-ignoring-weverything-turns-off-no-check.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-570-the-gcc-spelling-of-the-pragma-turns-off-no-check.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-570-three-checks-ignored-turn-off-none.hero 1  1 error[ffi_parameter_type];
tests/golden/unsupported/fixedbugs-585-a-name-a-switch-of-the-program-s-own-header-hides-is-told-the-switch.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-585-a-name-no-switch-hides-is-still-told-the-header-declares-none.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-585-a-switch-two-headers-define-is-told-with-both.hero 1  1 error[ffi_unknown_name];
tests/golden/unsupported/fixedbugs-588-a-function-its-header-marks-unavailable-bound-and-not-called.hero 1  1 error[ffi_unavailable];
tests/golden/unsupported/fixedbugs-588-a-function-its-header-marks-unavailable-called-is-told-once.hero 1  1 error[ffi_unavailable];
tests/golden/unsupported/fixedbugs-591-a-bound-function-a-header-labels-by-asm-is-told.hero 0 
tests/golden/unsupported/fixedbugs-591-a-bound-function-a-header-renames-by-redefine-extname-is-told.hero 0 
tests/golden/unsupported/fixedbugs-591-a-rename-in-a-header-the-group-s-header-includes.hero 0 
tests/golden/unsupported/fixedbugs-592-a-header-labelling-a-runtime-function-by-asm-is-told.hero 0 
tests/golden/unsupported/fixedbugs-592-a-header-renaming-a-runtime-name-is-told-on-its-group.hero 0 
tests/golden/unsupported/fixedbugs-592-a-header-renaming-main-is-told-on-its-group.hero 0 
tests/golden/unsupported/fixedbugs-pointer-reached-through-a-field.hero 1 
tests/golden/unsupported/fixedbugs-pointer-reached-through-a-payload.hero 1 
tests/golden/unsupported/fixedbugs555/defines.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs555/half.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs555/long_twice.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs556/also.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs556/counts.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs556/other.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs559/long_twice.hero 1  1 error[ffi_missing_header];
tests/golden/unsupported/fixedbugs559/wide.hero 1  1 error[ffi_missing_header];
tests/harness/cases.hero 0 
tests/harness/issue_pace.hero 0 
tests/harness/issues.hero 0 
tests/harness/shell.hero 0 
tests/harness/suite_cache.hero 0 
