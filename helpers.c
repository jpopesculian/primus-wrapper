#include <stdbool.h>
#include <stdio.h>
#include <pkcs11.h>

#include "macrologger.h"
#include "helpers.h"

unsigned long primusLoopObjects(
    CK_SESSION_HANDLE hSession,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulAttrCount,
    CK_OBJECT_HANDLER handler
) {
    continue_if_ok(C_FindObjectsInit(hSession, pTemplate, ulAttrCount), "find objects init");

    CK_ULONG ulObjectCount;
    CK_OBJECT_HANDLE hObject;
    unsigned long rv = CKR_OK;
    while (1) {
        rv |= C_FindObjects(hSession, &hObject, 1, &ulObjectCount);
        if (!is_ok(rv)) break;
        if (ulObjectCount < 1) break;

        handler(hSession, hObject);
    }
    continue_if_ok(rv, "looping objects");
    continue_if_ok(C_FindObjectsFinal(hSession), "finalizing loop");
    return CKR_OK;
}

bool primusObjectExists(
    CK_SESSION_HANDLE hSession,
    CK_ATTRIBUTE_PTR pTemplate,
    CK_ULONG ulAttrCount,
    bool * exists
) {
    continue_if_ok(C_FindObjectsInit(hSession, pTemplate, ulAttrCount), "find objects init");

    CK_ULONG ulObjectCount;
    CK_OBJECT_HANDLE hObject;
    unsigned long rv = CKR_OK;
    while (1) {
        rv |= C_FindObjects(hSession, &hObject, 1, &ulObjectCount);
        if (!is_ok(rv)) break;
        if (ulObjectCount < 1) break;
        *exists = true;
        break;
    }
    continue_if_ok(rv, "looping objects");
    continue_if_ok(C_FindObjectsFinal(hSession), "finalizing loop");
    return CKR_OK;
}
