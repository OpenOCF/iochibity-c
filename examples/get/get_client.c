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

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"
#include "payload_logging.h"

#include "get_client.h"
#include "temperature.h"
#include "utils.h"

#include "debug.h"		/* for libcoap debugging */
#include "tinydtls/debug.h"		/* for tinydtls debugging */

#define TAG "GET client"

#define DEFAULT_CONTEXT_VALUE 0x99

static char CRED_FILE[] = "data/oic_svr_db_client.dat";

static int UnicastDiscovery = 0;

static char discoveryAddr[100];

static OCDevAddr device_address;

pthread_t ui_thread;

OCQualityOfService g_qos = OC_LOW_QOS;

//The following variable determines the interface protocol (IPv4, IPv6, etc)
//to be used for sending unicast messages. Default set to IP dual stack.
static OCConnectivityType conn_type = CT_ADAPTER_IP;

/* Platform Descriptor: OCPlatformInfo
 * This structure describes the platform properties. All non-Null properties will be
 * included in a platform discovery request. */
OCPlatformInfo platform_info =
  {
    .platformID			= "clientPlatformID",
    .manufacturerName		= "clientName",
    .manufacturerUrl		= "clientManufacturerUrl",
    .modelNumber		= "clientModelNumber",
    .dateOfManufacture		= "clientDateOfManufacture",
    .platformVersion		= "clientPlatformVersion",
    .operatingSystemVersion	= "clientOS",
    .hardwareVersion		= "clientHardwareVersion",
    .firmwareVersion		= "clientFirmwareVersion",
    .supportUrl			= "clientSupportUrl",
    .systemTime			= "2015-05-15T11.04"
  };

/* Device Descriptor: OCDeviceInfo
 * This structure is expected as input for device properties.
 * device name is mandatory and expected from the application.
 * device id of type UUID will be generated by the stack. */
OCDeviceInfo device_info =
  {
    .deviceName = "clientDeviceName",
    /* OCStringLL *types; */
    .specVersion = "clientDeviceSpecVersion" /* device specification version */
    //.dataModelVersions = "clientDeviceModleVersion"
  };

/* const char *deviceUUID = "myDeviceUUID"; */
/* const char *version = "myVersion"; */

OCEntityHandlerResult
default_request_dispatcher (OCEntityHandlerFlag flag,
			    OCEntityHandlerRequest *oic_request, /* just like HttpRequest */
			    char *uri,
			    void *cb /*callbackParam*/)
{
    OC_UNUSED(flag);
    OC_UNUSED(oic_request);
    OC_UNUSED(uri);
    OC_UNUSED(cb);
    OCEntityHandlerResult ehResult = OC_EH_OK;
    /* OCEntityHandlerResponse response; */
    return ehResult;
}

#define SAMPLE_MAX_NUM_OBSERVATIONS     8
#define SAMPLE_MAX_NUM_POST_INSTANCE  2

observers_t interestedObservers[SAMPLE_MAX_NUM_OBSERVATIONS];

pthread_t threadId_observe;
/* pthread_t threadId_presence; */

/* static bool observeThreadStarted = false; */

/* #ifdef WITH_PRESENCE */
/* #define numPresenceResources (2) */
/* #endif */

int gQuitFlag = 0;
int waiting   = 0;

struct ResourceNode *resourceList;

/* SIGINT handler: set gQuitFlag to 1 for graceful termination */
void handleSigInt(int signum)
{
    if (signum == SIGINT)
    {
        gQuitFlag = 1;
    }
}

OCStackResult send_oic_messsage(OCMethod method,
				OCDevAddr* device_address,
				char* query_uri,
				OCPayload* payload,
				OCQualityOfService qos,
				OCClientResponseHandler cb)
				/* OCHeaderOption * options, */
				/* uint8_t numOptions) */
{
    OCStackResult ret;
    OCCallbackData cbData;
    OCDoHandle handle;

    cbData.cb = cb;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    ret = OCDoResource(&handle,
		       method,
		       query_uri,
		       device_address,
		       NULL,  /* payload, */
		       conn_type, /* OCConnectivityType */
		       qos,
		       &cbData,
		       NULL,	/* OCHeaderOption* */
		       0);	/* numOptions */

    if (ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCDoResource returns error %d with method %d", ret, method);
    }

    return ret;
}

