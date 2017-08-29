/******************************************************
 *                                                    *
 *                  GrFinger 4.2 SDK                  *
 *       GrFinger Biometric Fingerprint Library       *
 *                                                    *
 * GrFinger.h                                         *
 * C++ Header file for GrFinger Library               *
 *                                                    *
 * Copyright (c) 2004-2006 Griaule Tecnologia LTDA.   *
 *                                                    *
 * Last Modified: 2006-05-23 Revision: 3              *
 *                                                    *
 ******************************************************/

#ifndef IMPORTDLL_H
#define IMPORTDLL_H
#include "windows.h"

//*****************************************************
//RETURN CODES
//*****************************************************
//success
#define GR_OK                                         0
#define GR_BAD_QUALITY                                0
#define GR_MEDIUM_QUALITY                             1
#define GR_HIGH_QUALITY                               2
#define GR_MATCH                                      1
#define GR_NOT_MATCH                                  0
#define GR_DEFAULT_USED                               3
//initialization errors codes
#define GR_ERROR_INITIALIZE_FAIL                     -1
#define GR_ERROR_NOT_INITIALIZED                     -2
#define GR_ERROR_FAIL_LICENSE_READ                   -3
#define GR_ERROR_NO_VALID_LICENSE                    -4
#define GR_ERROR_NULL_ARGUMENT                       -5
#define GR_ERROR_FAIL                                -6
#define GR_ERROR_ALLOC                               -7
#define GR_ERROR_PARAMETERS                          -8
//extract and match errors codes
#define GR_ERROR_WRONG_USE                         -107
#define GR_ERROR_EXTRACT                           -108
#define GR_ERROR_SIZE_OFF_RANGE                    -109
#define GR_ERROR_RES_OFF_RANGE                     -110
#define GR_ERROR_CONTEXT_NOT_CREATED               -111
#define GR_ERROR_INVALID_CONTEXT                   -112
#define GR_ERROR_ERROR                             -113
#define GR_ERROR_NOT_ENOUGH_SPACE                  -114
//capture error codes
#define GR_ERROR_CONNECT_SENSOR                    -201
#define GR_ERROR_CAPTURING                         -202
#define GR_ERROR_CANCEL_CAPTURING                  -203
#define GR_ERROR_INVALID_ID_SENSOR                 -204
#define GR_ERROR_SENSOR_NOT_CAPTURING              -205
#define GR_ERROR_INVALID_EXT                       -206
#define GR_ERROR_INVALID_FILENAME                  -207
#define GR_ERROR_INVALID_FILETYPE                  -208
#define GR_ERROR_SENSOR                            -209

//*****************************************************
//CONSTS
//*****************************************************
//file format codes
#define GRCAP_IMAGE_FORMAT_BMP                      501
//event values codes
#define GR_PLUG                                      21
#define GR_UNPLUG                                    20
#define GR_FINGER_DOWN                               11
#define GR_FINGER_UP                                 10
#define GR_IMAGE                                     30
//image consts 
#define GR_DEFAULT_RES                              500
#define GR_DEFAULT_DIM                              500
#define GR_MAX_SIZE_TEMPLATE                      10000
#define GR_MAX_IMAGE_WIDTH                         1280
#define GR_MAX_IMAGE_HEIGHT                        1280
#define GR_MAX_RESOLUTION                          1000 /*DPI*/
#define GR_MIN_IMAGE_WIDTH                           50
#define GR_MIN_IMAGE_HEIGHT                          50
#define GR_MIN_RESOLUTION                           125 /*DPI*/
//match const
#define GR_MAX_THRESHOLD                            200
#define GR_MIN_THRESHOLD                             10
#define GR_VERYLOW_FRR                               30 /*FAR 1 IN 1000*/
#define GR_LOW_FRR                                   45 /*FAR 1 IN 10000*/
#define GR_LOW_FAR                                   60 /*FAR 1 IN 30000*/
#define GR_VERYLOW_FAR                               80 /*FAR 1 IN 300000*/
#define GR_ROT_MIN                                    0
#define GR_ROT_MAX                                  180
//context const
#define GR_DEFAULT_CONTEXT                            0
#define GR_NO_CONTEXT                                -1
//colors for BiometricDisplay
#define GR_IMAGE_NO_COLOR                    0x1FFFFFFF
//version constants
#define GRFINGER_FULL                                 1
#define GRFINGER_LIGHT                                2
#define GRFINGER_FREE                                 3

