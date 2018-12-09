#pragma once
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

class SoundMixer
{
public:

	static void play(ALLEGRO_SAMPLE *sample);
	SoundMixer(void);
	~SoundMixer(void);
};

