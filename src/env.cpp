#include "env.h"
#include "core.h"

namespace ratio::core
{
    env::env(core &cr) : cr(cr) {}
    env::env(context ctx) : cr(ctx->get_core()), ctx(ctx) {}

    std::optional<expr> env::get(const std::string &name) noexcept
    {
        if (const auto at_xpr = vars.find(name); at_xpr != vars.cend())
            return at_xpr->second;
        else if (ctx)
            return ctx->get(name);
        else
            return {};
    }
} // namespace ratio::core