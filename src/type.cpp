#include "predicate.h"
#include "core.h"
#include "item.h"
#include "field.h"
#include "constructor.h"
#include "method.h"
#include "parser.h"
#include <queue>
#include <algorithm>
#include <stdexcept>
#include <cassert>

namespace ratio::core
{
    RATIOCORE_EXPORT type::type(scope &scp, const std::string &name, bool primitive) : scope(scp), name(name), primitive(primitive) {}
    RATIOCORE_EXPORT type::~type() {}

    RATIOCORE_EXPORT bool type::is_assignable_from(const type &t) const noexcept
    {
        std::queue<const type *> q;
        q.push(&t);
        while (!q.empty())
        {
            if (q.front() == this)
                return true;
            else
            {
                for (const auto &st : q.front()->supertypes)
                    q.push(st);
                q.pop();
            }
        }
        return false;
    }

    RATIOCORE_EXPORT expr type::new_instance()
    {
        auto itm = std::make_shared<complex_item>(*this);
        // we add the new item to the instances of this predicate and to the instances of all the super-predicates..
        std::queue<type *> q;
        q.push(this);
        while (!q.empty())
        {
            q.front()->instances.push_back(itm);
            for (const auto &st : q.front()->supertypes)
                q.push(st);
            q.pop();
        }
        return itm;
    }

    RATIOCORE_EXPORT expr type::new_existential()
    {
        switch (instances.size())
        {
        case 0:
            throw inconsistency_exception();
        case 1:
            return *instances.cbegin();
        default:
            std::vector<expr> c_vals;
            c_vals.reserve(instances.size());
            for (const auto &i : instances)
                c_vals.push_back(i);
            return get_core().new_enum(*this, c_vals);
        }
    }

    RATIOCORE_EXPORT void type::new_supertype(type &t) noexcept { supertypes.emplace_back(&t); }
    RATIOCORE_EXPORT void type::new_supertype(type &t, type &st) noexcept { t.new_supertype(st); }
    RATIOCORE_EXPORT void type::new_constructor(constructor_ptr c) noexcept { constructors.emplace_back(std::move(c)); }
    RATIOCORE_EXPORT void type::new_method(method_ptr m) noexcept { methods[m->get_name()].emplace_back(std::move(m)); }
    RATIOCORE_EXPORT void type::new_type(type_ptr t) noexcept { types.emplace(t->get_name(), std::move(t)); }
    RATIOCORE_EXPORT void type::new_predicate(predicate_ptr p, bool notify) noexcept
    {
        if (notify)
        { // we notify all the supertypes that a new predicate has been created..
            std::queue<type *> q;
            q.push(this);
            while (!q.empty())
            {
                q.front()->new_predicate(*p);
                for (const auto &st : q.front()->supertypes)
                    q.push(st);
                q.pop();
            }
        }

        predicates.emplace(p->get_name(), std::move(p));
    }

    constructor &type::get_constructor(const std::vector<const type *> &ts) const
    {
        assert(std::none_of(ts.cbegin(), ts.cend(), [](const type *t)
                            { return t == nullptr; }));
        bool found = false;
        for (const auto &cnstr : constructors)
            if (cnstr->args.size() == ts.size())
            {
                found = true;
                for (unsigned int i = 0; i < ts.size(); i++)
                    if (!cnstr->args[i]->get_type().is_assignable_from(*ts[i]))
                    {
                        found = false;
                        break;
                    }
                if (found)
                    return *cnstr;
            }

        throw std::out_of_range(name);
    }

    RATIOCORE_EXPORT const field &type::get_field(const std::string &fname) const
    {
        if (const auto at_f = get_fields().find(fname); at_f != get_fields().cend())
            return *at_f->second;

        try
        { // if not here, check any enclosing scope
            return get_scope().get_field(fname);
        }
        catch (const std::out_of_range &)
        { // if not in any enclosing scope, check any superclass
            for (const auto &st : supertypes)
                try
                {
                    return st->get_field(fname);
                }
                catch (const std::out_of_range &)
                {
                }
        }

        // not found
        throw std::out_of_range(fname);
    }

