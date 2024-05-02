#include "KdCSVData.h"

const std::vector<std::string> KdCSVData::c_nullDataList;

bool KdCSVData::Load(const std::string_view _filename)
{
	if (_filename.empty()) { return false; }

	m_filePass = _filename.data();

	std::ifstream ifs(m_filePass);

	if (!ifs)
	{ 
		assert(0 && "CSVDataが見つかりません");

		return false;
	}

	// 行ごとに分けてデータ格納
	while (true)
	{
		std::string rawLineData;
		if (!getline(ifs, rawLineData)) { break; }

		// [,]で分けて単語ごとにデータ格納
		std::vector<std::string> lineData;
		CommaSeparatedValue(rawLineData, lineData);

		m_dataLines.push_back(lineData);
	}

	return true;
}

// 行データを取得
const std::vector<std::string>& KdCSVData::GetLine(size_t _index) const
{
	if (_index >= m_dataLines.size()) { return c_nullDataList; }

	return m_dataLines[_index];
}

// [,]で分けて単語リスト作成
void KdCSVData::CommaSeparatedValue(std::string_view _line, std::vector<std::string>& _result)
{
	std::istringstream stream(_line.data());
	std::string element;

	while (getline(stream, element, ',')) { _result.push_back(element); }
}