/* **************************************************************** */
void queryResource()
{
    /* switch(TestCase) */
    /* { */
    /*     case TEST_DISCOVER_REQ: */
    /*         break; */
    /*     case TEST_NON_CON_OP: */
    /*         InitGetRequest(g_qos); */
    /*         InitPutRequest(g_qos); */
    /*         InitPostRequest(g_qos); */
    /*         break; */
    /*     case TEST_CON_OP: */
    /*         InitGetRequest(OC_HIGH_QOS); */
    /*         InitPutRequest(OC_HIGH_QOS); */
    /*         InitPostRequest(OC_HIGH_QOS); */
    /*         break; */
    /*     default: */
    /*         PrintUsage(); */
    /*         break; */
    /* } */
}

void print_resource_list()
{
    struct ResourceNode * iter;
    iter = resourceList;
    OIC_LOG(INFO, TAG, "Resource List: ");
    /* FIXME: group by server id */
    OIC_LOG_V(INFO, TAG, "Server: %s",iter->sid);
    while(iter)
    {
        OIC_LOG_V(INFO, TAG, "\tresource address   = [%s]:%d%s", iter->dev_addr.addr, iter->dev_addr.port, iter->uri);
        /* OIC_LOG_V(INFO, TAG, "uri = %s", iter->uri); */
        /* OIC_LOG_V(INFO, TAG, "ip = %s", iter->dev_addr.addr); */
        /* OIC_LOG_V(INFO, TAG, "port = %d", iter->dev_addr.port); */
        /* switch (iter->dev_addr.adapter) */
        /* { */
        /*     case OC_ADAPTER_IP: */
        /*         OIC_LOG(INFO, TAG, "connType = Default (IP)"); */
        /*         break; */
        /*     case OC_ADAPTER_GATT_BTLE: */
        /*         OIC_LOG(INFO, TAG, "connType = BLE"); */
        /*         break; */
        /*     case OC_ADAPTER_RFCOMM_BTEDR: */
        /*         OIC_LOG(INFO, TAG, "connType = BT"); */
        /*         break; */
        /*     default: */
        /*         OIC_LOG(INFO, TAG, "connType = Invalid connType"); */
        /*         break; */
        /* } */
        iter = iter->next;
    }
    waiting = 0;		/* tell ui thread we're done */
}

/* This function searches for the resource(sid:uri) in the ResourceList.
 * If the Resource is found in the list then it returns 0 else insert
 * the resource to front of the list and returns 1.
 */
int insertResource(const char * sid, char const * uri,
            const OCClientResponse * client_response)
{
    struct ResourceNode * iter = resourceList;
    char * sid_cpy =  OICStrdup(sid);
    char * uri_cpy = OICStrdup(uri);

    //Checking if the resource(sid:uri) is new
    while(iter)
    {
        if((strcmp(iter->sid, sid) == 0) && (strcmp(iter->uri, uri) == 0))
        {
            OICFree(sid_cpy);
            OICFree(uri_cpy);
            return 0;
        }
        else
        {
            iter = iter->next;
        }
    }

    //Creating new ResourceNode
    if((iter = (struct ResourceNode *) OICMalloc(sizeof(struct ResourceNode))))
    {
        iter->sid = sid_cpy;
        iter->uri = uri_cpy;
        iter->dev_addr = client_response->devAddr;
        iter->next = NULL;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Memory not allocated to ResourceNode");
        OICFree(sid_cpy);
        OICFree(uri_cpy);
        return -1;
    }

    //Adding new ResourceNode to front of the ResourceList
    if(!resourceList)
    {
        resourceList = iter;
    }
    else
    {
        iter->next = resourceList;
        resourceList = iter;
    }
    return 1;
}

