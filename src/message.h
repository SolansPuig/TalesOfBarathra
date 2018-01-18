#ifndef MESSAGE_H
#define MESSAGE_H

typedef enum {
    NONE,
    CREATE_PROP,
    CREATE_SIMPLE_IA,
    CREATE_COMPLEX_IA,
    DAMAGE,
    SUICIDE,
    MOVE,
    STOP,
    ANIMATE,
    FREEZE,
    COMMAND,
    TEXT
} message_types_t;

#define PAYLOAD_SIZE 32
typedef struct {
    int from;
    int to;
    message_types_t type;
    char payload [PAYLOAD_SIZE];
} message_t;

void message_send (int from, int to, message_types_t type, char *payload);
// Send a message to an entity. Type shall not be void. Payload can contain multiple fields, separated by commas and no space.
// Two examples:    type = DAMAGE       payload = "fire,5"              will tell the entity to take 5 points of fire damage.
//                  type = CREATE_PROP  payload = "tree0,15,-23,1"       will tell the entity to create a prop of type "tree0" at the (relative from itself) x 15, y 23 and z 1.

int message_read_one (int i, message_t * msg);
// Put the first existing message on the placeholder msg.
// Returns 1 if a message was found, and 0 if there are no more messages.

#endif
