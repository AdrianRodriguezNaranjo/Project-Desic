#ifndef BUS_STOP_H
#define BUS_STOP_H

class Bus_Stop
{
public:
    long           id;
    long           line_id;
    QString        location;

    Bus_Stop() : id(1) { ; }
    virtual ~Bus_Stop() { ; }
};

#endif // BUS_STOP_H
