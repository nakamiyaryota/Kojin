#include "TPSCamera.h"

void TPSCamera::Init()
{
}

void TPSCamera::Update()
{
}

const DirectX::SimpleMath::Matrix TPSCamera::GetRotationMatrix()
{
	return DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_degAngle.y),
		DirectX::XMConvertToRadians(m_degAngle.x),
		DirectX::XMConvertToRadians(m_degAngle.z));
}

const DirectX::SimpleMath::Matrix TPSCamera::GetRotationYMatrix()
{
	return DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_degAngle.y));
}

void TPSCamera::SetCameraMatrix(const DirectX::SimpleMath::Matrix& mWorld)
{
	DirectX::SimpleMath::Matrix gazeTrans;
	gazeTrans = gazeTrans.CreateTranslation(m_localGazePos);

	Math::Matrix localPos;
	localPos = Math::Matrix::CreateTranslation(m_localPos);

	Math::Matrix rotation;
	rotation = GetRotationMatrix();

	KdCamera::SetCameraMatrix(localPos * rotation * gazeTrans * mWorld);
}
