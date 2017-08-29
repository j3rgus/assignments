#include <stdio.h>
#include <string.h>
#include <cryptoki.h>
#include <ctextra.h>
#include <ctutil.h>

char USERPIN[5] = { '1','1','1','1',0};

#define CHECK_CK_RV_GOTO(rv, string, label)                 \
    if (rv != CKR_OK)                                       \
    {                                                       \
        fprintf(stderr, "Error occured : %s\n", string);    \
        goto label;                                         \
    }

int main(void) 
{
	CK_RV rv = CKR_OK;
    CK_SESSION_HANDLE hSession = CK_INVALID_HANDLE;

    CK_SLOT_ID slotId = 0;

    /* Initialise the cryptoki API */
	printf( "Initializing Cryptoki API ... " );
    rv = C_Initialize(NULL);
    CHECK_CK_RV_GOTO(rv, "C_Initialize", end);
	printf( "OK\n" );

	/* Check PKCS#11 version */
	printf( "Checking Cryptoki API version ... " );
	rv = CheckCryptokiVersion();
	CHECK_CK_RV_GOTO(rv, "Incompatible PKCS#11 version", end);
	printf( "OK\n" );

    /* Obtain a session so we can perform cryptoki operations */
	printf( "Obtaining a session ... " );
    rv = C_OpenSession(slotId, CKF_RW_SESSION, NULL, NULL, &hSession);
    CHECK_CK_RV_GOTO(rv, "C_OpenSession", end);
	printf( "OK\n" );

	/* Login as a user with a PIN */
	printf( "Logining as a user with a PIN ... " );
	rv = C_Login(hSession, CKU_USER, (CK_CHAR_PTR)USERPIN, (CK_SIZE) strlen((char*)USERPIN));
	CHECK_CK_RV_GOTO(rv, "C_Login as User", end);
	printf( "OK\n" );

    /* We've finished our work, close the session */
	printf( "Closing the session ... " );
    rv = C_CloseSession(hSession);
    CHECK_CK_RV_GOTO(rv, "C_CloseSession", end);
	printf("OK\n");

    /* We no longer need the cryptoki API ... */
	printf( "Finilizing the cryptoki ... " );
    rv = C_Finalize(NULL);
    CHECK_CK_RV_GOTO(rv, "C_Finalize", end);
	printf("OK\n");

end:

    if (rv != CKR_OK)
    {
        fprintf(stderr,
                "Error performing create key operation : 0x%lx\n",
                rv);

        /*
         * Clean up... we don't care if there are any errors.
         */
        if (hSession != CK_INVALID_HANDLE) C_CloseSession(hSession);

        C_Finalize(NULL);
    }

    return rv;
}
