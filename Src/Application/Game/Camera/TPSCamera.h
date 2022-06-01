#pragma once

class TPSCamera : public KdCamera
{
public:
	TPSCamera(){}
	~TPSCamera(){}

	void Init();

	virtual void Update();

	// 視線用のローカル座標の指定
	void SetLocalPos(DirectX::SimpleMath::Vector3& pos) { m_localPos = pos; }

	void SetLocalRotX(float rotX) { m_degAngle.x = rotX; }
	void SetLocalRotY(float rotY) { m_degAngle.y = rotY; }

	const DirectX::SimpleMath::Matrix GetRotationMatrix();

	const DirectX::SimpleMath::Matrix GetRotationYMatrix();

	// カメラ行列のセット
	void SetCameraMatrix(const DirectX::SimpleMath::Matrix& mWorld);

	// 注視点のローカル座標をセット
	void SetLocalGazePosition(const DirectX::SimpleMath::Vector3& pos)
	{
		m_localGazePos = pos;
	}

private:
	// 注視点調整用移動ベクトル
	DirectX::SimpleMath::Vector3 m_localGazePos;

	// 視線用のローカル座標(キャラクターの原点から指定の位置へ移動する座標ベクトル)
	DirectX::SimpleMath::Vector3 m_localPos;

	// 各軸の回転情報
	DirectX::SimpleMath::Vector3 m_degAngle;
};