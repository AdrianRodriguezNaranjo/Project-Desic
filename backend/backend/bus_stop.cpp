#include "precompiled.h"
#include "bus_stop.h"

#include <QxOrm_Impl.h>

namespace qx {
template <> void register_class(QxClass<Bus_Stop> & t)
{
    t.id(& Bus_Stop::id, "id");
    t.data(& Bus_Stop::line_id, "line_id");
    t.data(& Bus_Stop::location, "location");
}}
