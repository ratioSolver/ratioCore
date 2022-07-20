#include "core.h"
#include "parser.h"
#include "atom.h"
#include "predicate.h"
#include "method.h"
#include "constructor.h"

namespace ratio::core
{
    expr bool_literal_expression::evaluate(scope &scp, context &) const { return scp.get_core().new_bool(literal.val); }
    expr int_literal_expression::evaluate(scope &scp, context &) const { return scp.get_core().new_int(literal.val); }
    expr real_literal_expression::evaluate(scope &scp, context &) const { return scp.get_core().new_real(literal.val); }
    expr string_literal_expression::evaluate(scope &scp, context &) const { return scp.get_core().new_string(literal.str); }

    expr cast_expression::evaluate(scope &scp, context &ctx) const { return static_cast<const ratio::core::expression *>(xpr.get())->evaluate(scp, ctx); }

    expr plus_expression::evaluate(scope &scp, context &ctx) const { return dynamic_cast<const ratio::core::expression *>(xpr.get())->evaluate(scp, ctx); }
    expr minus_expression::evaluate(scope &scp, context &ctx) const { return scp.get_core().minus(dynamic_cast<const ratio::core::expression *>(xpr.get())->evaluate(scp, ctx)); }
    expr not_expression::evaluate(scope &scp, context &ctx) const { return scp.get_core().negate(dynamic_cast<const ratio::core::expression *>(xpr.get())->evaluate(scp, ctx)); }

    expr constructor_expression::evaluate(scope &scp, context &ctx) const
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

        return static_cast<type *>(s)->get_constructor(par_types).new_instance(std::move(exprs));
    }

    expr eq_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = dynamic_cast<const ratio::core::expression *>(left.get())->evaluate(scp, ctx);
        expr r = dynamic_cast<const ratio::core::expression *>(right.get())->evaluate(scp, ctx);
        return scp.get_core().eq(l, r);
    }

    expr neq_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = dynamic_cast<const ratio::core::expression *>(left.get())->evaluate(scp, ctx);
        expr r = dynamic_cast<const ratio::core::expression *>(right.get())->evaluate(scp, ctx);
        return scp.get_core().negate(scp.get_core().eq(l, r));
    }

    expr lt_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = dynamic_cast<const ratio::core::expression *>(left.get())->evaluate(scp, ctx);
        expr r = dynamic_cast<const ratio::core::expression *>(right.get())->evaluate(scp, ctx);
        return scp.get_core().lt(l, r);
    }

    expr leq_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = dynamic_cast<const ratio::core::expression *>(left.get())->evaluate(scp, ctx);
        expr r = dynamic_cast<const ratio::core::expression *>(right.get())->evaluate(scp, ctx);
        return scp.get_core().leq(l, r);
    }

    expr geq_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = dynamic_cast<const ratio::core::expression *>(left.get())->evaluate(scp, ctx);
        expr r = dynamic_cast<const ratio::core::expression *>(right.get())->evaluate(scp, ctx);
        return scp.get_core().geq(l, r);
    }

    expr gt_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = dynamic_cast<const ratio::core::expression *>(left.get())->evaluate(scp, ctx);
        expr r = dynamic_cast<const ratio::core::expression *>(right.get())->evaluate(scp, ctx);
        return scp.get_core().gt(l, r);
    }

    expr function_expression::evaluate(scope &scp, context &ctx) const
    {
        scope *s = &scp;
        for (const auto &id_tk : ids)
            s = &s->get_type(id_tk.id);

        std::vector<expr> exprs;
        std::vector<const type *> par_types;
        for (const auto &ex : expressions)
        {
            expr i = dynamic_cast<const ratio::core::expression *>(ex.get())->evaluate(scp, ctx);
            exprs.emplace_back(i);
            par_types.emplace_back(&i->get_type());
        }

        if (method &m = s->get_method(function_name.id, par_types); m.get_return_type())
        {
            const auto rt = m.get_return_type();
            if (rt == &scp.get_core().get_type(BOOL_KW))
                return m.invoke(ctx, std::move(exprs));
            else if (rt == &scp.get_core().get_type(INT_KW) || rt == &scp.get_core().get_type(REAL_KW) || rt == &scp.get_core().get_type(TIME_KW))
                return m.invoke(ctx, std::move(exprs));
            else
                return m.invoke(ctx, std::move(exprs));
        }
        else
            return scp.get_core().new_bool(true);
    }
} // namespace ratio::core
