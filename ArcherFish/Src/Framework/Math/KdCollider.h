#pragma once

class KdCollisionShape;

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* 当たり判定を内部で実行し判定結果を返してくれるクラス */
// ・当たり判定を受けたいゲーム内のオブジェクトにメンバーとして持たせる
// 　※当てる側ではなく当てられる側に持たせる
/* ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== */
/* === 運用には形状登録・判定実行の2つの手順が必要 === */
// ・形状登録：RegisterColisionSphape()を使って当たり判定の形状を登録しておく。
// 　CollisionShapeは形状と衝突タイプ（用途）が必要
// ・判定実行：Intersects()で当たり判定を実行する。
// 　詳細な結果が欲しい場合にはResultを引数としてセットする事
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
class KdCollider
{
public:

	// 衝突タイプBitフラグ：用途によって使い分ける・Bitフラグなので複数のタイプを付与可能
	enum Type
	{
		TypeGround		= 1 << 0,	// 地形：上に乗れるオブジェクトに対して付与する
		TypeBump		= 1 << 1,	// 衝突：横方向で重なりを防止したいオブジェクトに対して付与する
		TypeDamage		= 1 << 2,	// 攻撃：球形の攻撃判定を受けるオブジェクトに対して付与する
		TypeDamageLine	= 1 << 3,	// 攻撃：線形の攻撃判定を受けるオブジェクトに対して付与する
		TypeSight		= 1 << 4,	// 視界：敵がプレイヤーを発見するかどうかなど視界判定を受けるオブジェクトに付与する
		TypeEvent		= 1 << 5	// イベント：イベント特有の判定形状が欲しい場合にイベントを所有しているオブジェクトに付与する
	};

	// 球形の当たり判定情報：当たる側専用
	struct SphereInfo
	{
		SphereInfo() {}

		// BoundingSphereを直接指定
		SphereInfo(UINT _type, const DirectX::BoundingSphere _sphere)
			: m_type(_type), m_sphere(_sphere) 
		{
		}

		// 座標と半径からBoundingSphereを指定
		SphereInfo(UINT _type, const Math::Vector3& _pos, float _radius)
			: m_type(_type)
		{
			m_sphere.Center = _pos;
			m_sphere.Radius = _radius;
		}

		DirectX::BoundingSphere m_sphere;

		UINT m_type = 0;
	};

	// レイの当たり判定情報：当たる側専用
	struct RayInfo
	{
		RayInfo() {}

		// レイの情報を全て指定：自動的に方向ベクトルは正規化
		RayInfo(UINT _type,const Math::Vector3& _pos,const Math::Vector3& _dir,float _range)
			: m_type(_type), m_pos(_pos), m_dir(_dir), m_range(_range)
		{
			m_dir.Normalize();
		}

		// 開始地点と終了地点からレイの情報を作成：自動的に方向ベクトルは正規化
		RayInfo(UINT _type, const Math::Vector3& _start, const Math::Vector3& _end)
			: m_type(_type), m_pos(_start)
		{
			m_dir = _end - _start;
			m_range = m_dir.Length();
			m_dir.Normalize();
		}

		Math::Vector3 m_pos;		// レイの発射位置
		Math::Vector3 m_dir;		// レイの方向
		float m_range = 0;			// 判定限界距離

		UINT m_type = 0;
	};

	// 詳細な衝突結果
	struct CollisionResult
	{
		Math::Vector3 m_hitPos;			// 衝突した座標
		Math::Vector3 m_hitDir;			// 対象からの方向ベクトル（押し返しなどに使う
		float m_overlapDistance = 0.0f; // 重なり量
	};

	KdCollider() {}
	~KdCollider() {}

	/* === 当たり判定の形状登録関数群 === */
	// 当たり判定形状登録関数
	void RegisterCollisionShape(std::string_view _name, std::unique_ptr<KdCollisionShape> _spShape);
	// 球情報の当たり判定
	void RegisterCollisionShape(std::string_view _name, const DirectX::BoundingSphere& _sphere, UINT _type);
	// ローカル座標と半径の当たり判定
	void RegisterCollisionShape(std::string_view _name, const Math::Vector3& _localPos, float _radius, UINT _type);
	// スマポのモデルデータの当たり判定
	void RegisterCollisionShape(std::string_view _name, const std::shared_ptr<KdModelData>& _model, UINT _type);
	// 生ポのモデルデータの当たり判定
	void RegisterCollisionShape(std::string_view _name, KdModelData* _model, UINT _type);
	// スマポのモデルワークの当たり判定
	void RegisterCollisionShape(std::string_view _name, const std::shared_ptr<KdModelWork>& _model, UINT _type);
	// 生ポのモデルワークの当たり判定
	void RegisterCollisionShape(std::string_view _name, KdModelWork* _model, UINT _type);
	// スマポのポリゴンの当たり判定
	void RegisterCollisionShape(std::string_view _name, const std::shared_ptr<KdPolygon> _polygon, UINT _type);
	// 生ポのポリゴンの当たり判定
	void RegisterCollisionShape(std::string_view _name, KdPolygon* _polygon, UINT _type);

