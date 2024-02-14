#include "precompiled.h"
#include "bus_user.h"

namespace qx {
template <> void register_class(QxClass<Bus_User> & t)
{
    t.id(& Bus_User::id, "schedule_id");
    t.data(& Bus_User::username, "user_username");
    t.data(& Bus_User::password, "user_password");
    t.data(& Bus_User::name, "user_name");
    t.data(& Bus_User::discriminator, "user_discriminator");
}}
