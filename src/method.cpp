#include "method.h"
#include "riddle_parser.h"

namespace ratio::core
{
    ORATIOCORE_EXPORT method::method(scope &scp, type *return_type, const std::string &name, std::vector<field_ptr> args, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) : scope(scp, std::move(args)), return_type(std::move(return_type)), name(name), args(std::move(args)), statements(std::move(stmnts)) {}
} // namespace ratio::core
