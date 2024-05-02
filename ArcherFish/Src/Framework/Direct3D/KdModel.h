#pragma once

struct KdAnimationData;
struct KdGLTFModel;

class KdModelData
{
public:
	// ノード：モデルを形成するメッシュを扱うための最小単位
	struct Node
	{
		std::string		m_name;				// ノード名

		std::shared_ptr<KdMesh>	m_spMesh;	// メッシュ

		Math::Matrix	m_localTransform;			// 直属の親ボーンからの行列
		Math::Matrix	m_worldTransform;			// 原点からの行列
		Math::Matrix	m_boneInverseWorldMatrix;	// 原点からの逆行列

		int					m_parent = -1;	// 親インデックス
		std::vector<int>	m_children;		// 子供へのインデックスリスト

		//int		m_nodeIndex = -1;		// 先頭から何番目のノードか？
		int		m_boneIndex = -1;			// ボーンノードの時、先頭から何番目のボーンか？

		bool	m_isSkinMesh = false;
	};

	// コンストラクタとデストラクタ
	KdModelData() {}
	~KdModelData() { Release(); }

	bool Load(std::string_view _filename);

	void CreateNodes(const std::shared_ptr<KdGLTFModel>& _spGltfModel);									// ノード作成
	void CreateMaterials(const std::shared_ptr<KdGLTFModel>& _spGltfModel, const  std::string& _fileDir);	// マテリアル作成
	void CreateAnimations(const std::shared_ptr<KdGLTFModel>& _spGltfModel);								// アニメーション作成

	//アクセサ
	const std::shared_ptr<KdMesh> GetMesh(UINT _index) const { return _index < m_originalNodes.size() ? m_originalNodes[_index].m_spMesh : nullptr; }
	
	Node* FindNode(std::string _name)
	{
		for (auto&& node : m_originalNodes) { if (node.m_name == _name) { return &node; } }

		return nullptr;
	}

	// マテリアル配列取得
	const std::vector<KdMaterial>& GetMaterials() const { return m_materials; }

	// ノード配列取得
	const std::vector<Node>& GetOriginalNodes() const { return m_originalNodes; }

	// アニメーションデータ取得
	const std::shared_ptr<KdAnimationData> GetAnimation(std::string_view _animName) const;
	const std::shared_ptr<KdAnimationData> GetAnimation(UINT _index) const;

	// それぞれのノードのインデックスリスト取得
	const std::vector<int>& GetRootNodeIndices() const { return m_rootNodeIndices; }
	const std::vector<int>& GetBoneNodeIndices() const { return m_boneNodeIndices; }
	const std::vector<int>& GetMeshNodeIndices() const { return m_meshNodeIndices; }

	const std::vector<int>& GetDrawMeshNodeIndices() const { return m_drawMeshNodeIndices; }
	const std::vector<int>& GetCollisionMeshNodeIndices() const { return m_collisionMeshNodeIndices; }

	bool IsSkinMesh();

private:
	// 解放
	void Release();

	//マテリアル配列
	std::vector<KdMaterial> m_materials;

	// アニメーションデータリスト
	std::vector<std::shared_ptr<KdAnimationData>>	m_spAnimations;

	// 全ノード配列
	std::vector<Node> m_originalNodes;
	// 全ノード中、RootノードのみのIndex配列
	std::vector<int>  m_rootNodeIndices;
	// 全ノード中、ボーンノードのみのIndex配列
	std::vector<int>  m_boneNodeIndices;
	// 全ノード中、メッシュが存在するノードのみのIndexn配列
	std::vector<int>  m_meshNodeIndices;

	// 全ノード中、コリジョンメッシュが存在するノードのみのIndexn配列
	std::vector<int>  m_collisionMeshNodeIndices;
	// 全ノード中、描画するノードのみのIndexn配列
	std::vector<int>  m_drawMeshNodeIndices;
};

class KdModelWork
{
public:
	// ノード：活動中変化する可能性のあるデータ、検索用の名前
	struct Node
	{
		std::string		m_name;				// ノード名

		Math::Matrix	m_localTransform;	// 直属の親ボーンからの行列
		Math::Matrix	m_worldTransform;	// 原点からの行列

		void copy(const KdModelData::Node& _rNode)
		{
			m_name = _rNode.m_name;

			m_localTransform = _rNode.m_localTransform;
			m_worldTransform = _rNode.m_worldTransform;
		}
	};

	// コンストラクタ
	KdModelWork(){}
	KdModelWork(const std::shared_ptr<KdModelData>& _spModel) { SetModelData(_spModel); }

	~KdModelWork() {}

	// 全身のボーンの行列を計算
	void CalcNodeMatrices();

	// 有効フラグ
	bool IsEnable() const { return (m_enable && m_spData); }
	void SetEnable(bool _flag) { m_enable = _flag; }

	// ノード検索：文字列
	const KdModelData::Node* FindDataNode(std::string_view _name) const;
	const Node* FindNode(std::string_view _name) const;
	Node* FindWorkNode(std::string_view _name);

	// アクセサ
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	inline const std::shared_ptr<KdModelData> GetData() const { return m_spData; }
	// メッシュ取得
	inline const std::shared_ptr<KdMesh> GetMesh(UINT _index) const { return _index >= m_coppiedNodes.size() ? nullptr : GetDataNodes()[_index].m_spMesh; }

	// データノードリスト取得
	const std::vector<KdModelData::Node>& GetDataNodes() const { assert(m_spData && "モデルデータが存在しません"); return m_spData->GetOriginalNodes(); }
	// コピーノードリスト取得
	const std::vector<Node>& GetNodes() const { return m_coppiedNodes; }
	std::vector<Node>& WorkNodes() { m_needCalcNode = true; return m_coppiedNodes; }

	// アニメーションデータ取得
	const std::shared_ptr<KdAnimationData> GetAnimation(std::string_view _animName) const { return !m_spData ? nullptr : m_spData->GetAnimation(_animName); }
	const std::shared_ptr<KdAnimationData> GetAnimation(int _index) const { return !m_spData ? nullptr : m_spData->GetAnimation(_index); }

	// モデル設定：コピーノードの生成
	void SetModelData(const std::shared_ptr<KdModelData>& _rModel);
	void SetModelData(std::string_view _fileName);

	bool NeedCalcNodeMatrices() { return m_needCalcNode; }

private:
	// 再帰呼び出し用計算関数
	void recCalcNodeMatrices(int _nodeIdx, int _parentNodeIdx = -1);

	// 有効
	bool	m_enable = true;

	// モデルデータへの参照
	std::shared_ptr<KdModelData>	m_spData = nullptr;

	// 活動中変化する可能性のある全ノードデータのコピー配列
	std::vector<Node>	m_coppiedNodes;

	bool m_needCalcNode = false;
};