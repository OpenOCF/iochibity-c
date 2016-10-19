/* temperature.h */
#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"

#include "get_server.h"

/* Temperature Instrument Resource */
/* code to represent hw state and operations */

#define RSC_URI_TEMPERATURE  "/a/temperature"
#define RSC_T_TEMPERATURE    "core.temperature"
#define RSC_IF_TEMPERATURE   "oc.mi.def"

struct rsc_temperature_properties
{
  bool state;
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

OCEntityHandlerResult
temperature_request_dispatcher (OCEntityHandlerFlag flag,
			   OCEntityHandlerRequest *oic_request, /* just like HttpRequest */
			   void* cb /*callbackParam*/);

OCStackResult rmgr_register_temperature_rsvp (struct rsvp_temperature *rsvp_temperature);

#endif

