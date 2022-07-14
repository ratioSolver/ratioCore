#include "core.h"
#include "predicate.h"

namespace ratio::core
{
    core::core() : scope(*this), env(std::make_shared<env>(*this)) {}
    core::~core()
    {
        // we delete the types..
        for ([[maybe_unused]] const auto &[tp_name, tp] : types)
            delete tp;
    }

    std::optional<expr> core::get(const std::string &name) noexcept
    {
        if (const auto at_xpr = vars.find(name); at_xpr != vars.cend())
            return at_xpr->second;
        else
            return {};
    }
} // namespace ratio::core
