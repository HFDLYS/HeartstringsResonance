#include "BGM.h"
#include <iostream>

QMediaPlayer *BGM::GetMediaPlayer(const QUrl &content, int volume) {
    QMediaPlayer *player = new QMediaPlayer();
    player->setSource(content);
    QAudioOutput *audioOutopt = new QAudioOutput();
    player->setAudioOutput(audioOutopt);
    audioOutopt -> setVolume(volume);
    player->play();
    player->stop();

    return player;
}

BGM::BGM() {
    bgm1 = GetMediaPlayer(QUrl::fromLocalFile("E:/Users/admin/Desktop/Software major practical training/HeartstringsResonance/resource/sounds/bgm1.wav"), 100);
    bgm2 = GetMediaPlayer(QUrl::fromLocalFile("E:/Users/admin/Desktop/Software major practical training/HeartstringsResonance/resource/sounds/bgm2.wav"), 100);
 }

BGM *BGM::instance_ = nullptr;

BGM *BGM::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new BGM();
    }
    return instance_;
}

void BGM::PlayBgm1() {
    bgm1->play();
}

void BGM::StopBgm1() { bgm1->stop(); }

void BGM::PlayBgm2() {
    bgm2->play();
}

void BGM::StopBgm2() { bgm2->stop(); }
