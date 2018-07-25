#include  "stdafx.h"
#include "TMXTiledMap.h"
#include "Config.h"

std::vector<int> GetVectorFromString(const std::string& s);

TMXTiledMap::TMXTiledMap()
{}

bool TMXTiledMap::InitWithTMXFile(const char* nameFile) {
	try {
		rapidxml::file<> file(nameFile);
		rapidxml::xml_document<> doc;
		doc.parse<0>(file.data());

		rapidxml::xml_node<>* root = doc.first_node();
		rapidxml::xml_node<>* res = root->first_node();

		if (!root) {
			Assert(false);
			throw std::runtime_error("No root node");
		}

		Map myMap;

		if (utils::equals(root->name(), "map")) {
			InitMapSize(root);
		}

		for (auto it = res; it != nullptr; it = it->next_sibling()) {
			if (utils::equals(it->name(), "tileset")) {
				std::string nameMap = utils::lexical_cast<std::string>(res->first_attribute("name")->value());
				_textureMap = Core::resourceManager.Get<Render::Texture>(nameMap);
				myMap = GetMapTileSet(it->first_node());
			}
			else if (utils::equals(it->name(), "layer")) {
				std::string stringValue = it->first_node()->value();
				std::vector<int> vect = GetVectorFromString(stringValue);
				InitTileSet(vect, myMap);
			}
			else if (utils::equals(it->name(), "objectgroup")) {
				InitDynamicObjects(it->first_node());
			}
		}
	}
	catch (std::exception const& e) {
		Log::log.WriteError(e.what());
		Assert(false);
	}
	return true;
}

bool TMXTiledMap::InitMapSize(rapidxml::xml_node<>* node) {

	int tileheight = utils::lexical_cast<int>(node->first_attribute("tileheight")->value());
	int tilewidth = utils::lexical_cast<int>(node->first_attribute("tilewidth")->value());
	_tileSize = IPoint(tilewidth, tileheight);

	int height = utils::lexical_cast<int>(node->first_attribute("height")->value());
	int width = utils::lexical_cast<int>(node->first_attribute("width")->value());
	_mapSize = IPoint(width, height);

	return true;
}

bool TMXTiledMap::InitTileSet(const std::vector<int>& vect, const Map& m) {

	Config& instance = Config::GetInstance();
	auto world = instance.GetWorld();
	
	float widthText = _textureMap->getBitmapRect().width / _tileSize.x;
	float heightText = _textureMap->getBitmapRect().height / _tileSize.y;

	std::vector<Tile> tiles;

	for (size_t i = 0; i < vect.size(); ++i) {
		int id = vect[i];

		if (id <= 0) {
			continue;
		}

		float deg = 0.001f;
		FRect uv = FRect(((id - 1) % int(widthText)) / widthText + deg, (id % int(widthText)) / widthText - deg,
			(heightText - 1 - (id - 1) / int(widthText)) / heightText, (heightText - (id) / int(widthText)) / heightText);
		IPoint position = IPoint((i % _mapSize.x) * _tileSize.x, (_mapSize.y - 1 - (i / _mapSize.x)) * _tileSize.y);
		Tile tile = Tile(_textureMap, uv, _tileSize, position);
		tiles.push_back(tile);
		
		if (m.count(id - 1)) {
			auto info = m.at(id - 1);
			float y = 0.f;
			if (info.posOnBody.y == 0) {
				y = position.y + (128 - info.sizeBody.y) + info.sizeBody.y / 2.f;
			}
			else if(info.sizeBody.y + info.posOnBody.y >= _tileSize.y - 1 && info.sizeBody.y + info.posOnBody.y <= _tileSize.y + 1.f){
				y = position.y + info.sizeBody.y / 2.f;
			}
			else {
				y = position.y + info.posOnBody.y + info.sizeBody.y / 2.f;
			}
			StaticBody staticBody(FPoint(position.x + info.posOnBody.x + info.sizeBody.x /2.f, y)
				, FPoint(info.sizeBody.x / 2.f, info.sizeBody.y /2.f));
			staticBody.SetUserData("wall");
		}
	}

	_layers.push_back(tiles);

	return true;
}

bool TMXTiledMap::InitDynamicObjects(rapidxml::xml_node<>* node) {

	for (auto it = node; it != nullptr; it = it->next_sibling()) {
		std::string name = utils::lexical_cast<std::string>(it->first_attribute("name")->value());
		float posX = utils::lexical_cast<float>(it->first_attribute("x")->value());
		float posY = utils::lexical_cast<float>(it->first_attribute("y")->value());
		_dynamicObjects.insert(MultiMap::value_type(name, FPoint(posX, posY)));
	}

	return true;
}

void TMXTiledMap::Draw() {

	for (auto l : _layers) {
		for (auto t : l) {
			t.Draw();
		}
	}
}

const TMXTiledMap::Map TMXTiledMap::GetMapTileSet(rapidxml::xml_node<>* res) {

	Map myMap;

	for (auto it = res; it != nullptr; it = it->next_sibling()) {

		if (utils::equals(it->name(), "image")) {
			continue;
		}

		auto it1 = it->first_node()->first_node();

		if (it1 == nullptr) {
			continue;
		}

		BodyInfo bIn;

		int id = utils::lexical_cast<int>(it->first_attribute("id")->value());
		float posX = utils::lexical_cast<float>(it1->first_attribute("x")->value());
		float posY = utils::lexical_cast<float>(it1->first_attribute("y")->value());
		float height = utils::lexical_cast<float>(it1->first_attribute("height")->value());
		float width = utils::lexical_cast<float>(it1->first_attribute("width")->value());

		bIn.posOnBody = FPoint(posX, posY);
		bIn.sizeBody = FPoint(width, height);

		myMap.insert(Map::value_type(id, bIn));
	}

	res = res->next_sibling();

	return myMap;
}

std::vector<int> GetVectorFromString(const std::string& str) {
	std::vector<int> vect;
	std::string digitStr = "";
	for (size_t i = 0; i < str.size(); ++i) {

		if (str[i] == '\n' || str[i] == '\r') {
			continue;
		}

		if (isdigit(str[i])) {
			digitStr += str[i];
		}
		else {
			int digit = utils::lexical_cast<int>(digitStr.c_str());
			digitStr = "";
			vect.push_back(digit);
		}
	}
	int digit = utils::lexical_cast<int>(digitStr.c_str());
	vect.push_back(digit);
	return vect;
}