    RATIOCORE_EXPORT method &type::get_method(const std::string &mname, const std::vector<const type *> &ts) const
    {
        if (const auto at_m = methods.find(mname); at_m != methods.cend())
        {
            bool found = false;
            for (const auto &mthd : at_m->second)
                if (mthd->get_args().size() == ts.size())
                {
                    found = true;
                    for (size_t i = 0; i < ts.size(); ++i)
                        if (!mthd->get_args()[i]->get_type().is_assignable_from(*ts[i]))
                        {
                            found = false;
                            break;
                        }
                    if (found)
                        return *mthd;
                }
        }

        try
        { // if not here, check any enclosing scope
            return get_scope().get_method(mname, ts);
        }
        catch (const std::out_of_range &)
        { // if not in any enclosing scope, check any superclass
            for (const auto &st : supertypes)
            {
                try
                {
                    return st->get_method(mname, ts);
                }
                catch (const std::out_of_range &)
                {
                }
            }
        }

        // not found
        throw std::out_of_range(mname);
    }

    RATIOCORE_EXPORT type &type::get_type(const std::string &tname) const
    {
        if (const auto at_tp = types.find(tname); at_tp != types.cend())
            return *at_tp->second;

        try
        { // if not here, check any enclosing scope
            return get_scope().get_type(tname);
        }
        catch (const std::out_of_range &)
        { // if not in any enclosing scope, check any superclass
            for (const auto &st : supertypes)
            {
                try
                {
                    return st->get_type(tname);
                }
                catch (const std::out_of_range &)
                {
                }
            }
        }

        // not found
        throw std::out_of_range(tname);
    }

    RATIOCORE_EXPORT predicate &type::get_predicate(const std::string &pname) const
    {
        if (const auto at_p = predicates.find(pname); at_p != predicates.cend())
            return *at_p->second;

        try
        { // if not here, check any enclosing scope
            return get_scope().get_predicate(pname);
        }
        catch (const std::out_of_range &)
        { // if not in any enclosing scope, check any superclass
            for (const auto &st : supertypes)
            {
                try
                {
                    return st->get_predicate(pname);
                }
                catch (const std::out_of_range &)
                {
                }
            }
        }

        // not found
        throw std::out_of_range(pname);
    }

    bool_type::bool_type(core &cr) : type(cr, BOOL_KW, true) {}
    expr bool_type::new_instance() noexcept { return get_core().new_bool(); }

    int_type::int_type(core &cr) : type(cr, INT_KW, true) {}
    bool int_type::is_assignable_from(const type &t) const noexcept { return &t == this || &t == &get_core().get_type(TIME_KW); }
    expr int_type::new_instance() noexcept { return get_core().new_int(); }

    real_type::real_type(core &cr) : type(cr, REAL_KW, true) {}
    bool real_type::is_assignable_from(const type &t) const noexcept { return &t == this || &t == &get_core().get_type(TIME_KW); }
    expr real_type::new_instance() noexcept { return get_core().new_real(); }

    time_type::time_type(core &cr) : type(cr, TIME_KW, true) {}
    bool time_type::is_assignable_from(const type &t) const noexcept { return &t == this || &t == &get_core().get_type(INT_KW) || &t == &get_core().get_type(REAL_KW); }
    expr time_type::new_instance() noexcept { return get_core().new_time_point(); }

    string_type::string_type(core &cr) : type(cr, STRING_KW, true) {}
    expr string_type::new_instance() noexcept { return get_core().new_string(); }

    typedef_type::typedef_type(scope &scp, const std::string &name, const type &base_type, const std::unique_ptr<const riddle::ast::expression> &e) : type(scp, name), base_type(base_type), xpr(e) {}
    expr typedef_type::new_instance() noexcept
    {
        auto c_ctx = std::make_shared<var_map>(get_core().get_context().get());
        return dynamic_cast<const expression &>(*xpr).evaluate(get_core(), c_ctx);
    }

    enum_type::enum_type(scope &scp, std::string name) : type(scp, name) {}

    expr enum_type::new_instance() { return get_core().new_enum(*this, get_all_instances()); }

    std::vector<expr> enum_type::get_all_instances() const noexcept
    {
        std::vector<expr> c_instances;
        for (const auto &i : instances)
            c_instances.emplace_back(i);

        for (const auto &es : enums)
        {
            std::vector<expr> es_instances = es->get_all_instances();
            c_instances.reserve(c_instances.size() + es_instances.size());
            c_instances.insert(c_instances.cend(), es_instances.cbegin(), es_instances.cend());
        }
        return c_instances;
    }
} // namespace ratio::core
