#include "core.h"
#include "parser.h"
#include "atom.h"
#include "predicate.h"
#include "method.h"
#include "constructor.h"
#include "field.h"

namespace ratio::core
{
    inline bool is_scope(const scope &scp) noexcept { return &scp == &scp.get_core(); }

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

    expr id_expression::evaluate(scope &, context &ctx) const
    {
        expr c_e = ctx->get(ids.begin()->id);
        for (auto it = std::next(ids.begin()); it != ids.end(); ++it)
            c_e = static_cast<complex_item *>(c_e.get())->get(it->id);
        return c_e;
    }

    expr implication_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = dynamic_cast<const ratio::core::expression *>(left.get())->evaluate(scp, ctx);
        expr r = dynamic_cast<const ratio::core::expression *>(right.get())->evaluate(scp, ctx);
        return scp.get_core().disj({scp.get_core().negate(l), r});
    }

    expr disjunction_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(dynamic_cast<const ratio::core::expression *>(e.get())->evaluate(scp, ctx));
        return scp.get_core().disj(exprs);
    }

    expr conjunction_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(dynamic_cast<const ratio::core::expression *>(e.get())->evaluate(scp, ctx));
        return scp.get_core().conj(exprs);
    }

    expr exct_one_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(dynamic_cast<const ratio::core::expression *>(e.get())->evaluate(scp, ctx));
        return scp.get_core().exct_one(exprs);
    }

    expr addition_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(dynamic_cast<const ratio::core::expression *>(e.get())->evaluate(scp, ctx));
        return scp.get_core().add(exprs);
    }

    expr subtraction_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(dynamic_cast<const ratio::core::expression *>(e.get())->evaluate(scp, ctx));
        return scp.get_core().sub(exprs);
    }

    expr multiplication_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(dynamic_cast<const ratio::core::expression *>(e.get())->evaluate(scp, ctx));
        return scp.get_core().mult(exprs);
    }

    expr division_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(dynamic_cast<const ratio::core::expression *>(e.get())->evaluate(scp, ctx));
        return scp.get_core().div(exprs);
    }

    void local_field_statement::execute(scope &scp, context &ctx) const
    {
        for (size_t i = 0; i < names.size(); ++i)
        {
            if (xprs[i])
                ctx->vars.emplace(names[i].id, dynamic_cast<const ratio::core::expression *>(xprs[i].get())->evaluate(scp, ctx));
            else
            {
                scope *s = &scp;
                for (const auto &tp : field_type)
                    s = &s->get_type(tp.id);
                type *t = static_cast<type *>(s);
                if (t->is_primitive())
                    ctx->vars.emplace(names[i].id, t->new_instance());
                else if (!t->get_instances().empty())
                    ctx->vars.emplace(names[i].id, t->new_existential());
                else
                    throw inconsistency_exception();
            }

            if (is_scope(scp)) // we create fields for root items..
                scp.get_core().fields.emplace(names[i].id, std::make_unique<field>(ctx->vars.at(names[i].id)->get_type(), names[i].id));
        }
    }

    void assignment_statement::execute(scope &scp, context &ctx) const
    {
        expr c_e = ctx->get(ids.begin()->id);
        for (auto it = std::next(ids.begin()); it != ids.end(); ++it)
            c_e = static_cast<complex_item *>(c_e.get())->get(it->id);
        static_cast<complex_item *>(c_e.get())->vars.emplace(id.id, dynamic_cast<const ratio::core::expression *>(xpr.get())->evaluate(scp, ctx));
    }
} // namespace ratio::core
