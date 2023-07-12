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
	void DrawSprite();

	void DrawDebug();

	// �I�u�W�F�N�g���X�g���擾
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList() { return m_objList; }
	void AddObject(const std::shared_ptr<KdGameObject>& _obj) { m_objList.push_back(_obj); }
protected:
	virtual void Init() {}
	virtual void Event() {}

	// GUI����
	virtual void ImGuiUpdate() {};

	std::list<std::shared_ptr<KdGameObject>> m_objList;

	std::unique_ptr<KdCamera> m_camera = nullptr;

};