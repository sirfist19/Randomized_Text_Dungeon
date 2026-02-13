#include "static_object.h"
#include "static_object_factory.h"
#include <stdexcept>

static_object* create_static_object(const std::string& id)
{
    if (id == "static.pit")          return new Pit();
    if (id == "static.pedestal")     return new Pedistal();
    if (id == "static.moss")         return new Moss();
    if (id == "static.boulder")      return new Boulder();
    if (id == "static.waterfall")    return new Waterfall();
    if (id == "static.dragon_doors") return new Dragon_doors();
    if (id == "static.dining_table") return new Dining_Table();

    throw std::runtime_error("Unknown static object id: " + id);
}
