#pragma once

extern "C" {
 #include "lwmodeler.h"
 #include "lwsurf.h"
 #include "lwcmdseq.h"
}

enum ModelerCommands {
	mcMAKEBALL,
	mcSURFSETCOLOR,
	mcSURFSETSURF,
	mcLastCommand
};

class lwmCommand {
private:
	LWModCommand* mc;
	int          cc[mcLastCommand];
	EltOpSelect  mode;
	DynaValue	 dval[10];
	DynaValue    dres;

public:
	lwmCommand(LWModCommand* m) {
		mc = m;
		mode = OPSEL_GLOBAL;

		cc[mcMAKEBALL] = (*mc->lookup)(mc->data, "makeball");
		cc[mcSURFSETCOLOR] = (*mc->lookup)(mc->data, "Surf_SetColor");
		cc[mcSURFSETSURF] = (*mc->lookup)(mc->data, "Surf_SetSurf");
	}

	// Surfacing Commands (documented in Lightwave/SDK/Docs/commands/common.html)
	int SurfSetColor(float* rgb) {
		int result = 0;
		if (cc[mcSURFSETCOLOR]) {
			dval[0].type = DY_STRING;
			dval[0].str.buf = (char*)SURF_COLR;
			dval[0].str.bufLen = 0;
			dval[1].type = DY_FLOAT;
			dval[1].flt.value = rgb[0];
			dval[2].type = DY_FLOAT;
			dval[2].flt.value = rgb[1];
			dval[3].type = DY_FLOAT;
			dval[3].flt.value = rgb[2];
			result = (*mc->execute)(mc->data, cc[mcSURFSETCOLOR], 4, dval, mode, &dres);
		}
		return result;
	}
	int SurfSetSurf(const char* surfname, const char* objname) {
		int result = 0;
		if (cc[mcSURFSETSURF]) {
			dval[0].type = DY_STRING;
			dval[0].str.buf = (char*)surfname;
			dval[0].str.bufLen = 0;
			dval[1].type = DY_STRING;
			dval[1].str.buf = (char*)objname;
			dval[1].str.bufLen = 0;
			result = (*mc->execute)(mc->data, cc[mcSURFSETSURF], 2, dval, mode, &dres);
		}
		return result;
	}

	MeshEditOp* EditBegin(EltOpSelect op = OPSEL_DIRECT)
	{
		return (*mc->editBegin) (0, 0, op);
	}
};