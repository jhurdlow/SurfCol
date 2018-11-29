#include <unordered_set>
#include <string>
#include "lwmMesh.h"
#include "lwmCommand.h"
#include <lwsurf.h>
#include <lwmodeler.h>

// activate function
extern "C" {

XCALL_(int)
Activate(long version, GlobalFunc * global, void * local, void * user)
{
	LWStateQueryFuncs  *query = (LWStateQueryFuncs*)global(LWSTATEQUERYFUNCS_GLOBAL, GFUSE_TRANSIENT);
	LWSurfaceFuncs *surfFuncs = (LWSurfaceFuncs *)(*global) (LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT);
	lwmCommand* cmd = new lwmCommand((LWModCommand*)local);
	lwmMesh* mesh = new lwmMesh(cmd->EditBegin(OPSEL_GLOBAL));

	std::unordered_set<std::string> surfaces;

	const char*	objectName = query->object();
	
	if (objectName == nullptr)
		objectName = "Unnamed";

	LWSurfaceID* surfsArray = surfFuncs->byObject(objectName);  // get array of surface names for this object.
	int s = 0;
	LWSurfaceID thisSurf = surfsArray[s++];
	while (thisSurf != nullptr) {
		std::string name = surfFuncs->name(thisSurf);
		if (surfaces.find(name) == surfaces.end()) {
			surfaces.insert(name);
		}
		thisSurf = surfsArray[s++];
	}

	const int c = 3;
	std::string surfName[c] = { "RedSurf", "GreenSurf", "BlueSurf" };

	// Draw 3 polygons, each with a different surface and color
	for (int i = 0; i < c; i++) {
		// Create a new surface (if it doesn't exist)
		if (surfaces.find(surfName[i]) == surfaces.end()) {
			surfFuncs->create(objectName, surfName[i].c_str());
		}

		// Make it the current surface for our object.
		cmd->SurfSetSurf(surfName[i].c_str(), objectName);

		// Set the color of the current surface (our new one, if we created it)
		// Comment back in the conditional for this block to prevent us from resetting the color if the surface
		// already exists. When the surface already exists we see really strange behavior on subsequent runs,
		// such as getting the wrong colors on surfaces. Appears to be a bug in Modeler.
	///	if (surfaces.find(surfName[i]) == surfaces.end()) {
			float rgb[3] = { 0.5, 0.5, 0.5 };
			rgb[i] = 1.0;		// should make the first red, the second green, the third blue.
			cmd->SurfSetColor(rgb); // !! Modeler 2018 does not seem to actually set the surface color here... !!
									// Also, strangely the first surface gets a roughness value, where subsequent surfs don't.
	///	}

		// Draw a poly
		double rawPnts[4][3] = { {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 0.0} };
		LWPntID pntIds[4];

		for (int p = 0; p < 4; p++) {
			double* pnt = rawPnts[p];
			pnt[0] += (double)i;	// shift each poly in the x plane
			pntIds[p] = mesh->addPoint(pnt);
		}

		mesh->addPoly(LWPOLTYPE_FACE, surfName[i].c_str(), 4, pntIds);
	}

	mesh->done(EDERR_NONE, EDSELM_CLEARCURRENT);

	if (mesh) {
		delete (mesh);
	}

	if (cmd) {
		delete (cmd);
	}

	return AFUNC_OK;
}

#pragma optimize("", off)
void *
Startup(void)
{
	return ((void *)1);
}

void
Shutdown(void * user)
{
}

ServerTagInfo ServerTags[] = {
	{"SurfColsBug", SRVTAG_USERNAME},
	{"SurfColsBug", SRVTAG_BUTTONNAME},
	{"BugRepro", SRVTAG_CMDGROUP},
	{"Surface Colors SDK Bug Repro", SRVTAG_DESCRIPTION | LANGID_USENGLISH},
};

ServerRecord ServerDesc[] = {
	{ "CommandSequence", "SurfColsBug", Activate, ServerTags},
	{ NULL }
};
#pragma optimize("", on)
} // extern "C"