//*****************************************************
//TYPE DEFINITIONS
//*****************************************************
#define DLLIMPORT extern "C" __declspec(dllimport) int __stdcall
#define CALLBACK __stdcall
typedef int GRCAP_STATUS_EVENTS;
typedef int GRCAP_FINGER_EVENTS;
typedef int GRCAP_IMAGE_FORMAT;
typedef long BITMAP_HANDLE;
//CALLBACK functions
typedef void CALLBACK GRCAP_STATUS_EVENT_PROC(char* idSensor, GRCAP_STATUS_EVENTS event);
typedef void CALLBACK GRCAP_FINGER_EVENT_PROC(char* idSensor, GRCAP_FINGER_EVENTS event);
typedef void CALLBACK GRCAP_IMAGE_EVENT_PROC(char* idSensor, unsigned int width, unsigned int height, unsigned char* rawImage, int res);

//*****************************************************
// Functions - CAPTURE
//*****************************************************
DLLIMPORT
GrCapInitialize(GRCAP_STATUS_EVENT_PROC *StatusEventHandler);

DLLIMPORT
GrCapFinalize();

DLLIMPORT
GrCapStartCapture(char* idSensor, GRCAP_FINGER_EVENT_PROC *FingerEventHandler, GRCAP_IMAGE_EVENT_PROC *ImageEventHandler);

DLLIMPORT
GrCapStopCapture(char* idSensor);

DLLIMPORT
GrCapSaveRawImageToFile(unsigned char* rawImage, unsigned int width, unsigned int height, char* fileName, GRCAP_IMAGE_FORMAT imageFormat);

DLLIMPORT
GrCapLoadImageFromFile(char* fileName, int res);

DLLIMPORT
GrCapRawImageToHandle(unsigned char* rawImage, unsigned int width, unsigned int height, HDC hdc, HBITMAP &handle);

DLLIMPORT
GrBiometricDisplay(char* tptQuery, unsigned char* rawImage, int width, int height, int res, HDC hdc, HBITMAP &handle, int matchContext);

//*****************************************************
// Functions - EXTRACT
//*****************************************************
DLLIMPORT
GrExtract(unsigned char *rawimage, int width, int height, int res, char *tpt, int *tptSize, int context);

//*****************************************************
// Functions - MATCH
//*****************************************************
DLLIMPORT
GrInitialize();

DLLIMPORT
GrFinalize();

DLLIMPORT
GrCreateContext (int *contextId);

DLLIMPORT
GrDestroyContext (int contextId);

DLLIMPORT
GrVerify (char *queryTemplate, char *referenceTemplate, int *verifyScore, int context);

DLLIMPORT
GrIdentifyPrepare (char *templateQuery, int context);

DLLIMPORT
GrIdentify (char *templateReference, int *identifyScore, int context);

//*****************************************************
// Functions - CONFIG
//*****************************************************
DLLIMPORT
GrSetIdentifyParameters (int identifyThreshold, int identifyRotationTolerance, int context);

DLLIMPORT
GrSetVerifyParameters (int verifyThreshold, int verifyRotationTolerance, int context);

DLLIMPORT
GrGetIdentifyParameters(int &identifyThreshold, int &identifyRotationTolerance, int context);

DLLIMPORT
GrGetVerifyParameters(int &verifyThreshold, int &verifyRotationTolerance, int context);

DLLIMPORT
GrSetBiometricDisplayColors(int minutiaeColor, int minutiaeMatchedColor, int segmentColor, int segmentMatchedColor, int directionColor, int directionMatchedColor);

DLLIMPORT
GrGetGrFingerVersion(unsigned char *majorRelease, unsigned char *minorRelease);

#endif
