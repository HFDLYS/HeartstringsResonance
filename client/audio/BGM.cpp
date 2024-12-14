#include "BGM.h"

QMediaPlayer *BGM::GetMediaPlayer(const QUrl &content, int volume) {
    QMediaPlayer *player = new QMediaPlayer();
    player->setSource(content);
    QAudioOutput *audioOutopt = new QAudioOutput();
    player->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)volume / 100);
    player->play();
    player->stop();

    return player;
}

BGM::BGM() {
    bgm1 = GetMediaPlayer(QUrl("qrc:/sounds/bgm1.wav"), allMusicPercent);
    bgm2 = GetMediaPlayer(QUrl("qrc:/sounds/bgm2.wav"), allMusicPercent);
    label = GetMediaPlayer(QUrl("qrc:/sounds/label.wav"), allSoundPercent);
    open = GetMediaPlayer(QUrl("qrc:/sounds/open.wav"), allSoundPercent);
    skill = GetMediaPlayer(QUrl("qrc:/sounds/skill.wav"), allSoundPercent);
 }

BGM *BGM::instance_ = nullptr;

BGM *BGM::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new BGM();
    }
    return instance_;
}

void BGM::PlayClose() {
    QMediaPlayer *player = new QMediaPlayer();
    if (!BGM::GetInstance()->stopAllSound) {
        close->play();
    }
}
void BGM::StopClose() { close->stop(); }

void BGM::PlayBgm1() {
    if (!BGM::GetInstance()->stopAllMusic) bgm1->play();
}
void BGM::ModifyBgm1(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    bgm1->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}
void BGM::ModifyBgm2(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    bgm2->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}
void BGM::StopBgm1() { bgm1->stop(); }

void BGM::PlayBgm2() {
    if (!BGM::GetInstance()->stopAllMusic) bgm2->play();
}

void BGM::StopBgm2() { bgm2->stop(); }

void BGM::PauseBgm2(){
    bgm2->pause();
}

void BGM::ContinueBgm2(){
    bgm2->play();
}

void BGM::PlayLabel(){
    if(!BGM::GetInstance()->stopAllSound) label->play();
}
void BGM::ModifyLabel(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    label->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}

void BGM::PlayOpen(){
    if(!BGM::GetInstance()->stopAllSound) open->play();
}
void BGM::ModifyOpen(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    open->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}

void BGM::PlaySkill(){
    if(!BGM::GetInstance()->stopAllSound) skill->play();
}
void BGM::ModifySkill(int val) {
    QAudioOutput *audioOutopt = new QAudioOutput();
    skill->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume((double)val / 100);
}

