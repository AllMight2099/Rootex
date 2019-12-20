#pragma once

#include "common/types.h"
#include "core/resource_data.h"

class ResourceFile
{
public:
	enum class Type : int
	{
		NONE = 0,
		LUA,
		WAV,
		TXT
	};

protected:
	Type m_Type;
	ResourceData* m_ResourceData;

	explicit ResourceFile(const Type& type, ResourceData* resData);

	friend class ResourceLoader;

public:
	virtual ~ResourceFile();
	explicit ResourceFile(ResourceFile&) = delete;
	explicit ResourceFile(ResourceFile&&) = delete;

	bool isValid();
	bool isOpen();

	FilePath getPath();
	Type getType();
	ResourceData* getData();
};

class TextResourceFile : public ResourceFile
{
protected:
	explicit TextResourceFile(const Type& type, ResourceData* resData);
	virtual ~TextResourceFile();

	friend class ResourceLoader;

public:
	explicit TextResourceFile(TextResourceFile&) = delete;
	explicit TextResourceFile(TextResourceFile&&) = delete;

	String getString() const;
};

class LuaTextResourceFile : public TextResourceFile
{
	explicit LuaTextResourceFile(ResourceData* resData);
	~LuaTextResourceFile();

	friend class ResourceLoader;

public:
	explicit LuaTextResourceFile(TextResourceFile&) = delete;
	explicit LuaTextResourceFile(TextResourceFile&&) = delete;
};

typedef int ALsizei;
typedef int ALenum;

class AudioResourceFile : public ResourceFile
{
	ALenum m_Format;
	float m_Frequency;
	int m_BitDepth;
	int m_Channels;

	const char* m_DecompressedAudioBuffer;
	ALsizei m_AudioDataSize;

	explicit AudioResourceFile(ResourceData* resData);
	~AudioResourceFile();

	friend class ResourceLoader;

public:
	explicit AudioResourceFile(AudioResourceFile&) = delete;
	explicit AudioResourceFile(AudioResourceFile&&) = delete;

	ALsizei getAudioDataSize() const { return m_AudioDataSize; }
	ALenum getFormat() const { return m_Format; }
	float getFrequency() const { return m_Frequency; }
	int getBitDepth() const { return m_BitDepth; }
	int getChannels() const { return m_Channels; }
};
