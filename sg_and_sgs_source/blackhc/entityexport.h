// header file for the entity exporter [6/6/2012 kirschan2]

#include <vector>
#include <string>

#include "../system/Logger.h"
#include "IStorm3D_Mesh.h"
#include "IStorm3D_Model.h"
#include "c2_vectors.h"

#include <yaml-cpp/yaml.h>

#include "visitor.h"
#include <fstream>
#include <ostream>

namespace blackhc {
	inline std::ostream &operator <<( std::ostream &out, const VC3 &v ) {
		return out << v.x << " " << v.y << " " << v.z;
	}

	inline std::ostream &operator <<( std::ostream &out, const QUAT &v ) {
		return out << v.x << " " << v.y << " " << v.z << " " << v.w;
	}

	struct EntityExporter {
		std::string filepath;
		std::ofstream out;
		std::string objectName;

		struct Visitor : ::Visitor {
			EntityExporter &_;

			Visitor( EntityExporter &_ ) : _(_) {}

			void visit( IStorm3D_Model &model ) {
				_.exportModel( model );
			}
			
			void terrainObjectName( const std::string &name ) {
				_.setObjectName( name );
			}
		};

		EntityExporter( const std::string &filepath ) : filepath( filepath ), out( filepath ) {
			out << "objName x y z qx qy qz qw bb_minx bb_miny bb_minz bb_maxx bb_maxy bb_maxz\n";
		}

		Visitor getVisitor() {
			return Visitor( *this );
		}

		void setObjectName( const std::string &name ) {
			objectName = name;
		}

		void exportModel( IStorm3D_Model &model ) {
			const VC3 &position = model.GetPosition();
			const QUAT &rotation = model.GetRotation();
			const AABB &aabb = model.GetBoundingBox();
			out << "\"" << objectName << "\" " << position << " " << rotation << " " << aabb.mmin << " " << aabb.mmax << "\n";
		}

		void finish() {
			out.close();
		}
	};
}