#pragma once

class SceneBase;
class TitleScene;
class GameScene;
class ResultScene;

class SceneManager
{
public:
	enum class SceneType
	{
		Title,
		Game,
		Result,
	};

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void DrawLit();
	void DrawSprite();

	void DrawDebug();

	void Init();
	
	void SetNextScene(const SceneType& _next) { m_nextSceneType = _next; }

	// オブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList() const;
	void AddObject(const std::shared_ptr<KdGameObject>& _obj);

private:
	
	void ChangeScene(const SceneType& _type);

	// 現在のシーンを管理しているポインタ
	std::shared_ptr<SceneBase>	m_spCurtScene = nullptr;

	std::shared_ptr<TitleScene>		title;
	std::shared_ptr<GameScene>		game;
	std::shared_ptr<ResultScene>	result;

	// 現在のシーンを管理している変数
	SceneType m_curtSceneType = SceneType::Game;

	// 次のシーンを管理している変数
	SceneType m_nextSceneType = m_curtSceneType;

	/* --- シングルトンパターン --- */
private:
	SceneManager() { Init(); }
	~SceneManager() {}

public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}
};