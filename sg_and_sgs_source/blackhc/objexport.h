// header file for the obj exporter [5/19/2012 kirschan2]
#pragma once

#include <vector>
#include <string>

#include "../system/Logger.h"
#include "IStorm3D_Mesh.h"
#include "IStorm3D_Model.h"

#include "visitor.h"
#include <fstream>

namespace blackhc {
	typedef std::vector<IStorm3D_Model> ModelVector;
	
	struct ObjExporter {
		std::string filename;
		std::ofstream out;

		struct ExporterVisitor : Visitor {
			ObjExporter &_;

			ExporterVisitor( ObjExporter &_ ) : _(_) {}

			void visit( IStorm3D_Model &model ) {
				_.exportModel( model );
			}

			void heightmap( const std::vector<unsigned short> heightmap, const VC2I &mapSize, const VC3 &realSize ) {
				_.exportTerrain( heightmap, mapSize, realSize );
			}
		};

		ObjExporter( const std::string &filename ) : filename( filename ) {}

		ExporterVisitor getVisitor() {
			return ExporterVisitor( *this );
		}

		void startExport() {
			out.open( filename );
		}

		void exportModelObject( IStorm3D_Model_Object &modelObject ) {
			IStorm3D_Mesh *mesh = modelObject.GetMesh();
			MAT transformMat = modelObject.GetMXG();
			MAT normalTransformMat = transformMat.GetWithoutTranslation();

			const int numFaces = mesh->GetFaceCount();
			const int numVertices = mesh->GetVertexCount();

			const Storm3D_Face *faces = mesh->GetFaceBufferReadOnly();
			const Storm3D_Vertex *vertices = mesh->GetVertexBufferReadOnly();

			out << "#\tnew object\n";

			// output vertices
			for( int v = numVertices - 1 ; v >= 0 ; v-- ) {
				const Storm3D_Vertex &vertex = vertices[v];

				VC3 transformedPosition = transformMat.GetTransformedVector( vertex.position );
				VC3 transformedNormal = normalTransformMat.GetTransformedVector( vertex.normal );

				out << "v " << transformedPosition.x << " " << transformedPosition.y << " " << transformedPosition.z << "\n"
					<< "vn " << transformedNormal.x << " " << transformedNormal.y << " " << transformedNormal.z << "\n";

				// TODO: add texture coordinates
			}

			out << "\n";

			// output faces
			for( int f = 0 ; f < numFaces ; f++ ) {
				const Storm3D_Face &face = faces[f];

				int indices[3];
				for( int i = 0 ; i < 3 ; i++ ) { 
					indices[i] = -int( face.vertex_index[i] ) - 1;
				}

				out << "f " << indices[0] << "//" << indices[0] << " "
					<< indices[1] << "//" << indices[1] << " "
					<< indices[2] << "//" << indices[2] << "\n";
			}

			out << "\n";
		}

		struct ExportTerrain {
			std::ofstream &out;

			const std::vector<unsigned short> heightmap;
			const VC2I &mapSize;
			const VC3 &realSize;

			const VC3 scale;

			int numVertices;

			int getIndex( int xIndex, int yIndex ) {
				return yIndex * mapSize.x + xIndex;
			}

			unsigned short getHeight( int xIndex, int yIndex ) {
				return heightmap[ getIndex( xIndex, yIndex ) ];
			}

			VC3 getPosition( int xIndex, int yIndex, unsigned short height ) {
				return VC3( xIndex * scale.x - realSize.x / 2, height * scale.y, yIndex * scale.z - realSize.z / 2 );
			}

			VC3 getPosition( int xIndex, int yIndex ) {
				return getPosition( xIndex, yIndex, getHeight( xIndex, yIndex ) );
			}

			VC3 getNormal( const VC3 &a, const VC3 &b, const VC3 &c ) {
				return (b-a).GetCrossWith(c-a);
			}

			int getOffset( int xIndex, int yIndex ) {
				return getIndex( xIndex, yIndex ) - numVertices;
			}

			void outputOffset( int offset ) {
				out << " " << offset << "//" << offset;
			}
			
			ExportTerrain( std::ofstream &out, const std::vector<unsigned short> heightmap, const VC2I &mapSize, const VC3 &realSize )
				: out( out ),
				heightmap( heightmap ),
				mapSize( mapSize ),
				realSize( realSize ),
				scale(realSize.x / (mapSize.x - 1), realSize.y / 65535.f, realSize.z / (mapSize.y - 1)) ,
				numVertices( mapSize.x * mapSize.y )
			{
				out << "# terrain\n";
				for( int yIndex = 0 ; yIndex < mapSize.y ; yIndex++ ) {
					for( int xIndex = 0 ; xIndex < mapSize.x ; xIndex++ ) {
						VC3 position = getPosition( xIndex, yIndex );
						VC3 normal;

						if( xIndex > 0 && xIndex < mapSize.x - 1 && yIndex > 0 && yIndex < mapSize.y - 1 ) {
							static int neighborXOffset[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
							static int neighborYOffset[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
							
							VC3 lastNeighbor = getPosition( xIndex + neighborXOffset[7], yIndex + neighborYOffset[7] );
							for( int i = 0 ; i < 8 ; i++ ) {
								VC3 currentNeighbor = getPosition( xIndex + neighborXOffset[i], yIndex + neighborYOffset[i] );
								normal += getNormal( lastNeighbor, position, currentNeighbor );
								lastNeighbor = currentNeighbor;
							}

							if( normal.GetSquareLength() > 0.0001f ) {
								normal.Normalize();
							}
						}
						else {
							normal.x = 0.0f;
							normal.y = 1.0f;
							normal.z = 0.0f;
						}

						out << "v " << position.x << " " << position.y << " " << position.z << "\n"
							<< "vn " << normal.x << " " << normal.y << " " << normal.z << "\n";
					}
				}

				out << "\n";
				for( int yIndex = 0 ; yIndex < mapSize.y - 1 ; yIndex++ ) {
					for( int xIndex = 0 ; xIndex < mapSize.x - 1 ; xIndex++ ) {
						int base = getOffset( xIndex, yIndex );
						int right = getOffset( xIndex + 1, yIndex );
						int down = getOffset( xIndex, yIndex + 1 );
						int rightdown = getOffset( xIndex + 1, yIndex + 1 );

						out << "f";
						outputOffset( right );
						outputOffset( base );
						outputOffset( down );
						out << "\n"
							<< "f";
						outputOffset( right );
						outputOffset( down );
						outputOffset( rightdown );
						out << "\n";
					}
				}
			}
		};


		void exportTerrain( const std::vector<unsigned short> heightmap, const VC2I &mapSize, const VC3 &realSize ) {
			ExportTerrain( out, heightmap, mapSize, realSize );
		}

		void exportModel( IStorm3D_Model &model ) {
			Iterator<IStorm3D_Model_Object *> *objectIterator;

			out << "# new model\n";

			for(objectIterator = model.ITObject->Begin(); !objectIterator->IsEnd(); objectIterator->Next()) {
				IStorm3D_Model_Object &modelObject = *objectIterator->GetCurrent();

				// ignore editor only objects
				std::string name = modelObject.GetName();
				if( name.find( "EditorOnly" ) != name.npos ) {
					continue;
				}
				
				exportModelObject( modelObject );
			}

			out << "\n";

			delete objectIterator;
		}

		void finishExport() {
			out.close();
		}
	};
}