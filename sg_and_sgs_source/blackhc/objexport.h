// header file for the obj exporter [5/19/2012 kirschan2]
#pragma once

#include <vector>
#include <string>

#include "../system/Logger.h"
#include "IStorm3D_Mesh.h"
#include "IStorm3D_Model.h"
#include "c2_vectors.h"

//#include <boost/filesystem/path.hpp>

#include "visitor.h"
#include <fstream>
#include <unordered_set>
#include <boost/algorithm/string.hpp>

#include <GdiPlus.h>

static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;

	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

namespace blackhc {
	typedef std::vector<IStorm3D_Model> ModelVector;
	
	struct ObjExporter {
		std::string exportDirectory;
		std::string filename;
		std::string filenameMtl;
		std::string filenameColormap;

		std::ofstream out;
		std::ofstream outMtl;
		int index;
		
		std::tr1::unordered_set<std::string> textures;

		enum TextureType {
			TT_NONE,
			TT_BASE,
			TT_BASE2
		};
		
		struct ExporterVisitor : Visitor {
			ObjExporter &_;

			ExporterVisitor( ObjExporter &_ ) : _(_) {}

			void visitObjectInstance( IStorm3D_Model &model ) {
				_.exportModel( model );
			}

			void visitHeightmap( const std::vector<unsigned short> heightmap, const VC2I &mapSize, const VC3 &realSize ) {
				_.exportTerrain( heightmap, mapSize, realSize );
			}

			void visitColormap( const std::vector<unsigned char> rgbData, VC2I size ) {
				_.exportColormap( rgbData, size );
			}

			bool visitNeedColormap() { return true; }
		};

		ObjExporter( const std::string &filepath ) : index( 0 ) {
			// set export directory and filename
			int lastSeparator = filepath.find_last_of("\\");
			if( lastSeparator != filepath.npos ) {
				exportDirectory = filepath.substr( 0, lastSeparator + 1 );
				filename = filepath.substr( lastSeparator + 1 );
			}
			else {
				exportDirectory = "";
				filename = filepath;
			}

			// set the filenames of the mtl and colormap file
			const std::string stem = filename.substr( 0, filename.find_last_of(".") );
			filenameMtl = stem + ".mtl";
			filenameColormap = stem + "_colormap.png";
		}

		ExporterVisitor getVisitor() {
			return ExporterVisitor( *this );
		}

		void startExport() {
			out.open( exportDirectory + filename );
			outMtl.open( exportDirectory + filenameMtl );

			out << "mtllib " << filenameMtl
				<< "\n\n";
		}

		static TextureType getDiffuseTextureType( IStorm3D_Mesh *mesh ) {
			TextureType type;
			if( !mesh->GetMaterial() ) {
				type = TT_NONE;
			}
			else if( mesh->GetMaterial()->GetBaseTexture() ) {
				type = TT_BASE;
			}
			else if( mesh->GetMaterial()->GetBaseTexture2() ) {
				type = TT_BASE2;
			}
			else {
				type = TT_NONE;
			}

			// make sure the texture filename exists
			if( !getDiffuseTextureFilename( mesh, type ).empty() ) {
				return type;
			}
			else {
				return TT_NONE;
			}
		}

		static std::string getDiffuseTextureFilename( IStorm3D_Mesh *mesh, const TextureType textureType ) {
			switch( textureType ) {
			case TT_NONE:
				return std::string();
			case TT_BASE:
				return mesh->GetMaterial()->GetBaseTexture()->GetFilename();
			case TT_BASE2:
				return mesh->GetMaterial()->GetBaseTexture2()->GetFilename();
			}
		}

		static std::string getMaterialName( const std::string &textureFilename ) {
			// assume that every texture has an extension (and directory names contain none)
			size_t lastDot = textureFilename.find_last_of( '.' );
			return textureFilename.substr( 0, lastDot );
		}

		static std::string getLocalTextureFilename( const std::string &filename ) {
			std::string localName = filename;

			boost::replace_all( localName, "/", "__" );
			boost::replace_all( localName, "\\", "__" );
			boost::replace_all( localName, " ", "_" );

			return localName;
		}

		void outputOffset( int index, TextureType textureType ) {
			out << " " << index << "/";
			if( textureType != TT_NONE ) {
				out << index;
			}
			out << "/" << index;
		}

