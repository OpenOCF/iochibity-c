//******************************************************************
//
// Copyright 2016 NORC at the University of Chicago
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <pthread.h>

#include "server.h"
#include "led.h"
#include "utils.h"

#define TAG "led"

/****************************************************************
    LED Resource Service Provider - implementation
 ****************************************************************/
/* REQUEST dispatcher */
OCEntityHandlerResult
led_request_dispatcher (OCEntityHandlerFlag flag,
			OCEntityHandlerRequest *oic_request, /* just like HttpRequest */
			void* cb /*callbackParam*/)
{
    UNUSED(flag);
    UNUSED(oic_request);
    UNUSED(cb);
    OCEntityHandlerResult ehResult = OC_EH_OK;
    /* OCEntityHandlerResponse response; */
    /* TODO: dispatch to service routine (get, put, etc.) */
    return ehResult;
}

/* CREATE service routine */
OCEntityHandlerResult
svc_led_create_request (OCEntityHandlerRequest *oic_request,
			OCRepPayload          **payload)
{
    UNUSED(oic_request);
    UNUSED(payload);
    OCEntityHandlerResult ehResult = OC_EH_OK;
    return ehResult;
}

/* RETRIEVE service routine. Also handles observe cancellation. */
OCEntityHandlerResult
svc_led_retrieve_request (OCEntityHandlerRequest *oic_request,
			  OCRepPayload **payload)
{
    UNUSED(oic_request);
    UNUSED(payload);
    OCEntityHandlerResult ehResult = OC_EH_OK;
    return ehResult;
}

/* UPDATE service routine */
OCEntityHandlerResult
svc_led_update_request (OCEntityHandlerRequest *oic_request,
			OCRepPayload **payload)
{
    UNUSED(oic_request);
    UNUSED(payload);
    OCEntityHandlerResult ehResult = OC_EH_OK;
    return ehResult;
}

/* DELETE service routine */
OCEntityHandlerResult
svc_led_delete_request (OCEntityHandlerRequest *oic_request,
			OCRepPayload **payload)
{
    UNUSED(oic_request);
    UNUSED(payload);
    OCEntityHandlerResult ehResult = OC_EH_OK;
    return ehResult;
}

void rmgr_register_led_rsvp (struct rsvp_led *rsvp)
{
    OCStackResult op_result = OC_STACK_OK;
    op_result = OCCreateResource(&(rsvp->handle),
				 rsvp->t,
				 rsvp->iface,
				 rsvp->uri,
				 rsvp->dispatch_request,
				 NULL,
				 OC_DISCOVERABLE|OC_OBSERVABLE | OC_SECURE);
    if (op_result != OC_STACK_OK)
    {
        printf("LED resource service provider registration failed!\n");
        exit (EXIT_FAILURE);
    }
}
