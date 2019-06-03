#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pkcs11.h>

#include "macrologger.h"

typedef void (*CK_OBJECT_HANDLER)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject);

void notify(CK_SESSION_HANDLE session, CK_NOTIFICATION event, CK_VOID_PTR app_ptr) {
    printf("happens");
}

void login(
    CK_SESSION_HANDLE_PTR phSession,
    unsigned const char *pin,
    unsigned const long ulPinLen
) {
    int rv;
    /* rv = C_Initialize(NULL); */
    /* rv = C_InitializeOpenSSL("/usr/local/primus/etc/primus.cfg"); */
    rv = C_InitializeWithLogFile(NULL, "/usr/local/primus/etc/primus.cfg");
    assert(rv == CKR_OK && "initialize");

    rv = C_OpenSession(
        0,
        CKF_SERIAL_SESSION | CKF_RW_SESSION,
        NULL,
        (CK_NOTIFY) notify,
        phSession
    );
    assert(rv == CKR_OK && "open session");

    rv = C_Login(*phSession, CKU_USER, (CK_UTF8CHAR_PTR) pin, ulPinLen);
    assert(rv == CKR_OK && "login");
}

void logout(
    CK_SESSION_HANDLE hSession
) {
    int rv = 0;
    rv |= C_Logout(hSession);
    rv |= C_CloseSession(hSession);
    rv |= C_Finalize(NULL);
    assert(rv == CKR_OK && "logout");
}

#ifdef UNIT_TESTS
#include "CuTest.h"

#include "secrets.h"

void TestLogin(CuTest* tc) {
    CK_SESSION_HANDLE hSession;

    login(&hSession, (unsigned char *) SECRET_PIN, SECRET_PIN_LEN);
    LOG_DEBUG("logged in");
    logout(hSession);
}

#endif // UNIT_TESTS
