#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pkcs11.h>

#include "config.h"

typedef void (*CK_OBJECT_HANDLER)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject);

int primus_login(
    CK_SESSION_HANDLE_PTR phSession,
    unsigned const char *pin,
    unsigned const long ulPinLen,
    CK_NOTIFY notify
) {
    return C_InitializeWithLogFile(NULL, PRIMUS_CFG) &&
        C_OpenSession(
            0,
            CKF_SERIAL_SESSION | CKF_RW_SESSION,
            NULL,
            notify,
            phSession
        ) &&
        C_Login(*phSession, CKU_USER, (CK_UTF8CHAR_PTR) pin, ulPinLen);
}

int primus_logout(
    CK_SESSION_HANDLE hSession
) {
    return C_Logout(hSession) &&
        C_CloseSession(hSession) &&
        C_Finalize(NULL);
}

#ifdef UNIT_TESTS
#include "CuTest.h"
#include "macrologger.h"
#include "secrets.h"

void TestPrimusLogin(CuTest* tc) {
    CK_SESSION_HANDLE hSession;

    CuAssertIntEquals(
        tc,
        primus_login(&hSession, (unsigned char *) SECRET_PIN, SECRET_PIN_LEN, NULL),
        CKR_OK
    );
    CuAssertIntEquals(tc, primus_logout(hSession), CKR_OK);
}

#endif // UNIT_TESTS
