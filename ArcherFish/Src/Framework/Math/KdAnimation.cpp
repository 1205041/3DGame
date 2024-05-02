#include "KdAnimation.h"
#include "../Direct3D//KdModel.h"

// 二分探索で、指定時間から次の配列要素のKeyIndexを求める関数
// list		… キー配列
// time		… 時間
// 戻り値	… 次の配列要素のIndex
template<class T>
int BinarySearchNextAnimKey(const std::vector<T>& _list, float _time)
{
	int low = 0;
	int high = (int)_list.size();
	while (low < high)
	{
		int mid = (low + high) / 2;
		float midTime = _list[mid].m_time;

		if (midTime <= _time) low = mid + 1;
		else high = mid;
	}
	return low;
}

bool KdAnimationData::Node::InterpolateTranslations(Math::Vector3& _result, float _time)
{
	if (m_translations.size() == 0)return false;

	// キー位置検索
	UINT keyIdx = BinarySearchNextAnimKey(m_translations, _time);

	// 先頭のキーなら、先頭のデータを返す
	if (keyIdx == 0) 
	{
		_result = m_translations.front().m_vec;
		return true;
	}
	// 配列外のキーなら、最後のデータを返す
	else if (keyIdx >= m_translations.size()) 
	{
		_result = m_translations.back().m_vec;
		return true;
	}
	// それ以外(中間の時間)なら、その時間の値を補間計算で求める
	else 
	{
		auto& prev = m_translations[keyIdx - 1];	// 前のキー
		auto& next = m_translations[keyIdx];		// 次のキー
		// 前のキーと次のキーの時間から、0～1間の時間を求める
		float f = (_time - prev.m_time) / (next.m_time - prev.m_time);
		// 補間
		_result = DirectX::XMVectorLerp(prev.m_vec, next.m_vec, f);
	}

	return true;
}

bool KdAnimationData::Node::InterpolateRotations(Math::Quaternion& _result, float _time)
{
	if (m_rotations.size() == 0)return false;

	// キー位置検索
	UINT keyIdx = BinarySearchNextAnimKey(m_rotations, _time);
	// 先頭のキーなら、先頭のデータを返す
	if (keyIdx == 0) { _result = m_rotations.front().m_quat; }
	// 配列外のキーなら、最後のデータを返す
	else if (keyIdx >= m_rotations.size()) { _result = m_rotations.back().m_quat; }
	// それ以外(中間の時間)なら、その時間の値を補間計算で求める
	else 
	{
		auto& prev = m_rotations[keyIdx - 1];	// 前のキー
		auto& next = m_rotations[keyIdx];		// 次のキー
		// 前のキーと次のキーの時間から、0～1間の時間を求める
		float f = (_time - prev.m_time) / (next.m_time - prev.m_time);
		// 補間
		_result = DirectX::XMQuaternionSlerp(prev.m_quat, next.m_quat, f);
	}

	return true;
}

bool KdAnimationData::Node::InterpolateScales(Math::Vector3& _result, float _time)
{
	if (m_scales.size() == 0)return false;

	// キー位置検索
	UINT keyIdx = BinarySearchNextAnimKey(m_scales, _time);

	// 先頭のキーなら、先頭のデータを返す
	if (keyIdx == 0) 
	{
		_result = m_scales.front().m_vec;
		return true;
	}
	// 配列外のキーなら、最後のデータを返す
	else if (keyIdx >= m_scales.size()) 
	{
		_result = m_scales.back().m_vec;
		return true;
	}
	// それ以外(中間の時間)なら、その時間の値を補間計算で求める
	else 
	{
		auto& prev = m_scales[keyIdx - 1];	// 前のキー
		auto& next = m_scales[keyIdx];		// 次のキー
		// 前のキーと次のキーの時間から、0～1間の時間を求める
		float f = (_time - prev.m_time) / (next.m_time - prev.m_time);
		// 補間
		_result = DirectX::XMVectorLerp(prev.m_vec, next.m_vec, f);
	}

	return true;
}

void KdAnimationData::Node::Interpolate(Math::Matrix& _rDst, float _time)
{
	// ベクターによる拡縮補間
	bool isChange = false;
	Math::Matrix scale;
	Math::Vector3 resultVec;
	if (InterpolateScales(resultVec, _time))
	{
		scale = scale.CreateScale(resultVec);
		isChange = true;
	}

	// クォタニオンによる回転補間
	Math::Matrix rotate;
	Math::Quaternion resultQuat;
	if (InterpolateRotations(resultQuat, _time))
	{
		rotate = rotate.CreateFromQuaternion(resultQuat);
		isChange = true;
	}

	// ベクターによる座標補間
	Math::Matrix trans;
	if (InterpolateTranslations(resultVec, _time))
	{
		trans = trans.CreateTranslation(resultVec);
		isChange = true;
	}

	if (isChange) { _rDst = scale * rotate * trans; }
}

void KdAnimator::AdvanceTime(std::vector<KdModelWork::Node>& _rNodes, float _speed)
{
	if (!m_spAnimation) { return; }

	// 全てのアニメーションノード（モデルの行列を補間する情報）の行列補間を実行する
	for (auto& rAnimNode : m_spAnimation->m_nodes)
	{
		// 対応するモデルノードのインデックス
		UINT idx = rAnimNode.m_nodeOffset;

		auto prev = _rNodes[idx].m_localTransform;

		// アニメーションデータによる行列補間
		rAnimNode.Interpolate(_rNodes[idx].m_localTransform, m_time);

		prev = _rNodes[idx].m_localTransform;
	}

	// アニメーションのフレームを進める
	m_time += _speed;

	// アニメーションデータの最後のフレームを超えたら
	if (m_time >= m_spAnimation->m_maxLength)
	{
		// アニメーションの最初に戻る（ループさせる
		if (m_isLoop) { m_time = 0.0f; }
		else { m_time = m_spAnimation->m_maxLength; }
	}
}