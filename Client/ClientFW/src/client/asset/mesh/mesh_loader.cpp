#include "stdafx.h"
#include "client/asset/mesh/mesh_loader.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/primitive/vertex.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/material/material.h"
#include "client/physics/core/bounding_mesh.h"
#include "client/physics/collision/mesh_bounding_tree.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/version.h>

namespace client_fw
{
	SPtr<Mesh> MeshLoader::LoadMesh(const std::string& path, const std::string& extension) const
	{
		SPtr<Mesh> mesh = nullptr;

		switch (HashCode(extension.c_str()))
		{
			//mesh = LoadObj(path, extension);
			//break;
		case HashCode(".obj"):
		case HashCode(".fbx"):
		case HashCode(".glb"):
		case HashCode(".gltf"):
			mesh = LoadStaticMeshFromAssimp(path, extension);
			break;
		default:
			LOG_ERROR("Files in {0} format cannot be supported", extension);
			break;
		}

		return mesh;
	}

	struct CombineData
	{
		std::string mtl_name;
		std::vector<UINT> pos_indices;
		std::vector<UINT> tex_indices;
		std::vector<UINT> normal_indices;
	};

	SPtr<StaticMesh> MeshLoader::LoadObj(const std::string& path, const std::string& extension) const
	{
		std::ifstream obj_file(path);

		if (obj_file.is_open() == false)
		{
			LOG_ERROR("Could not find path : [{0}]", path);
			return nullptr;
		}

		std::string parent_path = file_help::GetParentPathFromPath(path);
		std::string stem = file_help::GetStemFromPath(path);

		UINT lod = 0;
		SPtr<StaticMesh> mesh = CreateSPtr<StaticMesh>();

		while (lod < 5)
		{
			std::vector<Vec3> positions;
			std::vector<Vec3> normals;
			std::vector<Vec2> tex_coords;
			std::map<std::string, SPtr<Material>> materials;
			std::vector<CombineData> combine_data;
			UINT combine_data_index = 0;

			std::stringstream ss;
			std::string line;
			std::string prefix;

			Vec3 temp_vec;
			UINT temp_uint = 0;
			std::string temp_string;

			while (std::getline(obj_file, line))
			{
				ss.clear();
				prefix.clear();
				ss.str(line);
				ss >> prefix;

				switch (HashCode(prefix.c_str()))
				{
				case HashCode("mtllib"):
				{
					ss >> temp_string;
					materials = AssetStore::LoadMaterials(file_help::GetParentPathFromPath(path) + "\\" + temp_string);
					break;
				}
				case HashCode("v"):
					ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
					temp_vec.z *= -1.0f;
					positions.emplace_back(std::move(temp_vec));
					break;
				case HashCode("vt"):
					ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
					temp_vec.y = 1.0f - temp_vec.y;
					tex_coords.emplace_back(Vec2{ temp_vec.x, temp_vec.y });
					break;
				case HashCode("vn"):
					ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
					temp_vec.z *= -1.0f;
					normals.emplace_back(std::move(temp_vec));
					break;
				case HashCode("usemtl"):
				{
					ss >> temp_string;
					auto iter = std::find_if(combine_data.cbegin(), combine_data.cend(),
						[temp_string](const CombineData& data) {
							return data.mtl_name == temp_string;
						});
					if (iter == combine_data.cend())
					{
						combine_data.push_back({ temp_string });
						combine_data_index = static_cast<UINT>(combine_data.size()) - 1;
					}
					else
					{
						combine_data_index = static_cast<UINT>(std::distance(combine_data.cbegin(), iter));
					}
				}
				break;
				case HashCode("f"):
				{
					int count = 0;

					while (ss >> temp_uint)
					{
						if (count == 0)
							combine_data[combine_data_index].pos_indices.emplace_back(temp_uint - 1);
						else if (count == 1)
							combine_data[combine_data_index].tex_indices.emplace_back(temp_uint - 1);
						else if (count == 2)
							combine_data[combine_data_index].normal_indices.emplace_back(temp_uint - 1);

						if (ss.peek() == '/')
						{
							++count;
							ss.ignore();
						}
						else if (ss.peek() == ' ')
						{
							ss.ignore(1, ' ');
							count = 0;
						}
					}
				}
				break;
				default:
					break;
				}
			}

			mesh->CreateDataForLodMesh(lod);

			UINT vertex_count = 0;
			for (const auto& data : combine_data)
				vertex_count += static_cast<UINT>(data.pos_indices.size());

			std::vector<TextureLightNormalMapVertex> vertices;
			vertices.reserve(vertex_count);
			std::vector<Triangle> triangles;
			triangles.reserve(vertex_count / 3);

			vertex_count = 0;
			for (const auto& data : combine_data)
			{
				UINT count = 0;
				for (size_t i = 0; i < data.pos_indices.size() / 3; ++i)
				{
					size_t index = i * 3;

					Vec3 v1 = positions[data.pos_indices[index + 2]];
					Vec3 v2 = positions[data.pos_indices[index + 1]];
					Vec3 v3 = positions[data.pos_indices[index]];

					Vec3 normal = vec3::Cross(v3 - v1, v2 - v1, true);
					if (normal == vec3::ZERO) continue;

					triangles.emplace_back(Triangle{ v1, v2, v3, normal });

					Vec2 uv1 = tex_coords[data.tex_indices[index + 2]];
					Vec2 uv2 = tex_coords[data.tex_indices[index + 1]];
					Vec2 uv3 = tex_coords[data.tex_indices[index]];

					Vec3 delta_pos1 = v2 - v1;
					Vec3 delta_pos2 = v3 - v1;

					Vec2 delta_uv1 = uv2 - uv1;
					Vec2 delta_uv2 = uv3 - uv1;

					float f = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv2.x * delta_uv1.y);

					Vec3 tangent;
					tangent.x = f * (delta_uv2.y * delta_pos1.x - delta_uv1.y * delta_pos2.x);
					tangent.y = f * (delta_uv2.y * delta_pos1.y - delta_uv1.y * delta_pos2.y);
					tangent.z = f * (delta_uv2.y * delta_pos1.z - delta_uv1.y * delta_pos2.z);

					Vec3 bitangent;
					bitangent.x = f * (-delta_uv2.x * delta_pos1.x + delta_uv1.x * delta_pos2.x);
					bitangent.y = f * (-delta_uv2.x * delta_pos1.y + delta_uv1.x * delta_pos2.y);
					bitangent.z = f * (-delta_uv2.x * delta_pos1.z + delta_uv1.x * delta_pos2.z);

					for (INT j = 2; j >= 0; --j)
					{
						TextureLightNormalMapVertex vertex;
						vertex.SetPosition(positions[data.pos_indices[index + j]]);
						vertex.SetTexCoord(tex_coords[data.tex_indices[index + j]]);
						vertex.SetNormal(normals[data.normal_indices[index + j]]);
						vertex.SetTangent(tangent);
						vertex.SetBitangent(bitangent);
						vertices.emplace_back(std::move(vertex));
					}

					count += 3;
				}

				mesh->AddMeshVertexInfo(lod, { count, vertex_count });
				mesh->AddMaterial(lod, std::move(materials[data.mtl_name]));

				vertex_count += count;
			}

			const auto& vertex_info = mesh->GetVertexInfo(lod);
			if (vertex_info->CreateVertexBlob<TextureLightNormalMapVertex>(vertex_count) == false)
			{
				LOG_ERROR("Could not create blob for vertex");
				return nullptr;
			}
			vertex_info->CopyData(vertices.data(), vertex_count);

			if (lod == 0)
			{
#ifdef SHOW_TREE_INFO
				LOG_INFO("Triangle Tree : {0}", path);
#endif // SHOW_TREE_INFO
				BOrientedBox box = BOrientedBox(std::move(positions));
				mesh->SetOrientBox(box);
				/*auto bounding_tree = CreateSPtr<KDTree>();
				bounding_tree->Initialize(box, triangles);
				mesh->SetBoundingTree(std::move(bounding_tree));*/
			}

			++lod;
			std::string lod_path = parent_path + "/" + stem + "_lod" + std::to_string(lod) + extension;
			obj_file = std::ifstream(lod_path);

			if (obj_file.is_open() == false)
				break;
		}

