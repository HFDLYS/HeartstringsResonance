#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

class GlobalConfig {
    public:
        static GlobalConfig& getInstance() {
            static GlobalConfig instance;
            return instance;
        }

    private:
        GlobalConfig() {}
        ~GlobalConfig() {}
        GlobalConfig(const GlobalConfig&) = delete;
        GlobalConfig& operator=(const GlobalConfig&) = delete;

        int gemStyle = 0;
    public:
        int getGemStyle() {
            return gemStyle;
        }
        void setGemStyle(int style) {
            gemStyle = style;
        }
};

#endif