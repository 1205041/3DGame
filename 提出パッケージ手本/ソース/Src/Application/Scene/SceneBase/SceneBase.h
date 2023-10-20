#pragma once

class SceneBase
{
public:
	SceneBase() { Init(); }
	~SceneBase() {}

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

	// �I�u�W�F�N�g���X�g
	std::list<std::shared_ptr<KdGameObject>> m_objList;
};
