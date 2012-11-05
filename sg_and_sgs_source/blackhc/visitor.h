// visitor object to simplify the export process [5/20/2012 kirschan2]
#pragma once

class IStorm3D_Model;
class IStorm3D_Texture;

struct Visitor {
	// instance model
	virtual void visitObjectInstance( bool terrainBuilding, IStorm3D_Model &model ) {}
	// base model
	virtual void visitObjectModel( bool terrainBuilding, const std::string &name, IStorm3D_Model &model ) {}

	virtual void visitColormap( const std::vector<unsigned char> &rgbData, VC2I size ) {}

	virtual void visitTerrainObjectName( const std::string &name ) {}

	virtual bool visitNeedColormap() { return false; }

	// terrain export
	virtual void visitHeightmap( const std::vector<unsigned short> &map, const VC2I &mapSize, const VC3 &realSize ) {}
	// visit in ascending textureIndex order
	virtual void visitTerrainTexture( IStorm3D_Texture *texture ) {}
	virtual void visitTerrainBlendingSize( const VC2I &blendSize ) {}
	// visit in ascending textureIndex order
	virtual void visitTerrainBlending( const std::vector< unsigned char > &weights ) {}
};