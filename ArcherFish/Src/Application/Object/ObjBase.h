#pragma once

class ObjBase :public KdGameObject
{
public:
	ObjBase() { Init(); }
	~ObjBase() {}

	// 更新に関する関数
	virtual void PreUpdate()	override {}
	virtual void Update()		override {}
	virtual void PostUpdate()	override {};

	// 描画に関する関数
	virtual void PreDraw()						override {}
	virtual void GenerateDepthMapFromLight()	override {}
	virtual void DrawLit()						override {}
	virtual void DrawUnLit()					override {}
	virtual void DrawBright()					override {}
	virtual void DrawSprite()					override {}
	virtual void PostDraw()						override {}
	
	virtual void Init()			override {}

	void DrawDebug()final { m_debugWire.Draw(); }

	void RegistHitObj(const std::shared_ptr<KdGameObject>& _obj) { m_wpHitObjList.push_back(_obj); }
	void SetTarget(const std::shared_ptr<KdGameObject>& _target);

	float GetNowYPos() const { return m_nowPos.y; }
protected:
	//srcから見たdestの角度を習得する関数
	float GetAngleDeg(Math::Vector3 _src, Math::Vector3 _dest);	//角度

	// モデルとカメラのスマポ
	std::shared_ptr<KdModelWork>		m_spModelWork	= nullptr;
	std::shared_ptr<KdModelData>		m_spModelData	= nullptr;
	std::shared_ptr<KdCamera>			m_spCamera		= nullptr;

	// ウィークポインタ
	std::weak_ptr<KdGameObject>			m_wpTarget;

	// 行列一覧
	Math::Matrix	m_scaleMat	= Math::Matrix::Identity;	// 拡縮行列
	Math::Matrix	m_rotMat	= Math::Matrix::Identity;	// 回転行列
	Math::Matrix	m_transMat	= Math::Matrix::Identity;	// 座標行列
	
	// オブジェクトの移動速度
	Math::Vector3	m_nowPos	= Math::Vector3::Zero;		// 現在座標
	Math::Vector3	m_moveVec	= Math::Vector3::Zero;
	float			m_moveSpd	= 0.0f;
	
	// マウス座標
	POINT m_FixMousePos;

	// 当たり判定用ベクター
	std::vector<std::weak_ptr<KdGameObject>>	m_wpHitObjList;

	// デバック用
	KdDebugWireFrame m_debugWire;
};