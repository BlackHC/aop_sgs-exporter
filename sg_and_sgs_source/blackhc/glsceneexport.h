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

#include "serializer/serializer.h"
#include "serializer/serializer_std.h"

#include "sgsScene/sgsScene.h"

#include <stdio.h>

namespace blackhc {
	typedef std::vector<IStorm3D_Model> ModelVector;

	struct GLSceneExporter : Visitor {
		std::string filepath;

		int currentModelId;

		SGSScene scene;
		std::map< std::string, int > textureNameIdMap;
		std::map< std::string, int > modelNameObjectIdMap;

		GLSceneExporter( const std::string &filepath ) {
			this->filepath = filepath;

			const int initalTotalSize = 4 << 20;
			scene.vertices.reserve( initalTotalSize );
			scene.indices.reserve( initalTotalSize );
		}

		void visitObjectInstance( IStorm3D_Model &model ) {
			exportModel( model );
		}

		void terrainObjectName( const std::string &name ) { exportModelType( name ); }

		bool needColormap() { return false; }

		void setColor3ubFromCOL( SGSScene::Color3ub &color3ub, const COL &col ) {
			color3ub.r = col.r * 255;
			color3ub.g = col.g * 255;
			color3ub.b = col.b * 255;
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
			
			if( stormMaterial.GetBaseTexture() && stormMaterial.GetBaseTexture()->GetFilename() ) {

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