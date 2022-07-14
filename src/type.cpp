#include "type.h"
#include "core.h"
#include "item.h"

namespace ratio::core
{
    type::type(core &cr, const std::string &name, bool primitive) : scope(cr), name(name), primitive(primitive) {}

    expr type::new_instance()
    {
        auto itm = std::make_shared<complex_item>(*this);
        instances.push_back(itm);
        return itm;
    }
} // namespace ratio::core
