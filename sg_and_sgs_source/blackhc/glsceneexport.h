// header file for the obj exporter [5/19/2012 kirschan2]
#pragma once

#include <vector>
#include <string>
#include <map>

#include "../system/Logger.h"
#include "IStorm3D_Mesh.h"
#include "IStorm3D_Model.h"
#include "c2_vectors.h"

#include "visitor.h"
#include <fstream>
#include <unordered_set>
#include <boost/algorithm/string.hpp>

#include "gl/GL.h"

#pragma once
#undef min
#undef max
#include "serializer/serializer.h"
#include "serializer/serializer_std.h"

#include "sgsScene/sgsScene.h"

#include <stdio.h>

namespace blackhc {
	typedef std::vector<IStorm3D_Model> ModelVector;

	inline int iceil( int a, int b ) {
		return (a + b - 1) / b;
	}
	
	struct GLSceneExporter : Visitor {
		std::string filepath;

		int currentModelId;

		SGSScene scene;
		std::map< std::string, int > textureNameIdMap;
		std::map< std::string, int > modelNameObjectIdMap;
		// terrain texture index -> my texture id
		std::vector< int > terrainTextureIds;

		GLSceneExporter( const std::string &filepath ) {
			this->filepath = filepath;

			const int initalTotalSize = 4 << 20;
			scene.vertices.reserve( initalTotalSize );
			scene.indices.reserve( initalTotalSize );
		}

		void visitObjectInstance( IStorm3D_Model &model ) {
			exportModel( model );
		}

		void visitHeightmap( const std::vector<unsigned short> &map, const VC2I &mapSize, const VC3 &realSize ) {
			exportTerrainHeightmap( map, mapSize, realSize );
		}

		void visitTerrainObjectName( const std::string &name ) { exportModelType( name ); }

		bool visitNeedColormap() { return false; }

		void setColor3ubFromCOL( SGSScene::Color3ub &color3ub, const COL &col ) {
			color3ub.r = col.r * 255;
			color3ub.g = col.g * 255;
			color3ub.b = col.b * 255;
		}

		void setFloat3FromVC3( float *v, const VC3 &w ) {
			v[0] = w.x;
			v[1] = w.y;
			v[2] = w.z;
		}
		
		void setInt2FromVCI2( int *i, const VC2I &v ) {
			i[0] = v.x;
			i[1] = v.y;
		}

		// zOrder: ...yxyxyxyx
		int zOrderFromXY( int x, int y ) {
			int result = 0;
			int bit = 1;
			int targetBitX = 1;
			int targetBitY = 2;
			for( int i = 0 ; i < 16 ; ++i, bit <<= 1, targetBitX <<= 2, targetBitY <<= 2 ) {
				if( x & bit ) {
					result |= targetBitX;
				}
				if( y & bit ) {
					result |= targetBitY;
				}
			}
			return result;
		}

		VC2I xyFromZOrder( int z ) {
			VC2I split;
			int bit = 1;
			int targetBitX = 1;
			int targetBitY = 2;
			for( int i = 0 ; i < 16 ; ++i, bit <<= 1, targetBitX <<= 2, targetBitY <<= 2 ) {
				if( z & targetBitX ) {
					split.x |= bit;
				}
				if( z & targetBitY ) {
					split.y |= bit;
				}
			}
			return split;
		}

		void visitTerrainTexture( IStorm3D_Texture *texture ) {
			terrainTextureIds.push_back( exportTexture( texture ) );
		}

		void visitTerrainBlendingSize( const VC2I &blendSize ) {
			setInt2FromVCI2( scene.terrain.layerSize, blendSize );
		}

		void visitTerrainBlending( const std::vector< unsigned char > &weights ) {
			int terrainTextureIndex = scene.terrain.layers.size();

			scene.terrain.layers.push_back( SGSScene::Terrain::Layer() );

			auto &layer = scene.terrain.layers.back();
			layer.textureIndex = terrainTextureIds[ terrainTextureIndex ];
			layer.weights = weights;
		}

