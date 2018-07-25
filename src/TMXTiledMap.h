#pragma once
#include "Tile.h"
#include "StaticBody.h"

class TMXTiledMap {
public:
	typedef std::multimap<std::string, FPoint> MultiMap;

	TMXTiledMap();

	inline IPoint GetTileSize() const { return _tileSize; }
	inline IPoint GetMapSize() const { return _mapSize;  }
	inline const MultiMap GetMultiMapDynamicObjects() const { return _dynamicObjects;  }

	bool InitWithTMXFile(const char* nameFile);

	void Draw();
	
private:
	struct BodyInfo {
		FPoint sizeBody;
		FPoint posOnBody;
	};

	typedef std::map<int, BodyInfo> Map;

	bool InitTileSet(const std::vector<int>& vect, const Map& m);
	bool InitMapSize(rapidxml::xml_node<>* node);
	bool InitDynamicObjects(rapidxml::xml_node<>* node);

	const Map GetMapTileSet(rapidxml::xml_node<>* node);

private:
	Render::Texture* _textureMap = nullptr;
	std::vector< std::vector<Tile> > _layers;

	IPoint _tileSize = IPoint();
	IPoint _mapSize = IPoint();
	MultiMap _dynamicObjects;
};
