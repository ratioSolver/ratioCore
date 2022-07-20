#include "conjunction.h"
#include "type.h"
#include "env.h"
#include "riddle_parser.h"

namespace ratio::core
{
    conjunction::conjunction(scope &scp, context ctx, semitone::rational cst, const std::vector<std::unique_ptr<const riddle::ast::statement>> &stmnts) : scope(scp), ctx(ctx), cost(std::move(cst)), statements(stmnts) {}
    conjunction::~conjunction() {}
} // namespace ratio::core
