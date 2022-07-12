#include "Archive.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>

class Cls7zDllInitializer
{

private:
	bit7z::Bit7zLibrary& m_lib;
	bit7z::BitMemExtractor& m_mem_extractor;

	bit7z::Bit7zLibrary& _get_lib()
	{
		LPWSTR buf { new wchar_t[MAX_PATH] };
		std::wcout << L"== Loading extension module (.pyd)\n" << std::flush;;
		HMODULE hm{ GetCurrentModule() };
		std::wcout << L"HMODULE is " << hm << "\n" << std::flush;;
		GetModuleFileNameW(hm, buf, MAX_PATH);
		std::wcout << L"module name: " << buf << "\n" << std::flush;;
		
		std::filesystem::path path{ buf };
		path = path.parent_path();
		path.append(L"class_7zip_arch");
		path.append(L"7z.dll");
		std::wcout << L"loading 7z.dll: " << path << "\n" << std::flush;;

		delete[] buf;
		////////////////////////////////////////

		std::wcout << "Loading 7z.dll\n" << std::flush;
		static bit7z::Bit7zLibrary lllib{ path.wstring() };
		return lllib;
	}

	bit7z::BitMemExtractor& _get_extractor()
	{
		static bit7z::BitMemExtractor eeeextractor{ m_lib };
		return eeeextractor;
	}

public:

	Cls7zDllInitializer(const Cls7zDllInitializer&) = delete; // not copyable!
	Cls7zDllInitializer& operator=(const Cls7zDllInitializer&) = delete; // not assignable!

	Cls7zDllInitializer()
		:m_lib{ _get_lib() }, m_mem_extractor{_get_extractor()}
	{
		std::wcout << "Extension module loaded\n\n" << std::flush;
	}

	~Cls7zDllInitializer()
	{
		std::wcout << "Extension module unloaded\n\n" << std::flush;
	}

	bit7z::Bit7zLibrary& lib()
	{
		return m_lib;
	};

	bit7z::BitMemExtractor& mem_extractor()
	{
		return m_mem_extractor;
	};

};

static Cls7zDllInitializer Obj7zlib;

//static bit7z::Bit7zLibrary lib{ L"7z.dll" };
//static bit7z::BitMemExtractor mem_extractor{ lib };//, bit7z::BitFormat::SevenZip};   // ����� ������� ��������� � ������� (�� ����� �� ���������� ��� ������������� �������������???)

bytes_vector bytes_to_vector(const char* bytes_pointer, long long int size)
{
	// !!!!!!!!!!!!! ����������� ������ �� ����������� ��������� !!!!!!!!!!!!!!!! todo !!!

	// ������� ������ � ������ ����������� �����
	bytes_vector bv;
	bv.reserve(size);
	bv.resize(size);
	// �������� ������ � ������
	std::memcpy(bv.data(), bytes_pointer, size);

	// ������� �������� ������ ������
	//delete[] bytes_pointer;

	// ���������� ������
	return bv;
}

Archive::Archive(bytes_vector bv)
	: arch_vector{ bv }
	, arch_raw_len{ bv.size() }
	, arch_informator{ Obj7zlib.lib(), bv }
{
	arch_properties = arch_informator.archiveProperties();  //map< BitProperty, BitPropVariant >

	files = arch_informator.items();
	files_count = files.size();

	/* ����� ������ ���� ������
	for (auto& item : files)
	{
		std::wcout << item.index() << "\t" << item.name() << "\t" << item.extension() << "\t";
		std::wcout << item.path() << "\t" << item.size() << "\t" << item.packSize() << "\n";
	}
	*/

	/* �������� ������� ������
	for (auto& item : arch_properties)
	{
		std::cout << "haha";
	}
	*/
}

Archive::Archive(char* data, std::size_t len)
	:Archive(bytes_to_vector(data, len))
{
}

std::wstring Archive::filepath(int num)
{
	return files[num].path();
}

std::size_t Archive::filesize(int num)
{
	return files[num].size();
}

bytes_vector Archive::extract_filedata(int num)
{
	bytes_vector extracted;
	Obj7zlib.mem_extractor().extract(arch_vector, extracted, files[num].index());
	return extracted;
}

std::size_t Archive::save_to_file(int num, const std::wstring& path = L"")
{
	std::wstring rezult_path{ path + L"/" };
	rezult_path = rezult_path + files[num].path();

	bytes_vector data = extract_filedata(num);

	std::ofstream out(rezult_path, std::ofstream::binary);
	out.write(reinterpret_cast<const char*>(data.data()), data.size());

	return data.size();
}

std::size_t Archive::files_in_arch()
{
	return files_count;
}

Archive::~Archive()
{
	//std::cout << "destructing Archive\n";
}