#pragma once

#include "FPSCamera.h"

class TPSCamera : public FPSCamera
{
public:
	TPSCamera(){}
	~TPSCamera(){}

	// カメラ行列のセット
	void SetCameraMatrix(const DirectX::SimpleMath::Matrix& mWorld) override;

	// 注視点のローカル座標をセット
	void SetLocalGazePosition(const DirectX::SimpleMath::Vector3& pos)
	{
		m_localGazePos = pos;
	}

private:
	// 注視点調整用移動ベクトル
	DirectX::SimpleMath::Vector3 m_localGazePos;
};