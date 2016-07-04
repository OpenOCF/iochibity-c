/* led.h */
#ifndef LED_H_
#define LED_H_

#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"

#include "server.h"

/* LED Instrument Resource */
/* code to represent hw state and operations */

#define RSC_URI_LED  "/a/led"
#define RSC_T_LED    "core.led"
#define RSC_IF_LED   "oc.mi.def"

struct rsc_led_properties
{
  bool state;
  int power;
};

/* Resource SerVice Provider for LED resource */
struct rsvp_led
{
  OCResourceHandle           handle;
  char                      *t;	/* type name, dotted str */
  char                      *iface;	/* interface name, dotted str */
  char                      *uri;
  struct rsc_led_properties  props;
  OCEntityHandler            dispatch_request;
  ServiceRoutine	     svc_create_request;
  ServiceRoutine	     svc_retrieve_request;
  ServiceRoutine	     svc_update_request;
  ServiceRoutine	     svc_delete_request;
  ServiceRoutine	     svc_observe_request;
};

OCEntityHandlerResult
led_request_dispatcher (OCEntityHandlerFlag flag,
			OCEntityHandlerRequest *oic_request, /* just like HttpRequest */
			void* cb /*callbackParam*/);

void rmgr_register_led_rsvp (struct rsvp_led *rsvp);

#endif