		return mesh;
	}

	SPtr<StaticMesh> MeshLoader::LoadStaticMeshFromAssimp(const std::string& path, const std::string& extension) const
	{
		UINT flag = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace |
			aiProcess_FlipUVs | aiProcess_FlipWindingOrder | aiProcess_OptimizeMeshes | aiProcess_SortByPType;
		const aiScene* scene = aiImportFile(path.c_str(), flag);

		if (scene == nullptr || scene->HasMeshes() == false)
		{
			LOG_ERROR("Could not find path : [{0}]", path);
			return nullptr;
		}

		/*LOG_INFO("Num Meshes : {0}, Num Materials : {1}", scene->mNumMeshes, scene->mNumMaterials);
		for (int i = 0; i < scene->mNumMaterials; ++i)
		{
			LOG_INFO("Material {0} : {1}", i, scene->mMaterials[i]->GetName().C_Str());
		}*/

		std::string parent_path = file_help::GetParentPathFromPath(path);
		std::string stem = file_help::GetStemFromPath(path);

		UINT lod = 0;
		SPtr<StaticMesh> mesh = CreateSPtr<StaticMesh>();

		while (lod < 5)
		{
			std::map<std::string, SPtr<Material>> materials = AssetStore::LoadMaterials(path, scene);
			mesh->CreateDataForLodMesh(lod);

			UINT vertex_count = 0;

			std::vector<Vec3> positions;
			std::vector<TextureLightNormalMapVertex> vertices;

			for (int i = 0; i < scene->mNumMeshes; ++i)
			{
				const aiMesh* ai_mesh = scene->mMeshes[i];
				//LOG_INFO("Mesh name : {0}, count : {1}", ai_mesh->mName.C_Str(), ai_mesh->mNumVertices);
				//LOG_INFO("Material : {0}", scene->mMaterials[ai_mesh->mMaterialIndex]->GetName().C_Str());

				for (UINT v_index = 0; v_index < ai_mesh->mNumVertices; ++v_index)
				{
					TextureLightNormalMapVertex vertex;
					Vec3 position = Vec3{ ai_mesh->mVertices[v_index].x, ai_mesh->mVertices[v_index].y, ai_mesh->mVertices[v_index].z };
					Vec3 normal = Vec3{ ai_mesh->mNormals[v_index].x,ai_mesh->mNormals[v_index].y,ai_mesh->mNormals[v_index].z };
					Vec2 texcoord = Vec2{ ai_mesh->mTextureCoords[0][v_index].x, ai_mesh->mTextureCoords[0][v_index].y };
					Vec3 tangent = Vec3{ ai_mesh->mTangents[v_index].x,ai_mesh->mTangents[v_index].y, ai_mesh->mTangents[v_index].z };
					Vec3 bitangent = Vec3{ ai_mesh->mBitangents[v_index].x, ai_mesh->mBitangents[v_index].y, ai_mesh->mBitangents[v_index].z };

					positions.emplace_back(position);
					vertex.SetPosition(std::move(position));
					vertex.SetNormal(std::move(normal));
					vertex.SetTexCoord(std::move(texcoord));
					vertex.SetTangent(std::move(tangent));
					vertex.SetBitangent(std::move(bitangent));
					vertices.emplace_back(std::move(vertex));
				}

				mesh->AddMeshVertexInfo(lod, { ai_mesh->mNumVertices, vertex_count });
				mesh->AddMaterial(lod, std::move(materials[scene->mMaterials[ai_mesh->mMaterialIndex]->GetName().C_Str()]));

				vertex_count += ai_mesh->mNumVertices;
			}

			//LOG_INFO("Vertex count : {0}", vertex_count);

			const auto& vertex_info = mesh->GetVertexInfo(lod);
			if (vertex_info->CreateVertexBlob<TextureLightNormalMapVertex>(vertex_count) == false)
			{
				LOG_ERROR("Could not create blob for vertex");
				return nullptr;
			}
			vertex_info->CopyData(vertices.data(), vertex_count);

			if (lod == 0)
			{
				BOrientedBox box = BOrientedBox(std::move(positions));
				mesh->SetOrientBox(box);
			}

			++lod;

			std::string lod_path = parent_path + "/" + stem + "_lod" + std::to_string(lod) + extension;
			scene = aiImportFile(lod_path.c_str(), flag);

			if (scene == nullptr || scene->HasMeshes() == false)
				break;
		}
		
		return mesh;
	}
}
