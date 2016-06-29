/* server.h */
#ifndef SERVER_H_
#define SERVER_H_

#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"

typedef struct observers
{
    OCObservationId observationId;
    bool valid;
    OCResourceHandle resourceHandle;
} observers_t;

static int g_observe_notify_type = 3;

#endif

