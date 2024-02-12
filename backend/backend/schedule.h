#ifndef SCHEDULE_H
#define SCHEDULE_H

class Schedule
{
public:
    long           id;
    long           line_id;
    QTime        time;

    Schedule() : id(1) { ; }
    virtual ~Schedule() { ; }
};

#endif // SCHEDULE_H
