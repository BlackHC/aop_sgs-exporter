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

namespace blackhc {
	inline YAML::Emitter &operator << (YAML::Emitter &out, const VC3 &v) {
		return out << YAML::Flow << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	}

	inline YAML::Emitter &operator << (YAML::Emitter &out, const QUAT &v) {
		return out << YAML::Flow << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	}

	struct EntityExporter {
		std::string filepath;
		YAML::Emitter out;
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

		EntityExporter( const std::string &filepath ) : filepath( filepath ) {
			out << YAML::BeginSeq;
		}

		Visitor getVisitor() {
			return Visitor( *this );
		}

		void setObjectName( const std::string &name ) {
			objectName = name;
		}

		void exportModel( IStorm3D_Model &model ) {
			const VC3 &position = model.GetPosition();
			const QUAT &model.GetRotation();
			const AABB &aabb = model.GetBoundingBox();
			out << YAML::BeginSeq << objectName << position << aabb.mmin << aabb.mmax << YAML::EndSeq;
		}

		void finish() {
			out << YAML::EndSeq;

			std::ofstream outFile( filepath );
			outFile << out.c_str();
			outFile.close();
		}
	};
}