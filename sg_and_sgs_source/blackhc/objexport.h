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
		};

		ObjExporter( const std::string &filename ) : filename( filename ) {}

		ExporterVisitor getVisitor() {
			return ExporterVisitor( *this );
		}

		void startExport() {
			out.open( filename );
		}

		void exportModel( IStorm3D_Model &model ) {
			Iterator<IStorm3D_Model_Object *> *objectIterator;

			out << "# new model\n";

			for(objectIterator = model.ITObject->Begin(); !objectIterator->IsEnd(); objectIterator->Next()) {
				IStorm3D_Mesh *mesh = objectIterator->GetCurrent()->GetMesh();
				MAT transformMat = objectIterator->GetCurrent()->GetMXG();
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

			out << "\n";

			delete objectIterator;
		}

		void finishExport() {
			out.close();
		}
	};
}