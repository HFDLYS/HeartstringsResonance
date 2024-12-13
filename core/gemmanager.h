#ifndef GEMMANAGER_H
#define GEMMANAGER_H

class GemManager {
public:
    virtual ~GemManager() = default;
    
    // 初始化
    // - 成功，返回 kSuccess
    // - 若已经初始化过，失败，返回 kFailureHaveInitialized
    virtual int Init(int nx, int ny) = 0;

    // 启动（弃用deprecated）
    virtual void Start() = 0;

    // 在(x, y)格生成一个编号为id、类型为type的宝石。
    // 下落fallen_pixel像素后到达目标
    // - 成功，返回 kSuccess
    // - 如果该编号的宝石已经生成，失败，返回 kFailureArgumentError
    virtual int Generate(int id, int x, int y, int type, int fallen_pixel = -1) = 0;

    // 删除编号为id的宝石。如果playAnimation为真，则播放爆炸动画
    // - 成功，返回 kSuccess
    // - 如果该宝石不存在，失败，返回 kFailureArgumentError
    virtual int Remove(int id, bool playAnimation = false) = 0;

    // 设置编号为id的宝石的旋转模式
    // - 成功，返回 kSuccess
    // - 如果该宝石不存在，失败，返回 kFailureArgumentError
    virtual int SetRotate(int id, int rotateMode) = 0;

    // 让编号为id的宝石下落到(x, tar_y)处
    // - 成功，返回 kSuccess
    // - 如果该宝石不存在，失败，返回 kFailureArgumentError
    virtual int FallTo(int id, int tar_y) = 0;

    // 交换编号为id1的宝石和编号为id2的宝石
    // - 成功，返回 kSuccess
    // - 如果宝石不存在，失败，返回 kFailureArgumentError
    virtual int SwapStone(int id1, int id2) = 0;

    // 是否正在播放动画
    virtual bool isPlayingAnimation() = 0;

    // 设置暂停状态
    virtual int SetPause(bool is_pause) = 0;

    // 是否暂停
    virtual bool IsPause() const = 0;

    // 获取最近是否有过Remove动画发生
    // - 通过接口获取后，记录变量会重置为false
    virtual bool haveRemoveInRecentFrame() = 0;

    // 获取最近是否有过Fall动画落到底
    // - 通过接口获取后，记录变量会重置为false
    virtual bool haveFallInRecentFrame() = 0;

    // 刷新动画
    virtual void Update() = 0;

public:
    // Generate's arguments
    static constexpr int kSuccess = 0;
    static constexpr int kFailureArgumentError = 1;
    static constexpr int kFailureOccupied = 2;
    static constexpr int kFailureEmpty = 3;
    static constexpr int kFailureHaveInitialized = 4;
    static constexpr int kFailureHaveNotInitialized = 5;
    static constexpr int kFailureIDHaveBeenUsed = 6;
    static constexpr int kFailureIDNotFound = 7;
    static constexpr int kFailureHavePaused = 8;
    static constexpr int kFailureHaveContinued = 9;

    // SetRotate's arguments
    static constexpr int kStatic = 0;
    static constexpr int kRotate = 1;
    static constexpr int kRotateFast = 2;
    static constexpr int kRotateInverse = 3;
    static constexpr int kRotateFastInverse = 4;
};

#endif  // GEMMANAGER_H