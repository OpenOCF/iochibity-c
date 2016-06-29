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
#include "types.h"
#include "button.h"
#include "led.h"
#include "temperature.h"

#define TAG "minserver"

/* Platform Descriptor: OCPlatformInfo
 * This structure describes the platform properties. All non-Null properties will be
 * included in a platform discovery request. */
OCPlatformInfo platform_info =
  {
    .platformID			= "minPlatformID",
    .manufacturerName		= "minName",
    .manufacturerUrl		= "minManufacturerUrl",
    .modelNumber		= "minModelNumber",
    .dateOfManufacture		= "minDateOfManufacture",
    .platformVersion		= "minPlatformVersion",
    .operatingSystemVersion	= "minOS",
    .hardwareVersion		= "minHardwareVersion",
    .firmwareVersion		= "minFirmwareVersion",
    .supportUrl			= "minSupportUrl",
    .systemTime			= "2015-05-15T11.04"
  };

/* Device Descriptor: OCDeviceInfo
 * This structure is expected as input for device properties.
 * device name is mandatory and expected from the application.
 * device id of type UUID will be generated by the stack. */
OCDeviceInfo device_info =
  {
    .deviceName = "minDeviceName",
    /* OCStringLL *types; */
    .specVersion = "minDeviceSpecVersion", /* device specification version */
    .dataModelVersion = "minDeviceModelVersion"
  };

/* const char *deviceUUID = "myDeviceUUID"; */
/* const char *version = "myVersion"; */

OCEntityHandlerResult
default_request_dispatcher (OCEntityHandlerFlag flag,
			    OCEntityHandlerRequest *oic_request, /* just like HttpRequest */
			    char *uri,
			    void *cb /*callbackParam*/)
{
    OCEntityHandlerResult ehResult = OC_EH_OK;
    OCEntityHandlerResponse response;
    return ehResult;
}

#define SAMPLE_MAX_NUM_OBSERVATIONS     8
#define SAMPLE_MAX_NUM_POST_INSTANCE  2

observers_t interestedObservers[SAMPLE_MAX_NUM_OBSERVATIONS];

pthread_t threadId_observe;
/* pthread_t threadId_presence; */

static bool observeThreadStarted = false;

/* #ifdef WITH_PRESENCE */
/* #define numPresenceResources (2) */
/* #endif */

int gQuitFlag = 0;

/* SIGINT handler: set gQuitFlag to 1 for graceful termination */
void handleSigInt(int signum)
{
    if (signum == SIGINT)
    {
        gQuitFlag = 1;
    }
}

static void PrintUsage()
{
    printf("Usage : server -o <0|1>\n");
    printf("-o 0 : Notify all observers\n");
    printf("-o 1 : Notify list of observers\n");
}

/****************************************************************
 *  MAIN
 *  1. initialize rmgr (stack)
 *  2. register platform info
 *  3. register device info
 *  4. register service routine for default rsvp
 *  5. register Resource Service Provider for each resource
 *  6. enter processing loop
 ****************************************************************/
int main(int argc, char* argv[])
{
    int opt = 0;

    while ((opt = getopt(argc, argv, "o:s:p:d:u:w:r:j:")) != -1) {
      switch(opt) {
      case 'o':
	g_observe_notify_type = atoi(optarg);
	break;
      default:
	PrintUsage();
	return -1;
      }
    }

    if ((g_observe_notify_type != 0) && (g_observe_notify_type != 1)) {
        PrintUsage();
        return -1;
    }

    OIC_LOG(DEBUG, TAG, "Server is starting...");

    OCStackResult op_result;

    /* 1. initialize */
    op_result = OCInit(NULL, 0, OC_SERVER);
    if (op_result != OC_STACK_OK) {
        printf("OCStack init error\n");
        return 0;
    }

    /* 2. register platform info */
    op_result = OCSetPlatformInfo(platform_info);
    if (op_result != OC_STACK_OK) {
        printf("Platform Registration failed!\n");
        exit (EXIT_FAILURE);
    }

    /* 3. register default device info */
    OCResourcePayloadAddStringLL(&device_info.types, "oic.wk.d");
    op_result = OCSetDeviceInfo(device_info);
    if (op_result != OC_STACK_OK) {
        printf("Device Registration failed!\n");
        exit (EXIT_FAILURE);
    }

    /* 4. register service routine for default rsvp */
    op_result = OCSetDefaultDeviceEntityHandler(default_request_dispatcher, NULL);
    if (op_result != OC_STACK_OK) {
        printf("Default rsvp service routine registration failed!\n");
        exit (EXIT_FAILURE);
    }

    /* 5. register rsvps */
    struct rsvp_led my_led_rsvp = { .t     = RSC_T_LED,
				    .iface = RSC_IF_LED,
				    .uri   = RSC_URI_LED,
				    .props = { .state = 0, .power = 0 },
				    .dispatch_request = led_request_dispatcher };

    rmgr_register_led_rsvp (&my_led_rsvp);
    if (op_result != OC_STACK_OK) {
      printf("LED resource service provider registration failed!\n");
      exit (EXIT_FAILURE);
    }

    struct rsvp_button my_button_rsvp = { .t     = RSC_T_BUTTON,
					  .iface = RSC_IF_BUTTON,
					  .uri   = RSC_URI_BUTTON,
					  .props = { .state = 0 },
					  .dispatch_request = button_request_dispatcher };

    rmgr_register_button_rsvp (&my_button_rsvp);
    if (op_result != OC_STACK_OK) {
      printf("BUTTON resource service provider registration failed!\n");
      exit (EXIT_FAILURE);
    }
    rmgr_register_temperature_rsvp (&my_temperature_rsvp);
    if (op_result != OC_STACK_OK) {
      printf("TEMPERATURE resource service provider registration failed!\n");
      exit (EXIT_FAILURE);
    }

    // Initialize observations data structure for the resource
    /* for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++) */
    /* { */
    /*     interestedObservers[i].valid = false; */
    /* } */


    /*
     * Create a thread for generating changes that cause presence notifications
     * to be sent to clients
     */

    /* #ifdef WITH_PRESENCE */
    /* pthread_create(&threadId_presence, NULL, presenceNotificationGenerator, (void *)NULL); */
    /* #endif */

    // Break from loop with Ctrl-C
    printf("Entering server main loop...\n");

    /* DeletePlatformInfo(); */
    /* DeleteDeviceInfo(); */

    signal(SIGINT, handleSigInt);

    while (!gQuitFlag)
    {
        if (OCProcess() != OC_STACK_OK)
        {
            printf("OCStack process error\n");
            return 0;
        }
    }

    /* if (observeThreadStarted) */
    /* { */
    /*     pthread_cancel(threadId_observe); */
    /*     pthread_join(threadId_observe, NULL); */
    /* } */

    /* pthread_cancel(threadId_presence); */
    /* pthread_join(threadId_presence, NULL); */

    printf("Exiting server main loop...\n");

    if (OCStop() != OC_STACK_OK)
    {
        printf("OCStack process error\n");
    }

    return 0;
}

