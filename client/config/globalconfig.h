#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QObject>

class GlobalConfig : public QObject {
    Q_OBJECT
public:

    static GlobalConfig& getInstance();


    int getMusicStyle() const;


    void setMusicStyle(int style);


    int getGemStyle() const;


    void setGemStyle(int style);


    int getBoardStyle() const;


    void setBoardStyle(int style);


    int getPicStyle() const;


    void setPicStyle(int style);

signals:
    // 当音乐风格变化时发出此信号
    void musicStyleChanged(int newStyle);



private:

    explicit GlobalConfig(QObject* parent = nullptr);
    ~GlobalConfig() = default;


    GlobalConfig(const GlobalConfig&) = delete;
    GlobalConfig& operator=(const GlobalConfig&) = delete;


    int gemStyle;
    int boardStyle;
    int musicStyle;
    int picStyle;
};

#endif // GLOBALCONFIG_H
