#pragma once

//=========================================
// カメラクラス
//=========================================
class KdCamera
{
public:
	// コンストラクター
	KdCamera() {}

	~KdCamera() {}

	void SetProjectionMatrix(float fov, float maxRange = 2000, float minRange = 0.01f, float aspectRatio = D3D.WorkBackBuffer()->GetAspectRatio());
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj) { m_mProj = rProj; }

	void SetProjectionMatrixOrth(float width = D3D.WorkBackBuffer()->GetWidth() * 0.008, float height = D3D.WorkBackBuffer()->GetHeight() * 0.008, float maxRange = 2000, float minRange = 0.01f);

	// カメラ行列取得
	inline const DirectX::SimpleMath::Matrix& GetCameraMatrix() const { return m_mCam; }

	// ビュー行列取得
	inline const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_mView; }
	
	// 射影行列取得
	inline const DirectX::SimpleMath::Matrix& GetProjMatrix() const { return m_mProj; }

	// カメラ行列・ビュー行列
	virtual void SetCameraMatrix(const DirectX::SimpleMath::Matrix& mCam);

	// カメラ情報(ビュー・射影行列など)をシェーダへセット
	void SetToShader() const;

	const DirectX::SimpleMath::Matrix& GetCamera() { return m_mCam; }
	DirectX::SimpleMath::Matrix& WorkCamera() { return m_mCam; }

protected:

	// カメラ行列
	DirectX::SimpleMath::Matrix	m_mCam;
	// ビュー行列
	DirectX::SimpleMath::Matrix	m_mView;
	// 射影行列
	DirectX::SimpleMath::Matrix	m_mProj;
};
