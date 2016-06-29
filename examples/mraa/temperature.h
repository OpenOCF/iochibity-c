#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"

#include "types.h"

/* Temperature Instrument Resource */
/* code to represent hw state and operations */

#define RSC_URI_TEMPERATURE  "/a/temperature"
#define RSC_T_TEMPERATURE    "core.temperature"
#define RSC_IF_TEMPERATURE   "oc.mi.def"

struct rsc_temperature_properties
{
  int temperature;
};

/* Resource SerVice Provider for Temperature resource */
struct rsvp_temperature
{
  OCResourceHandle           handle;
  char                      *t;	/* type name, dotted str */
  char                      *iface;	/* interface name, dotted str */
  char                      *uri;
  struct rsc_temperature_properties props;
  OCEntityHandler            dispatch_request;
  ServiceRoutine	     svc_create_request;
  ServiceRoutine	     svc_retrieve_request;
  ServiceRoutine	     svc_update_request;
  ServiceRoutine	     svc_delete_request;
  ServiceRoutine	     svc_observe_request;
};

/* ServiceRoutine svc_temperature_update_request; */

void rmgr_register_temperature_rsvp (struct rsvp_temperature *rsvp_temperature);

OCEntityHandlerResult
temperature_request_dispatcher (OCEntityHandlerFlag flag,
			   OCEntityHandlerRequest *oic_request, /* just like HttpRequest */
			   void* cb /*callbackParam*/);

OCEntityHandlerResult
svc_temperature_create_request (OCEntityHandlerRequest *oic_request,
				OCEntityHandlerResponse *response,
				OCRepPayload          **payload);

OCEntityHandlerResult
svc_temperature_retrieve_request (OCEntityHandlerRequest *oic_request,
				  OCRepPayload **payload);

OCEntityHandlerResult
svc_temperature_update_request (OCEntityHandlerRequest *oic_request, OCRepPayload **payload);
/* ServiceRoutine svc_temperature_update_request; */


extern struct rsvp_temperature my_temperature_rsvp;

#endif

