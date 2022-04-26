#include "KdCamera.h"

// 射影行列の設定
void KdCamera::SetProjectionMatrix(float fov, float maxRange, float minRange, float aspectRatio)
{
	m_mProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspectRatio, minRange, maxRange);
}

void KdCamera::SetProjectionMatrixOrth(float width, float height, float maxRange, float minRange)
{
	m_mProj = DirectX::XMMatrixOrthographicLH(width, height, minRange, maxRange);
}

// カメラ行列・ビュー行列セット
void KdCamera::SetCameraMatrix(const DirectX::SimpleMath::Matrix& mCam)
{
	// カメラ行列セット
	m_mCam = mCam;

	// カメラ行列からビュー行列を算出
	m_mView = m_mCam.Invert();
}

// カメラ情報(ビュー・射影行列など)をシェーダへセット
void KdCamera::SetToShader() const
{
	// カメラ座標をセット
	SHADER->m_cb7_Camera.Work().CamPos = m_mCam.Translation();
	// ビュー行列をセット
	SHADER->m_cb7_Camera.Work().mView = m_mView;


	// 射影行列をセット
	SHADER->m_cb7_Camera.Work().mProj = m_mProj;

	// カメラ情報(ビュー行列、射影行列)を、シェーダの定数バッファへ書き込む
	SHADER->m_cb7_Camera.Write();
}