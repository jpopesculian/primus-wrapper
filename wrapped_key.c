
#ifdef UNIT_TESTS
#include <stdio.h>

#include "CuTest.h"
#include "primus_wrapper.h"
#include "helpers.h"
#include "macrologger.h"

void handleObj(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject) {
    LOG_DEBUG("happens");
}

void _TestWrappedKeyCreate(CuTest* tc) {
    CK_SESSION_HANDLE hSession;

    CuAssertIntEquals(tc, primusDefaultLogin(&hSession), CKR_OK);
    primusLoopObjects(hSession, NULL, 0, handleObj);
    bool exists = false;
    primusObjectExists(hSession, NULL, 0, &exists);
    if (exists) {
        LOG_DEBUG("Object exists!");
    }
    CuAssertIntEquals(tc, primusLogout(hSession), CKR_OK);
}

#endif // UNIT_TESTS
