#pragma once

#include "../SceneObject.h"

class TitleObject : public SceneObject
{
public:
	TitleObject();
	~TitleObject();

	void Init() override;


private:
	void Release();

};
