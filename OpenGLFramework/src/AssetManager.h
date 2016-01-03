#pragma once

#include <unordered_map>
#include <string>
#include "Asset.h"

//TODO: this is not yet an asset manager, more like an asset repository
class AssetManager
{
public:
	static AssetManager& getInstance() {
		// Guaranteed to be lazy initialized
		// Guaranteed that it will be destroyed correctly
		static AssetManager instance;
		return instance;
	}

	void destroy();

	template<typename T>
	void addAsset(const std::string &name, T *asset);

	bool hasAsset(const std::string &name);

	template<typename T>
	T* getAsset(const std::string &name);

private:
	AssetManager();
	~AssetManager();

	AssetManager(AssetManager const& copy);            // Not Implemented
	AssetManager& operator=(AssetManager const& copy); // Not Implemented

	std::unordered_map<std::string, Asset*> map;
};