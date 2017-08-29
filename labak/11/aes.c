#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <cryptoki.h>
#include <ctextra.h>
#include <ctutil.h>
#include <ctvdef.h>
#include <genmacro.h>

CK_CHAR g_dataToEncrypt[] = "Testovacie data";
char USERPIN[5] = { '1','1','1','1',0};

#define CHECK_CK_RV_GOTO(rv, string, label)                 \
    if (rv != CKR_OK)                                       \
    {														\
        fprintf(stderr, "Error occured : %s\n", string);    \
        goto label;                                         \
    }

static CK_RV CreateSecretKeyObject(CK_SESSION_HANDLE hSession,
                                   CK_OBJECT_HANDLE_PTR phObject,
								   CK_CHAR *objLabel)
{
    CK_RV rv = CKR_OK;
	CK_MECHANISM mechanism = {CKM_AES_KEY_GEN};
	static CK_OBJECT_CLASS tKeyClass = CKO_SECRET_KEY;
    static CK_BBOOL ckTrue = TRUE;
    static CK_BBOOL ckFalse = FALSE;
	static CK_KEY_TYPE tKeyType = CKK_AES;
	static CK_ULONG ulKeyLen = 32;

    CK_ATTRIBUTE objectTemplate[] = 
    {
		{ CKA_CLASS, &tKeyClass, sizeof(tKeyClass ) },
		{ CKA_TOKEN, &ckTrue, sizeof(CK_BBOOL ) },
		{ CKA_PRIVATE, &ckTrue, sizeof(CK_BBOOL ) },
		{ CKA_LABEL, NULL, 0},
		{ CKA_MODIFIABLE, &ckFalse, sizeof(CK_BBOOL ) },
		{ CKA_KEY_TYPE, &tKeyType, sizeof(tKeyType ) },
		{ CKA_ENCRYPT, &ckTrue, sizeof(CK_BBOOL ) },
		{ CKA_DECRYPT, &ckTrue, sizeof(CK_BBOOL)},
		{ CKA_VALUE_LEN, &ulKeyLen, sizeof(ulKeyLen ) },
    };

    CK_SIZE objectSize = sizeof(objectTemplate) / sizeof(CK_ATTRIBUTE);

    CK_ATTRIBUTE* pAttr = NULL;

    /* Fill in the public key label */
    pAttr = FindAttribute(CKA_LABEL, objectTemplate, objectSize);
    pAttr->pValue = objLabel;
    pAttr->ulValueLen = (CK_ULONG)strlen((char*)objLabel);

    rv = C_GenerateKey(hSession, &mechanism, objectTemplate, objectSize, phObject);
    CHECK_CK_RV_GOTO(rv, "C_GenerateKey", end);

end:
    return rv;
}

CK_RV encryptData(CK_SESSION_HANDLE hSession,
                         CK_OBJECT_HANDLE hKey,
                         CK_CHAR* pData,
                         CK_SIZE dataLen,
                         CK_CHAR** ppEncData,
                         CK_SIZE* pEncDataLen)
{
    CK_RV rv = CKR_OK;
	static CK_CHAR iv[16] = {0};
	CK_MECHANISM mechanism = {CKM_AES_CBC_PAD, iv, 16};

    /* Initialise the encrypt operation */
    rv = C_EncryptInit(hSession, &mechanism, hKey);
    CHECK_CK_RV_GOTO(rv, "C_EncryptInit", end);

    /* Do a length prediction so we allocate enough memory for the ciphertext */
    rv = C_Encrypt(hSession, pData, dataLen, NULL, pEncDataLen);
    CHECK_CK_RV_GOTO(rv, "C_Encrypt 1", end);
    
    *ppEncData = (CK_CHAR*)malloc(*pEncDataLen);
    if (*ppEncData == NULL) return CKR_HOST_MEMORY;

    /* Do the proper encrypt */
    rv = C_Encrypt(hSession, pData, dataLen, *ppEncData, pEncDataLen);
    CHECK_CK_RV_GOTO(rv, "C_Encrypt 2", end);

end:

    return rv;
}

