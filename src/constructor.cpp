#include "constructor.h"
#include "type.h"
#include "item.h"
#include "riddle_parser.h"
#include <cassert>

namespace ratio::core
{
    constructor::constructor(type &tp, std::vector<field_ptr> args, std::vector<riddle::id_token> ins, std::vector<std::vector<std::unique_ptr<const riddle::ast::expression>>> ivs, std::vector<std::unique_ptr<const riddle::ast::statement>> stmnts) : scope(tp, args), args(std::move(args)), init_names(std::move(ins)), init_vals(std::move(ivs)), statements(std::move(stmnts)) { new_field(std::make_shared<field>(tp, THIS_KEYWORD)); }

    expr constructor::new_instance(const std::vector<expr> &exprs) noexcept
    {
        assert(args.size() == exprs.size());

        type &t = static_cast<type &>(get_scope());
        expr i = t.new_instance();

        invoke(*static_cast<complex_item *>(i.get()), exprs);

        return i;
    }

    void constructor::invoke(complex_item &itm, const std::vector<expr> &exprs)
    {
        auto ctx = std::make_shared<env>(itm);
        ctx->vars.emplace(THIS_KEYWORD, expr(&itm));
        for (size_t i = 0; i < args.size(); ++i)
            ctx->vars.emplace(args.at(i)->get_name(), exprs.at(i));
    }
} // namespace ratio::core
