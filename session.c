#include <stdio.h>
#include <stdlib.h>
#include <pkcs11.h>

#include "macrologger.h"
#include "primus_wrapper.h"
#include "secrets.h"
#include "config.h"
#include "helpers.h"

int primusLogin(
    CK_SESSION_HANDLE_PTR phSession,
    unsigned const char *pin,
    unsigned const long ulPinLen,
    CK_NOTIFY notify
) {
    continue_if_ok(C_InitializeWithLogFile(NULL, PRIMUS_CFG), "pkcs initialize");
    continue_if_ok(C_OpenSession(
        0,
        CKF_SERIAL_SESSION | CKF_RW_SESSION,
        NULL,
        notify,
        phSession
    ), "open session");
    continue_if_ok(C_Login(*phSession, CKU_USER, (CK_UTF8CHAR_PTR) pin, ulPinLen), "login");
    return CKR_OK;
}

unsigned long primusDoChallenge(
    unsigned const char *user,
    unsigned const long ulUserLen,
    unsigned const char *pass,
    unsigned const long ulPassLen,
    unsigned const char *pin,
    unsigned const long ulPinLen
) {
    unsigned long ulChallengeLen;
    continue_if_ok(C_GetUserChallenge(
        (unsigned char*) user,
        ulUserLen,
        (unsigned char*) pass,
        ulPassLen,
        (unsigned char*) pin,
        ulPinLen,
        NULL,
        &ulChallengeLen
    ), "get challenge");
#if LOG_LEVEL >= DEBUG_LEVEL
    unsigned char *challenge;
    challenge = (unsigned char*) malloc(ulChallengeLen);
    continue_if_ok(C_GetUserChallenge(
        (unsigned char*) user,
        ulUserLen,
        (unsigned char*) pass,
        ulPassLen,
        (unsigned char*) pin,
        ulPinLen,
        challenge,
        &ulChallengeLen
    ), "set challenge");

    int i, size = sizeof(challenge) / sizeof(char);
    char *buf_str = (char*) malloc(3 * size), *buf_ptr = buf_str;
    if (buf_str) {
        for (i = 0; i < size; i++)
            buf_ptr += sprintf(buf_ptr, i < size - 1 ? "%02X:" : "%02X\0", challenge[i]);
        LOG_DEBUG("received: %s", buf_str);
        free(buf_str);
    }
#endif
    return CKR_OK;
}

int primusDefaultLogin(CK_SESSION_HANDLE_PTR phSession) {
    return primusLogin(
        phSession,
        (unsigned char *) PRIMUS_SECRET_PIN,
        PRIMUS_SECRET_PIN_LEN,
        NULL
    );

}

int primusLogout(CK_SESSION_HANDLE hSession) {
    continue_if_ok(C_Logout(hSession), "logout");
    continue_if_ok(C_CloseSession(hSession), "session");
    continue_if_ok(C_Finalize(NULL), "pkcs finalize");
    return CKR_OK;
}

#ifdef UNIT_TESTS
#include "CuTest.h"

void TestPrimusLogin(CuTest* tc) {
    CK_SESSION_HANDLE hSession;

/*
    CuAssertIntEquals(tc, CKR_OK, primusLogin(
        &hSession,
        (unsigned char *) PRIMUS_SECRET_PIN,
        PRIMUS_SECRET_PIN_LEN,
        NULL
    ));
    CuAssertIntEquals(tc, CKR_OK, primusDoChallenge(
        (unsigned char *) PRIMUS_SECRET_USERNAME,
        PRIMUS_SECRET_USERNAME_LEN,
        (unsigned char *) PRIMUS_SECRET_PASSWORD,
        PRIMUS_SECRET_PASSWORD_LEN,
        (unsigned char *) PRIMUS_SECRET_PIN,
        PRIMUS_SECRET_PIN_LEN
    ));
    CuAssertIntEquals(tc, CKR_OK, primusLogout(hSession));
*/
    CuAssertIntEquals(tc, CKR_OK, primusDefaultLogin(&hSession));
    CuAssertIntEquals(tc, CKR_OK, primusLogout(hSession));
}

#endif // UNIT_TESTS
