#include "core.h"
#include "predicate.h"
#include <sstream>
#include <fstream>

namespace ratio::core
{
    ORATIOCORE_EXPORT core::core() : scope(*this), env(*this) {}

    ORATIOCORE_EXPORT void core::read(const std::string &script)
    {
        std::stringstream ss(script);
    }

    ORATIOCORE_EXPORT void core::read(const std::vector<std::string> &files)
    {
        for (const auto &f : files)
            if (std::ifstream ifs(f); ifs)
            {
            }
            else
                throw std::invalid_argument("cannot find file '" + f + "'");
    }

    ORATIOCORE_EXPORT expr core::get(const std::string &name) noexcept
    {
        if (const auto at_xpr = vars.find(name); at_xpr != vars.cend())
            return at_xpr->second;
        else
            return nullptr;
    }

    ORATIOCORE_EXPORT type &core::get_type(const std::string &name) const
    {
        if (const auto at_tp = types.find(name); at_tp != types.cend())
            return *at_tp->second;

        // not found
        throw std::out_of_range(name);
    }
} // namespace ratio::core
