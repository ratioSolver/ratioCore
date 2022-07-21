#include "predicate.h"
#include "atom.h"
#include "field.h"
#include "parser.h"
#include <queue>

namespace ratio::core
{
    ORATIOCORE_EXPORT predicate::predicate(scope &scp, const std::string &name, std::vector<field_ptr> args, const std::vector<std::unique_ptr<const riddle::ast::statement>> &stmnts) : type(scp, name), statements(stmnts)
    {
        this->args.reserve(args.size());
        for (auto &f : args)
        {
            this->args.push_back(f.get());
            new_field(std::move(f));
        }
    }

    ORATIOCORE_EXPORT expr predicate::new_instance()
    {
        auto itm = std::make_shared<atom>(*this);
        // we add the new atom to the instances of this predicate and to the instances of all the super-predicates..
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

    ORATIOCORE_EXPORT void predicate::apply_rule(atom &a)
    {
        for (const auto &sp : supertypes)
            if (auto p = dynamic_cast<predicate *>(sp))
                p->apply_rule(a);

        auto ctx = std::make_shared<env>(a);
        ctx->vars.emplace(THIS_KW, &a);
        for (const auto &s : statements)
            dynamic_cast<const statement *>(s.get())->execute(*this, ctx);
    }
} // namespace ratio::core
