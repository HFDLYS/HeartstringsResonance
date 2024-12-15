#include "gem.h"

#include <cstdlib>
#include <ctime>

int Gem::MAX_TYPE = 5;

void Gem::init_rand() { srand((unsigned int)time(0)); }

Gem::Gem() {
    id_ = 0;
    type_ = rand() % MAX_TYPE + 1;
    empty_ = 0;
}

Gem::Gem(int id) : id_(id) {
    empty_ = 0;
    type_ = rand() % MAX_TYPE + 1;
}

Gem::Gem(int id, int type) : type_(type), id_(id) { empty_ = 0; }

Gem::Gem(int id, int type, int empty) : type_(type), empty_(empty), id_(id) {}

int Gem::GetMaxType() { return MAX_TYPE; }

void Gem::SetMaxType(int new_max_type) { MAX_TYPE = new_max_type; }

int Gem::GetType() { return type_; }

int Gem::GetId() { return id_; }

void Gem::SetId(int id) { id_ = id; }

bool Gem::Empty() { return empty_; }

void Gem::SetType(int type) { type_ = type; }

void Gem::SetEmpty(bool empty) { empty_ = empty; }
