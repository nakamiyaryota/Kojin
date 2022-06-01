#pragma once

class SceneObject
{
public:
	SceneObject() {}
	~SceneObject() {}

	virtual void Init() {}
	void Draw2D();
	void Update();



private:
	void Release() {}

protected:
	float m_alpha = 1.0f;
	float a = -0.02f;

	std::shared_ptr<KdTexture> m_spTex[2];

};