void collectUniqueResource(const OCClientResponse * client_response)
{
    OCDiscoveryPayload* pay = (OCDiscoveryPayload*) client_response->payload;
    OCResourcePayload* res = pay->resources;

    // Including the NUL terminator, length of UUID string of the form:
    //   "a62389f7-afde-00b6-cd3e-12b97d2fcf09"
#   define UUID_LENGTH 37

    char sidStr[UUID_LENGTH];

    while(res) {

        int ret = snprintf(sidStr, UUID_LENGTH,
                "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                pay->sid[0], pay->sid[1], pay->sid[2], pay->sid[3],
                pay->sid[4], pay->sid[5], pay->sid[6], pay->sid[7],
                pay->sid[8], pay->sid[9], pay->sid[10], pay->sid[11],
                pay->sid[12], pay->sid[13], pay->sid[14], pay->sid[15]
                );

        if (ret == UUID_LENGTH - 1)
        {
	    if ( strncmp(res->uri, RSC_URI_TEMPERATURE, strnlen(RSC_URI_TEMPERATURE, 14)) == 0 ) {
		printf("SAVING addr:  %s : %d\n", client_response->devAddr.addr,
		       res->port);
		       /* client_response->devAddr.port); */
		device_address = client_response->devAddr;
		device_address.port = res->port;
		conn_type      = client_response->connType;
	    }
            if(insertResource(sidStr, res->uri, client_response) == 1)
            {
                OIC_LOG_V(INFO,TAG,"%s%s%s%s",sidStr, ":", res->uri, " is new");
                /* print_resource_list(); */
                /* queryResource(); */
            }
            else {
                OIC_LOG_V(INFO,TAG,"%s%s%s%s",sidStr, ":", res->uri, " is old");
            }
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Could Not Retrieve the Server ID");
        }

        res = res->next;
    }
}

/* ****************************************************************
 *     Service Routines
 ****************************************************************/
OCStackApplicationResult svc_platform_discovery_response(void* ctx,
							 OCDoHandle handle,
							 OCClientResponse * client_response)
{
    OC_UNUSED(handle);
    if (ctx == (void*) DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "PLATFORM DISCOVERY callback context: success");
	print_client_response_header(client_response);
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "PLATFORM DISCOVERY callback context: failure (%x)", (unsigned int)ctx);
    }

    if (client_response)
    {
        OIC_LOG_PAYLOAD(INFO, client_response->payload);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "PLATFORM DISCOVERY client_response failure: %x", (unsigned int)client_response);
    }

    /* FIXME: this doesn't really work - we don't know how many responses to expect */
    waiting = 0;		/* tell ui thread we're done */
    /* return (UnicastDiscovery) ? OC_STACK_DELETE_TRANSACTION : OC_STACK_KEEP_TRANSACTION; */
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult svc_device_discovery_response(void* ctx,
						       OCDoHandle handle,
						       OCClientResponse * client_response)
{
    OC_UNUSED(handle);
    if (ctx == (void*) DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "DEVICE DISCOVERY callback context: success");
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "DEVICE DISCOVERY callback context: failure (%x)", (unsigned int)ctx);
    }

    if (client_response)
    {
        OIC_LOG(INFO, TAG, "DEVICE DISCOVERY response header:");
	print_client_response_header(client_response);
        OIC_LOG(INFO, TAG, "DEVICE DISCOVERY response payload:");
        OIC_LOG_PAYLOAD(INFO, client_response->payload);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "DEVICE DISCOVERY client_response failure: %x", (unsigned int)client_response);
    }

    waiting = 0;		/* tell ui thread we're done */
    /* return (UnicastDiscovery) ? OC_STACK_DELETE_TRANSACTION : OC_STACK_KEEP_TRANSACTION; */
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult svc_resource_discovery_response(void *ctx,
							 OCDoHandle handle,
							 OCClientResponse *client_response)
{
    OIC_LOG_V(DEBUG, TAG, "%s: ENTRY", __func__);
    OC_UNUSED(handle);
    if (ctx == (void*)DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "RESOURCE DISCOVERY callback context: success");
	print_client_response_header(client_response);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "RESOURCE DISCOVERY callback context: failure");
    }

    if (client_response)
    {
        OIC_LOG_PAYLOAD(INFO, client_response->payload);
        collectUniqueResource(client_response);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "RESOURCE DISCOVERY client_response failure: %x", (unsigned int)client_response);
    }

    waiting = 0;		/* tell ui thread we're done */
    /* return (UnicastDiscovery) ? OC_STACK_DELETE_TRANSACTION : OC_STACK_KEEP_TRANSACTION; */
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult svc_get_response(void* ctx,
					  OCDoHandle handle,
					  OCClientResponse* client_response)
{
    OIC_LOG_V(DEBUG, TAG, "%s: ENTRY", __func__);
    OC_UNUSED(handle);
    if (ctx == (void*)DEFAULT_CONTEXT_VALUE)
    {
	print_client_response_header(client_response);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s: RESOURCE GET callback context: failure", __func__);
    }

    if(client_response == NULL)
    {
        OIC_LOG(INFO, TAG, "RESOURCE GET client_response is NULL");
        return   OC_STACK_DELETE_TRANSACTION;
    }

    OIC_LOG(INFO, TAG, "GET Response payload:");
    OIC_LOG_PAYLOAD(INFO, client_response->payload);

    if(//GAR client_response->rcvdVendorSpecificHeaderOptions &&
            client_response->numRcvdVendorSpecificHeaderOptions > 0 )
    {
        OIC_LOG_V(INFO, TAG, "Received %d vendor specific options",
		  client_response->numRcvdVendorSpecificHeaderOptions);
        uint8_t i = 0;
        OCHeaderOption * rcvdOptions = client_response->rcvdVendorSpecificHeaderOptions;
        for( i = 0; i < client_response->numRcvdVendorSpecificHeaderOptions; i++)
        {
	    printf("Option %d: Protocol ID=%d, Option ID=%u\n", i,
		   ((OCHeaderOption)rcvdOptions[i]).protocolID,
		   ((OCHeaderOption)rcvdOptions[i]).optionID);
            if(((OCHeaderOption)rcvdOptions[i]).protocolID == OC_COAP_ID)
            {
                OIC_LOG_V(INFO, TAG, "Received option with OC_COAP_ID and ID %u with",
                        ((OCHeaderOption)rcvdOptions[i]).optionID );

                OIC_LOG_BUFFER(INFO, TAG, ((OCHeaderOption)rcvdOptions[i]).optionData,
                    MAX_HEADER_OPTION_DATA_LENGTH);
            }
        }
    }
    waiting = 0;		/* tell ui thread we're done */
    /* return (UnicastDiscovery) ? OC_STACK_DELETE_TRANSACTION : OC_STACK_KEEP_TRANSACTION; */
    return OC_STACK_DELETE_TRANSACTION;
}

