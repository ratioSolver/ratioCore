#include "scope.h"
#include "field.h"

namespace ratio::core
{
    RATIOCORE_EXPORT scope::scope(scope &scp) : cr(scp.get_core()), scp(scp) {}
    RATIOCORE_EXPORT scope::~scope() {}

    RATIOCORE_EXPORT void scope::new_field(scope &s, field_ptr f) { s.fields.emplace(f->get_name(), std::move(f)); }
    RATIOCORE_EXPORT void scope::new_field(field_ptr f) { fields.emplace(f->get_name(), std::move(f)); }

    RATIOCORE_EXPORT const field &scope::get_field(const std::string &name) const
    {
        if (const auto at_f = fields.find(name); at_f != fields.cend())
            return *at_f->second;

        // if not here, check any enclosing scope
        return scp.get_field(name);
    }

    RATIOCORE_EXPORT method &scope::get_method(const std::string &name, const std::vector<const type *> &ts) const { return scp.get_method(name, ts); }
    RATIOCORE_EXPORT const std::map<std::string, std::vector<method_ptr>> &scope::get_methods() const noexcept { return scp.get_methods(); }

    RATIOCORE_EXPORT type &scope::get_type(const std::string &name) const { return scp.get_type(name); }
    RATIOCORE_EXPORT const std::map<std::string, type_ptr> &scope::get_types() const noexcept { return scp.get_types(); }

    RATIOCORE_EXPORT predicate &scope::get_predicate(const std::string &name) const { return scp.get_predicate(name); }
    RATIOCORE_EXPORT const std::map<std::string, predicate_ptr> &scope::get_predicates() const noexcept { return scp.get_predicates(); }
} // namespace ratio::core
