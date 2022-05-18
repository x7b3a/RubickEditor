// file qsswitch.h
#ifndef QSSWITCH_H
#define QSSWITCH_H

#define QSSWITCH(__switch_value__, __switch_cases__) do{\
    const QString& ___switch_value___(__switch_value__);\
    {__switch_cases__}\
    }while(0);\

#define QSCASE(__str__, __whattodo__)\
    if(___switch_value___ == __str__)\
    {\
    __whattodo__\
    break;\
    }\

#define QSDEFAULT(__whattodo__)\
    {__whattodo__}\

#endif // QSSWITCH_H
