#pragma once

extern "C" {
 #include "lwmeshedt.h"
}

class lwmMesh {
private:
	MeshEditOp* op;
	void*		vmapID;

public:
	lwmMesh(MeshEditOp* o) { op = o; }

	void done(EDError err, int selm)
	{
		(*op->done)(op->state, err, selm);
	}

	LWPntID addPoint(double* pt)
	{
		return (*op->addPoint)(op->state, pt);
	}
	LWPolID addPoly(LWID type, const char *surfacename, int vcount, const LWPntID *vlist, LWPolID id = nullptr)
	{
		return (*op->addPoly)(op->state, type, id, surfacename, vcount, vlist);
	}
};