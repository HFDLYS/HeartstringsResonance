#include "globalconfig.h"


GlobalConfig& GlobalConfig::getInstance() {
    static GlobalConfig instance;
    return instance;
}


GlobalConfig::GlobalConfig(QObject* parent)
    : QObject(parent),
    gemStyle(0),
    boardStyle(0),
    musicStyle(0),
    picStyle(0) {}


int GlobalConfig::getMusicStyle() const {
    return musicStyle;
}


void GlobalConfig::setMusicStyle(int style) {
    if (musicStyle != style) {
        musicStyle = style;
        emit musicStyleChanged(style);
    }
}

int GlobalConfig::getGemStyle() const {
    return gemStyle;
}


void GlobalConfig::setGemStyle(int style) {
    if (gemStyle != style) {
        gemStyle = style;
    }
}


int GlobalConfig::getBoardStyle() const {
    return boardStyle;
}


void GlobalConfig::setBoardStyle(int style) {
    if (boardStyle != style) {
        boardStyle = style;

    }
}

int GlobalConfig::getPicStyle() const {
    return picStyle;
}

void GlobalConfig::setPicStyle(int style) {
    if (picStyle != style) {
        picStyle = style;

    }
}
