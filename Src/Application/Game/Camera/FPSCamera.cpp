#include "FPSCamera.h"

FPSCamera::FPSCamera()
{
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::Init()
{
	
}

void FPSCamera::Update()
{
}

const DirectX::SimpleMath::Matrix FPSCamera::GetRotationMatrix()
{
	return DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_degAngle.y),
		DirectX::XMConvertToRadians(m_degAngle.x),
		DirectX::XMConvertToRadians(m_degAngle.z));
}

const DirectX::SimpleMath::Matrix FPSCamera::GetRotationYMatrix()
{
	return DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_degAngle.y));
}

void FPSCamera::SetCameraMatrix(const DirectX::SimpleMath::Matrix& mWorld)
{
	DirectX::SimpleMath::Matrix trans;
	trans = trans.CreateTranslation(m_localPos);

	DirectX::SimpleMath::Matrix rotation;
	rotation = GetRotationMatrix();

	KdCamera::SetCameraMatrix(rotation * trans * mWorld);
}
