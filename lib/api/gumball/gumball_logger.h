#include <gimbal/core/gimbal_logger.h>

#define GUM_LOG_VERBOSE(...) GBL_LOG_VERBOSE("gumball", __VA_ARGS__)
#define GUM_LOG_DEBUG(...)   GBL_LOG_DEBUG("gumball", __VA_ARGS__)
#define GUM_LOG_INFO(...)    GBL_LOG_INFO("gumball", __VA_ARGS__)
#define GUM_LOG_WARN(...)    GBL_LOG_WARN("gumball", __VA_ARGS__)
#define GUM_LOG_ERROR(...)   GBL_LOG_ERROR("gumball", __VA_ARGS__)
#define GUM_LOG_PUSH()       GBL_LOG_PUSH()
#define GUM_LOG_POP(n)       GBL_LOG_POP(n)