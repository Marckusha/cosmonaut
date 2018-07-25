#include "stdafx.h"
#include "Tile.h"

Tile::Tile() 
{	}

Tile::Tile(Render::Texture* text)
	: _texture(text)
{	}

Tile::Tile(Render::Texture* text, const FRect& uv, const IPoint& tileSize, const IPoint& pos)
	: _texture(text)
	, _uvRect(uv)
	, _screenRect(IRect(pos, tileSize.x, tileSize.y))
{	}

void Tile::Draw() {
	Render::device.PushMatrix();

	_texture->Draw(_screenRect, _uvRect);

	Render::device.PopMatrix();
}