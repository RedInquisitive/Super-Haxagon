#ifndef SUPER_HAXAGON_PLAYER_3DS_HPP
#define SUPER_HAXAGON_PLAYER_3DS_HPP

#include <3ds.h>

#include "Driver/Player.hpp"

namespace SuperHaxagon {
	class PlayerWav3DS : public Player {
	public:
		PlayerWav3DS(u8* data, u32 sampleRate, u32 dataSize, u16 channels, u16 bitsPerSample, u16 ndspFormat);
		~PlayerWav3DS() override;

	private:
		void setChannel(const int channel) override { _channel = channel; }
		void setLoop(const bool loop) override { _loop = loop; }
		
		void play() override;
		void pause() override {};
		bool isDone() override;
		double getTime() override;

		u8* _data;
		u32 _sampleRate;
		u32 _dataSize;
		u16 _channels;
		u16 _bitsPerSample;
		u16 _ndspFormat;

		ndspWaveBuf _buffer{};

		int _channel = 0;
		bool _loop = false;
	};
}

#endif //SUPER_HAXAGON_PLAYER_3DS_HPP
