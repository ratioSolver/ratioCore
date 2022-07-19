#include "core.h"
#include "predicate.h"
#include <sstream>
#include <fstream>

namespace ratio::core
{
    ORATIOCORE_EXPORT core::core() : scope(*this), env(*this) {}

    ORATIOCORE_EXPORT void core::read(const std::string &script)
    {
        std::stringstream ss(script);
    }

    ORATIOCORE_EXPORT void core::read(const std::vector<std::string> &files)
    {
        for (const auto &f : files)
            if (std::ifstream ifs(f); ifs)
            {
            }
            else
                throw std::invalid_argument("cannot find file '" + f + "'");
    }

    ORATIOCORE_EXPORT expr core::new_bool() noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::new_bool(const bool &) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::new_int() noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::new_int(const semitone::I &) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::new_real() noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::new_real(const semitone::rational &) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::new_time_point() noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::new_time_point(const semitone::rational &) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::new_string() noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::new_string(const std::string &) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::new_enum(type &, std::vector<expr>) { return nullptr; }

    ORATIOCORE_EXPORT expr core::negate(expr) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::conj(const std::vector<expr> &) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::disj(const std::vector<expr> &) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::exct_one(const std::vector<expr> &) noexcept { return nullptr; }

    ORATIOCORE_EXPORT expr core::add(const std::vector<expr> &) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::sub(const std::vector<expr> &) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::mult(const std::vector<expr> &) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::div(const std::vector<expr> &) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::minus(expr) noexcept { return nullptr; }

    ORATIOCORE_EXPORT expr core::lt(expr, expr) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::leq(expr, expr) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::eq(expr, expr) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::geq(expr, expr) noexcept { return nullptr; }
    ORATIOCORE_EXPORT expr core::gt(expr, expr) noexcept { return nullptr; }

    ORATIOCORE_EXPORT expr core::get(const std::string &name) const noexcept
    {
        if (const auto at_xpr = vars.find(name); at_xpr != vars.cend())
            return at_xpr->second;
        else
            return nullptr;
    }

    ORATIOCORE_EXPORT type &core::get_type(const std::string &name) const
    {
        if (const auto at_tp = types.find(name); at_tp != types.cend())
            return *at_tp->second;

        // not found
        throw std::out_of_range(name);
    }
} // namespace ratio::core
