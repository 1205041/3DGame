#include "KdDebugWireFrame.h"

void KdDebugWireFrame::AddDebugLine(const Math::Vector3& _start, const Math::Vector3& _end, const Math::Color& _col)
{
	// デバッグラインの始点
	KdPolygon::Vertex v1;
	v1.color = _col.RGBA().v;
	v1.UV = Math::Vector2::Zero;
	v1.pos = _start;

	// デバッグラインの終点
	KdPolygon::Vertex v2;
	v2.color = _col.RGBA().v;
	v2.UV = Math::Vector2::Zero;
	v2.pos = _end;

	m_debugVertices.push_back(v1);
	m_debugVertices.push_back(v2);
}

void KdDebugWireFrame::AddDebugLine(const Math::Vector3& _start, const Math::Vector3& _dir, float _length, const Math::Color& _col)
{
	// デバッグラインの始点
	KdPolygon::Vertex v1;
	v1.color = _col.RGBA().v;
	v1.UV = Math::Vector2::Zero;
	v1.pos = _start;

	// デバッグラインの終点
	KdPolygon::Vertex v2;
	v2.color = _col.RGBA().v;
	v2.UV = Math::Vector2::Zero;
	v2.pos = v1.pos + (_dir * _length);

	m_debugVertices.push_back(v1);
	m_debugVertices.push_back(v2);
}

void KdDebugWireFrame::AddDebugLineFromMatrix(const Math::Matrix& _mat, float _scale)
{
	Math::Vector3 start = _mat.Translation();
	Math::Vector3 vAxis;

	// X軸描画
	vAxis = _mat.Right();
	vAxis.Normalize();
	AddDebugLine(start, start + vAxis * _scale, kRedColor);

	// Y軸描画
	vAxis = _mat.Up();
	vAxis.Normalize();
	AddDebugLine(start, start + vAxis * _scale, kGreenColor);

	// Z軸描画
	vAxis = _mat.Backward();
	vAxis.Normalize();
	AddDebugLine(start, start + vAxis * _scale, kBlueColor);
}

// デバッグスフィアの描画
void KdDebugWireFrame::AddDebugSphere(const Math::Vector3& _pos, float _radius, const Math::Color& _col)
{
	KdPolygon::Vertex v;
	v.UV = Math::Vector2::Zero;
	v.color = _col.RGBA().v;

	int detail = 16;
	for (int i = 0; i < detail + 1; ++i)
	{
		// XZ面
		v.pos = _pos;
		v.pos.x += cos((float)i * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		v.pos.z += sin((float)i * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		m_debugVertices.push_back(v);

		v.pos = _pos;
		v.pos.x += cos((float)(i + 1) * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		v.pos.z += sin((float)(i + 1) * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		m_debugVertices.push_back(v);

		// XY面
		v.pos = _pos;
		v.pos.x += cos((float)i * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		v.pos.y += sin((float)i * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		m_debugVertices.push_back(v);

		v.pos = _pos;
		v.pos.x += cos((float)(i + 1) * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		v.pos.y += sin((float)(i + 1) * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		m_debugVertices.push_back(v);

		// YZ面
		v.pos = _pos;
		v.pos.y += cos((float)i * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		v.pos.z += sin((float)i * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		m_debugVertices.push_back(v);

		v.pos = _pos;
		v.pos.y += cos((float)(i + 1) * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		v.pos.z += sin((float)(i + 1) * (EVERY_DIRECTION / detail) * KdToRadians) * _radius;
		m_debugVertices.push_back(v);
	}
}

void KdDebugWireFrame::Draw()
{
	if (m_debugVertices.size() < 2) { return; }

	
	KdShaderManager::GetInstance().m_HD2DShader.DrawVertices(m_debugVertices, Math::Matrix::Identity);

	m_debugVertices.clear();
}

void KdDebugWireFrame::Release()
{
	m_debugVertices.clear();
}