		void exportModelObject( IStorm3D_Model_Object &modelObject ) {
			IStorm3D_Mesh *mesh = modelObject.GetMesh();
			MAT transformMat = modelObject.GetMXG();
			MAT normalTransformMat = transformMat.GetWithoutTranslation();

			const int numFaces = mesh->GetFaceCount();
			const int numVertices = mesh->GetVertexCount();

			if( numFaces == 0 || numVertices == 0 ) {
				return;
			}

			const Storm3D_Face *faces = mesh->GetFaceBufferReadOnly();
			const Storm3D_Vertex *vertices = mesh->GetVertexBufferReadOnly();

			out << "#\tnew object " << modelObject.GetName() << "\n";

			out << "g " << "object_" << index++ << "\n";
			
			TextureType textureType = getDiffuseTextureType( mesh );
			std::string diffuseTextureFilename = getDiffuseTextureFilename( mesh, textureType );			
			if( textureType != TT_NONE ) {
				std::string materialName = getMaterialName( getLocalTextureFilename( diffuseTextureFilename ) );

				out << "usemtl " << materialName << "\n";
				textures.insert( diffuseTextureFilename );
			} 
			else {
				out << "usemtl nullTexture\n";
			}
			
			// output vertices
			for( int v = numVertices - 1 ; v >= 0 ; v-- ) {
				const Storm3D_Vertex &vertex = vertices[v];

				VC3 transformedPosition = transformMat.GetTransformedVector( vertex.position );
				VC3 transformedNormal = normalTransformMat.GetTransformedVector( vertex.normal );

				out << "v " << transformedPosition.x << " " << transformedPosition.y << " " << transformedPosition.z << "\n"
					<< "vn " << transformedNormal.x << " " << transformedNormal.y << " " << transformedNormal.z << "\n";

				// output texture coordinates if available
				switch( textureType ) {
				case TT_BASE:
					out << "vt " << vertex.texturecoordinates.x << " " << vertex.texturecoordinates.y << "\n";
					break;
				case TT_BASE2:
					out << "vt " << vertex.texturecoordinates2.x << " " << vertex.texturecoordinates2.y << "\n";
					break;
				}
			}

			out << "\n";

			// output faces
			for( int f = 0 ; f < numFaces ; f++ ) {
				const Storm3D_Face &face = faces[f];

				out << "f";
				for( int i = 0 ; i < 3 ; i++ ) { 
					int index = -int( face.vertex_index[i] ) - 1;
					outputOffset( index, textureType );
				}
				out	<< "\n";
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
				out << " " << offset << "/" << offset << "/" << offset;
			}
			
			ExportTerrain( std::ofstream &out, const std::vector<unsigned short> heightmap, const VC2I &mapSize, const VC3 &realSize )
				: out( out ),
				heightmap( heightmap ),
				mapSize( mapSize ),
				realSize( realSize ),
				scale(realSize.x / (mapSize.x - 1), realSize.y / 65535.f, realSize.z / (mapSize.y - 1)) ,
				numVertices( mapSize.x * mapSize.y )
			{
				out << 
					"# terrain\n"
					"g __Terrain__\n"
					"usemtl __Terrain_ColorMap__\n"
					"\n";
					
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
							<< "vn " << normal.x << " " << normal.y << " " << normal.z << "\n"
							<< "vt " << float( xIndex ) / (mapSize.x - 1) << " " << float( yIndex ) / (mapSize.y - 1) << "\n";
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
							"f";
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
				
				exportModelObject( modelObject );
			}

			out << "\n";

			delete objectIterator;
		}

		void exportTextures() {
			outMtl << "newmtl nullTexture\n"
				"illum 4\n"
				"Kd 1.00 1.00 1.00\n"
				"Ka 1.00 1.00 1.00\n"
				"Tf 1.00 1.00 1.00\n"
				"Ni 1.00\n\n";

			outMtl << "newmtl __Terrain_ColorMap__\n"
				"illum 4\n"
				"Kd 1.00 1.00 1.00\n"
				"Ka 1.00 1.00 1.00\n"
				"Tf 1.00 1.00 1.00\n"
				"map_Kd " << filenameColormap << "\n"
				"Ni 1.00\n\n";

			for( auto it = textures.begin() ; it != textures.end() ; it++ ) {
				std::string localName = getLocalTextureFilename( *it );
				std::string localFilename = exportDirectory + localName;
				std::string materialName = getMaterialName( localName );
				
				/*char absolutePath[512];
				GetFullPathNameA(it->c_str(), 1024, absolutePath, NULL );*/
				
				CopyFileA( it->c_str(), localFilename.c_str(), TRUE );

				outMtl << 
					"newmtl " << materialName << "\n"
					"illum 4\n"
					"Kd 1.00 1.00 1.00\n"
					"Ka 0.00 0.00 0.00\n"
					"Tf 1.00 1.00 1.00\n"
					<< "map_Kd " << localName << "\n"
					"Ni 1.00\n\n";
			}
		}

		void finishExport() {
			exportTextures();

			out.close();
			outMtl.close();
		}

		void exportColormap( const std::vector<unsigned char> rgbData, VC2I size ) 
		{
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			ULONG_PTR gdiplusToken;
			GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

			{
				Gdiplus::Bitmap gdiBitmap( size.x, size.y, size.x * 3, PixelFormat24bppRGB, (BYTE*) &rgbData.front() );
				CLSID pngClsid;

				GetEncoderClsid(L"image/png", &pngClsid);
				std::wstring wpath;
				std::string path = exportDirectory + filenameColormap;
				wpath.assign( path.begin(), path.end() );
				gdiBitmap.Save( wpath.c_str(), &pngClsid, NULL);
			}
			Gdiplus::GdiplusShutdown(gdiplusToken);
		}
	};
}