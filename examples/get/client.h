/* server.h */
#ifndef SERVER_H_
#define SERVER_H_

#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocpayload.h"
#include "logger.h"

typedef				/* FIXME: put this in the sdk? */
OCEntityHandlerResult (* ServiceRoutine) (OCEntityHandlerRequest *ehRequest,
					  OCRepPayload **payload);

typedef struct observers
{
    OCObservationId observationId;
    bool valid;
    OCResourceHandle resourceHandle;
} observers_t;

static int g_observe_notify_type = 3;

/*
 * ResourceNode
 */
struct ResourceNode
{
    const char * sid;
    const char * uri;
    OCDevAddr dev_addr;
    /* port? */
    struct ResourceNode * next;
};

#endif

