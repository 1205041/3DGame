#pragma once

class ObjBase :public KdGameObject
{
public:
	ObjBase() { Init(); }
	~ObjBase() {}

	// �X�V�Ɋւ���֐�
	virtual void PreUpdate()	override {}
	virtual void Update()		override {}
	virtual void PostUpdate()	override;

	// �`��Ɋւ���֐�
	virtual void PreDraw()						override {}
	virtual void GenerateDepthMapFromLight()	override {}
	virtual void DrawLit()						override {}
	virtual void DrawUnLit()					override {}
	virtual void DrawBright()					override {}
	virtual void DrawSprite()					override;
	virtual void PostDraw()						override {}
	
	virtual void Init()			override {}
	
	virtual void SetTexture(std::string _fileName) { m_tex.Load(_fileName); }

	void DrawDebug()final { m_debugWire.Draw(); }

	void RegistHitObj(const std::shared_ptr<KdGameObject>& _obj) { m_wpHitObjList.push_back(_obj); }
protected:
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void UpdateCollision();

	//src���猩��dest�̊p�x���K������֐�
	float GetAngleDeg(Math::Vector3 _src, Math::Vector3 _dest);	//�p�x

	// ���f���ƃJ�����̃X�}�|
	std::shared_ptr<KdModelWork>		m_spModel	= nullptr;
	std::shared_ptr<KdCamera>			m_spCamera	= nullptr;

	// �e�N�X�`���\��
	KdTexture m_tex;
	Math::Vector3 m_texPos = Math::Vector3::Zero;
	Math::Matrix m_texMat = Math::Matrix::Identity;
	
	// �s��ꗗ
	Math::Matrix m_scaleMat	= Math::Matrix::Identity;	// �g�k�s��
	Math::Matrix m_rotMat		= Math::Matrix::Identity;	// ��]�s��
	Math::Matrix m_transMat	= Math::Matrix::Identity;	// ���W�s��
	
	// �d��
	float	m_gravity = 0.0f;
	
	// �I�u�W�F�N�g�̈ړ����x
	float			m_moveSpd	= 0.0f;
	Math::Vector3	m_nowPos	= Math::Vector3::Zero;
	Math::Vector3	m_moveVec	= Math::Vector3::Zero;
	
	// �����蔻��p�ϐ�
	std::vector<std::weak_ptr<KdGameObject>>	m_wpHitObjList;
	const float		enableStepHigh	= 0.2f;
	float			maxOverLap		= 0.0f;
	int				notHitCnt		= 0;
	bool			hit				= false;
	Math::Vector3	hitPos			= Math::Vector3::Zero;
	
	// �f�o�b�N�p
	KdDebugWireFrame m_debugWire;
};