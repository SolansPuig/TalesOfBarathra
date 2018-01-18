#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "message.h"

#define MAILBOX_SIZE 255

static message_t mailbox [MAILBOX_SIZE];

static void destroy (int i) {
    message_t * msg = &(mailbox[i]);
    msg->type = NONE;
    strcpy(msg->payload, "");
}

void message_send (int from, int to, message_types_t type, char *payload) {
    // Assert the payload isn't too large
    assert(strlen(payload) <= PAYLOAD_SIZE);

    // Check whether the message is already in the mailbox. Also, keep the first empty position
    message_t * msg;
    bool found = false;
    int first_empty = MAILBOX_SIZE, i = 0;
    while (!found && i < MAILBOX_SIZE) {
        msg = &mailbox[i];
        if (msg->from == from && msg->to == to && msg->type == type && msg->payload == payload) found = true;
        else {
            if (msg->type == NONE && first_empty == MAILBOX_SIZE) first_empty = i;
            i++;
        }
    }

    // If not, add it to the mailbox
    if (!found) {
        // Assert that one possible positon was found
        assert(first_empty != MAILBOX_SIZE);

        // Create the message
        message_t * message = &(mailbox[first_empty]);
        message->from = from;
        message->to = to;
        message->type = type;
        strcpy(message->payload, payload);
    }
}

int message_read_one (int i, message_t * msg) {
    // Find the first message to the reader
    bool found = false;
    while (!found && i < MAILBOX_SIZE) {
        if (mailbox[i].type != NONE) found = true;
        else i++;
    }

    if (found) {
        // Copy the message
        * msg = mailbox[i];
        // Remove the message from the mailbox
        destroy(i);
        return i+1;
    } else {
        return 0;
    }
}
