/* server.h */
#ifndef SERVER_H_
#define SERVER_H_

#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"

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

//-----------------------------------------------------------------------------
//ResourceNode
//-----------------------------------------------------------------------------
struct ResourceNode
{
    const char * sid;
    const char * uri;
    OCDevAddr endpoint;
    struct ResourceNode * next;
};

#endif

