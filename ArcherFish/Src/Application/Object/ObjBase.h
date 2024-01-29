#pragma once

class ObjBase :public KdGameObject
{
public:
	ObjBase() { Init(); }
	virtual ~ObjBase() {}

	// �X�V�Ɋւ���֐�
	virtual void PreUpdate()	override {}
	virtual void Update()		override {}
	virtual void PostUpdate()	override {}

	// �`��Ɋւ���֐�
	virtual void PreDraw()						override {}
	virtual void GenerateDepthMapFromLight()	override {}
	virtual void DrawLit()						override {}
	virtual void DrawUnLit()					override {}
	virtual void DrawBright()					override {}
	virtual void DrawSprite()					override {}
	virtual void PostDraw()						override {}
	
	virtual void Init()	override {}

	void DrawDebug()final { m_debugWire.Draw(); }

	void RegistHitObj(const std::shared_ptr<KdGameObject>& _obj) { m_wpHitObjList.push_back(_obj); }
	void SetTarget(const std::shared_ptr<KdGameObject>& _target);

	float GetNowYPos() const { return m_nowPos.y; }

protected:
	// �����蔻��p�x�N�^�[
	std::vector<std::weak_ptr<KdGameObject>>	m_wpHitObjList;

	// �ړ��֐�
	const Math::Vector3 GetVecNowMove(Math::Vector3 _vec, Math::Matrix _mat);

	// �I�u�W�F�N�g�̈ړ����x
	Math::Vector3	m_nowPos  = Math::Vector3::Zero;		// ���ݍ��W
	Math::Vector3	m_moveVec = Math::Vector3::Zero;
	float			m_moveSpd = 0.0f;

	// �s��ꗗ
	Math::Matrix	m_scaleMat  = Math::Matrix::Identity;	// �g�k�s��
	Math::Matrix	m_rotMat	= Math::Matrix::Identity;	// ��]�s��
	Math::Matrix	m_transMat  = Math::Matrix::Identity;	// ���W�s��

	// ���f���ƃJ�����̃X�}�|
	std::shared_ptr<KdModelWork> m_spModelWork = nullptr;
	std::shared_ptr<KdModelData> m_spModelData = nullptr;
	std::shared_ptr<KdCamera>	 m_spCamera	   = nullptr;

	// �E�B�[�N�|�C���^
	std::weak_ptr<KdGameObject>	 m_wpTarget;

	// �f�o�b�N�p
	KdDebugWireFrame m_debugWire;
};