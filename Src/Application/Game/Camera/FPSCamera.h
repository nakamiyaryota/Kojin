#pragma once

class FPSCamera : public KdCamera
{
public:
	FPSCamera();
	~FPSCamera();

	void Init();

	virtual void Update();

	// 視線用のローカル座標の指定
	void SetLocalPos(DirectX::SimpleMath::Vector3& pos) { m_localPos = pos; }

	void SetLocalRotX(float rotX) { m_degAngle.x = rotX; }
	void SetLocalRotY(float rotY) { m_degAngle.y = rotY;}

	const DirectX::SimpleMath::Matrix GetRotationMatrix();

	const DirectX::SimpleMath::Matrix GetRotationYMatrix();

	const float GetRotationYAngle() const { return m_degAngle.y; }

	// カメラ行列のセット
	virtual void SetCameraMatrix(const DirectX::SimpleMath::Matrix& mWorld) override ;

	// カメラのX軸の制限をかける関数
	void SetClampAngle(float minAng, float maxAng)
	{ m_minAngleX = minAng; m_maxAngleX = maxAng; }

	const DirectX::SimpleMath::Vector3& GetRotationAngles() const { return m_degAngle; }

protected:
	// 視線用のローカル座標(キャラクターの原点から指定の位置へ移動する座標ベクトル)
	DirectX::SimpleMath::Vector3 m_localPos;

	// 各軸の回転情報
	DirectX::SimpleMath::Vector3 m_degAngle;

	// X軸の回転制御
	float m_minAngleX = -FLT_MAX;
	float m_maxAngleX = FLT_MAX;

	float m_rotationSpd = 0.25f;
};