#ifndef _QX_BUS_BUS_H_
#define _QX_BUS_BUS_H_

class QX_BUS_DLL_EXPORT Line
{
public:
    long           id;
    int            number;
    QString        firstbusstop;
    QString        lastbusstop;

    Line() : id(1) { ; }
    virtual ~Line() { ; }
};

#endif // LINE_H
