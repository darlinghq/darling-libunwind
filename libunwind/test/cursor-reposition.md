# Cursor reposition regression

`cursor-reposition.pass.cpp` follows the existing libunwind lit test naming.
Compile with assertions and unwind tables enabled and link against the library
being tested. For Darling, use the configured Darwin toolchain and run the
executable in a matching private runtime:

```sh
darling shell /Volumes/SystemRoot/absolute/path/to/cursor-reposition
```

The test captures its own cursor, records a valid IP, moves to zero (which must
have no unwind info), and restores that IP twice. It checks both recovered
function information and a subsequent unwind step. Before the fix, the first
restored `unw_get_proc_info` returns `UNW_ENOINFO`; after the fix it succeeds.
It uses only this test's own compiled frames and does not test guest stack walks.
