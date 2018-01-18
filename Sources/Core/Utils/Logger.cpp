/*************************************************************************
> File Name: Logger.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Super simple logger implementation.
> Created Time: 2017/04/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Utils/Logger.h>
#include <Core/Utils/Macros.h>

#include <iostream>
#include <mutex>

namespace CubbyFlow
{
	static std::mutex critical;

	static std::ostream* infoOutStream = &std::cout;
	static std::ostream* warnOutStream = &std::cout;
	static std::ostream* errorOutStream = &std::cerr;
	static std::ostream* debugOutStream = &std::cout;

	inline std::ostream* levelToStream(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Info:
			return infoOutStream;
		case LogLevel::Warn:
			return warnOutStream;
		case LogLevel::Error:
			return errorOutStream;
		case LogLevel::Debug:
			return debugOutStream;
		}

		return nullptr;
	}

	inline std::string levelToString(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Info:
			return "INFO";
		case LogLevel::Warn:
			return "WARN";
		case LogLevel::Error:
			return "ERROR";
		case LogLevel::Debug:
			return "DEBUG";
		}

		return nullptr;
	}
	
	Logger::Logger(LogLevel level) :
		m_level(level)
	{
		// Do nothing
	}

	Logger::~Logger() {
		std::lock_guard<std::mutex> lock(critical);
#if defined(DEBUG) || defined(_DEBUG)
		if (m_level != LogLevel::Debug) {
#endif
			auto stream = levelToStream(m_level);
			(*stream) << m_buffer.str() << std::endl;
			stream->flush();
#if defined(DEBUG) || defined(_DEBUG)
		}
#endif
	}

	void Logging::SetInfoStream(std::ostream* stream)
	{
		std::lock_guard<std::mutex> lock(critical);
		infoOutStream = stream;
	}

	void Logging::SetWarnStream(std::ostream* stream)
	{
		std::lock_guard<std::mutex> lock(critical);
		warnOutStream = stream;
	}

	void Logging::SetErrorStream(std::ostream* stream)
	{
		std::lock_guard<std::mutex> lock(critical);
		errorOutStream = stream;
	}

	void Logging::SetDebugStream(std::ostream* stream)
	{
		std::lock_guard<std::mutex> lock(critical);
		debugOutStream = stream;
	}

	void Logging::SetAllStream(std::ostream* stream)
	{
		SetInfoStream(stream);
		SetWarnStream(stream);
		SetErrorStream(stream);
		SetDebugStream(stream);
	}

	std::string Logging::GetHeader(LogLevel level)
	{
		auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		char timeStr[20];
#ifdef CUBBYFLOW_WINDOWS
		tm time;
		localtime_s(&time, &now);
		strftime(timeStr, sizeof(timeStr), "%F %T", &time);
#else
		strftime(timeStr, sizeof(timeStr), "%F %T", std::localtime(&now));
#endif
		char header[256];
		snprintf(
			header, sizeof(header), "[%s] %s ",
			levelToString(level).c_str(),
			timeStr);

		return header;
	}
}