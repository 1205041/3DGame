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

	// GUI処理
	virtual void ImGuiUpdate() {};

	// オブジェクトリスト
	std::list<std::shared_ptr<KdGameObject>> m_objList;

	// シーン切替用フラグ
	bool m_SceneFlg = false;

	// BGM・SE
	std::shared_ptr<KdSoundInstance> m_SESound;
	std::shared_ptr<KdSoundInstance> m_BGMSound;
	float m_BGMVol = 0.3f;
	float m_SEVol = 0.3f;
private:
	// imGui表示 有効/無効
	bool m_pushAct = false;
};
