#include "core.h"
#include "parser.h"
#include "atom.h"
#include "predicate.h"
#include "constructor.h"

namespace ratio::core
{
    expr bool_literal_expression::evaluate(scope &scp, const context &) const { return scp.get_core().new_bool(literal.val); }
    expr int_literal_expression::evaluate(scope &scp, const context &) const { return scp.get_core().new_int(literal.val); }
    expr real_literal_expression::evaluate(scope &scp, const context &) const { return scp.get_core().new_real(literal.val); }
    expr string_literal_expression::evaluate(scope &scp, const context &) const { return scp.get_core().new_string(literal.str); }

    expr cast_expression::evaluate(scope &scp, const context &ctx) const { return static_cast<const ratio::core::expression *>(xpr.get())->evaluate(scp, ctx); }

    expr plus_expression::evaluate(scope &scp, const context &ctx) const { return dynamic_cast<const ratio::core::expression *>(xpr.get())->evaluate(scp, ctx); }
    expr minus_expression::evaluate(scope &scp, const context &ctx) const { return scp.get_core().minus(dynamic_cast<const ratio::core::expression *>(xpr.get())->evaluate(scp, ctx)); }
    expr not_expression::evaluate(scope &scp, const context &ctx) const { return scp.get_core().negate(dynamic_cast<const ratio::core::expression *>(xpr.get())->evaluate(scp, ctx)); }

    expr constructor_expression::evaluate(scope &scp, const context &ctx) const
    {
        scope *s = &scp;
        for (const auto &tp : instance_type)
            s = &s->get_type(tp.id);

        std::vector<expr> exprs;
        std::vector<const type *> par_types;
        for (const auto &ex : expressions)
        {
            expr i = dynamic_cast<const ratio::core::expression *>(ex.get())->evaluate(scp, ctx);
            exprs.emplace_back(i);
            par_types.emplace_back(&i->get_type());
        }

        return static_cast<type *>(s)->get_constructor(par_types).new_instance(exprs);
    }
} // namespace ratio::core
