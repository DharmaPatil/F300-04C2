#ifndef __LogicAdaptation_H_
#define __LogicAdaptation_H_


#define PRODUCT_PARAM_DELAY_TIME_UNIT	 100
/*******************************************************************************
块注释: 自适应参数
*******************************************************************************/
typedef	struct
{
    int32_t  *ps32Weight;			// 当前重量,  根据毛净重包装设置，指向不同的地址， 仅在加料前定位指向
    int32_t  *const ps32SP1;		// 指向大投提前量（修正时用）
    int32_t  *const ps32SP2;		// 指向中投提前量（修正时用）
    int32_t  *const ps32SP3;		// 指向落差提前量（修正时用）
    int32_t  *ps32SP12;				// 指向大投或中投提前量（修正时用）
    uint32_t *const pu32SP3RealTime;// 指向实际测得的慢加时间

	uint32_t u32AdatpEn;			// 自适应开关
	
	// 大/中投修正用
	bool 	 bFirstIn;				// 停止后置1，填充buf后清零，用于填充buf
	int32_t  s32RushWt;				// 过冲量
	int32_t  s32SP1Bkp;				// 大投重量值（备份）
	int32_t  s32SP2Bkp;				// 中投重量值（备份）
	
	uint32_t u32SP12En;				// 大/中投修正开关
    uint32_t u32SP12Range;			// 大/中投修正范围
    uint32_t u32SP12Scope;			// 大/中投修正幅度
    uint32_t u32SP12Count;			// 大/中投当前记录次数		本值只在上电或打开落差修正开关时清零
    uint32_t u32SP12SubCount;		// 大/中投当前记录次数		本值只在上电或打开落差修正开关时清零

	
	// 落差修正用
	int32_t  s32ConfirmWt;			// 定值后的重量，若欠差补料，则记录补料前的重量，若无补料，则值为0
    int32_t  s32TargetVal;			// 目标值
	int32_t  s32SP3Bkp;				// 落差重量值（备份）
	int32_t  s32SP3RealWt;			// 实际小投停止重量
	
	uint32_t u32SP3ForbidTime;		// 小投禁判时间
	uint32_t u32SP3En;				// 落差修正开关
    uint32_t u32SP3Range;			// 落差修正范围
    uint32_t u32SP3Scope;			// 落差修正幅度
    uint32_t u32SP3Count;			// 当前记录次数		本值只在上电或打开落差修正开关时清零
}_strAdapt;
extern _strAdapt 	gstrAdapt;


/*******************************************************************************
			对外接口
*******************************************************************************/
void Adapt_InitSP12Temp(void);					// 初始化大/中投修正暂存值
void Adapt_InitSP3Temp(void);					// 初始化落差修正暂存值
void Adapt_InitData(void);						// 初始化落差修正参数
uint32_t Adapt_Process(_strAdapt *pAdapt);		// 自适应处理  
void Bubble_Sort(int32_t *num, uint32_t n);

#endif

/***************************************The End Of This File***************************************/
