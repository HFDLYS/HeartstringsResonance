#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QAudioOutput>
class AudioManager {
private:
    static AudioManager *instance_;
    QMediaPlayer *player;
    QMediaPlayer *close;
    QMediaPlayer *bgm1;
    QMediaPlayer *bgm2;
    QMediaPlayer *bgm3;
    QMediaPlayer *label;
    QMediaPlayer *open;
    QMediaPlayer *skill;

public:
    AudioManager();
    bool stopAllMusic = false;
    bool stopAllSound = false;
    int allMusicPercent = 100;
    int allSoundPercent = 100;
    static AudioManager *GetInstance();

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

    void PlayBgm3();
    void StopBgm3();
    void ModifyBgm3(int);
    void PauseBgm3();
    void ContinueBgm3();


    void PlayLabel();
    void ModifyLabel(int);

    void PlayOpen();
    void ModifyOpen(int);

    void PlaySkill();
    void ModifySkill(int);

private:
    QMediaPlayer *GetMediaPlayer(const QUrl &content, int volume);
};

#endif // AUDIOMANAGER_H
