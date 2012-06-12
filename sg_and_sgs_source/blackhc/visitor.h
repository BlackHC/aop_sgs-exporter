// visitor object to simplify the export process [5/20/2012 kirschan2]
#pragma once

class IStorm3D_Model;

struct Visitor {
	virtual void visit( IStorm3D_Model &model ) {}
	virtual void heightmap( const std::vector<unsigned short> map, const VC2I &mapSize, const VC3 &realSize ) {}
	virtual void colormap( const std::vector<unsigned char> rgbData, VC2I size ) {}

	virtual void terrainObjectName( const std::string &name ) {}

	virtual bool needColormap() { return false; }
};