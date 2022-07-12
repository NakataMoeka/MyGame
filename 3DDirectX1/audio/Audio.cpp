#include"Audio.h"
#include<fstream>
#include<cassert>

bool Audio::Initialize()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
		return false;
	}

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
		return false;
	}

	return true;
}




SoundData Audio::SoundLoadWave(const char* filename)
{
	HRESULT result;

	//1.ファイル読み込み

	std::ifstream file;

	file.open(filename, std::ios_base::binary);

	assert(file.is_open());

	//2.wavデータ読み込み

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	FormatChunk format = {};

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}


	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	if (strncmp(data.id, "JUNK ", 4) == 0)
	{

		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0)
	{
		assert(0);
	}


	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//3.Waveファイルを閉じる
	file.close();

	//4.読み込んだデータをreturn

	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

void Audio::SEPlayWave(const SoundData& soundData)
{
	HRESULT result;


	result = xAudio2->CreateSourceVoice(&pSourceSEVoice, &soundData.wfex);
	assert(SUCCEEDED(result));


	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;


	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}




void Audio::SoundPlayWave(const SoundData& soundData)
{
	HRESULT result;


	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));


	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void Audio::StopWave()
{
	pSourceVoice->Stop();
}

void Audio::SetSEVolume(float volume)
{
	pSourceSEVoice->SetVolume(volume);
}
void Audio::SetBGMVolume(float volume)
{
	pSourceVoice->SetVolume(volume);
}