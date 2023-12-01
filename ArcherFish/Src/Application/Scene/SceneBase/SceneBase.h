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

	/* ゲッター */
	// オブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList() { return m_objList; }
	
	/* セッター */
	void AddObject(const std::shared_ptr<KdGameObject>& _obj) { m_objList.push_back(_obj); }
	
protected:
	virtual void Init() {}
	virtual void Event() {}

	// オブジェクトリスト
	std::list<std::shared_ptr<KdGameObject>> m_objList;

	// 長押しシーン切替防止
	bool m_pushAct = false;
};
