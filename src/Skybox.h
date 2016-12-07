#pragma once
#include "BaseDrawable.h"
#include "BaseObject.h"

#include <string>

class CTexture;

class CSkybox :
	public CBaseObject, public CBaseDrawable
{
public:
	CSkybox(std::string path);
	~CSkybox();
};

