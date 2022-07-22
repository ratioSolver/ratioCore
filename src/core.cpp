#include "core.h"
#include "method.h"
#include "predicate.h"
#include "conjunction.h"
#include "field.h"
#include "atom.h"
#include "parser.h"
#include <sstream>
#include <fstream>

namespace ratio::core
{
    ORATIOCORE_EXPORT core::core() : scope(*this), env(*this)
    {
        new_type(std::make_unique<bool_type>(*this));
    }
    ORATIOCORE_EXPORT core::~core() {}

    ORATIOCORE_EXPORT void core::read(const std::string &script)
    {
        std::stringstream ss(script);
        parser prs(ss);
        auto cu = prs.parse();
        static_cast<const ratio::core::compilation_unit *>(cu.get())->declare(*this);
        static_cast<const ratio::core::compilation_unit *>(cu.get())->refine(*this);
        context c_ctx(this);
        static_cast<const ratio::core::compilation_unit *>(cu.get())->execute(*this, c_ctx);
        cus.emplace_back(std::move(cu));
        RECOMPUTE_NAMES();
        FIRE_READ(script);
    }

    ORATIOCORE_EXPORT void core::read(const std::vector<std::string> &files)
    {
        std::vector<std::unique_ptr<const riddle::ast::compilation_unit>> c_cus;
        for (const auto &f : files)
            if (std::ifstream ifs(f); ifs)
            {
                parser prs(ifs);
                auto cu = prs.parse();
                cus.emplace_back(std::move(cu));
            }
            else
                throw std::invalid_argument("cannot find file '" + f + "'");

        for (const auto &cu : c_cus)
            static_cast<const ratio::core::compilation_unit *>(cu.get())->declare(*this);
        for (const auto &cu : c_cus)
            static_cast<const ratio::core::compilation_unit *>(cu.get())->refine(*this);
        context c_ctx(this);
        for (const auto &cu : c_cus)
            static_cast<const ratio::core::compilation_unit *>(cu.get())->execute(*this, c_ctx);
        cus.reserve(cus.size() + c_cus.size());
        for (auto &cu : c_cus)
            cus.emplace_back(std::move(cu));
        RECOMPUTE_NAMES();
        FIRE_READ(files);
    }

    ORATIOCORE_EXPORT expr core::new_bool(const bool &val) noexcept { return std::make_shared<bool_item>(*types.at(BOOL_KW), val ? semitone::TRUE_lit : semitone::FALSE_lit); }
    ORATIOCORE_EXPORT expr core::new_int(const semitone::I &val) noexcept { return std::make_shared<arith_item>(*types.at(INT_KW), semitone::lin(semitone::rational(val))); }
    ORATIOCORE_EXPORT expr core::new_real(const semitone::rational &val) noexcept { return std::make_shared<arith_item>(*types.at(REAL_KW), semitone::lin(val)); }
    ORATIOCORE_EXPORT expr core::new_time_point(const semitone::rational &val) noexcept { return std::make_shared<arith_item>(*types.at(TIME_KW), semitone::lin(val)); }
    ORATIOCORE_EXPORT expr core::new_string(const std::string &val) noexcept { return std::make_shared<string_item>(*types.at(STRING_KW), val); }

    ORATIOCORE_EXPORT expr core::get(const std::string &name) noexcept
    {
        if (const auto at_xpr = vars.find(name); at_xpr != vars.cend())
            return at_xpr->second;
        else
            return nullptr;
    }

    ORATIOCORE_EXPORT semitone::lbool core::bool_value([[maybe_unused]] const expr &x) const noexcept { return bool_value(*static_cast<bool_item *>(x.get())); }
    ORATIOCORE_EXPORT std::pair<semitone::inf_rational, semitone::inf_rational> core::arith_bounds([[maybe_unused]] const expr &x) const noexcept { return arith_bounds(*static_cast<arith_item *>(x.get())); }
    ORATIOCORE_EXPORT semitone::inf_rational core::arith_value([[maybe_unused]] const expr &x) const noexcept { return arith_value(*static_cast<arith_item *>(x.get())); }
    ORATIOCORE_EXPORT std::unordered_set<expr> core::enum_value([[maybe_unused]] const expr &x) const noexcept { return enum_value(*static_cast<enum_item *>(x.get())); }

