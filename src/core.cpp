#include "core.h"
#include "predicate.h"

namespace ratio::core
{
    ORATIOCORE_EXPORT core::core() : scope(*this), env(std::make_shared<env>(*this)) {}
    ORATIOCORE_EXPORT core::~core()
    {
        // we delete the types..
        for ([[maybe_unused]] const auto &[tp_name, tp] : types)
            delete tp;
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
