#pragma once
#include "../SceneObject.h"

class ClearObject : public SceneObject
{
public:
	ClearObject() {}
	~ClearObject() {}

	void Init() override;



private:
	void Release();
};