#include "stdafx.h"
#include "Config.h"

Config::Config() {
	rapidxml::file<> file("config.xml");
	rapidxml::xml_document<> doc;
	doc.parse<0>(file.data());

	rapidxml::xml_node<>* root = doc.first_node();
	for (auto it = root->first_node(); it != nullptr; it = it->next_sibling(0)) {
		float value = utils::lexical_cast<float>(it->first_attribute("value")->value());
		_mapConstants.insert(std::map<std::string, float>::value_type(it->name(), value));
	}

	b2Vec2 gravity(0.f, 0.f);
	_world = std::shared_ptr<b2World>(new b2World(gravity));
}

float Config::GetConstantsByKey(const std::string& key) const {
	if (_mapConstants.count(key)) {
		return _mapConstants.at(key);
	}
	else {
		return 1.f;
	}
}