	// 当たり判定実行
	bool Intersects(const SphereInfo& _targetShape, const Math::Matrix& _ownerMatrix, std::list<KdCollider::CollisionResult>* _pResults) const;
	bool Intersects(const RayInfo& _targetShape, const Math::Matrix& _ownerMatrix, std::list<KdCollider::CollisionResult>* _pResults) const;

	/* === 登録した当たり判定の有効/無効の設定 === */
	// 任意のCollisionShapeを検索して有効/無効を切り替える
	void SetEnable(std::string_view _name, bool _flag);
	// 特定のタイプの有効/無効を切り替える
	void SetEnable(int _type, bool _flag);
	// 全てのCollisionShapeの有効/無効を一気に切り替える
	void SetEnableAll(bool _flag);

private:
	std::unordered_map<std::string, std::unique_ptr<KdCollisionShape>> m_collisionShapes;

	int m_disableType = 0;
};


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* vs球とvsレイの判定を持つ何かしらの形状の基底クラス */
// ・当たり判定をする用途（type）と球・レイの当たり判定用インターフェースを持つ
// ・継承先では任意の形状をメンバーに追加し、
// 　その形状とvs球とvsレイ当たり判定関数を作成する
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
class KdCollisionShape
{
public:
	KdCollisionShape(UINT _type) { m_type = _type; }
	virtual ~KdCollisionShape() {}

	UINT GetType() const { return m_type; }

	virtual bool Intersects(const DirectX::BoundingSphere& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes) = 0;
	virtual bool Intersects(const KdCollider::RayInfo& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes) = 0;

	void SetEnable(bool _flag) { m_enable = _flag; }

protected:
	bool m_enable = true;

private:
	UINT m_type = 0;		// 衝突タイプ:何用の当たり判定か
};


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* コライダー：球形状 */
// ・球形状vs特定形状（球・レイ）の当たり判定実行クラス
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = */
class KdSphereCollision : public KdCollisionShape
{
public:
	KdSphereCollision(const DirectX::BoundingSphere& _sphere, UINT _type)
		:KdCollisionShape(_type), m_shape(_sphere)
	{
	}

	KdSphereCollision(const Math::Vector3& _localPos, float _radius, UINT _type) 
		:KdCollisionShape(_type) 
	{ 
		m_shape.Center = _localPos; m_shape.Radius = _radius; 
	}

	virtual ~KdSphereCollision() {}

	bool Intersects(const DirectX::BoundingSphere& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes) override;
	bool Intersects(const KdCollider::RayInfo& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes) override;

private:
	DirectX::BoundingSphere m_shape;
};


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* コライダー：モデル形状(dynamicAnimationModelWork) */
// ・モデル形状vs特定形状（球・レイ）の当たり判定実行クラス
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
class KdModelCollision : public KdCollisionShape
{
public:
	KdModelCollision(const std::shared_ptr<KdModelData>& _model, UINT _type)
		:KdCollisionShape(_type), m_shape(std::make_shared<KdModelWork>(_model))
	{
	}

	KdModelCollision(const std::shared_ptr<KdModelWork>& _model, UINT _type)
		:KdCollisionShape(_type), m_shape(_model) 
	{
	}

	virtual ~KdModelCollision() { m_shape.reset(); }

	bool Intersects(const DirectX::BoundingSphere& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes) override;
	bool Intersects(const KdCollider::RayInfo& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes) override;

private:
	std::shared_ptr<KdModelWork> m_shape;
};


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* コライダー：ポリゴン形状 */
// ・ポリゴン形状vs特定形状（球・レイ）の当たり判定実行クラス
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
class KdPolygonCollision : public KdCollisionShape
{
public:
	KdPolygonCollision(const std::shared_ptr<KdPolygon>& _polygon, UINT _type)
		:KdCollisionShape(_type), m_shape(_polygon) {}

	virtual ~KdPolygonCollision() { m_shape.reset(); }

	bool Intersects(const DirectX::BoundingSphere& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes) override;
	bool Intersects(const KdCollider::RayInfo& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes) override;

private:
	std::shared_ptr<KdPolygon> m_shape;
};