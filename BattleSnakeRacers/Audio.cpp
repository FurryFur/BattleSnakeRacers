#include "Audio.h"

std::unique_ptr<Audio> Audio::m_audio = nullptr;

Audio::Audio()
{
	InitFmod();
	LoadAudio();
}


Audio::~Audio()
{
}

void Audio::update()
{
	m_audioMgr->update();
}



bool Audio::InitFmod() {
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_audioMgr);
	if (result != FMOD_OK) return false;
	result = m_audioMgr->init(32, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) return false;
	return true;
}

const bool Audio::LoadAudio() {
	FMOD_RESULT result;
	result = m_audioMgr->createSound("Assets/Audio/80s-Space-Game-Loop_v001.mp3", FMOD_DEFAULT, 0, &m_80sMusic);
	result = m_audioMgr->createSound("Assets/Audio/Cool-Adventure-Intro.mp3", FMOD_DEFAULT, 0, &m_adventureMusic);
	result = m_audioMgr->createSound("Assets/Audio/background_music.mp3", FMOD_DEFAULT, 0, &m_bgMusic);
	result = m_audioMgr->createSound("Assets/Audio/Frantic-Gameplay.mp3", FMOD_DEFAULT, 0, &m_franticMusic);
	result = m_audioMgr->createSound("Assets/Audio/Racing-Menu.mp3", FMOD_DEFAULT, 0, &m_racingMusic);

	result = m_audioMgr->createSound("Assets/Audio/accelerate.wav", FMOD_DEFAULT, 0, &m_accelerate);
	result = m_audioMgr->createSound("Assets/Audio/loopingLinearEngine.wav", FMOD_DEFAULT, 0, &m_engineLoop);

	result = m_audioMgr->createSound("Assets/Audio/SoundFX/ButtonClicked.wav", FMOD_DEFAULT, 0, &m_buttonClick);
	result = m_audioMgr->createSound("Assets/Audio/SoundFX/PlayerJoined.wav", FMOD_DEFAULT, 0, &m_playerJoined);
	result = m_audioMgr->createSound("Assets/Audio/SoundFX/Collision.wav", FMOD_DEFAULT, 0, &m_playerCollision);
	result = m_audioMgr->createSound("Assets/Audio/SoundFX/SnakeCollision.wav", FMOD_DEFAULT, 0, &m_playerSnakeTailCollide);
	result = m_audioMgr->createSound("Assets/Audio/SoundFX/Die.wav", FMOD_DEFAULT, 0, &m_playerFalling);
	result = m_audioMgr->createSound("Assets/Audio/SoundFX/PickUp.wav", FMOD_DEFAULT, 0, &m_pickupCollected);
	result = m_audioMgr->createSound("Assets/Audio/SoundFX/Respawn.wav", FMOD_DEFAULT, 0, &m_respawnPlayers);

	result = m_audioMgr->createChannelGroup(NULL, &channelMusic);
	result = m_audioMgr->createChannelGroup(NULL, &channelEffects);
	result = m_audioMgr->createChannelGroup(NULL, &accelerationChannelGroup);

	m_bgMusic->setMode(FMOD_LOOP_NORMAL);
	m_racingMusic->setMode(FMOD_LOOP_NORMAL);
	m_80sMusic->setMode(FMOD_LOOP_NORMAL);
	m_adventureMusic->setMode(FMOD_LOOP_NORMAL);
	m_franticMusic->setMode(FMOD_LOOP_NORMAL);
	m_engineLoop->setMode(FMOD_LOOP_NORMAL);
	
	m_buttonClick->setMode(FMOD_LOOP_OFF);
	m_playerJoined->setMode(FMOD_LOOP_OFF);
	m_playerCollision->setMode(FMOD_LOOP_OFF);
	m_playerSnakeTailCollide->setMode(FMOD_LOOP_OFF);
	m_playerFalling->setMode(FMOD_LOOP_OFF);
	m_pickupCollected->setMode(FMOD_LOOP_OFF);
	m_respawnPlayers->setMode(FMOD_LOOP_OFF);

	return true;
}

void  Audio::playMenuMusic()
{
	// Play the background music
	m_bgMusicChannel->stop();
	m_audioMgr->playSound(m_bgMusic, channelMusic, false, &m_bgMusicChannel);
}

void  Audio::playTrack1Music()
{
	// Play the track1 music
	m_bgMusicChannel->stop();
	m_audioMgr->playSound(m_adventureMusic, channelMusic, false, &m_t1MusicChannel);
}

void  Audio::playTrack2Music()
{
	// Play the track2 music
	m_bgMusicChannel->stop();
	m_audioMgr->playSound(m_franticMusic, channelMusic, false, &m_t2MusicChannel);
}
void  Audio::playTrack3Music()
{
	// Play the track3 music
	m_bgMusicChannel->stop();
	m_audioMgr->playSound(m_racingMusic, channelMusic, false, &m_t3MusicChannel);
}

void Audio::playSFX(Sound sound)
{
	if (sound == PLAYER_ACCELERATE)
		m_audioMgr->playSound(m_accelerate, accelerationChannelGroup, false, &m_sfxChannel);
	else if (sound == PLAYER_ENGINE)
	{
		m_engineLoop->setMode(FMOD_LOOP_NORMAL);		
		m_accelerateChannel->setVolume(0.2);
		m_audioMgr->playSound(m_engineLoop, accelerationChannelGroup, false, &m_accelerateChannel);
	}
	else if (sound == BUTTON_CLICKED)
		m_audioMgr->playSound(m_buttonClick, channelEffects, false, &m_sfxChannel);
	else if (sound == PLAYER_JOINED)
		m_audioMgr->playSound(m_playerJoined, channelEffects, false, &m_sfxChannel);
	else if (sound == PLAYER_COLLISION)
		m_audioMgr->playSound(m_playerCollision, channelEffects, false, &m_sfxChannel);
	else if (sound == PLAYER_SNAKE_COLLIDE)
		m_audioMgr->playSound(m_playerSnakeTailCollide, channelEffects, false, &m_sfxChannel);
	else if (sound == PLAYER_FALLING)
		m_audioMgr->playSound(m_playerFalling, channelEffects, false, &m_sfxChannel);
	else if (sound == PICKUP_COLLECTED)
		m_audioMgr->playSound(m_pickupCollected, channelEffects, false, &m_sfxChannel);
	else if (sound == RESPAWN_PLAYERS)
		m_audioMgr->playSound(m_respawnPlayers, channelEffects, false, &m_sfxChannel);
}


void Audio::stopAccelerate()
{
	m_engineLoop->setMode(FMOD_LOOP_OFF);
}

Audio& Audio::getInstance()
{
	if (!m_audio)
		m_audio = std::unique_ptr<Audio>(new Audio);

	return (*m_audio);
}

