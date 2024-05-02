#pragma once

class KdTexture;

//===========================================
// 便利機能
//===========================================
// 算術系短縮名
namespace Math = DirectX::SimpleMath;

// 角度変換
constexpr float KdToRadians = (3.141592654f / 180.0f);
constexpr float KdToDegrees = (180.0f / 3.141592654f);

// 安全にReleaseするための関数
template<class T>
void KdSafeRelease(T*& _p)
{
	if (_p)
	{
		_p->Release();
		_p = nullptr;
	}
}

// 安全にDeleteするための関数
template<class T>
void KdSafeDelete(T*& _p)
{
	if (_p)
	{
		delete _p;
		_p = nullptr;
	}
}

template<class T>
void DebugOutputNumber(T _num)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << _num;
	std::string str = stream.str();

	OutputDebugStringA(str.c_str());
}


//===========================================
// 色定数
//===========================================
static const Math::Color	kWhiteColor		= Math::Color(1.0f, 1.0f, 1.0f, 1.0f); // 白
static const Math::Color	kBlackColor		= Math::Color(0.0f, 0.0f, 0.0f, 1.0f); // 黒
static const Math::Color	kRedColor		= Math::Color(1.0f, 0.0f, 0.0f, 1.0f); // 赤
static const Math::Color	kGreenColor		= Math::Color(0.0f, 1.0f, 0.0f, 1.0f); // 緑
static const Math::Color	kBlueColor		= Math::Color(0.0f, 0.0f, 1.0f, 1.0f); // 青
static const Math::Color	kYellowColor	= Math::Color(1.0f, 1.0f, 0.0f, 1.0f); // 黄
static const Math::Color	kNormalColor	= Math::Color(0.5f, 0.5f, 1.0f, 1.0f); // 垂直に伸びる法線情報


//===========================================
// ファイル
//===========================================
// ファイルの存在確認
inline bool KdFileExistence(std::string_view _path)
{
	std::ifstream ifs(_path.data());

	bool isExistence = ifs.is_open();

	ifs.close();

	return isExistence;
}

// ファイルパスから、親ディレクトリまでのパスを取得
inline std::string KdGetDirFromPath(const std::string &_path)
{
	const std::string::size_type pos = std::max<signed>(_path.find_last_of('/'), _path.find_last_of('\\'));
	return (pos == std::string::npos) ? std::string() : _path.substr(0, pos + 1);
}

// ファイルパスから、拡張子抜きのパスを取得
inline std::string KdGetNameFromPath(const std::string& _path, bool _onlyFileName = false)
{
	std::string::size_type dirPos = 0;

	if (_onlyFileName) { dirPos = std::max(0, std::max<signed>(_path.find_last_of('/'), _path.find_last_of('\\'))) + 1; }

	const std::string::size_type extPos = _path.find_last_of('.');

	return (extPos == std::string::npos) ? std::string() : _path.substr(dirPos, extPos - dirPos);
}


//===========================================
// 文字列関係
//===========================================
// std::string版 sprintf
template <typename ... Args>
std::string KdFormat(std::string_view _fmt, Args ... _args)
{
	size_t len = std::snprintf(nullptr, 0, _fmt.c_str(), _args ...);
	std::vector<char> buf(len + 1);
	std::snprintf(&buf[0], len + 1, _fmt.c_str(), _args ...);
	return std::string(&buf[0], &buf[0] + len);
}

void KdGetTextureInfo(ID3D11View* _view, D3D11_TEXTURE2D_DESC& _outDesc);

//===========================================
// 座標変換関係
//===========================================
bool ConvertRectToUV(const KdTexture* _srcTex, const Math::Rectangle& _src, Math::Vector2& _uvMin, Math::Vector2& _uvMax);