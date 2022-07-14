#include "scope.h"

namespace ratio::core
{
    ORATIOCORE_EXPORT scope::scope(scope &scp) : cr(scp.get_core()), scp(scp) {}

    ORATIOCORE_EXPORT type &scope::get_type(const std::string &name) const { return scp.get_type(name); }
    ORATIOCORE_EXPORT const std::map<std::string, type *> &scope::get_types() const noexcept { return scp.get_types(); }
} // namespace ratio::core
