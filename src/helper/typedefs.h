#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <stdint.h>

class BotUser;
#include <unordered_map>
#include <memory>

typedef std::unordered_map<int64_t, std::shared_ptr<BotUser>> UsersCollection;
#endif // TYPEDEFS_H
