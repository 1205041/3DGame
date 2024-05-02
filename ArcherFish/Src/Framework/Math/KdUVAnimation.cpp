#include "KdUVAnimation.h"

// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####
// KdUVAnimationData
// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####

// アニメーションデータの読み込みcsvデータとして読み込む
// アニメーション名,開始フレーム番号,終了フレーム番号
void KdUVAnimationData::Load(std::string_view _fileName)
{
	KdCSVData data;

	if (data.Load(_fileName.data()))
	{
		for (int i = 0; ; ++i)
		{
			const std::vector<std::string>& anim = data.GetLine(i);

			if (!anim.size()) { break; }

			AddAnimation(anim[0], atoi(anim[1].c_str()), atoi(anim[2].c_str()));
		}
	}
}

void KdUVAnimationData::AddAnimation(const std::string_view _animName, const KdAnimationFrame& _data)
{
	m_animations[_animName.data()] = std::make_shared<KdAnimationFrame>(_data);
}

void KdUVAnimationData::AddAnimation(const std::string_view _animName, int _start, int _end)
{
	m_animations[_animName.data()] = std::make_shared<KdAnimationFrame>(_start, _end);
}

const std::shared_ptr<KdAnimationFrame> KdUVAnimationData::GetAnimation(std::string_view _name)
{
	auto dataItr = m_animations.find(_name.data());

	if ( dataItr == m_animations.end() ) { return nullptr; }

	return dataItr->second;
}

// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####
// UVAnimator
// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####

// 再生したいアニメーションのセット
void KdUVAnimator::SetAnimation(const std::shared_ptr<KdAnimationFrame>& _animData, bool _loop, bool _restart)
{
	if (!m_spNowAnimation) { m_spNowAnimation = _animData; }

	// アニメーションの進捗を初期化しない
	if (!_restart)
	{
		float nowProgress = m_nowAnimPos - m_spNowAnimation->m_startFrame;

		m_nowAnimPos = std::min(_animData->m_startFrame + nowProgress, static_cast<float>(_animData->m_endFrame));
	}
	else { m_nowAnimPos = static_cast<float>(_animData->m_startFrame); }

	m_spNowAnimation = _animData;

	m_loopAnimation = _loop;
}

// アニメーションを進行させる
void KdUVAnimator::AdvanceTime(float _speed)
{
	if (!m_spNowAnimation) { return; }

	// アニメーション位置を進める
	m_nowAnimPos += _speed;

	// 終了判定
	if (IsAnimationEnd())
	{
		if (m_loopAnimation) { m_nowAnimPos = static_cast<float>(m_spNowAnimation->m_startFrame); }
		else { m_nowAnimPos = static_cast<float>(m_spNowAnimation->m_endFrame) - 0.001f; }// 最後のコマにする
	}
}

// アニメーションが終了しているかどうか
bool KdUVAnimator::IsAnimationEnd() const
{
	if (!m_spNowAnimation) { return true; }

	// 終了判定
	if (m_nowAnimPos >= m_spNowAnimation->m_endFrame - 0.001f) { return true; }

	return false;
}