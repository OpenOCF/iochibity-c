/* server.h */
#ifndef BUTTON_H_
#define BUTTON_H_

#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"

#include "server.h"

/* Button Instrument Resource */
/* code to represent hw state and operations */

#define RSC_URI_BUTTON  "/a/button"
#define RSC_T_BUTTON    "core.button"
#define RSC_IF_BUTTON   "oc.mi.def"

struct rsc_button_properties
{
  bool state;
};

/* Resource SerVice Provider for Button resource */
struct rsvp_button
{
  OCResourceHandle           handle;
  char                      *t;	/* type name, dotted str */
  char                      *iface;	/* interface name, dotted str */
  char                      *uri;
  struct rsc_button_properties props;
  OCEntityHandler            dispatch_request;
  ServiceRoutine	     svc_create_request;
  ServiceRoutine	     svc_retrieve_request;
  ServiceRoutine	     svc_update_request;
  ServiceRoutine	     svc_delete_request;
  ServiceRoutine	     svc_observe_request;
};

OCEntityHandlerResult
button_request_dispatcher (OCEntityHandlerFlag flag,
			   OCEntityHandlerRequest *oic_request, /* just like HttpRequest */
			   void* cb /*callbackParam*/);

void rmgr_register_button_rsvp (struct rsvp_button *rsvp_button);

#endif

