#include "precompiled.h"
#include "bus_stop.h"

#include <QxOrm_Impl.h>

// QX_REGISTER_CPP_QX_BUS(Line)

namespace qx {
template <> void register_class(QxClass<Bus_Stop> & t)
{
    t.id(& Bus_Stop::id, "bus_stop_id");
    t.data(& Bus_Stop::line_id, "bus_stop_line_id");
    t.data(& Bus_Stop::location, "bus_stop_location");
    t.data(& Bus_Stop::imagenFilePath, "bus_stop_file");
}}
