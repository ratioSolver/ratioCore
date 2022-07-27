#include "core.h"
#include "parser.h"
#include "atom.h"
#include "predicate.h"
#include "method.h"
#include "constructor.h"
#include "field.h"
#include "conjunction.h"
#include <unordered_map>
#include <queue>

namespace ratio::core
{
    inline bool is_core(const scope &scp) noexcept { return &scp == &scp.get_core(); }

    expr bool_literal_expression::evaluate(scope &scp, context &) const { return scp.get_core().new_bool(literal.val); }
    expr int_literal_expression::evaluate(scope &scp, context &) const { return scp.get_core().new_int(literal.val); }
    expr real_literal_expression::evaluate(scope &scp, context &) const { return scp.get_core().new_real(literal.val); }
    expr string_literal_expression::evaluate(scope &scp, context &) const { return scp.get_core().new_string(literal.str); }

    expr cast_expression::evaluate(scope &scp, context &ctx) const { return static_cast<const ratio::core::expression &>(*xpr).evaluate(scp, ctx); }

    expr plus_expression::evaluate(scope &scp, context &ctx) const { return static_cast<const ratio::core::expression &>(*xpr).evaluate(scp, ctx); }
    expr minus_expression::evaluate(scope &scp, context &ctx) const { return scp.get_core().minus(static_cast<const ratio::core::expression &>(*xpr).evaluate(scp, ctx)); }
    expr not_expression::evaluate(scope &scp, context &ctx) const { return scp.get_core().negate(static_cast<const ratio::core::expression &>(*xpr).evaluate(scp, ctx)); }

    expr constructor_expression::evaluate(scope &scp, context &ctx) const
    {
        scope *s = &scp;
        for (const auto &tp : instance_type)
            s = &s->get_type(tp.id);

        std::vector<expr> exprs;
        std::vector<const type *> par_types;
        for (const auto &ex : expressions)
        {
            expr i = static_cast<const ratio::core::expression &>(*ex).evaluate(scp, ctx);
            exprs.emplace_back(i);
            par_types.emplace_back(&i->get_type());
        }

        return static_cast<type *>(s)->get_constructor(par_types).new_instance(std::move(exprs));
    }

