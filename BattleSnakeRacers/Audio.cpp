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

bool Audio::InitFmod() {
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_audioMgr);
	if (result != FMOD_OK) return false;
	result = m_audioMgr->init(50, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) return false;
	return true;
}

const bool Audio::LoadAudio() {
	FMOD_RESULT result;
	result = m_audioMgr->createSound("Assets/Audio/80s-Space-Game-Loop_v001.mp3", FMOD_DEFAULT, 0, &m_80sMusic);
	result = m_audioMgr->createSound("Assets/Audio/accelerate.wav", FMOD_DEFAULT, 0, &m_accelerate);
	result = m_audioMgr->createSound("Assets/Audio/Cool-Adventure-Intro.mp3", FMOD_DEFAULT, 0, &m_adventureMusic);
	result = m_audioMgr->createSound("Assets/Audio/background_music.mp3", FMOD_DEFAULT, 0, &m_bgMusic);
	result = m_audioMgr->createSound("Assets/Audio/Frantic-Gameplay.mp3", FMOD_DEFAULT, 0, &m_franticMusic);
	result = m_audioMgr->createSound("Assets/Audio/loopingLinearEngine.mp3", FMOD_DEFAULT, 0, &m_engineLoop);
	result = m_audioMgr->createSound("Assets/Audio/Racing-Menu.mp3", FMOD_DEFAULT, 0, &m_racingMusic);
	m_bgMusic->setMode(FMOD_LOOP_NORMAL);
	m_racingMusic->setMode(FMOD_LOOP_NORMAL);
	m_80sMusic->setMode(FMOD_LOOP_NORMAL);
	m_adventureMusic->setMode(FMOD_LOOP_NORMAL);
	m_franticMusic->setMode(FMOD_LOOP_NORMAL);
	m_engineLoop->setMode(FMOD_LOOP_NORMAL);
	m_buttonClick->setMode(FMOD_LOOP_OFF);
	return true;
}

void  Audio::playMenuMusic()
{
	// Play the background music
	m_bgMusicChannel->stop();
	m_audioMgr->playSound(m_bgMusic, 0, false, &m_bgMusicChannel);
}

void  Audio::playTrack1Music()
{
	// Play the track1 music
	m_bgMusicChannel->stop();
	m_audioMgr->playSound(m_adventureMusic, 0, false, &m_bgMusicChannel);
}

void  Audio::playTrack2Music()
{
	// Play the track2 music
	m_bgMusicChannel->stop();
	m_audioMgr->playSound(m_franticMusic, 0, false, &m_bgMusicChannel);
}
void  Audio::playTrack3Music()
{
	// Play the track3 music
	m_bgMusicChannel->stop();
	m_audioMgr->playSound(m_racingMusic, 0, false, &m_bgMusicChannel);
}

void Audio::playButtonClick()
{
	// Play the click music
	m_audioMgr->playSound(m_buttonClick, 0, false, &m_sfxChannel);
}

void Audio::playSFX(Sound sound)
{
	if(sound == PLAYER_CRASH)
		m_audioMgr->playSound(m_enemyDead, 0, false, &m_sfxChannel);
	else if (sound == PLAYER_ACCELERATE)
		m_audioMgr->playSound(m_accelerate, 0, false, &m_sfxChannel);
	else if (sound == PLAYER_ENGINE)
		m_audioMgr->playSound(m_engineLoop, 0, false, &m_enemyChannel);
	else if (sound == SCORE_PICKUP)
		m_audioMgr->playSound(m_scorePickup, 0, false, &m_sfxChannel);
	else if (sound == NEXT_LEVEL)
		m_audioMgr->playSound(m_nextLevel, 0, false, &m_sfxChannel);
	else if (sound == PLAYER_DEAD)
		m_audioMgr->playSound(m_playerDead, 0, false, &m_sfxChannel);
	else if (sound == PLAYER_SHOOT)
		m_audioMgr->playSound(m_adventureMusic, 0, false, &m_bulletChannel);
	else if (sound == PLAYER_SPAWNING)
		m_audioMgr->playSound(m_playerSpawning, 0, false, &m_sfxChannel);
	else if (sound == PLAYER_DESCENDING)
		m_audioMgr->playSound(m_accelerate, 0, false, &m_sfxChannel);
}

Audio& Audio::getInstance()
{
	if (!m_audio)
		m_audio = std::unique_ptr<Audio>(new Audio);

	return (*m_audio);
}

