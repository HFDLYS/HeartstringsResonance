#ifndef BGM_H
#define BGM_H
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QAudioOutput>
class BGM {
private:
    static BGM *instance_;
    QMediaPlayer *player;
    QMediaPlayer *bgm1;
    QMediaPlayer *bgm2;

public:
    BGM();
    int allMusicPercent = 100;
    int allSoundPercent = 100;
    static BGM *GetInstance();
    void PlayBgm1();
    void StopBgm1();

    void PlayBgm2();
    void StopBgm2();

private:
    QMediaPlayer *GetMediaPlayer(const QUrl &content, int volume);
};

#endif // BGM_H
