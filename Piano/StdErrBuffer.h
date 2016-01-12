#pragma once

class StdErrBuffer : private boost::noncopyable
{
public:
	StdErrBuffer()
		: errStream_(),
		oldBuf_(std::cerr.rdbuf(errStream_.rdbuf()))
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
	const BYTE unused_[4] = { 0 };	// padding bytes
};