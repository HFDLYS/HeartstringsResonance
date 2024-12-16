#include "audiomanager.h"

QMediaPlayer *AudioManager::GetMediaPlayer(const QUrl &content, int volume) {
    QMediaPlayer *player = new QMediaPlayer();
    player->setSource(content);
    QAudioOutput *audioOutopt = new QAudioOutput();
    player->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)volume / 100);
    player->play();
    player->stop();

    return player;
}

AudioManager::AudioManager() {
    bgm1 = GetMediaPlayer(QUrl("qrc:/sounds/bgm1.wav"), allMusicPercent);
    bgm2 = GetMediaPlayer(QUrl("qrc:/sounds/bgm2.wav"), allMusicPercent);
    bgm3 = GetMediaPlayer(QUrl("qrc:/sounds/bgm_battle.wav"), allMusicPercent);
    label = GetMediaPlayer(QUrl("qrc:/sounds/label.wav"), allSoundPercent);
    open = GetMediaPlayer(QUrl("qrc:/sounds/open.wav"), allSoundPercent);
    skill = GetMediaPlayer(QUrl("qrc:/sounds/skill.wav"), allSoundPercent);
 }

AudioManager *AudioManager::instance_ = nullptr;

AudioManager *AudioManager::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new AudioManager();
    }
    return instance_;
}

void AudioManager::PlayClose() {
    QMediaPlayer *player = new QMediaPlayer();
    if (!AudioManager::GetInstance()->stopAllSound) {
        close->play();
    }
}
void AudioManager::StopClose() { close->stop(); }

void AudioManager::PlayBgm1() {
    if (!AudioManager::GetInstance()->stopAllMusic) bgm1->play();
}
void AudioManager::PlayBgm2() {
    if (!AudioManager::GetInstance()->stopAllMusic) bgm2->play();
}
void AudioManager::PlayBgm3() {
    if (!AudioManager::GetInstance()->stopAllMusic) bgm3->play();
}
void AudioManager::ModifyBgm1(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    bgm1->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}
void AudioManager::ModifyBgm2(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    bgm2->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}
void AudioManager::ModifyBgm3(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    bgm3->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}
void AudioManager::StopBgm1() { bgm1->stop(); }
void AudioManager::StopBgm2() { bgm2->stop(); }
void AudioManager::StopBgm3() { bgm3->stop(); }

void AudioManager::PauseBgm2(){
    bgm2->pause();
}

void AudioManager::ContinueBgm2(){
    bgm2->play();
}

void AudioManager::PauseBgm3(){
    bgm3->pause();
}

void AudioManager::ContinueBgm3(){
    bgm3->play();
}

void AudioManager::PlayLabel(){
    if(!AudioManager::GetInstance()->stopAllSound) label->play();
}
void AudioManager::ModifyLabel(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    label->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}

void AudioManager::PlayOpen(){
    if(!AudioManager::GetInstance()->stopAllSound) open->play();
}
void AudioManager::ModifyOpen(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    open->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}

void AudioManager::PlaySkill(){
    if(!AudioManager::GetInstance()->stopAllSound) skill->play();
}
void AudioManager::ModifySkill(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    skill->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}

