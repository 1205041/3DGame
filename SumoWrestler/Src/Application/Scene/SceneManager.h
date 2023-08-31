#pragma once

class SceneBase;

class SceneManager
{
public:
	// �V�[�����
	enum class SceneType
	{
		Title,
		Game,
		Win,
		Lose,
	};

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void DrawLit();
	void DrawSprite();

	void DrawDebug();

	void Init() { ChangeScene(SceneType::Title); }

	// ���̊֐��ł̓V�[���ؑւ��܂��s���Ȃ��I
	// �����܂ŗ\��I�I
	void SetNextScene(const SceneType& _nextScene) { m_nextSceneType = _nextScene; }

	// �I�u�W�F�N�g���X�g���擾
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList();
	void AddObject(const std::shared_ptr<KdGameObject>& _obj);

	const SceneType& GetCurtSceneType() { return m_curtSceneType; }
private:
	void ChangeScene(const SceneType& _sceneType);

	// ���݂̃V�[�����Ǘ����Ă���|�C���^
	SceneBase* m_curtScene = nullptr;

	// ���݂̃V�[�����Ǘ����Ă���ϐ�
	SceneType m_curtSceneType = SceneType::Title;

	// ���̃V�[�����Ǘ����Ă���ϐ�
	SceneType m_nextSceneType = m_curtSceneType;

/* --- �V���O���g���p�^�[�� --- */
private:
	SceneManager() { Init(); }
	~SceneManager() {}

public:
	static SceneManager& Instance()
	{
		static SceneManager instace;
		return instace;
	}
};