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
#include <math.h>

#include "mraa.h"

#include "temperature.h"

#define TAG "temperature"

#define ONBOARD_LED_PIN 13
#define TEMPERATURE_AIO_PIN 0
#define LIGHT_SENSOR_AIO_PIN 2
#define SAMPLE_NUM 5

/* We only support one of these, so do it here: */
struct rsvp_temperature my_temperature_rsvp = { .t     = RSC_T_TEMPERATURE,
						.iface = RSC_IF_TEMPERATURE,
						.uri   = RSC_URI_TEMPERATURE,
						.props = { .temperature = 0 },
						.dispatch_request = temperature_request_dispatcher,
						.svc_retrieve_request = svc_temperature_retrieve_request
};



/****************************************************************
    TEMPERATURE Instrument Model
 ****************************************************************/
mraa_gpio_context led_gpio = NULL;
mraa_aio_context tmp_aio = NULL;
mraa_aio_context light_aio = NULL;

void setup_pins()
{
    led_gpio = mraa_gpio_init(ONBOARD_LED_PIN); // Initialize pin 13
    if (led_gpio != NULL)
        mraa_gpio_dir(led_gpio, MRAA_GPIO_OUT); // Set direction to OUTPUT
    tmp_aio = mraa_aio_init(TEMPERATURE_AIO_PIN);   // initialize pin 0
    light_aio = mraa_aio_init(LIGHT_SENSOR_AIO_PIN);   // initialize pin 2
}

void close_pins()
{
    mraa_gpio_close(led_gpio);
    mraa_aio_close(tmp_aio);
    mraa_aio_close(light_aio);
}

float get_avg_temp_raw()
{
    if (tmp_aio == NULL)
    {
        tmp_aio = mraa_aio_init(TEMPERATURE_AIO_PIN); // initialize pin 0
    }

    uint16_t adc_value = 0;
    for (int i=0; i< SAMPLE_NUM; i++)
        adc_value += mraa_aio_read(tmp_aio);           // read the raw value

    float average = (float)adc_value/SAMPLE_NUM;
    printf("Temperature reading raw ... %f\n", average);

    return average;
}

int read_temp_mraa()
{
    setup_pins();

    /* http://www.seeedstudio.com/wiki/Grove_-_Temperature_Sensor_V1.2 */
    const int B=4275;                 // B value of the thermistor
    const int R0 = 100000;            // R0 = 100k

    int a = mraa_aio_read(tmp_aio);           // read the raw value

    float R = 1023.0/((float)a)-1.0;
    R = 100000.0*R;

    float temperature=1.0/(log(R/100000.0)/B+1/298.15)-273.15;//convert to temperature via datasheet ;

    printf("temperature = %f\n", temperature);

    /* from the iotivity sample code: */
    /* float beta = 4090.0;            //the beta of the thermistor, magic number */
    /* float t_raw = get_avg_temp_raw(); */
    /* float R = 1023.0/t_raw -1;      // */
    /* R = 10000.0/R;                  // 10K resistor divider circuit */

    /* float T1 = log(R/10000.0)/beta; // natural log */
    /* float T2 = T1 + 1.0/298.15;     // room temp 25C= 298.15K */
    /* float ret = 1.0/T2 - 273.0; */

    close_pins();

  return ret;
}

OCRepPayload*
temperature_read(struct rsvp_temperature rsvp)
{
  OCRepPayload* payload = OCRepPayloadCreate();
  if(!payload) {
    OIC_LOG(ERROR, TAG, PCF("Failed to allocate Payload"));
    return NULL;
  }

  uint8_t types_cnt = 0;
  uint8_t ifs_cnt   = 0;

  OCGetNumberOfResourceTypes(rsvp.handle, &types_cnt);
  printf("TEMP Resource types count: %d\n", types_cnt);
  for (int i = 0; i < types_cnt; i++) {
    printf("TEMP Resource type name %d: %s\n", i, OCGetResourceTypeName(rsvp.handle, i));
  }

  OCGetNumberOfResourceInterfaces(rsvp.handle, &ifs_cnt);
  printf("TEMP Resource interfaces count: %d\n", ifs_cnt);
  for (int i = 0; i < ifs_cnt; i++) {
    printf("TEMP Resource interface %d: %s\n", i, OCGetResourceInterfaceName(rsvp.handle, i));
  }

  OCRepPayloadSetUri(payload, rsvp.uri);
  OCRepPayloadSetPropInt(payload, "temperature", read_temp_mraa());
  OCRepPayloadSetPropInt(payload, "power", 27);

  return payload;
}

/****************************************************************
    TEMPERATURE Resource Service Provider - implementation
 ****************************************************************/
