//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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
#include <stdlib.h>
#include <ocstack.h>
#include <getopt.h>

#include "logger.h"
#include "utils.h"

void print_client_response_header(OCClientResponse* client_response)
{
    char* TAG = "OCClientResponse log";
    OIC_LOG_V(INFO, TAG, "\taddr:      %s", client_response->devAddr.addr);
    OIC_LOG_V(INFO, TAG, "\tport:      %d", client_response->devAddr.port);
    OIC_LOG_V(INFO, TAG, "\turi:       %s", client_response->resourceUri);
    OIC_LOG_V(INFO, TAG, "\tprotocol:  %d", client_response->connType);
    OIC_LOG_V(INFO, TAG, "\tidentity:  %s", client_response->identity.id);
    OIC_LOG_V(INFO, TAG, "\tstack res: %s", getResult(client_response->result));
    OIC_LOG_V(INFO, TAG, "\tseq nbr:   %d", client_response->sequenceNumber);
    OIC_LOG_V(INFO, TAG, "\tvendor opts: %d", client_response->numRcvdVendorSpecificHeaderOptions);
}

void print_platform_info(OCPlatformInfo* pi)
{
    char* TAG = "OCPlatformInfo Log";
    OIC_LOG_V(INFO, TAG, "\tID:               %s", pi->platformID);
    OIC_LOG_V(INFO, TAG, "\tMfg name:         %s", pi->manufacturerName);
    OIC_LOG_V(INFO, TAG, "\tMfg URL:          %s", pi->manufacturerUrl);
    OIC_LOG_V(INFO, TAG, "\tMfg date:         %s", pi->dateOfManufacture);
    OIC_LOG_V(INFO, TAG, "\tModel nbr:        %s", pi->modelNumber);
    OIC_LOG_V(INFO, TAG, "\tPlatform version: %s", pi->platformVersion);
    OIC_LOG_V(INFO, TAG, "\tOS version:       %s", pi->operatingSystemVersion);
    OIC_LOG_V(INFO, TAG, "\tHW version:       %s", pi->hardwareVersion);
    OIC_LOG_V(INFO, TAG, "\tFirmware version: %s", pi->firmwareVersion);
    OIC_LOG_V(INFO, TAG, "\tSupport URL:      %s", pi->supportUrl);
    OIC_LOG_V(INFO, TAG, "\tSystem Time:      %s", pi->systemTime);
}

const char *getResult(OCStackResult result)
{
    switch (result)
    {
        case OC_STACK_OK:
            return "OC_STACK_OK";
        case OC_STACK_RESOURCE_CREATED:
            return "OC_STACK_RESOURCE_CREATED";
        case OC_STACK_RESOURCE_DELETED:
            return "OC_STACK_RESOURCE_DELETED";
        case OC_STACK_INVALID_URI:
            return "OC_STACK_INVALID_URI";
        case OC_STACK_INVALID_QUERY:
            return "OC_STACK_INVALID_QUERY";
        case OC_STACK_INVALID_IP:
            return "OC_STACK_INVALID_IP";
        case OC_STACK_INVALID_PORT:
            return "OC_STACK_INVALID_PORT";
        case OC_STACK_INVALID_CALLBACK:
            return "OC_STACK_INVALID_CALLBACK";
        case OC_STACK_INVALID_METHOD:
            return "OC_STACK_INVALID_METHOD";
        case OC_STACK_NO_MEMORY:
            return "OC_STACK_NO_MEMORY";
        case OC_STACK_COMM_ERROR:
            return "OC_STACK_COMM_ERROR";
        case OC_STACK_INVALID_PARAM:
            return "OC_STACK_INVALID_PARAM";
        case OC_STACK_NOTIMPL:
            return "OC_STACK_NOTIMPL";
        case OC_STACK_NO_RESOURCE:
            return "OC_STACK_NO_RESOURCE";
        case OC_STACK_RESOURCE_ERROR:
            return "OC_STACK_RESOURCE_ERROR";
        case OC_STACK_SLOW_RESOURCE:
            return "OC_STACK_SLOW_RESOURCE";
        case OC_STACK_NO_OBSERVERS:
            return "OC_STACK_NO_OBSERVERS";
        case OC_STACK_UNAUTHORIZED_REQ:
            return "OC_STACK_UNAUTHORIZED_REQ";
#ifdef WITH_PRESENCE
        case OC_STACK_PRESENCE_STOPPED:
            return "OC_STACK_PRESENCE_STOPPED";
        case OC_STACK_PRESENCE_TIMEOUT:
            return "OC_STACK_PRESENCE_TIMEOUT";
#endif
        case OC_STACK_ERROR:
            return "OC_STACK_ERROR";
        default:
            return "UNKNOWN";
    }
}

void StripNewLineChar(char* str)
{
    int i = 0;
    if (str)
    {
        while( str[i])
        {
            if (str[i] == '\n')
            {
                str[i] = '\0';
            }
            i++;
        }
    }
}

