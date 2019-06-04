#ifndef __PRIMUS_HELPERS_H__
#define __PRIMUS_HELPERS_H__

#include <pkcs11.h>
#include <stdbool.h>

#include "macrologger.h"

#define noop (void)0
#define is_ok(value) (value == CKR_OK)
#define continue_if_ok(value, message)\
if (!is_ok(value)) {\
    LOG_ERROR("failed [%ld]: %s", value, message);\
    return value;\
}

typedef void (*CK_OBJECT_HANDLER)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject);

/*
 * Loop through objects according to search template
 */
unsigned long primusLoopObjects(
    CK_SESSION_HANDLE hSession,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulAttrCount,
    CK_OBJECT_HANDLER handler
);

/*
 * Check if an object exists
 */
bool primusObjectExists(
    CK_SESSION_HANDLE hSession,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulAttrCount,
    bool * exists
);

#endif // PRIMUS_HELPERS_H
