// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#include <assert.h>
#include <libunwind.h>

__attribute__((noinline)) static void reposition() {
  unw_context_t context;
  unw_cursor_t cursor;
  unw_proc_info_t info;
  unw_word_t originalIP;
  assert(unw_getcontext(&context) == UNW_ESUCCESS);
  assert(unw_init_local(&cursor, &context) == UNW_ESUCCESS);
  assert(unw_get_reg(&cursor, UNW_REG_IP, &originalIP) == UNW_ESUCCESS);
  assert(unw_get_proc_info(&cursor, &info) == UNW_ESUCCESS);

  for (unsigned i = 0; i < 2; ++i) {
    assert(unw_set_reg(&cursor, UNW_REG_IP, 0) == UNW_ESUCCESS);
    assert(unw_get_proc_info(&cursor, &info) == UNW_ENOINFO);
    assert(unw_set_reg(&cursor, UNW_REG_IP, originalIP) == UNW_ESUCCESS);
    assert(unw_get_proc_info(&cursor, &info) == UNW_ESUCCESS);
    assert(info.start_ip <= originalIP && originalIP < info.end_ip);
  }
  // Recovery must also permit walking the next frame, not just returning info.
  assert(unw_step(&cursor) > 0);
}

int main() {
  reposition();
}
