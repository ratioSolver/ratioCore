#include "method.h"
#include "field.h"
#include "riddle_parser.h"

namespace ratio::core
{
    ORATIOCORE_EXPORT method::method(scope &scp, type *return_type, const std::string &name, std::vector<field_ptr> args, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) : scope(scp), return_type(return_type), name(name), statements(std::move(stmnts))
    {
        this->args.reserve(args.size());
        for (auto &f : args)
        {
            this->args.push_back(f.get());
            new_field(std::move(f));
        }
    }
} // namespace ratio::core