/* REQUEST dispatcher */
OCEntityHandlerResult
temperature_request_dispatcher (OCEntityHandlerFlag flag,
				OCEntityHandlerRequest *oic_request,
				void* cb /*callbackParam*/)
{
    if (!oic_request)
    {
        OIC_LOG (ERROR, TAG, "Invalid request pointer");
        return OC_EH_ERROR;
    }
    OIC_LOG (INFO, TAG, "Received request");

    OCRepPayload* payload = NULL;

    OCEntityHandlerResult ehResult = OC_EH_OK;
    OCEntityHandlerResponse response = { 0, 0, OC_EH_ERROR, 0, 0, { },{ 0 }, false };

    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG (INFO, TAG, "Flag includes OC_REQUEST_FLAG");
	switch (oic_request->method) {
	case  OC_REST_GET:
	    ehResult = my_temperature_rsvp.svc_retrieve_request (oic_request, &payload);
	    break;
	case OC_REST_PUT:
	    OIC_LOG (INFO, TAG, "Received OC_REST_PUT from client");
	    ehResult = my_temperature_rsvp.svc_update_request (oic_request, &payload);
	    break;
	case OC_REST_POST:
	    OIC_LOG (INFO, TAG, "Received OC_REST_POST from client: unsupported");
	    /* if (foo) { */
	    /*   ehResult = svc_temperature_create_request (oic_request, &response, &payload); */
	    /* } else { */
	    /*   ehResult = svc_temperature_update_request (oic_request, &payload); */
	    /* } */
	    break;
	default:
	    OIC_LOG_V (INFO, TAG, "Received unsupported method %d from client",
		       oic_request->method);
	}

	if (ehResult == OC_EH_OK && ehResult != OC_EH_FORBIDDEN)
	  {
	    // Format the response.  Note this requires some info about the request
	    response.requestHandle = oic_request->requestHandle;
	    response.resourceHandle = oic_request->resource;
	    response.ehResult = ehResult;
	    response.payload = (void *) (payload);
	    response.numSendVendorSpecificHeaderOptions = 0;
	    memset(response.sendVendorSpecificHeaderOptions, 0, sizeof response.sendVendorSpecificHeaderOptions);
	    memset(response.resourceUri, 0, sizeof(response.resourceUri));
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
    OCPayloadDestroy(response.payload);
    return ehResult;
}

/* CREATE service routine */
OCEntityHandlerResult
svc_temperature_create_request (OCEntityHandlerRequest *oic_request,
				OCEntityHandlerResponse *response,
				OCRepPayload          **payload)
{
    OCEntityHandlerResult ehResult;
    return ehResult;
}

/* RETRIEVE service routine. Also handles observe cancellation. */
OCEntityHandlerResult
svc_temperature_retrieve_request (OCEntityHandlerRequest *oic_request,
				  OCRepPayload **payload)
{
    OIC_LOG (INFO, TAG, "Servicing OC_REST_GET request from client");
    OCEntityHandlerResult request_result;
    if(oic_request->payload && oic_request->payload->type != PAYLOAD_TYPE_REPRESENTATION) {
        OIC_LOG(ERROR, TAG, PCF("Incoming GET payload not a representation"));
        request_result = OC_EH_ERROR;
    }

    if (oic_request->resource != my_temperature_rsvp.handle) {
        OIC_LOG(ERROR, TAG, PCF("Incoming GET uri not found"));
    }

    OCRepPayload *temperature_retrieve_response = temperature_read(my_temperature_rsvp);

    if(temperature_retrieve_response)
    {
        *payload = temperature_retrieve_response;
        request_result = OC_EH_OK;
     }
    else
    {
        request_result = OC_EH_ERROR;
    }
    return request_result;
}

/* UPDATE service routine */
OCEntityHandlerResult
svc_temperature_update_request (OCEntityHandlerRequest *oic_request, OCRepPayload **payload)
{
    OCEntityHandlerResult ehResult;
    return ehResult;
}

/* DELETE service routine */
OCEntityHandlerResult
svc_temperature_delete_request (OCEntityHandlerRequest *oic_request,
				OCRepPayload **payload)
{
    OCEntityHandlerResult ehResult;
    return ehResult;
}

void rmgr_register_temperature_rsvp (struct rsvp_temperature *rsvp)
{
    OCStackResult op_result = OC_STACK_OK;
    op_result = OCCreateResource(&(rsvp->handle),
				 rsvp->t,
				 rsvp->iface,
				 rsvp->uri,
				 rsvp->dispatch_request,
				 NULL,
				 OC_DISCOVERABLE|OC_OBSERVABLE);
    if (op_result != OC_STACK_OK)
    {
        printf("TEMPERATURE resource service provider registration failed!\n");
        exit (EXIT_FAILURE);
    }
}