static CK_RV decryptData(CK_SESSION_HANDLE hSession,
                         CK_OBJECT_HANDLE hKey,
                         CK_CHAR* pEncData,
                         CK_SIZE encDataLen,
                         CK_CHAR** ppData,
                         CK_SIZE* pDataLen)
{

    CK_RV rv = CKR_OK;
	static CK_CHAR iv[16] = {0};
	CK_MECHANISM mechanism = {CKM_AES_CBC_PAD, iv, 16};

    /* Initialise the decrypt operation */
    rv = C_DecryptInit(hSession, &mechanism, hKey);
    CHECK_CK_RV_GOTO(rv, "C_DecryptInit", end);

    /* Length predication */
    rv = C_Decrypt(hSession, pEncData, encDataLen, NULL, pDataLen);
    CHECK_CK_RV_GOTO(rv, "C_Decrypt", end);

    *ppData = (CK_CHAR*)malloc(*pDataLen);
    if (*ppData == NULL) return CKR_HOST_MEMORY;

    /* Do the actual decrypt operation */
    rv = C_Decrypt(hSession, pEncData, encDataLen, *ppData, pDataLen);
    CHECK_CK_RV_GOTO(rv, "C_Decrypt", end);

end:

    return rv;
}

int main(void) 
{
	FILE *fd;
	CK_RV rv = CKR_OK;
    CK_SESSION_HANDLE hSession = CK_INVALID_HANDLE;
	CK_OBJECT_HANDLE hKey = CK_INVALID_HANDLE; 
    CK_SLOT_ID slotId = 0;
	CK_CHAR* pEncData = NULL;
    CK_SIZE encDataLen = 0;
	CK_CHAR* pData = NULL;
    CK_SIZE dataLen = 0;

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

	/* Create AES-256 key */
    printf("Creating AES CBC secret key ... ");
    rv = CreateSecretKeyObject(hSession, &hKey, "taes");
    CHECK_CK_RV_GOTO(rv, "CreateSecretKeyObject", end);
    printf("OK\n");

	/* Encrypt data */
    printf("Encrypting data with AES ... ");
    rv = encryptData(hSession, hKey, g_dataToEncrypt, (CK_SIZE)strlen((char*)g_dataToEncrypt) + 1, &pEncData, &encDataLen);
    CHECK_CK_RV_GOTO(rv, "encryptData", end);
	printf("OK\n");

	fd = fopen("test.txt.enc", "w");
	if (fd == NULL) {
		printf("Problem creating file");
	}
	fwrite(pEncData, 1, encDataLen, fd);
	fclose(fd);

	/* Decrypt data */
	printf("Decrypting data with AES ... ");
    rv = decryptData(hSession, hKey, pEncData, encDataLen, &pData, &dataLen);
    CHECK_CK_RV_GOTO(rv, "decryptData", end);
	printf("OK\n");

	fd = fopen("test.txt.plain", "w");
	if (fd == NULL) {
		printf("Problem creating file");
	}
	fwrite(pData, 1, dataLen, fd);
	fclose(fd);

	printf("Destrying the AES symmetric key ... ");
	rv = C_DestroyObject(hSession, hKey);
    CHECK_CK_RV_GOTO(rv, "DestroyObject", end);
	printf("OK\n");

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
		char ErrorString[100];
		C_ErrorString(rv,ErrorString,sizeof(ErrorString));
        fprintf(stderr,
                "Error performing create key operation : 0x%lx %s\n",
                rv, ErrorString);

        /*
         * Clean up... we don't care if there are any errors.
         */
        if (hSession != CK_INVALID_HANDLE) C_CloseSession(hSession);

        C_Finalize(NULL);
    }

	(void) getchar();
    return rv;
}
