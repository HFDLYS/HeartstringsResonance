#include "audiomanager.h"
#include "../config/globalconfig.h"
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
    bgmlist[0] =  GetMediaPlayer(QUrl("qrc:/sounds/bgm1.wav"), allMusicPercent);
    bgmlist[1] =  GetMediaPlayer(QUrl("qrc:/sounds/hajimi.wav"), allMusicPercent);
    bgmlist[2] =  GetMediaPlayer(QUrl("qrc:/sounds/fairytale.wav"), allMusicPercent);
    int bgmid = GlobalConfig::getInstance().getMusicStyle();
    if (bgmid >= 0 && bgmid <= 2 ) {
        bgm1 = bgmlist[bgmid];
    } else {
        bgm1 = bgmlist[0];
    }
    bgm2 = GetMediaPlayer(QUrl("qrc:/sounds/bgm2.wav"), allMusicPercent);
    bgm3 = GetMediaPlayer(QUrl("qrc:/sounds/bgm_battle.wav"), allMusicPercent);
    label = GetMediaPlayer(QUrl("qrc:/sounds/label.wav"), allSoundPercent);
    open = GetMediaPlayer(QUrl("qrc:/sounds/open.wav"), allSoundPercent);
    skill = GetMediaPlayer(QUrl("qrc:/sounds/skill.wav"), allSoundPercent);
    match1 = GetMediaPlayer(QUrl("qrc:/sounds/match1.wav"), allSoundPercent);
    match2 = GetMediaPlayer(QUrl("qrc:/sounds/match2.wav"), allSoundPercent);
    match3 = GetMediaPlayer(QUrl("qrc:/sounds/match3.wav"), allSoundPercent);
    connect(&GlobalConfig::getInstance(), &GlobalConfig::musicStyleChanged,
            this, &AudioManager::onMusicStyleChanged);
 }

void AudioManager::onMusicStyleChanged(int newStyle) {
    if (bgm1 != bgmlist[newStyle]) {
        bgm1->stop();
        bgm1 = bgmlist[newStyle];
        if (!stopAllMusic) {
            bgm1->play();
        }
    }
}


AudioManager *AudioManager::instance_ = nullptr;

AudioManager *AudioManager::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new AudioManager();
    }
    return instance_;
}

AudioManager *AudioManager::GetNewInstance() {
    instance_ = new AudioManager();
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
void AudioManager::PlayMatch1(){
    if(!AudioManager::GetInstance()->stopAllSound) match1->play();
}
void AudioManager::ModifyMatch1(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    match1->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}

void AudioManager::PlayMatch2(){
    if(!AudioManager::GetInstance()->stopAllSound) match2->play();
}

void AudioManager::PlayMatch3(){
    if(!AudioManager::GetInstance()->stopAllSound) match3->play();
}

void AudioManager::ModifyMatch2(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    match2->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}

void AudioManager::ModifyMatch3(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    match3->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}
