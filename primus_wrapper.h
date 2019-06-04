#ifndef __PRIMUS_WRAPPER_H__
#define __PRIMUS_WRAPPER_H__

#include <pkcs11.h>

#ifdef __cplusplus
extern "C"{
#endif

/*
 * Login to primus device
 * (Opens a session)
 */
int primusLogin(
    CK_SESSION_HANDLE_PTR phSession,
    unsigned const char *pin,
    unsigned const long ulPinLen,
    CK_NOTIFY notify
);

/*
 * Login to primus device with default credentials
 */
int primusDefaultLogin(CK_SESSION_HANDLE_PTR phSession);

/*
 * Logout of primus device
 * (Closes session)
 */
int primusLogout(CK_SESSION_HANDLE hSession);

#ifdef __cplusplus
}
#endif

#endif // PRIMUS_SESSION_H
