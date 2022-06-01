#pragma once

#include "../SceneObject.h"

class ResultObject : public SceneObject
{
public:
	ResultObject();
	~ResultObject();

	void Init() override;



private:
	void Release();

};
