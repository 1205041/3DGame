﻿#pragma once

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* ！ ！ ！ アセットを取り出し可能な状態で保持するクラス ！ ！ ！ */
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// ・データの読み込み・保持・検索の機能を持っている
// ・汎用性のため検索・読込命令は文字列を使用
// ・メモリ・処理効率を考えデザインパターンのFlyWeightパターンを利用
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
template<class DataType>
class KdDataStorage
{
public:
	KdDataStorage() {}
	~KdDataStorage() { ClearData(true); }

	/* = = = = = = = = = = = = = = = = = = = = = */
	/* 各アセットの読込・取得関数 */
	// ===== ===== ===== ===== ===== ===== =====
	// ・強制的にデータを読み込ませて更新する
	/* = = = = = = = = = = = = = = = = = = = = = */
	std::shared_ptr<DataType> LoadData(std::string_view _fileName)
	{
		std::shared_ptr<DataType> newData = std::make_shared<DataType>();

		if (!newData->Load(_fileName))
		{
			assert(0 && "KdDataStorage::LoadData ファイルが存在しません。ファイルパスを確認してください");

			return nullptr;
		}

		m_spDatas[_fileName.data()] = newData;

		return newData;
	}

	// データの取得：リスト内に存在しない場合は新しくロードする
	std::shared_ptr<DataType> GetData(std::string_view _fileName)
	{
		// リストの中に欲しいデータがあるか検索
		auto findData = m_spDatas.find(_fileName.data());

		// データがあった場合：そのままデータを共有
		if (findData != m_spDatas.end()) { return (*findData).second; }
		// データが無かった場合：新たにデータをロードする
		else { return LoadData(_fileName); }
	}

	// 保持しているデータの破棄
	void ClearData(bool _force)
	{
		if (_force)
		{
			// 強制的にすべてのデータを消去
			m_spDatas.clear();

			return;
		}

		// アプリ上で使用されておらず、Storageクラスが保持しているだけのデータを破棄
		for (auto dataIter = m_spDatas.begin(); dataIter != m_spDatas.end();)
		{
			if (dataIter->second.use_count() < 2)
			{
				dataIter = m_spDatas.erase(dataIter);

				continue;
			}
			++dataIter;
		}
	}

private:
	std::unordered_map<std::string, std::shared_ptr<DataType>> m_spDatas;
};

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* ！ ！ ！ フレームワークのクラスで使用するアセットをまとめたクラス ！ ！ ！ */
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// ・複数存在する事を許さないのでシングルトンパターンを使用
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
class KdAssets
{
public:
	// それぞれのアセット
	KdDataStorage<KdTexture>	m_textures;
	KdDataStorage<KdModelData>	m_modeldatas;

	void ClearData(bool _force)
	{
		m_textures.ClearData(_force);
		m_modeldatas.ClearData(_force);
	}

private:
	void Release() { ClearData(true); }

public:
	static KdAssets& GetInstance()
	{
		static KdAssets instance;
		return instance;
	}

private:
	KdAssets() {}
	~KdAssets() { Release(); }
};