    expr eq_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = static_cast<const ratio::core::expression &>(*left).evaluate(scp, ctx);
        expr r = static_cast<const ratio::core::expression &>(*right).evaluate(scp, ctx);
        return scp.get_core().eq(l, r);
    }

    expr neq_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = static_cast<const ratio::core::expression &>(*left).evaluate(scp, ctx);
        expr r = static_cast<const ratio::core::expression &>(*right).evaluate(scp, ctx);
        return scp.get_core().negate(scp.get_core().eq(l, r));
    }

    expr lt_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = static_cast<const ratio::core::expression &>(*left).evaluate(scp, ctx);
        expr r = static_cast<const ratio::core::expression &>(*right).evaluate(scp, ctx);
        return scp.get_core().lt(l, r);
    }

    expr leq_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = static_cast<const ratio::core::expression &>(*left).evaluate(scp, ctx);
        expr r = static_cast<const ratio::core::expression &>(*right).evaluate(scp, ctx);
        return scp.get_core().leq(l, r);
    }

    expr geq_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = static_cast<const ratio::core::expression &>(*left).evaluate(scp, ctx);
        expr r = static_cast<const ratio::core::expression &>(*right).evaluate(scp, ctx);
        return scp.get_core().geq(l, r);
    }

    expr gt_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = static_cast<const ratio::core::expression &>(*left).evaluate(scp, ctx);
        expr r = static_cast<const ratio::core::expression &>(*right).evaluate(scp, ctx);
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
            expr i = static_cast<const ratio::core::expression &>(*ex).evaluate(scp, ctx);
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
            c_e = static_cast<complex_item &>(*c_e).get(it->id);
        return c_e;
    }

    expr implication_expression::evaluate(scope &scp, context &ctx) const
    {
        expr l = static_cast<const ratio::core::expression &>(*left).evaluate(scp, ctx);
        expr r = static_cast<const ratio::core::expression &>(*right).evaluate(scp, ctx);
        return scp.get_core().disj({scp.get_core().negate(l), r});
    }

    expr disjunction_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(static_cast<const ratio::core::expression &>(*e).evaluate(scp, ctx));
        return scp.get_core().disj(exprs);
    }

    expr conjunction_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(static_cast<const ratio::core::expression &>(*e).evaluate(scp, ctx));
        return scp.get_core().conj(exprs);
    }

    expr exct_one_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(static_cast<const ratio::core::expression &>(*e).evaluate(scp, ctx));
        return scp.get_core().exct_one(exprs);
    }

    expr addition_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(static_cast<const ratio::core::expression &>(*e).evaluate(scp, ctx));
        return scp.get_core().add(exprs);
    }

    expr subtraction_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(static_cast<const ratio::core::expression &>(*e).evaluate(scp, ctx));
        return scp.get_core().sub(exprs);
    }

    expr multiplication_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(static_cast<const ratio::core::expression &>(*e).evaluate(scp, ctx));
        return scp.get_core().mult(exprs);
    }

    expr division_expression::evaluate(scope &scp, context &ctx) const
    {
        std::vector<expr> exprs;
        for (const auto &e : expressions)
            exprs.emplace_back(static_cast<const ratio::core::expression &>(*e).evaluate(scp, ctx));
        return scp.get_core().div(exprs);
    }

    void local_field_statement::execute(scope &scp, context &ctx) const
    {
        for (size_t i = 0; i < names.size(); ++i)
        {
            if (xprs[i])
                ctx->vars.emplace(names[i].id, static_cast<const ratio::core::expression &>(*xprs[i]).evaluate(scp, ctx));
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

            if (is_core(scp)) // we create fields for root items..
                scp.get_core().fields.emplace(names[i].id, std::make_unique<field>(ctx->vars.at(names[i].id)->get_type(), names[i].id, xprs[i]));
        }
    }

    void assignment_statement::execute(scope &scp, context &ctx) const
    {
        expr c_e = ctx->get(ids.begin()->id);
        for (auto it = std::next(ids.begin()); it != ids.end(); ++it)
            c_e = static_cast<complex_item &>(*c_e).get(it->id);
        static_cast<complex_item &>(*c_e).vars.emplace(id.id, static_cast<const ratio::core::expression &>(*xpr).evaluate(scp, ctx));
    }

    void expression_statement::execute(scope &scp, context &ctx) const
    {
        expr be = static_cast<const ratio::core::expression &>(*xpr).evaluate(scp, ctx);
        scp.get_core().assert_facts({be});
    }

    void disjunction_statement::execute(scope &scp, context &ctx) const
    {
        std::vector<std::unique_ptr<ratio::core::conjunction>> cs;
        cs.reserve(conjunctions.size());
        for (size_t i = 0; i < conjunctions.size(); ++i)
        {
            semitone::rational cost(1);
            if (conjunction_costs[i])
            { // a cost for the conjunction has been specified..
                expr a_xpr = static_cast<const ratio::core::expression &>(*conjunction_costs[i]).evaluate(scp, ctx);
                if (!static_cast<arith_item &>(*a_xpr).get_value().vars.empty())
                    throw std::invalid_argument("invalid disjunct cost: expected a constant..");
                cost = scp.get_core().arith_value(a_xpr).get_rational();
            }
            cs.emplace_back(std::make_unique<conjunction>(scp, ctx, cost, conjunctions[i]));
        }

        scp.get_core().new_disjunction(std::move(cs));
    }

    void conjunction_statement::execute(scope &scp, context &ctx) const
    {
        for (const auto &st : statements)
            static_cast<const ratio::core::statement &>(*st).execute(scp, ctx);
    }

    void formula_statement::execute(scope &scp, context &ctx) const
    {
        predicate *pred = nullptr;
        std::unordered_map<std::string, expr> assgnments;
        if (!formula_scope.empty())
        { // the scope is explicitely declared..
            expr c_scope = ctx->get(formula_scope.begin()->id);
            for (auto it = std::next(formula_scope.begin()); it != formula_scope.end(); ++it)
                c_scope = static_cast<complex_item &>(*c_scope).get(it->id);

            pred = &c_scope->get_type().get_predicate(predicate_name.id);

            if (enum_item *ee = dynamic_cast<enum_item *>(c_scope.get())) // the scope is an enumerative expression..
                assgnments.emplace(TAU_KW, ee);
            else // the scope is a single item..
                assgnments.emplace(TAU_KW, c_scope);
        }
        else
        { // we inherit the scope..
            pred = &scp.get_predicate(predicate_name.id);
            if (!is_core(pred->get_scope()))
                assgnments.emplace(TAU_KW, ctx->get(TAU_KW));
        }

        for (size_t i = 0; i < assignment_names.size(); ++i)
        {
            expr e = static_cast<const ratio::core::expression &>(*assignment_values[i]).evaluate(scp, ctx);
            const type &tt = pred->get_field(assignment_names[i].id).get_type(); // the target type..
            if (tt.is_assignable_from(e->get_type()))                            // the target type is a superclass of the assignment..
                assgnments.emplace(assignment_names[i].id, e);
            else if (e->get_type().is_assignable_from(tt)) // the target type is a subclass of the assignment..
                if (enum_item *ae = dynamic_cast<enum_item *>(&*e))
                { // some of the allowed values might be inhibited..
                    // the allowed values..
                    auto alwd_vals = scp.get_core().enum_value(*ae);
                    for (auto ev : alwd_vals)
                        if (!tt.is_assignable_from(e->get_type())) // the target type is not a superclass of the value..
                            scp.get_core().remove(e, ev);
                }
                else // the evaluated expression is a constant which cannot be assigned to the target type (which is a subclass of the type of the evaluated expression)..
                    throw inconsistency_exception();
            else // the evaluated expression is unrelated with the target type (we are probably in the presence of a modeling error!)..
                throw inconsistency_exception();
        }

        auto atm = pred->new_instance();
        auto &c_atm = *static_cast<atom *>(atm.get());
        c_atm.vars.insert(assgnments.cbegin(), assgnments.cend());

        // we initialize the unassigned atom's fields..
        std::queue<predicate *> q;
        q.push(pred);
        while (!q.empty())
        {
            for (const auto &arg : q.front()->get_args())
                if (!c_atm.vars.count(arg->get_name()))
                { // the field is uninstantiated..
                    type &tp = arg->get_type();
                    c_atm.vars.emplace(arg->get_name(), tp.is_primitive() ? tp.new_instance() : tp.new_existential());
                }
            for (const auto &sp : q.front()->get_supertypes())
                q.push(static_cast<predicate *>(sp));
            q.pop();
        }

        scp.get_core().new_atom(c_atm, is_fact);
        ctx->vars.emplace(formula_name.id, atm);
    }

    void return_statement::execute(scope &scp, context &ctx) const { ctx->vars.emplace(RETURN_KW, static_cast<const ratio::core::expression &>(*xpr).evaluate(scp, ctx)); }

    void method_declaration::refine(scope &scp) const
    {
        type *rt;
        if (!return_type.empty())
        {
            scope *s = &scp;
            for (const auto &id_tk : return_type)
                s = &s->get_type(id_tk.id);
            rt = static_cast<type *>(s);
        }

        std::vector<field_ptr> args;
        for (const auto &[id_tkns, id_tkn] : parameters)
        {
            scope *s = &scp;
            for (const auto &id_tk : id_tkns)
                s = &s->get_type(id_tk.id);
            type *tp = static_cast<type *>(s);
            args.emplace_back(std::make_unique<field>(*tp, id_tkn.id));
        }

        if (auto m = std::make_unique<method>(scp, rt, name.id, std::move(args), statements); core *c = dynamic_cast<core *>(&scp))
            c->new_method(std::move(m));
        else if (type *t = static_cast<type *>(&scp))
            t->new_method(std::move(m));
    }

    void predicate_declaration::declare(scope &scp) const
    {
        auto p = std::make_unique<predicate>(scp, name.id, std::vector<field_ptr>(), statements);
        if (core *c = dynamic_cast<core *>(&scp))
            c->new_predicate(std::move(p));
        else if (type *t = static_cast<type *>(&scp))
            t->new_predicate(std::move(p));
    }

    void predicate_declaration::refine(scope &scp) const
    {
        predicate &p = scp.get_predicate(name.id);

        // we add the predicate fields..
        for (const auto &[id_tkns, id_tkn] : parameters)
        {
            scope *s = &scp;
            for (const auto &id_tk : id_tkns)
                s = &s->get_type(id_tk.id);
            type *tp = static_cast<type *>(s);
            tp->new_field(std::make_unique<field>(*tp, id_tkn.id));
        }

        // we add the supertypes..
        for (const auto &sp : predicate_list)
        {
            scope *s = &scp;
            for (const auto &id_tk : sp)
                s = &s->get_predicate(id_tk.id);
            p.new_supertype(*static_cast<predicate *>(s));
        }
    }

    void typedef_declaration::declare(scope &scp) const
    { // A new typedef type has been declared..
        auto td = std::make_unique<typedef_type>(scp, name.id, scp.get_type(primitive_type.id), xpr);

        if (core *c = dynamic_cast<core *>(&scp))
            c->new_type(std::move(td));
        else if (type *t = static_cast<type *>(&scp))
            t->new_type(std::move(td));
    }

    void enum_declaration::declare(scope &scp) const
    {
        // A new enum type has been declared..
        auto et = std::make_unique<enum_type>(scp, name.id);

        // We add the enum values..
        for (const auto &e : enums)
            et->instances.emplace_back(scp.get_core().new_string(e.str));

        if (core *c = dynamic_cast<core *>(&scp))
            c->new_type(std::move(et));
        else if (type *t = static_cast<type *>(&scp))
            t->new_type(std::move(et));
    }

    void enum_declaration::refine(scope &scp) const
    {
        if (!type_refs.empty())
        {
            enum_type *et = static_cast<enum_type *>(&scp.get_type(name.id));
            for (const auto &tr : type_refs)
            {
                scope *s = &scp;
                for (const auto &id_tk : tr)
                    s = &s->get_type(id_tk.id);
                et->enums.emplace_back(static_cast<enum_type *>(s));
            }
        }
    }

    void field_declaration::refine(scope &scp) const
    { // we add fields to the current scope..
        scope *s = &scp;
        for (const auto &id_tk : field_type)
            s = &s->get_type(id_tk.id);
        type *tp = static_cast<type *>(s);

        for (const auto &vd : declarations)
            scp.new_field(std::make_unique<field>(*tp, static_cast<const variable_declaration &>(*vd).name.id, static_cast<const variable_declaration &>(*vd).xpr));
    }

    void constructor_declaration::refine(scope &scp) const
    {
        std::vector<field_ptr> args;
        for (const auto &[id_tkns, id_tkn] : parameters)
        {
            scope *s = &scp;
            for (const auto &id_tk : id_tkns)
                s = &s->get_type(id_tk.id);
            type *tp = static_cast<type *>(s);
            args.emplace_back(std::make_unique<field>(*tp, id_tkn.id));
        }

        static_cast<type &>(scp).new_constructor(std::make_unique<constructor>(static_cast<type &>(scp), std::move(args), init_names, init_vals, statements));
    }

    void class_declaration::declare(scope &scp) const
    { // A new type has been declared..
        auto tp = std::make_unique<type>(scp, name.id);

        if (core *c = dynamic_cast<core *>(&scp))
            c->new_type(std::move(tp));
        else if (type *t = static_cast<type *>(&scp))
            t->new_type(std::move(tp));

        for (const auto &c_tp : types)
            static_cast<const ratio::core::type_declaration &>(*c_tp).declare(*tp);

        for (const auto &c_p : predicates)
            static_cast<const ratio::core::predicate_declaration &>(*c_p).declare(*tp);
    }
    void class_declaration::refine(scope &scp) const
    {
        type &tp = scp.get_type(name.id);
        for (const auto &bc : base_classes)
        {
            scope *s = &scp;
            for (const auto &id_tk : bc)
                s = &s->get_type(id_tk.id);
            tp.new_supertype(*static_cast<type *>(s));
        }

        for (const auto &f : fields)
            static_cast<const ratio::core::field_declaration &>(*f).refine(tp);

        if (constructors.empty()) // we add a default constructor..
            tp.new_constructor(std::make_unique<constructor>(tp, std::vector<field_ptr>(), std::vector<riddle::id_token>(), std::vector<std::vector<std::unique_ptr<const riddle::ast::expression>>>(), std::vector<std::unique_ptr<const riddle::ast::statement>>()));
        else
            for (const auto &c : constructors)
                static_cast<const ratio::core::constructor_declaration &>(*c).refine(tp);

        for (const auto &m : methods)
            static_cast<const ratio::core::method_declaration &>(*m).refine(tp);
        for (const auto &p : predicates)
            static_cast<const ratio::core::predicate_declaration &>(*p).refine(tp);
        for (const auto &t : types)
            static_cast<const ratio::core::type_declaration &>(*t).refine(tp);
    }

    void compilation_unit::declare(scope &scp) const
    {
        for (const auto &t : types)
            static_cast<const ratio::core::type_declaration &>(*t).declare(scp);
    }
    void compilation_unit::refine(scope &scp) const
    {
        for (const auto &t : types)
            static_cast<const ratio::core::type_declaration &>(*t).refine(scp);
        for (const auto &m : methods)
            static_cast<const ratio::core::method_declaration &>(*m).refine(scp);
        for (const auto &p : predicates)
            static_cast<const ratio::core::predicate_declaration &>(*p).refine(scp);
    }
    void compilation_unit::execute(scope &scp, context &ctx) const
    {
        try
        {
            for (const auto &stmnt : statements)
                static_cast<const ratio::core::statement &>(*stmnt).execute(scp, ctx);
        }
        catch (const inconsistency_exception &)
        { // we found an inconsistency at root-level..
            throw unsolvable_exception();
        }
    }

    parser::parser(std::istream &is) : riddle::parser::parser(is) {}
} // namespace ratio::core
