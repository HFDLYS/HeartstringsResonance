#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QObject>

class GlobalConfig : public QObject {
    Q_OBJECT
public:
    // 获取单例实例
    static GlobalConfig& getInstance();

    // 获取当前音乐风格
    int getMusicStyle() const;

    // 设置音乐风格
    void setMusicStyle(int style);

    // 获取宝石风格
    int getGemStyle() const;

    // 设置宝石风格
    void setGemStyle(int style);

    // 获取棋盘风格
    int getBoardStyle() const;

    // 设置棋盘风格
    void setBoardStyle(int style);

    // 获取图片风格
    int getPicStyle() const;

    // 设置图片风格
    void setPicStyle(int style);

signals:
    // 当音乐风格变化时发出此信号
    void musicStyleChanged(int newStyle);

    // 如有需要，可以为其他风格变化添加信号
    // void gemStyleChanged(int newStyle);
    // void boardStyleChanged(int newStyle);
    // void picStyleChanged(int newStyle);

private:
    // 私有构造函数实现单例模式
    explicit GlobalConfig(QObject* parent = nullptr);
    ~GlobalConfig() = default;

    // 禁用拷贝构造和赋值运算符
    GlobalConfig(const GlobalConfig&) = delete;
    GlobalConfig& operator=(const GlobalConfig&) = delete;

    // 成员变量
    int gemStyle;
    int boardStyle;
    int musicStyle;
    int picStyle;
};

#endif // GLOBALCONFIG_H
