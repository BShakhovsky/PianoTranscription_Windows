#pragma once

class StdErrBuffer : boost::noncopyable
{
public:
	StdErrBuffer()
		: oldBuf_(std::cerr.rdbuf(errStream_.rdbuf()))
	{}
	~StdErrBuffer()
	{
		std::cerr.rdbuf(oldBuf_);
	}

	std::string Get() const
	{
		return std::move(errStream_.str());
	}
private:
	std::stringstream errStream_;
	std::streambuf* oldBuf_;
	const BYTE unusedPadding_[4] = { 0 };
};