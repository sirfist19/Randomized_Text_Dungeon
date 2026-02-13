#ifndef static_object_factory_h
#define static_object_factory_h

#include <string>

class static_object;  // forward declare to avoid including heavy headers

static_object* create_static_object(const std::string& id);
#endif
