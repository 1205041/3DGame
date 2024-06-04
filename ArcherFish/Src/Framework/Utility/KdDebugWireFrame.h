#pragma once

class KdDebugWireFrame
{
public:
	KdDebugWireFrame() {}
	~KdDebugWireFrame() { Release(); }

	void AddDebugLine(const Math::Vector3& _start, const Math::Vector3& _end, const Math::Color& _col = kWhiteColor);
	void AddDebugLine(const Math::Vector3& _start, const Math::Vector3& _dir = Math::Vector3(0,0,1), float _length = 1.0f, const Math::Color& _col = kWhiteColor);
	void AddDebugLineFromMatrix(const Math::Matrix& _mat, float _scale);
	void AddDebugSphere(const Math::Vector3& _pos, float _radius, const Math::Color& _col = kWhiteColor);

	void Draw();

private:
	void Release();

	std::vector<KdPolygon::Vertex>	m_debugVertices;
	const int m_EveryDirection = 360;
};