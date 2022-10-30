#include "item.h"
#include "predicate.h"
#include "core.h"
#include "riddle_lexer.h"
#include <queue>
#include <cassert>

namespace ratio::core
{
    item::item(type &tp) : tp(tp) {}

    RATIOCORE_EXPORT bool_item::bool_item(type &t, const semitone::lit &l) : item(t), l(l) { assert(t.get_name() == BOOL_KW); }

    RATIOCORE_EXPORT arith_item::arith_item(type &t, const semitone::lin &l) : item(t), l(l) { assert(t.get_name() == INT_KW || t.get_name() == REAL_KW || t.get_name() == TIME_KW); }

    RATIOCORE_EXPORT string_item::string_item(type &t, const std::string &l) : item(t), l(l) { assert(t.get_name() == STRING_KW); }

    RATIOCORE_EXPORT complex_item::complex_item(type &tp) : item(tp), env(std::make_shared<var_map>(tp.get_core().get_context())) {}

    RATIOCORE_EXPORT enum_item::enum_item(type &t, semitone::var ev) : complex_item(t), ev(ev) {}

    RATIOCORE_EXPORT expr enum_item::get(const std::string &name) noexcept
    {
        std::map<std::string, const field *> accessible_fields;
        std::queue<type *> q;
        q.push(&get_type());
        while (!q.empty())
        {
            for (const auto &f : q.front()->get_fields())
                accessible_fields.try_emplace(f.first, f.second.get());
            for (const auto &st : q.front()->get_supertypes())
                q.push(st);
            q.pop();
        }
        if (!accessible_fields.count(name))
            return complex_item::get(name);
        else
        {
            const auto &it_it = get_context()->vars.lower_bound(name);
            if (it_it == get_context()->vars.cend())
            {
                assert(!get_type().get_core().enum_value(*this).empty());
                if (auto vs = get_type().get_core().enum_value(*this); vs.size() == 1)
                    return (static_cast<complex_item *>(*vs.cbegin()))->get(name);
                else
                { // we generate a new variable..
                    auto e = get_type().get_core().get_enum(*this, name);
                    get_context()->vars.emplace_hint(it_it, name, e);
                    return e;
                }
            }
            else
                return it_it->second;
        }
    }

    RATIOCORE_EXPORT atom::atom(predicate &pred) : complex_item(pred) {}
} // namespace ratio::core
