#include "env.h"
#include "core.h"

namespace ratio::core
{
    var_map::var_map(var_map *ctx) : ctx(ctx) {}

    RATIOCORE_EXPORT expr var_map::get(const std::string &name)
    {
        if (const auto at_xpr = vars.find(name); at_xpr != vars.cend())
            return at_xpr->second;
        else if (ctx)
            return ctx->get(name);
        else
            return nullptr;
    }

    env::env(context ctx) : ctx(ctx) {}
} // namespace ratio::core