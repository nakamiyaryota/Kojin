#include "ResourceFactory.h"


void ResourceFactory::Release()
{
	m_modelMap.clear();
	m_textureMap.clear();
}

const std::shared_ptr<KdModelData> ResourceFactory::GetModelData(const std::string& filename)
{
	// filenameのモデルが既にあるかどうか？
	// 型推論(※型の予測が固定的で容易な場合のみ使う！)
	auto foundItr = m_modelMap.find(filename);
	
	// リストにある = モデルデータが既にアプリ上に１つ存在している = 既に読み込んでいる
	if (foundItr != m_modelMap.end())
	{
		// 見つけたデータを返す
		return (*foundItr).second;
	}

	// リストに無い = モデルデータがアプリ上に１つも確保されていない = 読み込まれていない
	std::shared_ptr<KdModelData> newModelData = std::make_shared<KdModelData>();

	// モデルを読み込む
	if (newModelData->Load(filename))
	{
		// リストに追加
		m_modelMap.insert(std::pair <std::string, std::shared_ptr<KdModelData>>(filename, newModelData));

		return m_modelMap[filename];
	}

	std::string errorMes = "ResourceFactory::GetModelData() Failed " + filename;
	assert(0 && errorMes.c_str());

	// 読み込みが失敗したらnullptrを返す
	return nullptr;
}

const std::shared_ptr<KdTexture> ResourceFactory::GetTexture(const std::string& filename)
{
	// filenameのテクスチャが既にあるかどうか？
	// 型推論(※型の予測が固定的で容易な場合のみ使う！)
	auto foundItr = m_textureMap.find(filename);

	// リストにある = テクスチャが既にアプリ上に１つ存在している = 既に読み込んでいる
	if (foundItr != m_textureMap.end())
	{
		// 見つけたテクスチャを返す
		return (*foundItr).second;
	}

	// リストに無い = テクスチャがアプリ上に１つも確保されていない = 読み込まれていない
	std::shared_ptr<KdTexture> newTexture = std::make_shared<KdTexture>();

	// テクスチャを読み込む
	if (newTexture->Load(filename))
	{
		m_textureMap[filename] = newTexture;

		return newTexture;
	}

	std::string errorMes = "ResourceFactory::GetTexture() Failed " + filename;
	assert(0 && errorMes.c_str());

	// 読み込みが失敗したらnullptrを返す
	return nullptr;
}