    ORATIOCORE_EXPORT void core::new_disjunction([[maybe_unused]] const std::vector<std::unique_ptr<conjunction>> conjs) {}

    ORATIOCORE_EXPORT void core::new_method(method_ptr m) noexcept { methods[m->get_name()].emplace_back(std::move(m)); }
    ORATIOCORE_EXPORT void core::new_type(type_ptr t) noexcept { types.emplace(t->get_name(), std::move(t)); }
    ORATIOCORE_EXPORT void core::new_predicate(predicate_ptr p) noexcept { predicates.emplace(p->get_name(), std::move(p)); }

    ORATIOCORE_EXPORT const field &core::get_field(const std::string &name) const
    {
        if (const auto at_f = get_fields().find(name); at_f != get_fields().cend())
            return *at_f->second;

        // not found
        throw std::out_of_range(name);
    }

    ORATIOCORE_EXPORT method &core::get_method(const std::string &name, const std::vector<const type *> &ts) const
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

        // not found
        throw std::out_of_range(name);
    }

    ORATIOCORE_EXPORT type &core::get_type(const std::string &name) const
    {
        if (const auto at_tp = types.find(name); at_tp != types.cend())
            return *at_tp->second;

        // not found
        throw std::out_of_range(name);
    }

    ORATIOCORE_EXPORT predicate &core::get_predicate(const std::string &name) const
    {
        if (const auto at_p = predicates.find(name); at_p != predicates.cend())
            return *at_p->second;

        // not found
        throw std::out_of_range(name);
    }

#ifdef COMPUTE_NAMES
    void core::recompute_names() noexcept
    {
        expr_names.clear();

        std::queue<std::pair<std::string, expr>> q;
        for (const auto &xpr : exprs)
        {
            expr_names.emplace(&*xpr.second, xpr.first);
            if (!xpr.second->get_type().is_primitive())
                if (const atom *a = dynamic_cast<const atom *>(&*xpr.second); !a)
                    q.push(xpr);
        }

        while (!q.empty())
        {
            const auto &c_xpr = q.front();
            for (const auto &xpr : c_xpr.second->exprs)
                if (expr_names.emplace(&*xpr.second, expr_names.at(&*c_xpr.second) + '.' + xpr.first).second)
                    q.push(xpr);
            q.pop();
        }
    }
#endif

#ifdef BUILD_LISTENERS
    ORATIOCORE_EXPORT void core::fire_log(const std::string msg) const noexcept
    {
        for (const auto &l : listeners)
            l->log(msg);
    }
    ORATIOCORE_EXPORT void core::fire_read(const std::string &script) const noexcept
    {
        for (const auto &l : listeners)
            l->read(script);
    }
    ORATIOCORE_EXPORT void core::fire_read(const std::vector<std::string> &files) const noexcept
    {
        for (const auto &l : listeners)
            l->read(files);
    }
    ORATIOCORE_EXPORT void core::fire_state_changed() const noexcept
    {
        for (const auto &l : listeners)
            l->state_changed();
    }
    ORATIOCORE_EXPORT void core::fire_started_solving() const noexcept
    {
        for (const auto &l : listeners)
            l->started_solving();
    }
    ORATIOCORE_EXPORT void core::fire_solution_found() const noexcept
    {
        for (const auto &l : listeners)
            l->solution_found();
    }
    ORATIOCORE_EXPORT void core::fire_inconsistent_problem() const noexcept
    {
        for (const auto &l : listeners)
            l->inconsistent_problem();
    }
#endif
} // namespace ratio::core