		void exportTerrainHeightmap( const std::vector<unsigned short> &heightmap, const VC2I &mapSize, const VC3 &realSize ) {
			scene.terrain.mapSize[0] = mapSize.x;
			scene.terrain.mapSize[1] = mapSize.y;

			float realMapSize[3];

			realMapSize[0] = realSize.x;
			realMapSize[1] = realSize.y;
			realMapSize[2] = realSize.z;

			float scale[3] = { realSize.x / (mapSize.x - 1), realSize.y / 65535.f, realSize.z / (mapSize.y - 1) };
			float origin[3] = { realSize.x / 2, 0, realSize.z / 2 };

			int numVertices = mapSize.x * mapSize.y;
			int numIndices = (mapSize.x - 1) * (mapSize.y - 1) * 2 * 3;

#define getIndex( xIndex, yIndex ) ((yIndex) * mapSize.x + (xIndex))
#define getHeight( xIndex, yIndex ) heightmap[ getIndex( xIndex, yIndex ) ]
#define getPosition( xIndex, yIndex ) \
			VC3( (xIndex) * scale[0] - origin[0], getHeight( xIndex, yIndex ) * scale[1] - origin[1], (yIndex) * scale[2] - origin[2] )
#define getNormal( a, b, c ) ( (b) - (a) ).GetCrossWith( (c) - (a) )
#define isValid( xIndex, yIndex ) ((xIndex) > 0 && (xIndex) < mapSize.x - 1 && (yIndex) > 0 && (yIndex) < mapSize.y - 1)

#define getTargetPosition( xIndex, yIndex ) ((yIndex) * mapSize.x + (xIndex))

			scene.terrain.vertices.resize( numVertices );
			scene.terrain.indices.resize( numIndices );

			for( int yIndex = 0 ; yIndex < mapSize.y ; yIndex++ ) {
				for( int xIndex = 0 ; xIndex < mapSize.x ; xIndex++ ) {
					VC3 position = getPosition( xIndex, yIndex );
					VC3 normal;

					static int neighborXOffset[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
					static int neighborYOffset[] = { 1, 1, 0, -1, -1, -1, 0, 1 };

					VC3 lastNeighbor;
					bool lastValid = isValid( xIndex + neighborXOffset[7], yIndex + neighborYOffset[7] );
					if( lastValid ) {
						lastNeighbor = getPosition( xIndex + neighborXOffset[7], yIndex + neighborYOffset[7] );
					}

					for( int i = 0 ; i < 8 ; i++ ) {
						int neighbor_xIndex = xIndex + neighborXOffset[i];
						int neighbor_yIndex = yIndex + neighborYOffset[i];
						if( isValid( neighbor_xIndex, neighbor_yIndex ) ){
							VC3 currentNeighbor = getPosition( neighbor_xIndex, neighbor_yIndex );
							if( lastValid ) {
								normal += getNormal( lastNeighbor, position, currentNeighbor );
							}
							lastNeighbor = currentNeighbor;
							lastValid = true;
						}
						else {
							lastValid = false;
						}
					}

					if( normal.GetSquareLength() > 0.0001f ) {
						normal.Normalize();
					}

					auto &vertex = scene.terrain.vertices[ getIndex( xIndex, yIndex ) ];
					setFloat3FromVC3( vertex.position, position );
					setFloat3FromVC3( vertex.normal, normal );

					vertex.blendUV[0] = float( xIndex ) / (mapSize.x - 1);
					vertex.blendUV[1] = float( yIndex ) / (mapSize.y - 1);
				}
			}


			int numXTiles = iceil(mapSize.x - 1, SGSScene::Terrain::EDGES_PER_TILE );
			int numYTiles = iceil(mapSize.y - 1, SGSScene::Terrain::EDGES_PER_TILE );
			
			int index = 0;

			for(int yTileIndex = 0 ; yTileIndex < numYTiles ; yTileIndex++ ) {
				for(int xTileIndex = 0 ; xTileIndex < numXTiles ; xTileIndex++ ) {
					const int xOffset = xTileIndex * SGSScene::Terrain::EDGES_PER_TILE;
					const int yOffset = yTileIndex * SGSScene::Terrain::EDGES_PER_TILE;
					const int numX = std::min( mapSize.x - xOffset, SGSScene::Terrain::TILE_SIZE);
					const int numY = std::min( mapSize.y - yOffset, SGSScene::Terrain::TILE_SIZE);

					// set the tile information
					SGSScene::Terrain::Tile tile;
					tile.startIndex = index;
					tile.numIndices = (numX - 1) * (numY - 1) * 2 * 3;

					// set the bounding box
					tile.bounding.box.min[0] = tile.bounding.box.min[1] = tile.bounding.box.min[2] = FLT_MAX;
					tile.bounding.box.max[0] = tile.bounding.box.max[1] = tile.bounding.box.max[2] = -FLT_MAX; 
					for( int yIndex = 0 ; yIndex < numY ; yIndex++ ) {
						for( int xIndex = 0 ; xIndex < numX ; xIndex++ ) {
							int globalXIndex = xOffset + xIndex;
							int globalYIndex = yOffset + yIndex;
							auto vertex = scene.terrain.vertices[ getIndex( globalXIndex, globalYIndex ) ];

							for( int i = 0 ; i < 3 ; ++i ) {
								tile.bounding.box.min[i] = std::min( tile.bounding.box.min[i], vertex.position[i] );
							}
							for( int i = 0 ; i < 3 ; ++i ) {
								tile.bounding.box.max[i] = std::max( tile.bounding.box.max[i], vertex.position[i] );
							}
						}
					}

					// set the bounding sphere
					for( int i = 0 ; i < 3 ; ++i ) {
						tile.bounding.sphere.center[i] = (tile.bounding.box.min[i] + tile.bounding.box.max[i]) * 0.5;
					}
					tile.bounding.sphere.radius = 0.0;
					for( int yIndex = 0 ; yIndex < numY ; yIndex++ ) {
						for( int xIndex = 0 ; xIndex < numX ; xIndex++ ) {
							int globalXIndex = xOffset + xIndex;
							int globalYIndex = yOffset + yIndex;
							auto vertex = scene.terrain.vertices[ getIndex( globalXIndex, globalYIndex ) ];

							float distance = 0;
							for( int i = 0 ; i < 3 ; ++i ) {
								float axisDistance = vertex.position[i] - tile.bounding.sphere.center[i];
								distance += axisDistance*axisDistance;
							}
							distance = sqrt( distance );

							tile.bounding.sphere.radius = std::max( tile.bounding.sphere.radius, distance );
						}
					}

					scene.terrain.tiles.push_back( tile );
					
					// set the indices
					for( int yIndex = 0 ; yIndex < numY - 1 ; yIndex++ ) {
						for( int xIndex = 0 ; xIndex < numX - 1 ; xIndex++ ) {
							int globalXIndex = xOffset + xIndex;
							int globalYIndex = yOffset + yIndex;

							int base = getIndex( globalXIndex, globalYIndex );
							int right = getIndex( globalXIndex + 1, globalYIndex );
							int down = getIndex( globalXIndex, globalYIndex + 1 );
							int right_down = getIndex( globalXIndex + 1, globalYIndex + 1 );

							scene.terrain.indices[ index++ ] = right;
							scene.terrain.indices[ index++ ] = base;
							scene.terrain.indices[ index++ ] = down;

							scene.terrain.indices[ index++ ] = right;
							scene.terrain.indices[ index++ ] = down;
							scene.terrain.indices[ index++ ] = right_down;
						}
					}
				}
			}
#undef getIndex
#undef getPosition
#undef getNormal
#undef getHeight
#undef isValid
		}

		int exportTexture( IStorm3D_Texture *texture ) {
			if( !texture || !texture->GetFilename() ) {
				return SGSScene::NO_TEXTURE;
			}

			std::string textureFilename( texture->GetFilename() );
			if( textureFilename.empty() ) {
				return SGSScene::NO_TEXTURE;
			}

			// has the texture already been loaded?
			auto it = textureNameIdMap.find( textureFilename );
			if( it != textureNameIdMap.end() ) {
				// already cached
				return it->second;
			}
			else {
				// try to load and store the texture
				FILE *textureFile = fopen( textureFilename.c_str(), "rb" );

				if( textureFile ) {
					int textureIndex = scene.textures.size();						

					scene.textures.push_back( SGSScene::Texture() );

					SGSScene::Texture &texture = scene.textures.back();
					texture.name = textureFilename;

					fseek( textureFile, 0, SEEK_END );
					unsigned size = ftell( textureFile );
					fseek( textureFile, 0, SEEK_SET );

					auto &rawContent = texture.rawContent;
					rawContent.resize( size );

					fread( &rawContent.front(), size, 1, textureFile );

					fclose( textureFile );

					// add to the lookup cache
					textureNameIdMap[ textureFilename ] = textureIndex;

					return textureIndex;
				}
			}
			return SGSScene::NO_TEXTURE;
		}

		void exportModelType( const std::string &name ) {
			auto it = modelNameObjectIdMap.find( name );
			if( it != modelNameObjectIdMap.end() ) {
				currentModelId = it->second;
			}
			else {
				currentModelId = scene.modelNames.size();

				scene.modelNames.push_back( name );
			}
		}

		void exportMaterial( SGSScene::Material &material, IStorm3D_Material &stormMaterial ) {
			stormMaterial.GetSpecial( material.doubleSided, material.wireFrame );

			setColor3ubFromCOL( material.ambient, stormMaterial.GetSelfIllumination() );
			setColor3ubFromCOL( material.diffuse, stormMaterial.GetColor() );

			COL stormSpecular;
			stormMaterial.GetSpecular( stormSpecular, material.specularSharpness );
			setColor3ubFromCOL( material.specular, stormSpecular );

			material.alpha = (1.0 - stormMaterial.GetTransparency()) * 255;

			// map the alpha type
			auto stormAlphaType = stormMaterial.GetAlphaType();
			switch( stormAlphaType ) {
			case IStorm3D_Material::ATYPE_USE_ALPHATEST:
				material.alphaType = SGSScene::Material::AT_ALPHATEST;
				break;
			case IStorm3D_Material::ATYPE_MUL2X:
				material.alphaType = SGSScene::Material::AT_MULTIPLY_2;
				break;
			case IStorm3D_Material::ATYPE_MUL:
				material.alphaType = SGSScene::Material::AT_MULTIPLY;
				break;
			case IStorm3D_Material::ATYPE_ADD:
				material.alphaType = SGSScene::Material::AT_ADDITIVE;
				break;
			case IStorm3D_Material::ATYPE_USE_TEXTRANSPARENCY:
				material.alphaType = SGSScene::Material::AT_TEXTURE;
				break;
			case IStorm3D_Material::ATYPE_USE_TRANSPARENCY:
				material.alphaType = SGSScene::Material::AT_MATERIAL;
				break;
			case IStorm3D_Material::ATYPE_NONE:
			default:
				material.alphaType = SGSScene::Material::AT_NONE;
				break;
			}
			
			material.textureIndex[0] = exportTexture( stormMaterial.GetBaseTexture() );
			material.textureIndex[1] = exportTexture( stormMaterial.GetBaseTexture2() );
		}

		void setDefaultMaterial( SGSScene::Material &material ) {
			material.doubleSided = false;
			material.wireFrame = false;

			material.textureIndex[0] = material.textureIndex[1] = SGSScene::NO_TEXTURE; // no texture

			material.specularSharpness = 1.0;

			material.diffuse.r = material.diffuse.g = material.diffuse.b = 255;
			material.specular.r = material.specular.g = material.specular.b = 255;			
		}

		void ensureCapacity( int numNewVertices, int numNewIndices ) {
			scene.indices.reserve( std::max<unsigned>( scene.indices.capacity(), scene.indices.size() + numNewIndices * 10 ) );
			scene.vertices.reserve( std::max<unsigned>( scene.vertices.capacity(), scene.vertices.size() + numNewVertices * 10 ) );
		}

		void exportBoundingSphere( SGSScene::BoundingSphere &boundingSphere, const Sphere &sphere ) {
			setFloat3FromVC3( boundingSphere.center, sphere.position );
			boundingSphere.radius = sphere.radius;
		}

		void exportModelObject( IStorm3D_Model &model, IStorm3D_Model_Object &modelObject ) {
			IStorm3D_Mesh *mesh = modelObject.GetMesh();
			MAT transformMat = modelObject.GetMXG();
			MAT normalTransformMat = transformMat.GetWithoutTranslation();

			const int numFaces = mesh->GetFaceCount();
			const int numVertices = mesh->GetVertexCount();

			if( numFaces == 0 || numVertices == 0 ) {
				return;
			}

			ensureCapacity( numVertices, numFaces );

			const Storm3D_Face *faces = mesh->GetFaceBufferReadOnly();
			const Storm3D_Vertex *vertices = mesh->GetVertexBufferReadOnly();

			scene.subObjects.push_back( SGSScene::SubObject() );
			SGSScene::SubObject &subObject = scene.subObjects.back();
			
			subObject.subModelName = modelObject.GetName();
			
			IStorm3D_Material *material = mesh->GetMaterial();
			if( material ) {				
				exportMaterial( subObject.material, *material );
			}
			else {
				setDefaultMaterial( subObject.material );
			}

			// set bounding sphere
			VC3 transformedCenter = transformMat.GetTransformedVector( modelObject.GetBoundingSphere().position );
			setFloat3FromVC3( subObject.boundingSphere.center, transformedCenter );
			subObject.boundingSphere.radius = modelObject.GetBoundingSphere().radius;

			int firstVertex = scene.vertices.size();

			// output vertices
			for( int v = 0 ; v < numVertices ; v++ ) {
				const Storm3D_Vertex &vertex = vertices[v];

				VC3 transformedPosition = transformMat.GetTransformedVector( vertex.position );
				VC3 transformedNormal = normalTransformMat.GetTransformedVector( vertex.normal );

				SGSScene::Vertex outVertex;
				outVertex.position[0] = transformedPosition.x;
				outVertex.position[1] = transformedPosition.y;
				outVertex.position[2] = transformedPosition.z;

				outVertex.normal[0] = transformedNormal.x;
				outVertex.normal[1] = transformedNormal.y;
				outVertex.normal[2] = transformedNormal.z;

				outVertex.uv[0][0] = vertex.texturecoordinates.x;
				outVertex.uv[0][1] = vertex.texturecoordinates.y;
				outVertex.uv[1][0] = vertex.texturecoordinates2.x;
				outVertex.uv[1][1] = vertex.texturecoordinates2.y;

				scene.vertices.push_back( outVertex );
			}

			subObject.startIndex = scene.indices.size();
			subObject.numIndices = numFaces * 3;

			// output faces
			for( int f = 0 ; f < numFaces ; f++ ) {
				const Storm3D_Face &face = faces[f];

				for( int i = 0 ; i < 3 ; i++ ) { 
					scene.indices.push_back( firstVertex + face.vertex_index[i] );
				}
			}
		}

		void exportModel( IStorm3D_Model &model ) {
			Iterator<IStorm3D_Model_Object *> *objectIterator;

			scene.objects.push_back( SGSScene::Object() );
			SGSScene::Object &object = scene.objects.back();

			object.modelId = currentModelId;
			
			object.startSubObject = scene.subObjects.size();

			for(objectIterator = model.ITObject->Begin(); !objectIterator->IsEnd(); objectIterator->Next() ) {
				IStorm3D_Model_Object &modelObject = *objectIterator->GetCurrent();

				// ignore editor only objects
				std::string name = modelObject.GetName();
				if( name.find( "EditorOnly" ) != name.npos ) {
					continue;
				}
				if (modelObject.GetName() != NULL)
				{
					int slen = strlen(modelObject.GetName());
					if (slen >= 9)
					{
						if (strcmp(&modelObject.GetName()[slen - 9], "Collision") == 0)
						{
							continue;
						}
					}
					if (strcmp(modelObject.GetName(), "effect_layer") == 0)
					{
						continue;
					}
					if (strcmp(modelObject.GetName(), "effect_2nd_layer") == 0)
					{
						continue;
					}
				}

				exportModelObject( model, modelObject );
				++object.numSubObjects;
			}

			delete objectIterator;
		}

		void save() {
			Serializer::BinaryWriter writer( filepath.c_str() );

			Serializer::write( writer, scene );
		}
	};
}