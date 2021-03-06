#include "string_util.h"

namespace elsa {

	std::string StringUtil::utf8_encode(const std::wstring & wstr)
	{
		if (wstr.empty())
			return std::string();

		auto size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string str_to(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str_to[0], size_needed, NULL, NULL);
		return str_to;
	}

	std::wstring StringUtil::to_wchar(const char* str)
	{
		size_t origsize = strlen(str) + 1;
		const size_t newsize = 300;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, str, _TRUNCATE);
		
		return std::wstring(wcstring);
	}

	std::wstring StringUtil::create_random_string(std::size_t length)
	{
		std::wstring str;
		static const std::wstring chars = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
		static const auto max_index = chars.size() - 1;

		for (auto i = 0; i < length; i++) {
			str.push_back(chars[rand() % max_index]);
		}

		return str;
	}
}