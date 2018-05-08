//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : Controls the audio in the game
//				  
// Author       : Jack Mair
// Mail         : jack.mai7246@mediadesign.school.nz
//

#pragma once

#include <fmod.hpp>
#include <memory>

enum Sound
{
	BUTTON_CLICKED,
	PLAYER_JOINED,
	PLAYER_COLLISION,
	PLAYER_SNAKE_COLLIDE,
	PLAYER_FALLING,
	PLAYER_ACCELERATE,
	PLAYER_ENGINE,
	PICKUP_COLLECTED,
	RESPAWN_PLAYERS
};

class Audio
{
private:

	Audio();

public:

	~Audio();

	static Audio& getInstance();

	void playMenuMusic();
	void playTrack1Music();
	void playTrack2Music();
	void playTrack3Music();
	void playButtonClick();

	// Play a game play Sound effect
	void playSFX(Sound sound);

protected:

	static std::unique_ptr<Audio> m_audio;

private:

	bool InitFmod();
	const bool LoadAudio();

	FMOD::System* m_audioMgr;
	FMOD::Channel* m_bgMusicChannel;
	FMOD::Channel* m_sfxChannel;

	FMOD::Sound* m_bgMusic;
	FMOD::Sound* m_franticMusic;
	FMOD::Sound* m_racingMusic;
	FMOD::Sound* m_adventureMusic;
	FMOD::Sound* m_80sMusic;

	FMOD::Sound* m_accelerate;
	FMOD::Sound* m_engineLoop;

	FMOD::Sound* m_buttonClick;
	FMOD::Sound* m_playerJoined;
	FMOD::Sound* m_playerCollision;
	FMOD::Sound* m_playerSnakeTailCollide;
	FMOD::Sound* m_playerFalling;
	FMOD::Sound* m_pickupCollected;
	FMOD::Sound* m_respawnPlayers;
};

