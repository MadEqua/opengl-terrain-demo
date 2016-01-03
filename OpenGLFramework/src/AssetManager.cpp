#include "AssetManager.h"

class Material;
class Mesh;
class Texture2D;
class Texture2DArray;
class ShaderProgram;
class Shader;
class TextureCube;

template Material* AssetManager::getAsset<Material>(const std::string &name);
template Mesh* AssetManager::getAsset<Mesh>(const std::string &name);
template Texture2D* AssetManager::getAsset<Texture2D>(const std::string &name);
template TextureCube* AssetManager::getAsset<TextureCube>(const std::string &name);
template ShaderProgram* AssetManager::getAsset<ShaderProgram>(const std::string &name);

template void AssetManager::addAsset<Material>(const std::string &name, Material *asset);
template void AssetManager::addAsset<Mesh>(const std::string &name, Mesh *asset);
template void AssetManager::addAsset<Texture2D>(const std::string &name, Texture2D *asset);
template void AssetManager::addAsset<TextureCube>(const std::string &name, TextureCube *asset);
template void AssetManager::addAsset<ShaderProgram>(const std::string &name, ShaderProgram *asset);
template void AssetManager::addAsset<Shader>(const std::string &name, Shader *asset);
template void AssetManager::addAsset<TextureCube>(const std::string &name, TextureCube *asset);
template void AssetManager::addAsset<Texture2DArray>(const std::string &name, Texture2DArray *asset);

AssetManager::AssetManager() {
}

AssetManager::~AssetManager()  {
	destroy();
}

void AssetManager::destroy()  {
	for (auto it = map.begin(); it != map.end(); ++it)
		delete (*it).second;
	map.clear();
}

template<typename T>
void AssetManager::addAsset(const std::string &name, T *asset) {
	map[name] = (Asset*)asset;
}

template<typename T>
T* AssetManager::getAsset(const std::string &name) {
	auto it = map.find(name);
	if (it == map.end())
		return 0;
	else
		return (T*)((*it).second);
}

bool AssetManager::hasAsset(const std::string &name) {
	return map.find(name) != map.end();
}