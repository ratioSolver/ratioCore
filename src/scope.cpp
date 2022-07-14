#include "scope.h"

namespace ratio::core
{
    scope::scope(scope &scp) : cr(scp.get_core()), scp(scp) {}
} // namespace ratio::core
