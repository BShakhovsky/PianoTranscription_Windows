#pragma once

class ResourceLoader : boost::noncopyable
{
	ResourceLoader() = delete;
public:
	explicit ResourceLoader(int resourceID, LPCTSTR resourceType)
	{
		const auto hResource(FindResource(GetModuleHandle(nullptr), MAKEINTRESOURCE(resourceID), resourceType));
		hMemory_ = LoadResource(GetModuleHandle(nullptr), hResource);
		data_ = LockResource(hMemory_);
		size_ = SizeofResource(GetModuleHandle(nullptr), hResource);
		assert("Could not find resource" && hResource && hMemory_ && data_ && size_);
	}
	~ResourceLoader()
	{
		const auto isReleased(FreeResource(hMemory_));
		assert("Could not release the resource" && !isReleased);
	}

	const void* Data() const
	{
		return data_;
	}
	size_t Size() const
	{
		return size_;
	}
private:
	HGLOBAL hMemory_;
	const void* data_;
	size_t size_;
};