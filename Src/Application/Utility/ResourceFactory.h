#pragma once

//====================================
// リソース管理クラス
// Flyweightパターン
//====================================
class ResourceFactory
{
public:
	ResourceFactory() {}
	~ResourceFactory() { Release(); }

	const std::shared_ptr<KdModelData> GetModelData(const std::string& filename);

	const std::shared_ptr<KdTexture> GetTexture(const std::string& filename);

private:
	void Release();

	// モデルデータ管理マップ
	std::unordered_map<std::string, std::shared_ptr<KdModelData>> m_modelMap;

	// テクスチャデータ管理マップ
	std::unordered_map<std::string, std::shared_ptr<KdTexture>> m_textureMap;

};