/* ****************************************************************
 *        ACTIONS
 **************************************************************** */

int discover_platform(OCQualityOfService qos)
{
    OCStackResult ret;
    OCCallbackData cbData;
    char query_uri[64] = { 0 };

    snprintf(query_uri, sizeof (query_uri) - 1, RSC_URI_PLATFORM, discoveryAddr);

    cbData.cb = svc_platform_discovery_response;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    OIC_LOG_V(INFO, TAG, "Sending OC_REST_DISCOVER request with query uri: %s", query_uri);
    ret = OCDoResource(NULL, OC_REST_DISCOVER, query_uri, NULL, 0, CT_DEFAULT,
                       (qos == OC_HIGH_QOS) ? OC_HIGH_QOS : OC_LOW_QOS,
                       &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack device error");
    }
    waiting = 1;		/* tell ui we're awaiting a response */
    return ret;
}

int discover_device(OCQualityOfService qos)
{
    OCStackResult ret;
    OCCallbackData cbData;
    char query_uri[100] = { 0 };

    snprintf(query_uri, sizeof (query_uri) - 1, RSC_URI_DEVICE, discoveryAddr);

    cbData.cb = svc_device_discovery_response;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    OIC_LOG_V(INFO, TAG, "Sending OC_REST_DISCOVER request with query uri: %s", query_uri);
    ret = OCDoResource(NULL, OC_REST_DISCOVER, query_uri, NULL, 0, CT_DEFAULT,
                       (qos == OC_HIGH_QOS) ? OC_HIGH_QOS : OC_LOW_QOS,
                       &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack device error");
    }
    waiting = 1;		/* tell ui we're awaiting a response */
    return ret;
}

