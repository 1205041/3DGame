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

	// �I�u�W�F�N�g���X�g���擾
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList() const;
	void AddObject(const std::shared_ptr<KdGameObject>& _obj);

private:
	
	void ChangeScene(const SceneType& _type);

	// ���݂̃V�[�����Ǘ����Ă���|�C���^
	std::shared_ptr<SceneBase>	m_spCurtScene = nullptr;

	std::shared_ptr<TitleScene>		title;
	std::shared_ptr<GameScene>		game;
	std::shared_ptr<ResultScene>	result;

	// ���݂̃V�[�����Ǘ����Ă���ϐ�
	SceneType m_curtSceneType = SceneType::Game;

	// ���̃V�[�����Ǘ����Ă���ϐ�
	SceneType m_nextSceneType = m_curtSceneType;

	/* --- �V���O���g���p�^�[�� --- */
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