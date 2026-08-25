#ifndef LEDMANAGER_H
#define LEDMANAGER_H

/* Contextdefinitions. The actual number is the priority, too */
typedef enum context_id {
  CTX_BLUETOOTH = 1,
  CTX_BUSY = 2,
  CTX_ERROR = 3,
  CTX_FATAL = 4,
  CTX_OK = 5,
  CTX_COUNT = 6
} context_id;

void led_context_init();
void led_context_enter(context_id ctx);
void led_context_leave(context_id ctx);
#endif // LEDMANAGER_H