int discover_resources(OCQualityOfService qos)
{
    OC_UNUSED(qos);
    OCStackResult ret;
    OCCallbackData cbData;
    char query_uri[200];
    char ipaddr[100] = { '\0' };

    /* if (UnicastDiscovery) */
    /* { */
    /*     OIC_LOG(INFO, TAG, "Enter IP address (with optional port) of the Server hosting resource\n"); */
    /*     OIC_LOG(INFO, TAG, "IPv4: 192.168.0.15:45454\n"); */
    /*     OIC_LOG(INFO, TAG, "IPv6: [fe80::20c:29ff:fe1b:9c5]:45454\n"); */

    /*     if (fgets(ipaddr, sizeof (ipaddr), stdin)) */
    /*     { */
    /*         StripNewLineChar(ipaddr); //Strip newline char from ipaddr */
    /*     } */
    /*     else */
    /*     { */
    /*         OIC_LOG(ERROR, TAG, "!! Bad input for IP address. !!"); */
    /*         return OC_STACK_INVALID_PARAM; */
    /*     } */
    /* } */

    snprintf(query_uri, sizeof (query_uri), RSC_URI_RESOURCES, ipaddr);

    cbData.cb = svc_resource_discovery_response;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    OIC_LOG_V(INFO, TAG, "Sending OC_REST_DISCOVER request with query uri: %s", query_uri);
    ret = OCDoResource(NULL,
		       OC_REST_DISCOVER,
		       query_uri,
		       0,	/* device address */
		       0,	/* OCPayload */
		       CT_DEFAULT, OC_LOW_QOS,
		       &cbData,
		       NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }
    waiting = 1;		/* tell ui we're awaiting a response */
    return ret;
}

char* dev_addr_to_string(OCDevAddr* devaddr)
{
    return devaddr->addr;
}

int get_resource(OCQualityOfService qos)
{
    OCStackResult ret;
    char query_uri[200];
    /* char ipaddr[100] = { '\0' }; */

    snprintf(query_uri, sizeof (RSC_URI_TEMPERATURE), RSC_URI_TEMPERATURE);

    device_address.flags = (OCTransportFlags)(device_address.flags|OC_SECURE);

    OIC_LOG_V(INFO, TAG, "%s: Sending OC_REST_GET request to '%s:%d' with query uri : %s",
	      __func__, device_address.addr, device_address.port, query_uri);

    ret = send_oic_messsage(OC_REST_GET,
			    &device_address,
			    query_uri,
			    NULL, /* OCPayload* */
			    (qos == OC_HIGH_QOS)? OC_HIGH_QOS : OC_LOW_QOS,
			    svc_get_response);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack device error");
    }
    waiting = 1;		/* tell ui we're awaiting a response */
    return ret;
}

int get_pstat(OCQualityOfService qos)
{
    OCStackResult ret;
    char query_uri[200];
    /* char ipaddr[100] = { '\0' }; */

    snprintf(query_uri, sizeof (RSC_URI_PSTAT), RSC_URI_PSTAT);

    OIC_LOG_V (INFO, TAG, "Sending OC_REST_GET request to '%s' with query uri : %s",
	       dev_addr_to_string(&device_address),
	       query_uri);

    ret = send_oic_messsage(OC_REST_GET,
			    &device_address,
			    query_uri,
			    NULL, /* OCPayload* */
			    (qos == OC_HIGH_QOS)? OC_HIGH_QOS : OC_LOW_QOS,
			    svc_get_response);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack device error");
    }
    waiting = 1;		/* tell ui we're awaiting a response */
    return ret;
}

FILE* server_fopen(const char *path, const char *mode)
{
    OIC_LOG_V(DEBUG, TAG, "%s: ENTRY, path: %s", __func__, path);
    (void)path;
    OIC_LOG_V(DEBUG, TAG, "%s: opening %s", __func__, CRED_FILE);
    return fopen(CRED_FILE, mode);
}

void *prompt_user(void * arg)
{
    OC_UNUSED(arg);
    char action[1];
    struct timespec ts = { .tv_sec = 0,
			   .tv_nsec = 100000000L };

    while(!gQuitFlag) {
	if (waiting)
	    nanosleep(&ts, NULL);
	else {
	    printf("\nChoose an action:\n");
	    printf("\t1) Platform discovery\n\t2) Device discovery\n\t3) Resource discovery\n");
	    printf("\t4) List resources\n");
	    printf("\t5) GET /a/led\n");
	    printf("\t6) GET /a/temperature\n");
	    printf("\t7) GET /oic/sec/doxm\n");
	    printf("\t8) GET /oic/sec/pstat\n");
	    printf("\tq) Quit\n");
	    scanf("%s", (char*)&action);
	    switch(*action) {
	    case '1':
		discover_platform(g_qos);
		break;
	    case '2':
		discover_device(g_qos);
		break;
	    case '3':
		discover_resources(g_qos);
		break;
	    case '4':
		print_resource_list();
		break;
	    case '6':
		get_resource(g_qos);
		break;
	    case '7':
		get_pstat(g_qos);
		break;
	    case 'q':
		gQuitFlag = 1;
		fflush(stdout);
		break;
	    default:
		printf("Unrecognized action: %s\n", action);
	    }
	}
    }
    return NULL;
}

