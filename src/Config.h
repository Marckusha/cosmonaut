#pragma once
#include "Box2D/Box2D.h"

class Config {
public:
	static Config& GetInstance() {
		static Config c;
		return c;
	}

	float GetConstantsByKey(const std::string& key) const;
	std::shared_ptr<b2World> GetWorld() const { return _world; }



private:
	Config();
	~Config() {}

	Config(Config const&) = delete;
	Config& operator=(Config const&) = delete;

private:
	std::map<std::string, float> _mapConstants;
	std::shared_ptr<b2World> _world;
};
