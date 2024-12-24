#ifndef STONE_H
#define STONE_H

class Gem {
   private:
    static int MAX_TYPE;
    int type_;
    bool empty_;
    int id_;

   public:
    static void init_rand();
    Gem();
    Gem(int id);
    Gem(int id, int type);
    Gem(int id, int type, int empty);
    int GetType();
    int GetId();
    bool Empty();
    void SetType(int type);
    void SetEmpty(bool empty);
    void SetId(int id);
    static int GetMaxType();
    static void SetMaxType(int new_max_type=5);
};

#endif  // STONE_H