/****************************************************************
 *  MAIN
 *  1. initialize rmgr (stack)
 *  2. register platform info
 *  3. register device info
 *  4. register service routine for default rsvp
 *  5. enter processing loop
 ****************************************************************/
int main(int argc, char* argv[])
{
    OC_UNUSED(argc);
    OC_UNUSED(argv);

    /* int opt = 0; */
    /* while ((opt = getopt(argc, argv, "o:s:p:d:u:w:r:j:")) != -1) { */
    /*   switch(opt) { */
    /*   case 'o': */
    /* 	g_observe_notify_type = atoi(optarg); */
    /* 	break; */
    /*   default: */
    /* 	PrintUsage(); */
    /* 	return -1; */
    /*   } */
    /* } */

    /* if ((g_observe_notify_type != 0) && (g_observe_notify_type != 1)) { */
    /*     PrintUsage(); */
    /*     return -1; */
    /* } */

    OCStackResult op_result;

    /* optionally: set log level for 3rd-party libs */
    coap_set_log_level(LOG_DEBUG);
    dtls_set_log_level(DTLS_LOG_DEBUG);

    printf("Initializing persistent storage manager\n");
    // Initialize Persistent Storage for SVR database
    OCPersistentStorage ps = {server_fopen, fread, fwrite, fclose, unlink};
    OCRegisterPersistentStorageHandler(&ps);

    /* 1. initialize */
    /* use default transport flags, so stack will pick a transport */
    op_result = OCInit(NULL, 0, OC_CLIENT_SERVER);  /* , OC_DEFAULT_FLAGS, OC_DEFAULT_FLAGS); */
    if (op_result != OC_STACK_OK) {
        printf("OCStack init error\n");
        return 0;
    }

    printf("OCStack inititalized; SID: %s\n", OCGetServerInstanceIDString());

    /* 2. register platform info */
    /* WARNING: platform registration only allowed for servers */
    /* To enable the following, use OC_SERVER or OC_CLIENT_SERVER in the call to OCInit1 above */
    /* op_result = OCSetPlatformInfo(platform_info); */
    /* if (op_result != OC_STACK_OK) { */
    /*     printf("Platform Registration failed!\n"); */
    /*     exit (EXIT_FAILURE); */
    /* } */

    /* /\* 3. register default device info *\/ */
    /* /\* WARNING: device registration only allowed for servers *\/ */
    /* OCResourcePayloadAddStringLL(&device_info.types, "oic.d.tv"); */
    /* op_result = OCSetDeviceInfo(device_info); */
    /* if (op_result != OC_STACK_OK) { */
    /*     printf("Device Registration failed!\n"); */
    /*     exit (EXIT_FAILURE); */
    /* } */

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
    printf("Entering client main loop...\n");

    /* DeletePlatformInfo(); */
    /* DeleteDeviceInfo(); */

    signal(SIGINT, handleSigInt);

    pthread_create (&ui_thread, NULL, prompt_user, NULL);

    while (!gQuitFlag)
    {
        if (OCProcess() != OC_STACK_OK)
        {
            printf("OCStack process error\n");
            return 0;
        }
    }

    pthread_cancel(ui_thread);
    pthread_join(ui_thread, NULL);

    /* if (observeThreadStarted) */
    /* { */
    /*     pthread_cancel(threadId_observe); */
    /*     pthread_join(threadId_observe, NULL); */
    /* } */

    /* pthread_cancel(threadId_presence); */
    /* pthread_join(threadId_presence, NULL); */

    printf("Exiting client main loop...\n");

    if (OCStop() != OC_STACK_OK)
    {
        printf("OCStack stop error\n");
    }

    return 0;
}

