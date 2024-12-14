#ifndef BGM_H
#define BGM_H
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QAudioOutput>
class BGM {
private:
    static BGM *instance_;
    QMediaPlayer *player;
    QMediaPlayer *close;
    QMediaPlayer *bgm1;
    QMediaPlayer *bgm2;
    QMediaPlayer *label;
    QMediaPlayer *open;

public:
    BGM();
    bool stopAllMusic = false;
    bool stopAllSound = false;
    int allMusicPercent = 100;
    int allSoundPercent = 100;
    static BGM *GetInstance();

    void PlayClose();
    void StopClose();

    void PlayBgm1();
    void StopBgm1();
    void ModifyBgm1(int);

    void PlayBgm2();
    void StopBgm2();
    void ModifyBgm2(int);
    void PauseBgm2();
    void ContinueBgm2();

    void PlayLabel();
    void ModifyLabel(int);

    void PlayOpen();
    void ModifyOpen(int);

private:
    QMediaPlayer *GetMediaPlayer(const QUrl &content, int volume);
};

#endif // BGM_H
