#include "ObjBase.h"

void ObjBase::SetTarget(const std::shared_ptr<KdGameObject>& _target)
{
	if (!_target) { return; }

	m_wpTarget = _target;
}

/* ==================== */
/* �����蔻��(������p) */
/* ==================== */
void ObjBase::UpdateCollision()
{
	// �@�����蔻��(������)�p�̏����쐬
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0.0f, 0.7f, 0.0f);
	sphereInfo.m_sphere.Radius = 1.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	/* === �f�o�b�N�p(��) === */
	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// �AHIT����ΏۃI�u�W�F�N�g�ɑ�������
	Math::Vector3 newPos = Math::Vector3::Zero;
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retBumpList;
				spGameObj->Intersects(sphereInfo, &retBumpList);

				maxOverLap = 0.0f;
				hit = false;
				hitDir = Math::Vector3::Zero;
				for (auto& ret : retBumpList)
				{
					if (maxOverLap < ret.m_overlapDistance)
					{
						maxOverLap = ret.m_overlapDistance;
						hit = true;
						hitDir = ret.m_hitDir;// �����Ԃ�����
					}
				}
				if (hit)
				{
					// �B���ʂ��g���č��W��⊮����
					hitDir.Normalize();

					// �����Ԃ�
					newPos = GetPos() + (hitDir * maxOverLap);
					SetPos(newPos);
				}
			}
		}
	}
}

float ObjBase::GetAngleDeg(Math::Vector3 _src, Math::Vector3 _dest)
{
	//�菇1. �������̋���a�Əc�����̋���b�����Z�ŋ��߂�
	float a = _dest.x - _src.x;
	float b = _dest.z - _src.z;

	//�菇2. atan2 �֐������p���Ċp�x�����߂�
	float rad = atan2(b, a);

	//�菇3. �␳����:�K�v�ɉ����āA�f�O���[(�x)�ɒ���
	float deg = DirectX::XMConvertToDegrees(rad);

	//�菇4.0�`360�ɕ␳
	if (deg < 0) { deg += 360; }

	return deg;
}
