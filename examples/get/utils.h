#ifndef UTILS_H_
#define UTILS_H_

/* %s will be replaced by IP address */
static const char *RSC_URI_PLATFORM  = "%s/oic/p";
static const char *RSC_URI_DEVICE    = "%s/oic/d";
static const char *RSC_URI_RESOURCES = "%s/oic/res";

#define RSC_URI_PSTAT "/oic/sec/pstat"

void print_client_response_header(OCClientResponse* client_response);

void print_platform_info(OCPlatformInfo* pi);

const char *getResult(OCStackResult result);

void StripNewLineChar(char* str);

#endif
