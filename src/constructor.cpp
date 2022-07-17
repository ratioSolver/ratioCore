#include "constructor.h"
#include "type.h"
#include "env.h"
#include "riddle_parser.h"

namespace ratio::core
{
    constructor::constructor(type &tp, std::vector<field_ptr> args, std::vector<riddle::id_token> ins, std::vector<std::vector<std::unique_ptr<const riddle::ast::expression>>> ivs, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) : scope(tp, args), args(std::move(args)), init_names(std::move(ins)), init_vals(std::move(ivs)), statements(std::move(stmnts)) { new_field(std::make_shared<field>(tp, THIS_KEYWORD)); }
} // namespace ratio::core
