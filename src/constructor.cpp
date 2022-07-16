#include "constructor.h"
#include "type.h"
#include "env.h"
#include "riddle_parser.h"

namespace ratio::core
{
    constructor::constructor(type &tp, const std::vector<field_ptr> &args, std::vector<std::pair<const std::string, const std::vector<std::unique_ptr<riddle::ast::expression>>>> il, std::vector<std::unique_ptr<riddle::ast::statement>> stmnts) : scope(tp, args), init_list(std::move(il)), statements(std::move(stmnts)) { new_field(std::make_shared<field>(tp, THIS_KEYWORD)); }
} // namespace ratio::core
