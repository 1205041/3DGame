#pragma once

struct KdCSVData
{
public:
	KdCSVData() {}
	KdCSVData(const std::string_view _filename) { Load(_filename); }

	bool Load(const std::string_view _filename);

	const std::vector<std::vector<std::string>>& GetLData() const { return m_dataLines; }

	const std::vector<std::string>& KdCSVData::GetLine(size_t _index) const;

	size_t GetLineSize() const { return m_dataLines.size(); }

private:
	void CommaSeparatedValue(std::string_view _src, std::vector<std::string>& _result);

	std::vector<std::vector<std::string>> m_dataLines;

	std::string m_filePass;

	static const std::vector<std::string> c_nullDataList;
};