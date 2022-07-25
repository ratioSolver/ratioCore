#include "conjunction.h"
#include "type.h"
#include "env.h"
#include "parser.h"

namespace ratio::core
{
    conjunction::conjunction(scope &scp, context ctx, semitone::rational cst, const std::vector<std::unique_ptr<const riddle::ast::statement>> &stmnts) : scope(scp), ctx(ctx), cost(std::move(cst)), statements(stmnts) {}
    conjunction::~conjunction() {}

    RATIOCORE_EXPORT void conjunction::execute()
    {
        context c_ctx(ctx);
        for (const auto &s : statements)
            dynamic_cast<const statement *>(s.get())->execute(*this, c_ctx);
    }
} // namespace ratio::core
