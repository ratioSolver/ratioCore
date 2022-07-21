#include "predicate.h"
#include "core.h"
#include "item.h"
#include "field.h"
#include "constructor.h"
#include "method.h"
#include "riddle_lexer.h"
#include <queue>
#include <algorithm>
#include <stdexcept>
#include <cassert>

namespace ratio::core
{
    ORATIOCORE_EXPORT type::type(core &cr, const std::string &name, bool primitive) : scope(cr), name(name), primitive(primitive) {}
    ORATIOCORE_EXPORT type::~type() {}

    ORATIOCORE_EXPORT bool type::is_assignable_from(const type &t) const noexcept
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

    ORATIOCORE_EXPORT expr type::new_instance()
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

    ORATIOCORE_EXPORT expr type::new_existential()
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

    ORATIOCORE_EXPORT const field &type::get_field(const std::string &name) const
    {
        if (const auto at_f = get_fields().find(name); at_f != get_fields().cend())
            return *at_f->second;

        try
        { // if not here, check any enclosing scope
            return get_scope().get_field(name);
        }
        catch (const std::out_of_range &)
        { // if not in any enclosing scope, check any superclass
            for (const auto &st : supertypes)
                try
                {
                    return st->get_field(name);
                }
                catch (const std::out_of_range &)
                {
                }
        }

        // not found
        throw std::out_of_range(name);
    }

    ORATIOCORE_EXPORT method &type::get_method(const std::string &name, const std::vector<const type *> &ts) const
    {
        if (const auto at_m = methods.find(name); at_m != methods.cend())
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
            return get_scope().get_method(name, ts);
        }
        catch (const std::out_of_range &)
        { // if not in any enclosing scope, check any superclass
            for (const auto &st : supertypes)
            {
                try
                {
                    return st->get_method(name, ts);
                }
                catch (const std::out_of_range &)
                {
                }
            }
        }

        // not found
        throw std::out_of_range(name);
    }

    ORATIOCORE_EXPORT type &type::get_type(const std::string &name) const
    {
        if (const auto at_tp = types.find(name); at_tp != types.cend())
            return *at_tp->second;

        try
        { // if not here, check any enclosing scope
            return get_scope().get_type(name);
        }
        catch (const std::out_of_range &)
        { // if not in any enclosing scope, check any superclass
            for (const auto &st : supertypes)
            {
                try
                {
                    return st->get_type(name);
                }
                catch (const std::out_of_range &)
                {
                }
            }
        }

        // not found
        throw std::out_of_range(name);
    }

    ORATIOCORE_EXPORT predicate &type::get_predicate(const std::string &name) const
    {
        if (const auto at_p = predicates.find(name); at_p != predicates.cend())
            return *at_p->second;

        try
        { // if not here, check any enclosing scope
            return get_scope().get_predicate(name);
        }
        catch (const std::out_of_range &)
        { // if not in any enclosing scope, check any superclass
            for (const auto &st : supertypes)
            {
                try
                {
                    return st->get_predicate(name);
                }
                catch (const std::out_of_range &)
                {
                }
            }
        }

        // not found
        throw std::out_of_range(name);
    }

    bool_type::bool_type(core &cr) : type(cr, BOOL_KW, true) {}
    expr bool_type::new_instance() noexcept { return nullptr; }

    int_type::int_type(core &cr) : type(cr, INT_KW, true) {}
    bool int_type::is_assignable_from(const type &t) const noexcept { return &t == this || &t == &get_core().get_type(TIME_KW); }
    expr int_type::new_instance() noexcept { return nullptr; }

    real_type::real_type(core &cr) : type(cr, REAL_KW, true) {}
    bool real_type::is_assignable_from(const type &t) const noexcept { return &t == this || &t == &get_core().get_type(TIME_KW); }
    expr real_type::new_instance() noexcept { return nullptr; }

    time_type::time_type(core &cr) : type(cr, TIME_KW, true) {}
    bool time_type::is_assignable_from(const type &t) const noexcept { return &t == this || &t == &get_core().get_type(INT_KW) || &t == &get_core().get_type(REAL_KW); }
    expr time_type::new_instance() noexcept { return nullptr; }

    string_type::string_type(core &cr) : type(cr, STRING_KW, true) {}
    expr string_type::new_instance() noexcept { return nullptr; }
} // namespace ratio::core
