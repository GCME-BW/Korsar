/* Automatically generated nanopb header */
/* Generated by nanopb-1.0.0-dev */

#ifndef PB_WIFI_SETTINGS_PB_H_INCLUDED
#define PB_WIFI_SETTINGS_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _WifiMode {
    WifiMode_STA = 0,
    WifiMode_SoftAP = 1
} WifiMode;

/* Struct definitions */
typedef struct _WifiSettings {
    WifiMode mode;
    char mdns_domain[64];
    char softap_ssid[64];
    char softap_password[64];
} WifiSettings;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _WifiMode_MIN WifiMode_STA
#define _WifiMode_MAX WifiMode_SoftAP
#define _WifiMode_ARRAYSIZE ((WifiMode)(WifiMode_SoftAP+1))

#define WifiSettings_mode_ENUMTYPE WifiMode


/* Initializer values for message structs */
#define WifiSettings_init_default                {_WifiMode_MIN, "", "", ""}
#define WifiSettings_init_zero                   {_WifiMode_MIN, "", "", ""}

/* Field tags (for use in manual encoding/decoding) */
#define WifiSettings_mode_tag                    1
#define WifiSettings_mdns_domain_tag             2
#define WifiSettings_softap_ssid_tag             3
#define WifiSettings_softap_password_tag         4

/* Struct field encoding specification for nanopb */
#define WifiSettings_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    mode,              1) \
X(a, STATIC,   SINGULAR, STRING,   mdns_domain,       2) \
X(a, STATIC,   SINGULAR, STRING,   softap_ssid,       3) \
X(a, STATIC,   SINGULAR, STRING,   softap_password,   4)
#define WifiSettings_CALLBACK NULL
#define WifiSettings_DEFAULT NULL

extern const pb_msgdesc_t WifiSettings_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define WifiSettings_fields &WifiSettings_msg

/* Maximum encoded size of messages (where known) */
#define WIFI_SETTINGS_PB_H_MAX_SIZE              WifiSettings_size
#define WifiSettings_size                        197

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
