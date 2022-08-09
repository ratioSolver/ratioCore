#include "constructor.h"
#include "type.h"
#include "item.h"
#include "field.h"
#include "parser.h"
#include <algorithm>
#include <cassert>

namespace ratio::core
{
    constructor::constructor(type &tp, std::vector<field_ptr> args, const std::vector<riddle::id_token> &ins, const std::vector<std::vector<std::unique_ptr<const riddle::ast::expression>>> &ivs, const std::vector<std::unique_ptr<const riddle::ast::statement>> &stmnts) : scope(tp), init_names(ins), init_vals(ivs), statements(stmnts)
    {
        this->args.reserve(args.size());
        for (auto &f : args)
        {
            this->args.push_back(f.get());
            new_field(std::move(f));
        }
        new_field(std::make_unique<field>(tp, THIS_KW));
    }

    expr constructor::new_instance(std::vector<expr> exprs) noexcept
    {
        assert(args.size() == exprs.size());

        type &t = static_cast<type &>(get_scope());
        expr i = t.new_instance();

        auto itm = std::dynamic_pointer_cast<ratio::core::env>(i);
        invoke(itm, std::move(exprs));

        return i;
    }

    void constructor::invoke(context &ctx, std::vector<expr> exprs)
    {
        auto c_ctx = std::make_shared<env>(ctx);
        c_ctx->vars.emplace(THIS_KW, std::dynamic_pointer_cast<ratio::core::item>(ctx));
        for (size_t i = 0; i < args.size(); ++i)
            c_ctx->vars.emplace(args.at(i)->get_name(), exprs.at(i));

        for (size_t il_idx = 0; il_idx < init_names.size(); il_idx++)
            try
            { // we try to find the field in the current type..
                const auto &f = get_field(init_names.at(il_idx).id);
                if (f.get_type().is_primitive())
                { // we evaluate the expression..
                    assert(init_vals[il_idx].size() == 1);
                    static_cast<complex_item &>(*ctx).vars.emplace(init_names[il_idx].id, dynamic_cast<const expression &>(*init_vals[il_idx][0]).evaluate(*this, c_ctx));
                }
                else
                { // we call the constructor..
                    std::vector<expr> c_exprs;
                    std::vector<const type *> par_types;
                    for (const auto &ex : init_vals.at(il_idx))
                    {
                        expr c_expr = dynamic_cast<const expression &>(*ex).evaluate(*this, c_ctx);
                        c_exprs.push_back(c_expr);
                        par_types.push_back(&c_expr->get_type());
                    }

                    // we assume that the constructor exists..
                    static_cast<complex_item &>(*ctx).vars.emplace(init_names[il_idx].id, f.get_type().get_constructor(par_types).new_instance(std::move(c_exprs)));
                }
            }
            catch (const std::out_of_range &e)
            { // there is no field in the current type with the given name, so we call the supertype's constructor..
                auto st = std::find_if(static_cast<type &>(get_scope()).get_supertypes().begin(), static_cast<type &>(get_scope()).get_supertypes().end(), [this, il_idx](auto &st)
                                       { return init_names.at(il_idx).id == st->get_name(); });
                assert(*st);
                std::vector<expr> c_exprs;
                std::vector<const type *> par_types;
                for (const auto &ex : init_vals.at(il_idx))
                {
                    expr c_expr = dynamic_cast<const expression &>(*ex).evaluate(*this, c_ctx);
                    c_exprs.push_back(c_expr);
                    par_types.push_back(&c_expr->get_type());
                }

                // we assume that the constructor exists..
                (*st)->get_constructor(par_types).invoke(ctx, std::move(c_exprs));
            }

        // we instantiate the uninstantiated fields..
        for (const auto &[f_name, f] : get_scope().get_fields())
            if (!f->is_synthetic() && !static_cast<complex_item &>(*ctx).vars.count(f_name))
            { // the field is uninstantiated..
                if (f->get_expression())
                    static_cast<complex_item &>(*ctx).vars.emplace(f_name, dynamic_cast<const expression &>(*f->get_expression()).evaluate(*this, c_ctx));
                else
                {
                    type &tp = f->get_type();
                    if (tp.is_primitive())
                        static_cast<complex_item &>(*ctx).vars.emplace(f_name, tp.new_instance());
                    else
                        static_cast<complex_item &>(*ctx).vars.emplace(f_name, tp.new_existential());
                }
            }

        // finally, we execute the constructor body..
        for (const auto &s : statements)
            dynamic_cast<const statement &>(*s).execute(*this, c_ctx);
    }
} // namespace ratio::core
