#ifndef SERVERGEMMANAGER_H
#define SERVERGEMMANAGER_H

#include "../../core/gemmanager.h"

class ServerGemManager : public GemManager {
public:
    ServerGemManager() {

    }
    ~ServerGemManager() {

    }

    int Init(int nx, int ny) {
        return kSuccess;
    }

    void Start() {
    }

    int Generate(int id, int x, int y, int type, int fallen_pixel = -1) {
        return kSuccess;
    }

    int Remove(int id, bool playAnimation = false) {
        return kSuccess;
    }

    int SetRotate(int id, int rotateMode) {
        return kSuccess;
    }



    int FallTo(int id, int tar_y) {
        return kSuccess;
    }

    int SwapStone(int id1, int id2) {
        return kSuccess;
    }

    bool isPlayingAnimation() {
        return false;
    }

    int SetPause(bool is_pause) {
        return kSuccess;
    }

    bool IsPause() const {
        return false;
    }


    void Update() {

    }

    bool haveRemoveInRecentFrame() {
        return false;
    }

    bool haveFallInRecentFrame() {
        return false;
    }

};

#endif // SERVERGEMMANAGER_H