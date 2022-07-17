#include "type.h"
#include "core.h"
#include "item.h"
#include "riddle_lexer.h"
#include <queue>

namespace ratio::core
{
    ORATIOCORE_EXPORT type::type(core &cr, const std::string &name, bool primitive) : scope(cr), name(name), primitive(primitive) {}

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

    bool_type::bool_type(core &cr) : type(cr, BOOL_KEYWORD, true) {}
    expr bool_type::new_instance() noexcept { return nullptr; }

    int_type::int_type(core &cr) : type(cr, INT_KEYWORD, true) {}
    bool int_type::is_assignable_from(const type &t) const noexcept { return &t == this || &t == &get_core().get_type(TIME_KEYWORD); }
    expr int_type::new_instance() noexcept { return nullptr; }

    real_type::real_type(core &cr) : type(cr, REAL_KEYWORD, true) {}
    bool real_type::is_assignable_from(const type &t) const noexcept { return &t == this || &t == &get_core().get_type(TIME_KEYWORD); }
    expr real_type::new_instance() noexcept { return nullptr; }

    time_type::time_type(core &cr) : type(cr, TIME_KEYWORD, true) {}
    bool time_type::is_assignable_from(const type &t) const noexcept { return &t == this || &t == &get_core().get_type(INT_KEYWORD) || &t == &get_core().get_type(REAL_KEYWORD); }
    expr time_type::new_instance() noexcept { return nullptr; }

    string_type::string_type(core &cr) : type(cr, STRING_KEYWORD, true) {}
    expr string_type::new_instance() noexcept { return nullptr; }
} // namespace ratio::core
