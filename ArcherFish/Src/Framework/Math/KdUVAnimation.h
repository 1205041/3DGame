#pragma once

// UVアニメーションのフレーム情報
struct KdAnimationFrame
{
	KdAnimationFrame(int _start, int _end)
		:m_startFrame(_start),m_endFrame(_end + 1) 
	{
		// 最後のコマを表示するため+1
		// 終了フレームになった瞬間終了判定になってしまっては、最後のコマが描画される時間が無くなる
	}

	int m_startFrame = 0;
	int m_endFrame = 0;
};

struct KdUVAnimationData
{
public:
	void Load(std::string_view _fileName);

	void AddAnimation(const std::string_view _animName, const KdAnimationFrame& _data);

	void AddAnimation(const std::string_view _animName, int _start, int _end);

	const std::shared_ptr<KdAnimationFrame> GetAnimation(std::string_view _name);

private:
	std::unordered_map<std::string, std::shared_ptr<KdAnimationFrame>> m_animations;
};

class KdUVAnimator
{
public:
	void SetAnimation(const std::shared_ptr<KdAnimationFrame>& _animData, bool _isLoop = true, bool _restart = true);

	// コマアニメーションを進行させる
	// ・speed		… 進行速度 1.0で1フレーム1コマ
	void AdvanceTime(float _speed);

	int GetFrame() { return static_cast<int>(m_nowAnimPos); }

	// アニメーションの再生が終わった？
	bool IsAnimationEnd() const;

private:
	// 現在のアニメーション位置
	float	m_nowAnimPos = 0.0f;

	std::shared_ptr<KdAnimationFrame> m_spNowAnimation = nullptr;

	bool m_loopAnimation = false;
};