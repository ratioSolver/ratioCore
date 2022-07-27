#include "conjunction.h"
#include "type.h"
#include "env.h"
#include "parser.h"

namespace ratio::core
{
    conjunction::conjunction(scope &scp, semitone::rational cst, const std::vector<std::unique_ptr<const riddle::ast::statement>> &stmnts) : scope(scp), cost(std::move(cst)), statements(stmnts) {}
    conjunction::~conjunction() {}

    RATIOCORE_EXPORT void conjunction::execute(context &ctx)
    {
        context c_ctx(ctx);
        for (const auto &s : statements)
            dynamic_cast<const statement &>(*s).execute(*this, c_ctx);
    }
} // namespace ratio::core
