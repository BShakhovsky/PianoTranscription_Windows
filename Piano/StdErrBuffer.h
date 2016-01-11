#pragma once

class StdErrBuffer : private boost::noncopyable
{
public:
	StdErrBuffer()
		: errBuf_("")
	{
		setvbuf(stderr, errBuf_, _IOFBF, sizeof errBuf_ / sizeof *errBuf_);
	}
	~StdErrBuffer()
	{
		setvbuf(stderr, nullptr, _IOFBF, 2);
	}

	const char* Get() const
	{
		return errBuf_;
	}
private:
	char errBuf_[0xFF];
};