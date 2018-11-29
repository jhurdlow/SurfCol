/*
 * LWSDK Header File
 * Copyright 2005, NewTek, Inc.
 *
 * LWCAMERA.H -- LightWave Camera
 */
#ifndef LWSDK_CAMERA_H
#define LWSDK_CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lwhandler.h>
#include <lwrender.h>


#define LWCAMERA_HCLASS "CameraHandler"
#define LWCAMERA_ICLASS "CameraInterface"
#define LWCAMERA_VERSION 1

typedef struct st_LWCameraAccess
{
	LWItemID cameraID;
	LWDVector worldPos;
	LWDVector toWorld[3];
	LWDVector toCamera[3];
	double hither, yon;
	double filmSize[2];
} LWCameraAccess;

typedef struct st_LWCameraHandler
{
	LWInstanceFuncs* inst;
	LWItemFuncs* item;
	LWRenderFuncs* rend;
	LWError (*newPass)(LWInstance, unsigned int pass, unsigned int maxnumpasses, const LWCameraAccess* camaccess);
	int (*preview)(LWInstance, LWDMatrix4 projection, const LWCameraAccess* camaccess);
	int (*evaluate)(LWInstance, double fpx, double fpy, LWCameraRay* ray, const LWCameraAccess* camaccess);
	unsigned int (*flags)(LWInstance);
} LWCameraHandler;


#define LWCAMTYPEF_SUPPORTSDOF (1 << 0)


#ifdef __cplusplus
}
#endif

#endif
