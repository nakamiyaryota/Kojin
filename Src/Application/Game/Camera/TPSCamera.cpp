#include "TPSCamera.h"

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
