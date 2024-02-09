#include "../include/precompiled.h"

#include "../include/bicycle.h"

#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_BICYCLES(bicycle)

namespace qx {
template <> void register_class(QxClass<bicycle> & t)
{
   t.id(& bicycle::m_id, "bicycle_id");
   t.data(& bicycle::m_brand, "bicycle_brand");
   t.data(& bicycle::m_model, "bicycle_model");
}}
