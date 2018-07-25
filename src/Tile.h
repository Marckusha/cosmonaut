#pragma once

class Tile {
public:
	Tile();

	explicit Tile(Render::Texture* text);

	Tile(Render::Texture* text, const FRect& uv, const IPoint& tileSize, const IPoint& pos);

	void Draw();

	inline void SetPosition(const IPoint& pos) { _screenRect = IRect(pos.x, pos.y, _screenRect.width, _screenRect.height); }
	inline void SetUVCoordinate(const FRect& uv) { _uvRect = uv; }
	inline void SetTileSize(const IPoint& size) { _screenRect = IRect(_screenRect.x, _screenRect.y, size.x, size.y); }

	inline const IPoint GetTileSize() const { return IPoint(_screenRect.width, _screenRect.height);  }
	inline const FRect GetUVCoordinate() const { return _uvRect; }
	inline const IPoint GetPosition() const { return IPoint(_screenRect.x, _screenRect.y);  }

private:
	Render::Texture* _texture = nullptr;
	FRect _uvRect = FRect();
	IRect _screenRect = IRect();
};
