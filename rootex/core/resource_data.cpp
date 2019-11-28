#include "core/resource_data.h"

unsigned int ResourceData::s_Count = 0;

unsigned int ResourceData::getID()
{
	return m_ID;
}

String ResourceData::getPath()
{
	return m_Path;
}

FileBuffer* ResourceData::getRawData()
{
	return &m_FileBuffer;
}

unsigned int ResourceData::getRawDataByteSize()
{
	return m_FileBuffer.m_Buffer.size();
}

void ResourceData::setPath(String path)
{
	m_Path = path;
}

ResourceData& ResourceData::operator>>(char& fillIn)
{
	if (m_StreamStart != m_StreamEnd)
	{
		m_StreamStart++;
	}
	fillIn = *m_StreamStart;

	return *this;
}

ResourceData::operator bool() const
{
	return m_StreamStart != m_StreamEnd;
}

bool ResourceData::isEndOfFile()
{
	return m_StreamStart == m_StreamEnd;
}

void ResourceData::resetStream()
{
	m_StreamStart = &m_FileBuffer.m_Buffer.front();
}

ResourceData::ResourceData(std::vector<char> data)
    : m_ID(s_Count)
    , m_FileBuffer(data)
    , m_StreamStart(&data.front())
    , m_StreamEnd(&data.back())
{
	s_Count++;
}
