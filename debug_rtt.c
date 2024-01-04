#include <stdio.h>
#include <errno.h>

#include "debug.h"
#include "SEGGER_RTT.h"

#define DEFAULT_BUFFER_INDEX 0u

/// @brief  Initializes the debug buffers
/// @param  void No arguments permitted
/// @return Status (>=0 success, <0 error)
///
static int8_t debug_init(void);

static int8_t debug_init(void) {
  static int8_t status = -ENXIO;

  if (status >= 0) { return status; }

  int8_t up_status = SEGGER_RTT_ConfigUpBuffer(
                         DEFAULT_BUFFER_INDEX,
                         NULL, NULL, 0,
                         SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

  ///@todo Configure down buffer
  int down_status = 0;

  if (up_status >= 0 && down_status >= 0) {
    status = 0;
  }

  return status;
}

void debugf(const char *format, ...) {
  char    buffer[DEBUGF_CONSOLE_WIDTH];
  int8_t  status;
  int8_t  truncated;
  uint8_t rtt_bytes;
  va_list vlist;

  if (debug_init() < 0) { return; }

  va_start(vlist, format);
  status = vsnprintf(buffer, sizeof(buffer), format, vlist);
  va_end(vlist);

  if (status >= 0) {
    rtt_bytes = SEGGER_RTT_Write(DEFAULT_BUFFER_INDEX,
                                 buffer, status);
    truncated = (sizeof(buffer) == status ||
                 rtt_bytes != status);

    if (truncated) {
      status = SEGGER_RTT_printf(
                   DEFAULT_BUFFER_INDEX,
                   "\n%s%s[TRUNCATED LINE ABOVE^]\n",
                   DEBUG_PREFIX, DEBUG_WARNING, DEBUG_RESET);
    }
  } 

  if (status < 0) {
    (void)SEGGER_RTT_printf(DEFAULT_BUFFER_INDEX, "%s%s %d%s\n",
              DEBUG_PREFIX, DEBUG_ERROR, status, DEBUG_RESET);
  }
}
