#include "scope.h"

namespace ratio::core
{
    ORATIOCORE_EXPORT scope::scope(scope &scp) : cr(scp.get_core()), scp(scp) {}
    ORATIOCORE_EXPORT scope::scope(scope &scp, const std::vector<field_ptr> &flds) : cr(scp.get_core()), scp(scp)
    {
        for (const auto &f : flds)
            fields.emplace(f->get_name(), f);
    }

    ORATIOCORE_EXPORT const field &scope::get_field(const std::string &name) const
    {
        if (const auto at_f = fields.find(name); at_f != fields.cend())
            return *at_f->second;

        // if not here, check any enclosing scope
        return scp.get_field(name);
    }

    ORATIOCORE_EXPORT type &scope::get_type(const std::string &name) const { return scp.get_type(name); }
    ORATIOCORE_EXPORT const std::map<std::string, type_ptr> &scope::get_types() const noexcept { return scp.get_types(); }
} // namespace ratio::core
