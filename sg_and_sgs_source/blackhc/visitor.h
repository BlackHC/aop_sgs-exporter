// visitor object to simplify the export process [5/20/2012 kirschan2]
#pragma once

class IStorm3D_Model;

struct Visitor {
	// instance model
	virtual void visitObjectInstance( IStorm3D_Model &model ) {}
	// base model
	virtual void visitObjectModel( IStorm3D_Model &model ) {}

	virtual void visitHeightmap( const std::vector<unsigned short> &map, const VC2I &mapSize, const VC3 &realSize ) {}
	virtual void visitColormap( const std::vector<unsigned char> &rgbData, VC2I size ) {}

	virtual void visitTerrainObjectName( const std::string &name ) {}

	virtual bool visitNeedColormap() { return false; }
};