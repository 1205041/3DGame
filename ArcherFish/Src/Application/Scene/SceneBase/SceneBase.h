#pragma once

class SceneBase
{
public:
	SceneBase() { Init(); }
	virtual ~SceneBase() {}

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void DrawLit();
	void DrawDebug();

	void DrawSprite();

	/* �Q�b�^�[ */
	// �I�u�W�F�N�g���X�g���擾
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList() { return m_objList; }
	
	/* �Z�b�^�[ */
	void AddObject(const std::shared_ptr<KdGameObject>& _obj) { m_objList.push_back(_obj); }
	
protected:
	virtual void Init() {}
	virtual void Event() {}

	// GUI����
	virtual void ImGuiUpdate() {};

	// �I�u�W�F�N�g���X�g
	std::list<std::shared_ptr<KdGameObject>> m_objList;

	// �V�[���ؑ֗p�t���O
	bool m_SceneFlg = false;

	// BGM�ESE
	std::shared_ptr<KdSoundInstance> m_SESound;
	std::shared_ptr<KdSoundInstance> m_BGMSound;
	float m_BGMVol = 0.3f;
	float m_SEVol = 0.3f;
private:
	// imGui�\�� �L��/����
	bool m_pushAct = false;
};
