/* types.h */
#ifndef TYPES_H_
#define TYPES_H_

#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"

typedef				/* FIXME: put this in the sdk? */
OCEntityHandlerResult (* ServiceRoutine) (OCEntityHandlerRequest *ehRequest,
					  OCRepPayload **payload);

#endif

