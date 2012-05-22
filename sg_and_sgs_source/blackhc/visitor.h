// visitor object to simplify the export process [5/20/2012 kirschan2]
#pragma once

class IStorm3D_Model;

struct Visitor {
	virtual void visit( IStorm3D_Model &model ) = 0;
	virtual void heightmap( const std::vector<unsigned short> heightmap, const VC2I &mapSize, const VC3 &realSize ) = 0;
};