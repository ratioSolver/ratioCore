#include "env.h"
#include "core.h"

namespace ratio::core
{
    env::env(env &e) : e(e) {}
    env::env(context ctx) : e(*ctx), ctx(ctx) {}

    ORATIOCORE_EXPORT expr env::get(const std::string &name) noexcept
    {
        if (const auto at_xpr = vars.find(name); at_xpr != vars.cend())
            return at_xpr->second;
        else if (ctx)
            return ctx->get(name);
        else
            return e.get(name);
    }
} // namespace ratio::core