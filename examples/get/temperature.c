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

#include "temperature.h"

#define TAG "temperature"

/****************************************************************
    TEMPERATURE Resource Service Provider - implementation
 ****************************************************************/
/* CREATE service routine */
OCEntityHandlerResult
svc_temperature_create_request (OCEntityHandlerRequest* oic_request,
				OCRepPayload**          payload)
{
    OIC_LOG(INFO, TAG, __func__);
    OC_UNUSED(oic_request);
    OC_UNUSED(payload);
    OCEntityHandlerResult ehResult = OC_EH_OK;
    return ehResult;
}

/* RETRIEVE service routine. Also handles observe cancellation. */
OCEntityHandlerResult
svc_temperature_retrieve_request (OCEntityHandlerRequest* oic_request,
				  OCRepPayload**          payload)
{
    OIC_LOG(INFO, TAG, __func__);
    OC_UNUSED(oic_request);
    OC_UNUSED(payload);

    /* if(ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION) */
    /* { */
    /*     OIC_LOG(ERROR, TAG, PCF("Incoming payload not a representation")); */
    /*     return nullptr; */
    /* } */

    OCEntityHandlerResult ehResult = OC_EH_OK;

    OCRepPayload* new_payload = OCRepPayloadCreate();
    if(!new_payload)
    {
        OIC_LOG(ERROR, TAG, PCF("Failed to allocate Payload"));
        return OC_EH_ERROR;
    }

    OCRepPayloadSetUri(new_payload, RSC_URI_TEMPERATURE);
    OCRepPayloadSetPropInt(new_payload, "temp", 72);

    *payload = new_payload;
    return ehResult;
}

/* UPDATE service routine */
OCEntityHandlerResult
svc_temperature_update_request (OCEntityHandlerRequest* oic_request,
				OCRepPayload**          payload)
{
    OIC_LOG(INFO, TAG, __func__);
    OC_UNUSED(oic_request);
    OC_UNUSED(payload);
    OCEntityHandlerResult ehResult = OC_EH_OK;
    return ehResult;
}

/* DELETE service routine */
OCEntityHandlerResult
svc_temperature_delete_request (OCEntityHandlerRequest* oic_request)
{
    OIC_LOG(INFO, TAG, __func__);
    OC_UNUSED(oic_request);
    OCEntityHandlerResult ehResult = OC_EH_OK;
    return ehResult;
}

OCEntityHandlerResult svc_temperature_unknown_request(OCEntityHandlerRequest*  oic_request)
{
    OIC_LOG(INFO, TAG, __func__);
    return OC_EH_RESOURCE_NOT_FOUND;
}

/* **************************************************************** */
/* REQUEST dispatcher */
OCEntityHandlerResult
temperature_request_dispatcher (OCEntityHandlerFlag flag,
				OCEntityHandlerRequest *oic_request,
				void* param /*callbackParam*/)
{
    OIC_LOG(INFO, TAG, __func__);

    // Validate pointer
    if (!oic_request)
    {
        OIC_LOG (ERROR, TAG, "Invalid request pointer");
        return OC_EH_ERROR;
    }
    OC_UNUSED(param);
    OCEntityHandlerResult ehResult = OC_EH_OK;
    OCEntityHandlerResponse response = { .requestHandle = NULL };
    OCRepPayload* payload;  /* = NULL; */

    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG (INFO, TAG, "Flag includes OC_REQUEST_FLAG");

        if (oic_request->resource == NULL) {
            OIC_LOG (INFO, TAG, "Received request from client for unknown resource");
            ehResult = svc_temperature_unknown_request(oic_request);
        } else {
	    switch(oic_request->method) {
	    case OC_REST_GET:
		OIC_LOG (INFO, TAG, "Received OC_REST_GET from client");
		ehResult = svc_temperature_retrieve_request(oic_request, &payload);
		break;
	    case OC_REST_PUT:
		OIC_LOG (INFO, TAG, "Received OC_REST_PUT from client");
		ehResult = svc_temperature_update_request (oic_request, &payload);
		break;
	    case OC_REST_DELETE:
		OIC_LOG (INFO, TAG, "Received OC_REST_DELETE from client");
		ehResult = svc_temperature_delete_request(oic_request);
		break;
	    default:
		OIC_LOG_V (INFO, TAG, "Received unsupported method %d from client",
			   oic_request->method);
		ehResult = OC_EH_ERROR;
		break;
	    }
	}

	// If the result isn't an error or forbidden, send response
	if (!((ehResult == OC_EH_ERROR) || (ehResult == OC_EH_FORBIDDEN)))
	    {
		// Format the response.  Note this requires some info about the request
		response.requestHandle = oic_request->requestHandle;
		response.resourceHandle = oic_request->resource;
		response.ehResult = ehResult;
		response.payload = (OCPayload*)payload; /* testing */
		// Indicate that response is NOT in a persistent buffer
		response.persistentBufferFlag = 0;

		// Send the response
		if (OCDoResponse(&response) != OC_STACK_OK)
		    {
			OIC_LOG(ERROR, TAG, "Error sending response");
			ehResult = OC_EH_ERROR;
		    }
	    }
    }
    return ehResult;
}

OCStackResult rmgr_register_temperature_rsvp (struct rsvp_temperature *rsvp)
{
    OIC_LOG(INFO, TAG, __func__);
    OCStackResult op_result = OC_STACK_OK;
    op_result = OCCreateResource(&(rsvp->handle),
				 rsvp->t,
				 rsvp->iface,
				 rsvp->uri,
				 rsvp->dispatch_request,
				 NULL,
				 OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
    if (op_result != OC_STACK_OK)
    {
        printf("TEMPERATURE resource service provider registration failed!\n");
        exit (EXIT_FAILURE);
    }
    return op_result;
}
