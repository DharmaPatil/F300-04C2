#ifndef _AUTOFLOW_H
#define _AUTOFLOW_H


typedef struct
{   //自适应开始过程相关参数
    u32 u32Sp2SampleTime;       //中投采样时间
    u32 u32Sp3SampleTime;       //小投采样时间
    
}AutoFlowStart;

typedef struct
{   //自适应加料过程相关参数
    u32 u32Sp2Time,             //中投时间
    u32 u32Sp2Weight,           //中投所投重量
    u32 u32Sp2Flux,             //中投流量
    u32 u32Sp2Lead,             //中投提前量
    u32 u32Sp3Time,             //小投时间
    u32 u32Sp3Weight,           //小投所投重量
    u32 u32Sp3Flux,             //小投流量
    u32 u32Sp3Lead,             //小投提前量
}AotoFlowProcess;





#endif
