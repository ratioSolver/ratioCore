#include "method.h"
#include "field.h"
#include "env.h"
#include "parser.h"
#include <cassert>

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
    ORATIOCORE_EXPORT method::~method() {}

    expr method::invoke(context &ctx, std::vector<expr> exprs)
    {
        assert(args.size() == exprs.size());
        context c_ctx(ctx);
        for (size_t i = 0; i < args.size(); ++i)
            c_ctx->vars.emplace(args.at(i)->get_name(), exprs.at(i));

        for (const auto &s : statements)
            dynamic_cast<const statement *>(s.get())->execute(*this, c_ctx);

        if (return_type)
            return c_ctx->vars.at(RETURN_KW);
        else
            return nullptr;
    }
} // namespace ratio::core
