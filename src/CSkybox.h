#pragma once
#include "CBaseDrawable.h"
#include "CBaseObject.h"

#include <string>

class CTexture;

class CSkybox :
	public CBaseObject, public CBaseDrawable
{
public:
	CSkybox(std::string path);
	~CSkybox();

	void Draw();

protected:
	CTexture * _texture;
};

