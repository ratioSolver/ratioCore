#include "conjunction.h"
#include "type.h"
#include "env.h"
#include "riddle_parser.h"

namespace ratio::core
{
    conjunction::conjunction(type &tp, semitone::rational cst, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) : scope(tp), cost(std::move(cst)), statements(std::move(stmnts)) {}
} // namespace ratio::core
