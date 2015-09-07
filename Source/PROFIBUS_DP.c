/*
Description:            PROFIBUS-DP总线相关功能
Author:                 丛海旭
Date:                   2014-4-4
IC:                     PB-OEM2-SE嵌入式PROFIBUS总线桥 北京鼎实创新科技有限公司
*/

/*GSD-1文件：
对应该新ARM平台的GSD文件，供新做设备的客户使用

; ======================================================
; GSD文件：GM8804C-5_ARM仪表使用 GSD-1
; 产品型号：GM8804C-5_ARM标准版
; 版本号：Version:  V1.1
; 文件名：04C-5.GSD 
;======================================================
#Profibus_DP
; Prm-Text-Def-List

PrmText=1
Text(1)="分度值: 1"
Text(2)="分度值: 2"
Text(5)="分度值: 5"
Text(10)="分度值: 10"
Text(20)="分度值: 20"
Text(50)="分度值: 50"
EndPrmText

PrmText=2
Text(0)="单位:  g"
Text(1)="单位: kg"
Text(2)="单位:  t"
Text(3)="单位: lb"
EndPrmText

PrmText=3
Text(0)="0-无小数"
Text(1)="0.0-1位小数"
Text(2)="0.00-2位小数"
Text(3)="0.000-3位小数"
Text(4)="0.0000-4位小数"
EndPrmText

;4 最大分度数

PrmText=5
Text(0)="无计量斗包装模式"
Text(1)="有计量斗包装模式"
Text(2)="双有斗秤互锁模式A"
Text(3)="双有斗秤互锁模式B"
Text(4)="双无斗秤互锁模式A"
Text(5)="双无斗秤互锁模式B"
EndPrmText

PrmText=6
Text(0)="毛重包装"
Text(1)="净重包装"
EndPrmText

PrmText=7
Text(0)="组合方式给料"
Text(1)="单独方式给料"
EndPrmText

;8 自动清零间隔
;9 运行状态判稳超时时间

PrmText=10
Text(0)="副显示配方号"
Text(1)="副显示总累计次数"
Text(2)="副显示当前配方累计次数"
EndPrmText

;11 清零范围
;12 判稳范围
;13 零点跟踪范围
;14 AD数字滤波级数
;15 卸料滤波级数
;16 定值滤波级数

PrmText=17
Text(0)="120次/秒"
Text(1)="240次/秒"
Text(2)="480次/秒"
Text(3)="960次/秒"
EndPrmText

PrmText=18
Text(1)="ON-打开"
Text(0)="OFF-关闭"
EndPrmText

PrmText=19
Text(1)="ON-打开"
Text(0)="OFF-关闭"
EndPrmText

PrmText=20
Text(1)="ON-打开"
Text(0)="OFF-关闭"
EndPrmText

PrmText=21
Text(1)="ON-打开"
Text(0)="OFF-关闭"
EndPrmText

PrmText=22
Text(1)="ON-打开"
Text(0)="OFF-关闭"
EndPrmText

PrmText=23
Text(1)="ON-打开"
Text(0)="OFF-关闭"
EndPrmText

PrmText=24
Text(0)="OFF-不同时松袋"
Text(1)="ON1-同时松袋普通模式"
Text(2)="ON2-同时松袋快速模式"
EndPrmText

;25 输送机启动延时时间
;26 输送机运行时间
;27 再次夹袋延时启动加料时间


;-----------------------------
ExtUserPrmData=1 "最小分度值："
Unsigned8 1 1-50
Prm_Text_Ref=1
EndExtUserPrmData

ExtUserPrmData=2 "单位："
Unsigned8 1 0-4
Prm_Text_Ref=2
EndExtUserPrmData

ExtUserPrmData=3 "小数位数："
Unsigned8 2 0-4
Prm_Text_Ref=3
EndExtUserPrmData

ExtUserPrmData=4 "最大分度数："	;关联仪表最大量程
Unsigned32 10000 1-100000
EndExtUserPrmData

ExtUserPrmData=5 "包装秤工作模式："
Unsigned8 1 0-5
Prm_Text_Ref=5
EndExtUserPrmData

ExtUserPrmData=6 "无计量斗包装模式："
Unsigned8 1 0-1
Prm_Text_Ref=6
EndExtUserPrmData

ExtUserPrmData=7 "给料方式："
Unsigned8 0 0-1
Prm_Text_Ref=7
EndExtUserPrmData

ExtUserPrmData=8 "自动清零间隔："
Unsigned8 0 0-99
EndExtUserPrmData

ExtUserPrmData=9 "运行状态判稳超时时间："
Unsigned16 0 0-999
EndExtUserPrmData

ExtUserPrmData=10 "副显示内容："
Unsigned8 0 0-2
Prm_Text_Ref=10
EndExtUserPrmData

ExtUserPrmData=11 "清零范围："
Unsigned8 50 1-99
EndExtUserPrmData


ExtUserPrmData=12 "判稳范围："
Unsigned8 1 0-99
EndExtUserPrmData

ExtUserPrmData=13 "零点跟踪范围："
Unsigned8 1 0-99
EndExtUserPrmData

ExtUserPrmData=14 "AD数字滤波级数："
Unsigned8 5 0-9
EndExtUserPrmData

ExtUserPrmData=15 "卸料滤波级数："
Unsigned8 3 0-9
EndExtUserPrmData

ExtUserPrmData=16 "定值滤波级数："
Unsigned8 4 0-9
EndExtUserPrmData

ExtUserPrmData=17 "AD采样速度："
Unsigned8 0 0-3
Prm_Text_Ref=16
EndExtUserPrmData

ExtUserPrmData=18 "上电自动清零："
Unsigned8 0 0-1
Prm_Text_Ref=18
EndExtUserPrmData

ExtUserPrmData=19 "手动卸料累计："
Unsigned8 0 0-1
Prm_Text_Ref=19
EndExtUserPrmData

ExtUserPrmData=20 "定值重量保持："
Unsigned8 0 0-1
Prm_Text_Ref=20
EndExtUserPrmData

ExtUserPrmData=21 "累计查询密码："
Unsigned8 0 0-1
Prm_Text_Ref=21
EndExtUserPrmData

ExtUserPrmData=22 "累计清除密码："
Unsigned8 0 0-1
Prm_Text_Ref=22
EndExtUserPrmData

ExtUserPrmData=23 "累计范围扩展："
Unsigned8 1 0-1
Prm_Text_Ref=23
EndExtUserPrmData

ExtUserPrmData=24 "双无斗秤同时松袋："
Unsigned8 0 0-2
Prm_Text_Ref=24
EndExtUserPrmData

ExtUserPrmData=25 "输送机启动延时时间："
Unsigned16 5 0-999
EndExtUserPrmData

ExtUserPrmData=26 "输送机运行时间："
Unsigned16 40 0-999
EndExtUserPrmData

ExtUserPrmData=27 "再次夹袋延时启动加料时间："
Unsigned16 20 0-999
EndExtUserPrmData


;-----------------------------------------
;General parameters
GSD_Revision    = 1
Vendor_Name     = "深圳杰曼科技股份有限公司"
Model_Name      = "GM8804C-2"
Revision        = "V1.02"
Ident_Number    = 0xC2FC
Protocol_Ident  = 0
Station_Type    = 0
;FMS_supp       = 0
Hardware_Release= "H1.0"
Software_Release= "S1.0"
9.6_supp        = 1
19.2_supp       = 1
45.45_supp      = 1
93.75_supp      = 1
187.5_supp      = 1
500_supp        = 1
1.5M_supp       = 1
3M_supp         = 1
6M_supp         = 1
;12M_supp       = 1	;暂不支持12M
MaxTsdr_9.6     = 60
MaxTsdr_19.2    = 60
MaxTsdr_45.45   = 250
MaxTsdr_93.75   = 60
MaxTsdr_187.5   = 60
MaxTsdr_500     = 100
MaxTsdr_1.5M    = 150
MaxTsdr_3M      = 250
MaxTsdr_6M      = 450
;MaxTsdr_12M    = 800  ;暂不支持12M
;Redundancy     = 0
;Repeater_Ctrl_Sig	= 2
;24V_Pins       = 0
Implementation_Type = "spc3"
;Bitmap_Device	="GM8804C-2"      ;位图名称
; Bitmap_Diag	= "Bitmap1D"
; Bitmap_SF     = "Bitmap1S"
; Slave-Specification:
OrderNumber="GM8804C-2"        ;位图名称？
Freeze_Mode_supp     = 1
Sync_Mode_supp       = 1
; Set_Slave_Add_Supp = 0
Auto_Baud_supp       = 1
Fail_Safe            = 0
Min_Slave_Intervall  = 6
Max_Diag_Data_Len    = 6
Modular_Station      = 0
Modul_Offset         = 0
Slave_Family         = 3@TdF@GMxx
;Max_Module          = 5
Max_Input_len        = 224
Max_Output_len       = 224
Max_Data_len         = 448

; UserPrmData: Length and Preset:
User_Prm_Data_Len    = 33

Ext_User_Prm_Data_Ref(0)=1  ;分度值
Ext_User_Prm_Data_Ref(1)=2  ;单位
Ext_User_Prm_Data_Ref(2)=3  ;小数点
Ext_User_Prm_Data_Ref(3)=4	;最大分度 4Bytes
Ext_User_Prm_Data_Ref(7)=5  ;包装秤工作模式：
Ext_User_Prm_Data_Ref(8)=6  ;无计量斗包装模式：
Ext_User_Prm_Data_Ref(9)=7  ;给料方式
Ext_User_Prm_Data_Ref(10)=8     ;自动清零间隔
Ext_User_Prm_Data_Ref(11)=9     ;判稳超时时间 2Byte
Ext_User_Prm_Data_Ref(13)=10    ;副显示内容：
Ext_User_Prm_Data_Ref(14)=11    ;清零范围                    
Ext_User_Prm_Data_Ref(15)=12    ;判稳范围                    
Ext_User_Prm_Data_Ref(16)=13    ;零点跟踪范围                
Ext_User_Prm_Data_Ref(17)=14    ;AD数字滤波参数              
Ext_User_Prm_Data_Ref(18)=15    ;卸料滤波参数                
Ext_User_Prm_Data_Ref(19)=16    ;定值滤波参数                
Ext_User_Prm_Data_Ref(20)=17    ;AD采样速度                  
Ext_User_Prm_Data_Ref(21)=18    ;上电自动清零开关            
Ext_User_Prm_Data_Ref(22)=19    ;手动卸料累计开关            
Ext_User_Prm_Data_Ref(23)=20    ;定值重量保持开关            
Ext_User_Prm_Data_Ref(24)=21    ;累计查询密码开关            
Ext_User_Prm_Data_Ref(25)=22    ;累计清除密码开关            
Ext_User_Prm_Data_Ref(26)=23    ;累计范围扩展开关            
Ext_User_Prm_Data_Ref(27)=24    ;双无斗秤同时松袋：
Ext_User_Prm_Data_Ref(28)=25    ;输送机启动延时时间：2Byte
Ext_User_Prm_Data_Ref(30)=25     输送机运行时间：2Byte
Ext_User_Prm_Data_Ref(32)=25     再次夹袋延时启动加料时间：2Byte
                                    
Module="10AI+24DI+32DI,2AO+32DO" 0x59,0x12,0x13,0x61,0x23

EndModule
*/
/*从仪表输出到主站：10AI+24DI+32DI====================================================
10AI:
Byte0/1/2/3: 当前重量
Byte4/5/6/7: 保留未用
Byte8/9/10/11: 总累计次数
Byte12/1/14/15: 总累计重量
Byte16/17/18/19: 请求数据

24DI+32DI:
Byte0:重量状态
    .2~.0:小数点000:无小数点
                001:一位小数点,0.0
                010:两位小数点,0.00
                011三位小数点,0.000
                100:四位小数点,0.0000
    .4~.3:单位  00:g
                01:kg
                10:t
                11:lb
    .5:符号     1:'-';0:'+'
    .6:稳定     1:稳定；0:不稳
    .7:溢出     1:溢出；0:正常

Byte1:保留未用

Byte2:总累计重量状态
    .2~.0:小数点000:无小数点
                001:一位小数点,0.0
                010:两位小数点,0.00
                011三位小数点,0.000
                100:四位小数点,0.0000
    .4~.3:单位  00:g
                01:kg
                10:t
                11:lb
    .7~.5:保留未用

Byte3:系统状态1
    .0  0: 停止；1: 运行。
    .1  加料前
    .2  大投
    .3  中投
    .4  小投
    .5  定值
    .6  超欠差
    .7  报警
Byte4:系统状态2
    .0  夹袋
    .1  拍袋
    .2  卸料
    .3  零区
    .4  供料
    .5  批次完成
    .6  缺料
    .7  卸料振打
Byte5:系统状态3
    .0  打码
    .1  截料
    .2  提袋
    .3  推袋
    .4  掉线检测
    .5  保留
    .7~.6  00写入数据成功
           01写入数据超范围
           10当前状态无法写入该数据
           11没有写入的数据类型
    
Byte6:请求数据类型，决定10AI中的“请求数据”传送的数据类型
    0：无意义
    1~20：对应的配方1~20的累计重量
    21~40：对应的配方1~20的累计次数
    41：批次数
    42：配方号
    43：剩余批次
    44~79：保留未用
    80~85：F1.1~F1.6
    86~89：F2~F2.3
    90~101：F3.1~F3.12
    102~105：F4~F4.3
    106~112：F5~F5.6
    113~115：F6~F6.2
    116：F7
    117~121：F8~F8.4
    122~125：F9~F9.3
    126~255：保留未用


从主站输出到仪表：2AO+32DO======================================================
2AO: 输入数据

32DO:
Byte0:输入值类型，决定2AO的输入数据类型
    0：不写入数据
    1~20：输入数据写入0清对应的配方1~20的累计数据
    21~40：保留未用
    41：批次数
    42：配方号
    43~79：保留未用
    80~85：F1.1~F1.6
    86~89：F2~F2.3
    90~101：F3.1~F3.12
    102~105：F4~F4.3
    106~112：F5~F5.6
    113~115：F6~F6.2
    116：F7
    117~121：F8~F8.4
    122~125：F9~F9.3
    126~149：保留未用
    150：写入1打印总累计数据
    151：写入0打印当前配方设置表
         写入1~20 打印对应配方号1~20的设置表
         写入21打印所有配方设置表（不打印目标值为0的配方）
    152：写入0 打印当前配方累计报表。
         写入1~20 打印对应配方号1~20的累计报表。
         写入21打印所有配方累计报表（不打印目标值为0的配方）。
    153~255：保留未用

Byte1:请求数据类型，决定10AI中的“请求数据”传送的数据类型
    0：无意义
    1~20：对应的配方1~20的累计重量
    21~40：对应的配方1~20的累计次数
    41：批次数
    42：配方号
    43：剩余批次
    44~79：保留未用
    80~85：F1.1~F1.6
    86~89：F2~F2.3
    90~101：F3.1~F3.12
    102~105：F4~F4.3
    106~112：F5~F5.6
    113~115：F6~F6.2
    116：F7
    117~121：F8~F8.4
    122~125：F9~F9.3
    126~255：保留未用

    
Byte2:执行操作1
    .0 保留未用
    .1 运行
    .2 停止
    .3 清零
    .4 清报警
    .5 选配方
    .6 急停
    .7 手动卸料
Byte3:执行操作2
    .0 手动大投
    .1 手动加料一次
    .2 清除总累计数据（包括每个配方的累计数据）
    .3 手动小投
    .4~.6 保留未用
    .7 掉线检测

*/



/*GSD-2文件：

该GSD文件是为了兼容老版本GM8804C-2的PROFIBUS通信，采用老版本的报文格式
16DI(两个字节)、16DO(两个字节) 、1Byte In、1Byte Out、2AI(四个字节)、2AO(四个字节)

; ======================================================
; GSD文件：带ProfiBus接口的 包装秤GM8804C2P　深圳杰曼科技有限公司
; 产品型号：GM8804C2P
; 版本号：Version:  V1.0
; 文件名：04C2P.GSD
;======================================================
#Profibus_DP
; Prm-Text-Def-List

PrmText=1
Text(0)="分度值: 1"
Text(1)="分度值: 2"
Text(2)="分度值: 5"
Text(3)="分度值: 10"
Text(4)="分度值: 20"
Text(5)="分度值: 50"
EndPrmText

PrmText=2
Text(0)="单位:  g"
Text(1)="单位: kg"
Text(2)="单位:  t"
EndPrmText

PrmText=3
Text(0)="0－无小数"
Text(1)="0.0－1位小数"
Text(2)="0.00－2位小数"
Text(3)="0.000－3位小数"
Text(4)="0.0000－4位小数"
EndPrmText

;4 最大分度数

PrmText=5
Text(0)="0－不进行零点跟踪"
Text(1)="1d－追零范围默认值"
Text(2)="2d－范围内零点跟踪"
Text(3)="3d－范围内零点跟踪"
Text(4)="4d－范围内零点跟踪"
Text(5)="5d－范围内零点跟踪"
Text(6)="6d－范围内零点跟踪"
Text(7)="7d－范围内零点跟踪"
Text(8)="8d－范围内零点跟踪"
Text(9)="9d－范围内零点跟踪"
EndPrmText

PrmText=6
Text(1)="1d－判稳范围"
Text(2)="2d－判稳范围"
Text(3)="3d－判稳范围"
Text(4)="4d－判稳范围"
Text(5)="5d－判稳范围"
Text(6)="6d－判稳范围"
Text(7)="7d－判稳范围"
Text(8)="8d－判稳范围"
Text(9)="9d－判稳范围"
EndPrmText

;7 清零范围 1%-99%

PrmText=8
Text(0)="0－上电不自动清零"
Text(1)="1－上电自动清零"
EndPrmText

;9 清零间隔 0~99

PrmText=10
Text(0)="0－无滤数字波"
Text(1)="1－数字滤波最弱"
Text(2)="2－数字滤波很弱"
Text(3)="3－数字滤波较弱"
Text(4)="4－数字滤波稍弱"
Text(5)="5－适中(默认值)"
Text(6)="6－数字滤波稍强"
Text(7)="7－数字滤波较强"
Text(8)="8－数字滤波很强"
Text(9)="9－数字滤波最强"
EndPrmText

PrmText=11
Text(0)="0－无斗包装模式"
Text(1)="1－有斗包装模式"
EndPrmText


;-----------------------------
ExtUserPrmData=1 "最小分度值(建议设置低于5)："
Unsigned8 0 0-5
Prm_Text_Ref=1
EndExtUserPrmData

ExtUserPrmData=2 "单位："
Unsigned8 1 0-4
Prm_Text_Ref=2
EndExtUserPrmData

ExtUserPrmData=3 "小数位数："
Unsigned8 0 0-4
Prm_Text_Ref=3
EndExtUserPrmData

ExtUserPrmData=4 "最大分度数：" ;关联仪表最大量程
Unsigned16 10000 350-30000
EndExtUserPrmData

ExtUserPrmData=5 "零点跟踪范围："
Unsigned8 1 0-9
Prm_Text_Ref=5
EndExtUserPrmData

ExtUserPrmData=6 "判稳范围："
Unsigned8 1 1-9
Prm_Text_Ref=6
EndExtUserPrmData

ExtUserPrmData=7 "清零范围%："
Unsigned8 50 0-99
EndExtUserPrmData

ExtUserPrmData=8 "上电自动清零开关："
Unsigned8 0 0-1
Prm_Text_Ref=8
EndExtUserPrmData

ExtUserPrmData=9 "运行时清零间隔："
Unsigned8 00 00-99
EndExtUserPrmData

ExtUserPrmData=10 "数字滤波参数："
Unsigned8 7 0-9
Prm_Text_Ref=10
EndExtUserPrmData

ExtUserPrmData=11 "包装秤工作模式开关："
Unsigned8 1 0-1
Prm_Text_Ref=11
EndExtUserPrmData
;-----------------------------------------
;General parameters
GSD_Revision    = 1
Vendor_Name     = "深圳杰曼科技有限公司"
Model_Name      = "GM8804C2P"
Revision        = "V1.0"
Ident_Number    = 0xC2FC
Protocol_Ident  = 0
Station_Type    = 0
;FMS_supp       = 0
Hardware_Release= "H1.0"
Software_Release= "S1.0"
9.6_supp        = 1
19.2_supp       = 1
45.45_supp      = 1
93.75_supp      = 1
187.5_supp      = 1
500_supp        = 1
1.5M_supp       = 1
3M_supp         = 1
6M_supp         = 1
;12M_supp       = 1 ;暂不支持12M
MaxTsdr_9.6     = 60
MaxTsdr_19.2    = 60
MaxTsdr_45.45   = 250
MaxTsdr_93.75   = 60
MaxTsdr_187.5   = 60
MaxTsdr_500     = 100
MaxTsdr_1.5M    = 150
MaxTsdr_3M      = 250
MaxTsdr_6M      = 450
;MaxTsdr_12M    = 800  ;暂不支持12M
;Redundancy     = 0
;Repeater_Ctrl_Sig  = 2
;24V_Pins       = 0
Implementation_Type = "spc3"
;Bitmap_Device  ="GM8804C2P"      ;位图名称
; Bitmap_Diag   = "Bitmap1D"
; Bitmap_SF     = "Bitmap1S"
; Slave-Specification:
OrderNumber="GM8804C2P"        ;位图名称？
Freeze_Mode_supp     = 1
Sync_Mode_supp       = 1
; Set_Slave_Add_Supp = 0
Auto_Baud_supp       = 1
Fail_Safe            = 0
Min_Slave_Intervall  = 6
Max_Diag_Data_Len    = 6
Modular_Station      = 0
Modul_Offset         = 0
Slave_Family         = 3@TdF@GMxx
;Max_Module          = 5
Max_Input_len        = 224
Max_Output_len       = 224
Max_Data_len         = 448

; UserPrmData: Length and Preset:
User_Prm_Data_Len    = 12

Ext_User_Prm_Data_Ref(0)=1
Ext_User_Prm_Data_Ref(1)=2
Ext_User_Prm_Data_Ref(2)=3
Ext_User_Prm_Data_Ref(3)=4  ;最大分度 2Byte
Ext_User_Prm_Data_Ref(5)=5
Ext_User_Prm_Data_Ref(6)=6
Ext_User_Prm_Data_Ref(7)=7
Ext_User_Prm_Data_Ref(8)=8
Ext_User_Prm_Data_Ref(9)=9
Ext_User_Prm_Data_Ref(10)=10
Ext_User_Prm_Data_Ref(11)=11

Module="16DI+1BI+2AI,16DO+1BI+2AO" 0x11,0x10,0x51,0x21,0x20,0x61
;DI:开关量,仪表输入到主站,1Word(2Byte)I
;DO:开关量,主站输出到仪表,1Word(2Byte)
;2AI:整  数,仪表输入到主站,2Word(4Byte)

;1B:字节,表示AO数据的类别.
;2words out(AO):整  数,主站输出到仪表,2Word(4Byte)

EndModule
*/

/*从仪表输出到主站：====================================================
16DI:
DI9:写入错误:数据超限
DI8:写入错误:写入条件不满足
DI7~DI5:配料进程
000:  停止
001:  加料前
010:  快加
011:  中加
100:  慢加
101:  定值
110:  拍袋或松袋
111:  批次数完成
DI4~DI3:毛重/净重
00: 毛重
01: 净重
10: 其他(例如,累计数据无毛净重之分)
11 : 其他,保留
DI2: 仪表溢出状态，0正常，1溢出
DI1: 仪表稳定状态，0不稳，1稳定
DI0: 仪表零点状态，0非零，1零点

1Byte In: (数据状态)
D7:符号位,1:”-”; 0:”+”
D6~D5:2AI数据单位.
00:g
01:kg
10:t
11:保留
D4~D2:2AI数据的小数点
000:无小数点
001:一位小数点,0.0
010:两位小数点,0.00
011三位小数点,0.000
100:四位小数点,0.0000
其余:保留.
D1~D0:当前传送的2AI数据类型
 00:当前重量.
 01: 累计重量
 10: 累计次数
 11: 保留
   
2AI : 按要求传输给主站的数据,4字节.高位数据在前,低位数据在后.

从主站输出到仪表======================================================
16DO：
DO15~DO11: 保留.
DO9 : 当该位由0置为1时,停止状态下,取反手动放料,则返回否定信息.
DO8 : 当该位由0置为1时,停止状态下,清除累计值,否则不响应.
DO7 : 当该位由0置为1时,停止状态下,取反手动小投,否则返回否定信息
DO6 : 当该位由0置为1时,松袋
DO5 : 当该位由0置为1时, 夹袋
DO4 : 当该位由0置为1时,停止状态下,跳转到下一配方.
DO3 : 当该位由0置为1时,清除报警
DO2 : 当该位由0置为1时,停止状态下,清零.
DO1 : 当该位由0置为1时,停止.
DO0 : 当该位由0置为1时,启动.

1Byte Out:
D7~D2:2AO写入数据类型
00000:(0)   保留,不会将2AO中数据写入仪表
00001:(1)   目标(重量控制参数均应小于最大量程).
00010:(2)   大投提前量.
00011:(3)   中投提前量.
00100:(4)   落差值.
00101:(5)   零区值.
00110:(6)   t7 
00111:(7)   超欠差开关(写入奇数打开,偶数关闭,以下开关类型均类似)
01000:(8)   超欠差暂停开关
01001:(9)   超差值.
01010:(10)  欠差值.
01011:(11)  t1(时间参数均应在00~99)
01100:(12)  t2
01101:(13)  t3
01110:(14)  t4
01111:(15)  t5
10000:(16)  t6
10001:(17)  自动落差修正开关
10010:(18)  落差修正次数(00~99)
10011:(19)  落差修正范围(00~99)
10100:(20)  落差修正幅度(1~3)
010101:(21) 拍袋开关
010110:(22) 拍袋延时
010111:(23) 拍袋持续时间
011000:(24) 拍袋次数
011001:(25) 拍袋有效电平时间
011010:(26) 拍袋无效电平时间
011011:(27) 第一次拍袋有效电平时间
011100:(28) 第一次拍袋无效电平时间
011101:(29) 点动小投
011110:(30) 点动小投有效电平时间
011111:(31) 点动小投无效电平时间
100000:(32) 连续放料次数
其 余:     保留,不会将2AO中数据写入仪表

D1~D0:要求2AI传送的数据类型
 00: (0) 当前重量.
 01: (1) 累计重量
 10: (2) 累计次数
 11: (3) 保留

2AO: 需要写入的数据, 4字节.高位数据在前,低位数据在后.

*/



#include "include.h"



//===========================移植该文件时需修改此处=========================================
//SE_REQ_IT - P2.9       0：SE初始化完毕         1：SE初始化等待
//SE_S_RTS - P1.11       0：SE等待（允许）接收   1：SE拒绝接收
//SE_LINK - P1.12     0：SE已经成功连接DP总线 1：SE未成功连接DP总线

//CUP和PB-OEM2-SE之间通讯的波特率，数据方式等 
//UART1连接DP板串口 波特率38400 8位数据 偶校验 1位停止位
#define PBUS_BAUDRATE   38400
#define PBUS_PARITY     UART_EVEN_PARITY
#define PBUS_STOPBITS   UART_1_StopBits
#define PBUS_DATALEN    UARTM_8D_P

#define PBUS_UARTX          UART1               //与SE接口板通讯使用的UART串口
#define PBUS_UARTX_IRQC     UART1_IRQChannel    //
#define PBUS_UARTX_SIPL     UART1_SIPL          //串口中断优先级

#define USER_PRM_LEN_GSD1       34              //用户参数长度
#define SE_A_LEN_GSD1           10               //ProfiBus输出数据长度(A报文)主站到仪表
#define SE_B_LEN_GSD1           (USER_PRM_LEN_GSD1+2)  //用户参数报文格式长度(B报文)主站到仪表
#define SE_IN_LEN_GSD1          29               //ProfiBus输入数据长度(REQ报文)仪表到主站

#define USER_PRM_LEN_GSD2       12              //用户参数长度
#define SE_A_LEN_GSD2           9               //ProfiBus输出数据长度(A报文)主站到仪表
#define SE_B_LEN_GSD2           (USER_PRM_LEN_GSD2+2)  //用户参数报文格式长度(B报文)主站到仪表
#define SE_IN_LEN_GSD2          9               //ProfiBus输入数据长度(REQ报文)仪表到主站

//PROFIBUS通讯时的串口收发缓存数组的最大长度，注意系统进行通讯的数组长度一定要大于这个值
#define  PBUS_RXDLEN   49
#define  PBUS_TXDLEN   49

//系统负责进行与接口板通讯的接收和发送缓存数组
u8 *gu8p_RxdArray = gu8_RxdArray_UART1;
u8 *gu8p_TxdArray = gu8_TxdArray_UART1;

//从机号
#define PBUS_ADDRESS    gs_Setup.TareNumber_1
//系统中断时间10ms
#define SYS10MS     gu32_Sys10ms
//初始化不成功后再次初始化的间隔，单位10ms
#define SE_INIT_INTERVAL   50
//SE接口板和仪表通讯的最大间隔,即发送完成后再次间隔时间内没有接收到完整数据则再次启动发送，单位10ms
#define SE_COM_INTERVAL    10

//接口板初始化相关数据，具体查询SE接口板配置表，该数组不包含第一个字节从机号
//分别是GSD1和GSD2对应的初始化数组
const u8 cu8_PbusInit[2][48] = {
//byte1     byte2        3                4     5 6 7 ……                     24             25              26          27……
//ID号高位 ID号低位 IO配置数据长≤20 IO配置数据1 2 3 4 ……                  接收数据长≤113 发送数据长≤113 用户参数长≤41  不用 
{0xC2,0xFC,0x05,0x59,0x12,0x13,0x61,0x23,   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SE_IN_LEN_GSD1,SE_A_LEN_GSD1,USER_PRM_LEN_GSD1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0xC2,0xFC,0x06,0x11,0x10,0x51,0x21,0x20,0x61,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SE_IN_LEN_GSD2,SE_A_LEN_GSD2,USER_PRM_LEN_GSD2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

//==========================================================================================
u8 gu8_ExtType;     //扩展板类型
u32 gu32_GSDType;   //GSD文件
u32 gu32_FollowMasterSwitch;    //工作参数标定参数由主站控制开关

//串口相关
volatile bool gb_PbusRxdReadyFlg;    //串口接收已经完成标志
volatile bool gb_PbusTxdingFlg;      //串口正在发送标志
volatile bool gb_PbusRxdingFlg;      //串口正在接收标志
u8 gu8_PbusTxdLen;          //发送数据长度
u8 gu8_PbusRxdLen;          //接收数据长度
u8 gu8_PbusTxdCnt;          //接收时的数据个数记录
u8 gu8_PbusRxdCnt;          //发送时的数据个数记录
//SE接口板和CPU的通讯内容相关
u8 gu8_ReqCom;      
/*请求数据命令req_com
D7~D2：不用
D1：0：无效 1：清接口板用户参数已由主站更新标记AB_status
D0：0：请求接口回答PROFIBUS输出数据（A报文） 1：请求接口回答PROFIBUS主站更新的用户参数数据（B报文）*/

/*报文类型及接口状态AB_status
D7：0：A报文 1：B报文
D6~D3：不用
D2~D1：00：接口板接收用户板报文无措
       01：接口板接收用户板报文字符有偶校验错
       10：接口板接收用户板报文有校验和错
       11：不用
D0：0：接口板还没有接收到主站发送的用户参数、或主站没有更新用户参数
    1：接口板用户参数数据已由主站更新*/
    
#define ERRDATA         0x01
#define ERRCANNOTWT     0x02

u8 gu8_WriteErrID;      //写数据错误类型
u8 gu8_PbusSendType;    //PROFIBUS发送数据类型
u8 gu8_PbusWriteType;   //PROFIBUS写入数据类型
u8 gu8_16DoTempHi;      //16DO的高位缓存
u8 gu8_16DoTempLow;     //16DO的低位缓存
u32 gu32_TimeTemp;
bool gb_ConnectCheckFlg;   //掉线检测标志

/*********************************************************************
函数名称: bool Pbus_CheckExt(void)
功    能: 检查是否连接DP扩展板
说    明: 
入口参数: 
返 回 值: true：已连接DP扩展板  false：未连接DP扩展板
设    计: 丛海旭               时    间: 2014-4-4
修    改:                      时    间: 
*********************************************************************/
bool Pbus_CheckExt(void)
{
    //定义PBUS_REQ_IT、PBUS_S_RTS、PBUS_LINK为TTL电平输入
    GPIO_Config(GPIO2, (1<<9), GPIO_IN_TRI_TTL);    //SE_REQ_IT
    GPIO_Config(GPIO1, (1<<11), GPIO_IN_TRI_TTL);   //SE_S_RTS 
    GPIO_Config(GPIO1, (1<<12), GPIO_IN_TRI_TTL);   //SE_LINK  
    
    /*
    未连接PBUS_UART板，上电时三个引脚状态：
    SE_REQ_IT = false;
    SE_S_RTS = true;
    SE_LINK = true;
    连接PBUS_UART板，上电时三个引脚状态：
    SE_REQ_IT = true;
    SE_S_RTS = false;
    SE_LINK = true;
    
    所以根据PBUS_S_RTS和PBUS_LINK的状态能判断当前仪表是否有配了DP通讯的扩展板
    */
    
    if (SE_REQ_IT == true && SE_S_RTS == false)   //for test
        return true;
    else
        return false;
}

/*********************************************************************
函数名称: void UARTInit_Pbus(void)
功    能: 与SE接口板进行通讯前串口初始化
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-04-16
修    改:                      时    间: 
*********************************************************************/
void UARTInit_Pbus(void)
{
    gb_PbusRxdReadyFlg = false;
    gb_PbusTxdingFlg = false;
    gb_PbusRxdingFlg = false;
    gu8_PbusTxdLen = 0;          //发送数据长度
    gu8_PbusRxdLen = 0;          //接收数据长度
    gu8_PbusTxdCnt = 0;          //接收时的数据个数记录
    gu8_PbusRxdCnt = 0;          //发送时的数据个数记录
    
    UART_Init(PBUS_UARTX);               //复位
    UART_GuardTimeConfig(PBUS_UARTX,0); //配置Txempty置位等待延时
    UART_FifoConfig(PBUS_UARTX,DISABLE);//关闭FIFO
    UART_LoopBackConfig(PBUS_UARTX,DISABLE);//关闭回写

    //根据串口相关设置初始化串口 波特率 校验方式
    UART_Config(PBUS_UARTX,PBUS_BAUDRATE,PBUS_PARITY,PBUS_STOPBITS,PBUS_DATALEN);
    UART_RxConfig(PBUS_UARTX,ENABLE);//使能接收
    UART_FifoReset(PBUS_UARTX, UART_RxFIFO);
    UART_OnOffConfig(PBUS_UARTX,ENABLE);//使能串口
    UART_ItConfig(PBUS_UARTX,UART_TxEmpty,DISABLE);   //关闭串口发送中断
//    UART_ItConfig(PBUS_UARTX,UART_RxBufNotEmpty,DISABLE);//关闭接收中断
    UART_ItConfig(PBUS_UARTX,UART_RxBufNotEmpty,ENABLE);//使能接收中断

    EIC_IRQChannelPriorityConfig(PBUS_UARTX_IRQC, PBUS_UARTX_SIPL);
    EIC_IRQChannelConfig(PBUS_UARTX_IRQC, ENABLE);
}

/*********************************************************************
函数名称: void IRQ_PBUS_UART1(void)
功    能: PROFIBUS通讯的串口中断处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-4-16
修    改:                      时    间: 
*********************************************************************/
void IRQ_Pbus(void)
{
    u8 lu8_RxdData;
    
    //发送中断
    if(UART_FlagStatus(PBUS_UARTX) & (UART_TxEmpty|UART_TxHalfEmpty))
    {
        if (gb_PbusTxdingFlg)   //正在发送标志
        {   //发送数据
            if (gu8_PbusTxdCnt < gu8_PbusTxdLen)
            {
                UART_ByteSend(PBUS_UARTX,&gu8p_TxdArray[gu8_PbusTxdCnt++]);
            }
            else
            {
                gb_PbusTxdingFlg = false;   //发送完成
                gb_PbusRxdingFlg = true;    //正在接收
                gu8_PbusTxdCnt = 0;
                UART_ItConfig(PBUS_UARTX,UART_TxEmpty,DISABLE); //关发送中断
            }
        }
        else
        {   //发送完成
            gu8_PbusTxdCnt = 0;
            UART_ItConfig(PBUS_UARTX,UART_TxEmpty,DISABLE); //关发送中断
        }
    }
    //接收中断
    if(UART_FlagStatus(PBUS_UARTX) & (UART_RxBufNotEmpty))
    {
        lu8_RxdData = (u8)(PBUS_UARTX->RxBUFR);
        UART_FifoReset(PBUS_UARTX, UART_RxFIFO);
        if (gb_PbusTxdingFlg == false)
        {   //已经发送完成，允许接收
            gu8p_RxdArray[gu8_PbusRxdCnt++] = lu8_RxdData;
            if (gu8_PbusRxdCnt >= gu8_PbusRxdLen)
            {
                gb_PbusRxdReadyFlg = true;  //接收完成
                gb_PbusRxdingFlg = false;   //接收完成
            }
            if (gu8_PbusRxdCnt >= (PBUS_RXDLEN+1))    //防止数组溢出
                gu8_PbusRxdCnt = 0;
        }
    }
}

/*********************************************************************
函数名称: void Pbus_Init(void)
功    能: SE接口板初始化
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-04-17
修    改:                      时    间: 
*********************************************************************/
void Pbus_Init(void)
{
    static bool sb_Initlg = false;
    static u32 su32_Time = 0;
    
    if (SE_REQ_IT)    //gb_PbusInitOKFlg是防止初始化后仪表接收到初始化命令有无认为没有初始化成功
    {   //DP扩展板有初始化请求
        if (SE_S_RTS == false)
        {   //DP扩展板允许接收 发送初始化报文
            if (gb_PbusTxdingFlg == false)
            {   //串口没有数据发送
                //清接收和发送缓存
                if (su32_Time == 0 || SYS10MS - su32_Time >= SE_INIT_INTERVAL)
                {   //初始化不成功后再次初始化的间隔
                    sb_Initlg = true;
                    memset(gu8p_RxdArray, 0, PBUS_RXDLEN);
                    memset(gu8p_TxdArray, 0, PBUS_TXDLEN);
                    gu8p_TxdArray[0] = PBUS_ADDRESS; //从机号
                    memcpy(&gu8p_TxdArray[1], &cu8_PbusInit[gu32_GSDType][0], PBUS_TXDLEN - 1);   //初始化数组
                    Pbus_StartSend(gu8p_TxdArray, PBUS_TXDLEN, PBUS_RXDLEN);   //开始发送
                    su32_Time = SYS10MS;
                }
            }
        }
    }
    else
    {
        if (sb_Initlg)
        {   //已经发送过初始化报文，并且SE_REQ_IT已经变低电平
            //能执行到这里证明SE接口板已经初始化成功
            sb_Initlg = false;
            gu8_ReqCom = 0;     //初始化后首先请求A报文
            gu8_WriteErrID = 0; //清写数据错误标志
            gu8_PbusSendType = 0;
            gu8_PbusWriteType = 0;
            gu8_16DoTempHi = 0;
            gu8_16DoTempLow = 0;
            BuzzerStart(5);
        }
    }
}

/*********************************************************************
函数名称: void Pbus_SendDataDeal(u8 *fu8p_Array, u32 fu32_GSDType)
功    能: PROFIBUS发送数据的构造，从仪表输出到主站的数据
说    明: 
入口参数: fu8p_Array：发送数据首地址 fu32_GSDType：GSD文件类型
返 回 值: 
设    计: 丛海旭               时    间: 2014-05-04
修    改:                      时    间: 
*********************************************************************/
void Pbus_SendDataDeal(u8 *fu8p_Array, u32 fu32_GSDType)
{
    ULDATA UL_Value;
    u8 i;
    
    if (fu32_GSDType == GSD_1)
    {   //GSD1对应的数据构造
        fu8p_Array[0] = gu8_ReqCom;     //请求数据命令
        //10AI:
        //Byte0~3:当前重量================================================
        if (gs_Setup.ResultHoldSwitch != 0 && gb_DiscWtGetedFlg == true && gb_RunFlg == true)
            UL_Value.ul = gu32_HoldWt;    //定值重量保持时显示的重量
        else
            UL_Value.ul = Modbus_WtOrMvData(CurWt);
        //按要求传输给主站的数据,4字节.高位数据在前,低位数据在后.
        for (i = 0; i < 4; i++)
            fu8p_Array[1+i] = UL_Value.uc[3-i];
        //Byte4~7:保留未用================================================
        UL_Value.ul = 0;
        //按要求传输给主站的数据,4字节.高位数据在前,低位数据在后.
        for (i = 0; i < 4; i++)
            fu8p_Array[5+i] = UL_Value.uc[3-i];
        //Byte8~11:总累计次数================================================
        UL_Value.ul = gu32_SumNum;
        //按要求传输给主站的数据,4字节.高位数据在前,低位数据在后.
        for (i = 0; i < 4; i++)
            fu8p_Array[9+i] = UL_Value.uc[3-i];
        //Byte12~15:总累计================================================
        UL_Value.ul = gu32_SumWt;
        //按要求传输给主站的数据,4字节.高位数据在前,低位数据在后.
        for (i = 0; i < 4; i++)
            fu8p_Array[13+i] = UL_Value.uc[3-i];
        //Byte16~19:请求数据================================================
        switch(gu8_PbusSendType)
        {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20://配方累计重量
                UL_Value.ul = gu32_SumWt_Rec[gu8_PbusSendType];
                break;
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 30:
            case 31:
            case 32:
            case 33:
            case 34:
            case 35:
            case 36:
            case 37:
            case 38:
            case 39:
            case 40://配方累计次数
                UL_Value.ul = gu32_SumNum_Rec[gu8_PbusSendType-20];
                break;
            case 41: //批次数
                UL_Value.ul = gu32_BatchTime;
                break;
            case 42://配方号
                UL_Value.ul = gu32_RecNum;
                break;
            case 43://剩余批次数
                UL_Value.ul = gu32_BatchTimeCnt;
                break;
            case 80://F1.1
                UL_Value.ul = gs_Rec.Target;
                break;
            case 81:        //大投提前量
                UL_Value.ul = gs_Rec.Large;
                break;
            case 82:        //中投提前量
                UL_Value.ul = gs_Rec.Middle;
                break;
            case 83:        //落差值
                UL_Value.ul = gs_Rec.Small;
                break;
            case 84:        //近零值
                UL_Value.ul = gs_Rec.NearZero;
                break;
            case 85:        //拍袋起始重量
                UL_Value.ul = gs_Rec.FlapBagWt;
                break;
            case 86:        //F2
                UL_Value.ul = gs_Rec.OUSwitch;
                break;
            case 87:        //F2.1
                UL_Value.ul = gs_Rec.OverValue;
                break;
            case 88:        //F2.2
                UL_Value.ul = gs_Rec.UnderValue;
                break;
            case 89:        //F2.3
                UL_Value.ul = gs_Rec.OUStopSwitch;
                break;
            case 90:        //F3.1
                UL_Value.ul = gs_Rec.TimeT1;
                break;
            case 91:        //F3.2
                UL_Value.ul = gs_Rec.TimeT2b;
                break;
            case 92:        //F3.3
                UL_Value.ul = gs_Rec.TimeT3;
                break;
            case 93:        //F3.4
                UL_Value.ul = gs_Rec.TimeT4;
                break;
            case 94:        //F3.5
                UL_Value.ul = gs_Rec.TimeT5;
                break;
            case 95:        //F3.6
                UL_Value.ul = gs_Rec.TimeT6;
                break;
            case 96:        //F3.7
                UL_Value.ul = gs_Rec.TimeT7;
                break;
            case 97:        //F3.8
                UL_Value.ul = gs_Rec.TimeT2a;
                break;
            case 98:        //F3.9
                UL_Value.ul = gs_Rec.TimeT9;
                break;
            case 99:        //F3.10
                UL_Value.ul = gs_Rec.TimeT10;
                break;
            case 100:        //F3.11
                UL_Value.ul = gs_Rec.TimeT11;
                break;
            case 101:        //F3.12
                UL_Value.ul = gs_Rec.TimeT12;
                break;
            case 102:        //F4
                UL_Value.ul = gs_Rec.CorrectFallSwitch;
                break;
            case 103:        //F4.1
                UL_Value.ul = gs_Rec.CorrectFallTime;
                break;
            case 104:        //F4.2
                UL_Value.ul = gs_Rec.CorrectFallRng1;
                break;
            case 105:        //F4.3
                UL_Value.ul = gs_Rec.CorrectFallRng2;
                break;
            case 106:        //F5
                UL_Value.ul = gs_Rec.FlapBagSwitch;
                break;
            case 107:        //F5.1
                UL_Value.ul = gs_Rec.FlapBagAddNum;
                break;
            case 108:        //F5.2
                UL_Value.ul = gs_Rec.FlapBagNum;
                break;
            case 109:        //F5.3
                UL_Value.ul = gs_Rec.FlapBagDelay;
                break;
            case 110:        //F5.4
                UL_Value.ul = gs_Rec.FlapBagTimeHi;
                break;
            case 111:        //F5.5
                UL_Value.ul = gs_Rec.FlapBagTimeLow;
                break;
            case 112:        //F5.6
                UL_Value.ul = gs_Rec.FlapBagExtTime;
                break;
            case 113:        //F6
                UL_Value.ul = gs_Rec.SmallPluseSwitch;
                break;
            case 114:        //F6.1
                UL_Value.ul = gs_Rec.SmallPluseHi;
                break;
            case 115:        //F6.2
                UL_Value.ul = gs_Rec.SmallPluseLow;
                break;
            case 116:        //F7
                UL_Value.ul = gs_Rec.CombinedNum;
                break;
            case 117:        //F8
                UL_Value.ul = gs_Rec.DiscShakeSwitch;
                break;
            case 118:        //F8.1
                UL_Value.ul = gs_Rec.DiscEffTime;
                break;
            case 119:        //F8.2
                UL_Value.ul = gs_Rec.DiscShakeTimeHi;
                break;
            case 120:        //F8.3
                UL_Value.ul = gs_Rec.DiscShakeTimeLow;
                break;
            case 121:        //F8.4
                UL_Value.ul = gs_Rec.DiscShakeNum;
                break;
            case 122:        //F9
                UL_Value.ul = gs_Rec.PrintCodeSwitch;
                break;
            case 123:        //F9.1
                UL_Value.ul = gs_Rec.PrintCodeRunDelay;
                break;
            case 124:        //F9.2
                UL_Value.ul = gs_Rec.PrintCodeTime;
                break;
            case 125:        //F9.3
                UL_Value.ul = gs_Rec.SheildDiscSwitch;
                break;
            default:
                UL_Value.ul = 0;
                break;
        }
        //按要求传输给主站的数据,4字节.高位数据在前,低位数据在后.
        for (i = 0; i < 4; i++)
            fu8p_Array[17+i] = UL_Value.uc[3-i];
        //24DI+32DI:
        //Byte0:重量状态====================================================
        fu8p_Array[21] |= gs_Cal.Point;
        fu8p_Array[21] |= gs_Cal.Unit << 3;
        fu8p_Array[21] |= (gb_SigWtFlg ? (1 << 5) : 0);
        fu8p_Array[21] |= (gb_StabFlg ? (1 << 6) : 0);
        fu8p_Array[21] |= (gb_OflFlg ? (1 << 7) : 0);
        //Byte1:保留未用====================================================
        fu8p_Array[22] = 0;
        //Byte2:总累计重量状态==============================================
        fu8p_Array[23] |= gu32_SumWtPoint;
        fu8p_Array[23] |= gu32_SumWtUnit << 3;
        //Byte3:系统状态1===================================================
              fu8p_Array[24] = 0;
/*运行    */  fu8p_Array[24] |= (gb_RunFlg ? (1 << 0) : 0);
/*加料前  */  fu8p_Array[24] |= ((gu32_Process == T1PROCESS && gb_RunFlg == true) ? (1 << 1) : 0);
/*大投    */  fu8p_Array[24] |= (Sp_Disc_State(SP1_STATE) ? (1 << 2) : 0);
/*中投    */  fu8p_Array[24] |= (Sp_Disc_State(SP2_STATE) ? (1 << 3) : 0);
/*小投    */  fu8p_Array[24] |= (Sp_Disc_State(SP3_STATE) ? (1 << 4) : 0);
/*定值    */  fu8p_Array[24] |= (gb_CompFlg ? (1 << 5) : 0);
/*超欠差  */  fu8p_Array[24] |= ((gb_OverFlg || gb_UnderFlg || gb_OUOverFlg || gb_OUUnderFlg) ? (1 << 6) : 0);
/*报警    */  fu8p_Array[24] |= (gb_AlarmFlg ? (1 << 7) : 0);
        //Byte4:系统状态2===================================================
              fu8p_Array[25] = 0;
/*夹袋    */  fu8p_Array[25] |= (gb_PackFlg ? (1 << 0) : 0);
/*拍袋    */  fu8p_Array[25] |= ((gb_FlapBagOutFlg || gb_FlapAllHighFlg) ? (1 << 1) : 0);
/*卸料    */  fu8p_Array[25] |= (Sp_Disc_State(DISC_STATE) ? (1 << 2) : 0);
/*零区    */  fu8p_Array[25] |= (gb_NzFlg ? (1 << 3) : 0);
/*供料    */  fu8p_Array[25] |= (gb_FillFlg ? (1 << 4) : 0);
/*批次完成*/  fu8p_Array[25] |= (gb_BatchOverFlg ? (1 << 5) : 0);
/*缺料    */  fu8p_Array[25] |= (gb_LackFlg ? (1 << 6) : 0);
/*卸料振打*/  fu8p_Array[25] |= (gb_DiscShakeFlg ? (1 << 7) : 0);
        //Byte5:系统状态3===================================================
              fu8p_Array[26] = 0;
/*打码    */  fu8p_Array[26] |= (gb_PrintCodeOutFlg ? (1 << 0) : 0);
/*截料    */  fu8p_Array[26] |= (gb_Sp3Flg ? (1 << 1) : 0);
/*提袋    */  fu8p_Array[26] |= (gb_LiftBagFlg ? (1 << 2) : 0);
/*推袋    */  fu8p_Array[26] |= (gb_PushBagFlg ? (1 << 3) : 0);
/*掉线检测*/  fu8p_Array[26] |= (gb_ConnectCheckFlg ? (1 << 4) : 0);
/*数据写入*/  fu8p_Array[26] |= gu8_WriteErrID << 6;
        //Byte6:请求数据类型
        fu8p_Array[27] |= gu8_PbusSendType;
        Pbus_StartSend(fu8p_Array, SE_IN_LEN_GSD1, (gu8_ReqCom & 0x01 ? SE_B_LEN_GSD1 : SE_A_LEN_GSD1));
    }
    else
    {   //GSD2对应的数据构造
        fu8p_Array[0] = gu8_ReqCom;     //请求数据命令
        /*16DI：
        DI9:写入错误:数据超限
        DI8:写入错误:写入条件不满足
        DI7~DI5:配料进程
        000:  停止
        001:  加料前
        010:  快加
        011:  中加
        100:  慢加
        101:  定值
        110:  拍袋或松袋
        111:  批次数完成
        DI4~DI3:毛重/净重
        00: 毛重
        01: 净重
        10: 其他(例如,累计数据无毛净重之分)
        11 : 其他,保留
        DI2: 仪表溢出状态，0正常，1溢出
        DI1: 仪表稳定状态，0不稳，1稳定
        DI0: 仪表零点状态，0非零，1零点*/
        fu8p_Array[1] = gu8_WriteErrID; //写数据错误类型
        gu8_WriteErrID = 0;     //清除错误类型
        fu8p_Array[2] |= (gb_ZeroPosition ? 0x01 : 0x00);   //零位
        fu8p_Array[2] |= (gb_StabFlg ? 0x02 : 0x00);        //稳定
        fu8p_Array[2] |= ((gb_OflFlg || gb_AdOflFlg) ? 0x04 : 0x00);        //溢出
        fu8p_Array[2] |= (gb_DispNetFlg ? 0x08 : 0x00);        //净重
        fu8p_Array[2] |= (gu8_PbusSendType ? 0x10 : 0x00);        //数据类型
        if (gb_RunFlg)
        {
            if (gb_BatchOverFlg)    //批次完成
                fu8p_Array[2] |= 0xE0;
            else if (gu32_Process == T1PROCESS) //加料前
                fu8p_Array[2] |= 0x20;
            else if (gu32_Process == LARGEPROCESS)  //快加
                fu8p_Array[2] |= 0x40;
            else if (gu32_Process == MIDDLEPROCESS) //中加
                fu8p_Array[2] |= 0x60;
            else if (gu32_Process == SMALLPROCESS)  //慢加
                fu8p_Array[2] |= 0x80;
            else if (gu32_Process == T3PROCESS || gu32_Process == T4PROCESS)    //定值
            {
                if ((gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B) && (gb_FlapWaitFlg || gb_FlapStartFlg))  //无斗 拍袋
                    fu8p_Array[2] |= 0xC0;
                else
                    fu8p_Array[2] |= 0xA0;
            }
            else if (gu32_Process == FALLPROCESS || gu32_Process == T5PROCESS)
            {
                if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)    //有斗 卸料
                    fu8p_Array[2] |= 0xC0;
            }
        }
        /*
        1Byte In: (数据状态)
        D7:符号位,1:”-”; 0:”+”
        D6~D5:2AI数据单位.
        00:g
        01:kg
        10:t
        11:保留
        D4~D2:2AI数据的小数点
        000:无小数点
        001:一位小数点,0.0
        010:两位小数点,0.00
        011三位小数点,0.000
        100:四位小数点,0.0000
        其余:保留.
        D1~D0:当前传送的2AI数据类型
        00: 当前重量
        01: 累计重量
        10: 累计次数
        11: 保留*/
        switch (gu8_PbusSendType)
        {
            case 1: //总累计重量
                //符号
                fu8p_Array[3] |= gu32_SumWtUnit << 5;  //单位
                fu8p_Array[3] |= gu32_SumWtPoint << 2;  //小数点
                UL_Value.ul = gu32_SumWt;  //累计重量
                break;
            case 2: //总累计次数
                //符号
                fu8p_Array[3] |= 3 << 5;  //累计次数没有单位，旧程序等于3时没有单位，但是新程序3代表lb
                fu8p_Array[3] |= 0 << 2;  //小数点
                UL_Value.ul = gu32_SumNum;  //累计次数
                break;
            case 0:
            default:    //当前重量
                fu8p_Array[3] |= (gb_SigWtFlg ? 0x80 : 0x00);   //符号
                fu8p_Array[3] |= gs_Cal.Unit << 5;  //单位
                fu8p_Array[3] |= gs_Cal.Point << 2;  //小数点
                UL_Value.ul = gu32_Wt;  //当前重量
                break;
        }
        fu8p_Array[3] |= (gu8_PbusSendType & 0x03);
        //2AI : 按要求传输给主站的数据,4字节.高位数据在前,低位数据在后.
        for (i = 0; i < 4; i++)
        {
            fu8p_Array[4+i] = UL_Value.uc[3-i];
        }
        Pbus_StartSend(fu8p_Array, SE_IN_LEN_GSD2, (gu8_ReqCom & 0x01 ? SE_B_LEN_GSD2 : SE_A_LEN_GSD2));
    }
}

/*********************************************************************
函数名称: void Pbus_Send(void)
功    能: 仪表PROFIBUS发送数据处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-05-04
修    改:                      时    间: 
*********************************************************************/
void Pbus_Send(void)
{
    if (SE_REQ_IT == false && SE_S_RTS == false && gb_PbusTxdingFlg == false)
    {   //SE接口板初始化成功，并且允许接收，串口没有发送和接收 那么才可以处理发送数据
        if (gb_PbusRxdReadyFlg == false && gb_PbusRxdingFlg == false)
        {
            //清收发数组
            memset(gu8p_RxdArray, 0, PBUS_RXDLEN);
            memset(gu8p_TxdArray, 0, PBUS_TXDLEN);
            //构造发送数据的数据
            Pbus_SendDataDeal(gu8p_TxdArray, gu32_GSDType);
        }
        else
        {
            if (SYS10MS - gu32_TimeTemp >= SE_COM_INTERVAL)
            {   //发送后超过SE_COM_INTERVAL时间后没有接收到正常数据则重新发送
                gb_PbusTxdingFlg = false;
                gb_PbusRxdReadyFlg = false;
                gb_PbusRxdingFlg = false;
            }
        }
    }
}

/*********************************************************************
函数名称: u8 Pbus_ReciveCheck(u8 *fu8p_Array, u32 fu32_GSDType, u8 fu8_ReqCom)
功    能: 
说    明: 
入口参数: gu8_RxdArray：接收数组首地址 fu32_GSDType：GSD文件类型 fu8_ReqCom：请求数据命令
返 回 值: 
!=0：错误代码
0x84 长度错误
0x81 校验和A错
0x82 校验和B错
0x85 接收A报文格式错
0x86 接收B报文格式错
0x90 接收的是初始化返回的命令,并且正确
0x91 接收的是初始化返回的命令,IO配置数据错误
0x92 接收的是初始化返回的命令,IO配置数据个数大于20
0x93 接收的是初始化返回的命令,站地址大于126
0x94 接收的是初始化返回的命令,报文字符校验和错
0x95 接收的是初始化返回的命令,报文字符有偶校验错误
0x96 接收的是初始化返回的命令,报文字符个数<49

D2~D1：00：接口板接收用户板报文无措
       01：接口板接收用户板报文字符有偶校验错
       10：接口板接收用户板报文有校验和错
       11：不用

 0：数据正常
设    计: 丛海旭               时    间: 2014-05-05
修    改:                      时    间: 
*********************************************************************/
u8 Pbus_ReciveCheck(u8 *fu8p_Array, u32 fu32_GSDType, u8 fu8_ReqCom)
{
    u8 lu8_ErrCode = 0;
    u8 lu8_SeLen;
    
    if (gu8_PbusRxdCnt == PBUS_RXDLEN)
    {   //接收的是初始化返回报文
        lu8_ErrCode = gu8p_RxdArray[1];
        lu8_ErrCode |= 0x90;
    }
    else
    {   //判断接收的是A或B报文
        if (fu32_GSDType == GSD_1)
            lu8_SeLen = ((fu8_ReqCom & 0x01) ? SE_B_LEN_GSD1 : SE_A_LEN_GSD1);
        else
            lu8_SeLen = ((fu8_ReqCom & 0x01) ? SE_B_LEN_GSD2 : SE_A_LEN_GSD2);
                
        if (gu8_PbusRxdCnt != lu8_SeLen)
        {   //长度错误
            lu8_ErrCode = 0x84;
        }
        else
        {
            if (fu8p_Array[lu8_SeLen-1] == Pbus_Checksum(fu8p_Array, lu8_SeLen - 1))
            {   //校验和正确
                if ((fu8p_Array[0] & 0x80) != ((fu8_ReqCom & 0x01) ? 0x80 : 0))
                {   //接收A格式错误，此数据是B报文
                    lu8_ErrCode = ((fu8_ReqCom & 0x01) ? 0x86 : 0x85);
                }
                else if ((fu8p_Array[0] & 0x06) != 0)
                {   //SE接收用户报文有错误
                    lu8_ErrCode = fu8p_Array[0];
                }
                else
                {   //无错误，赋值请求数据命令
                    gu8_ReqCom = ((fu8_ReqCom & 0x01) ? 0x02 : (fu8p_Array[0] & 0x01));
                }
            }
            else
            {   //校验和错误
                lu8_ErrCode = ((fu8_ReqCom & 0x01) ? 0x82 : 0x81);
            }
        }
    }
    return lu8_ErrCode;
}

/*********************************************************************
函数名称: void Pbus_ReciveDataDeal(u8 *fu8p_Array, u32 fu32_GSDType)
功    能: 
说    明: 
入口参数: fu8p_Array：接收数组首地址 fu32_GSDTypee：GSD文件类型 fu8_ReqCom：请求数据命令
返 回 值: 
设    计: 丛海旭               时    间: 2014-05-06
修    改:                      时    间: 
*********************************************************************/
void Pbus_ReciveDataDeal(u8 *fu8p_Array, u32 fu32_GSDType)
{
    ULDATA UL_Value;
    u8 i;
    u32 lu32_Temp;
    const u8 cu8_DivTab[] = {1,2,5,10,20,50};
    
    UL_Value.ul = 0;
    
    if (SE_LINK)   //PROFIBUS总线已经联通才能接下来处理下面的
        return;
    
    if (fu32_GSDType == GSD_1)  //GSD1处理
    {
        if (fu8p_Array[0] & 0x80)  //B报文处理 用户参数
        {
            if (gu32_FollowMasterSwitch == 0)   //工作参数标定参数不受主站控制
                return;
            //分度值
            if (gs_Cal.Div != gu8p_RxdArray[1])
            {
                lu32_Temp = FRAM_ReadDW(FMAddr_CalDiv);
                //需要调整分度,和追零零点
                gs_Cal.ADK *= (float)gu8p_RxdArray[1];    //新分度值=原分度值*new dd/old dd
                gs_Cal.ADK /= (float)gs_Cal.Div;
                FRAM_WriteFloat(FMAddr_CalADK, gs_Cal.ADK);
            
                //调整追零零点:调整分度值后,如果是0,仍然显示零
                gu32_FloatZero *= gs_Cal.Div;
                gu32_FloatZero /= gu8p_RxdArray[1];
                //保存新的分度值
                gs_Cal.Div = gu8p_RxdArray[1];
                FRAM_WriteDW(FMAddr_CalDiv, gs_Cal.Div);

                //分度值改小后的最大量程处理
                if (gs_Cal.Div < lu32_Temp)                        //分度值由大变小时最大量程需要调整
                {
                    if (gs_Cal.Cap > (gs_Cal.Div * 100000))
                    {
                        gs_Cal.Cap = gs_Cal.Div * 100000;
                        FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                    }
                }
                else if (gs_Cal.Div > gs_Cal.Cap)            //修改分度后量程小于分度，则修改量程为分度值
                {
                    gs_Cal.Cap = gs_Cal.Div;
                    FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                }
            }
            //单位
            if (gs_Cal.Unit != gu8p_RxdArray[2])
            {
                gs_Cal.Unit = gu8p_RxdArray[2];
                FRAM_WriteDW(FMAddr_CalUnit, gs_Cal.Unit);
                ClrSum();
            }
            //小数点
            if (gs_Cal.Point != gu8p_RxdArray[3])
            {
                gs_Cal.Point = gu8p_RxdArray[3];
                FRAM_WriteDW(FMAddr_CalPoint, gs_Cal.Point);
                ClrSum();
            }
            //最大分度数
            UL_Value.uc[3] = gu8p_RxdArray[4];
            UL_Value.uc[2] = gu8p_RxdArray[5];
            UL_Value.uc[1] = gu8p_RxdArray[6];
            UL_Value.uc[0] = gu8p_RxdArray[7];
            UL_Value.ul *= gs_Cal.Div;  //得到最大量程
            if (gs_Cal.Cap != UL_Value.ul)
            {
                gs_Cal.Cap = UL_Value.ul;
                FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
            }
            //包装秤工作模式
            if (gu8p_RxdArray[8] != gs_Setup.TareMode)
            {
                gs_Setup.TareMode = gu8p_RxdArray[8];
                FRAM_WriteDW(FMAddr_TareMode, gs_Setup.TareMode);
            }
            //无计量斗包装模式：
            if (gu8p_RxdArray[9] != gs_Setup.GorNMode)
            {
                gs_Setup.GorNMode = gu8p_RxdArray[9];
                FRAM_WriteDW(FMAddr_GorNMode, gs_Setup.GorNMode);
            }
            //给料方式
            if (gu8p_RxdArray[10] != gs_Setup.FeedMode)
            {
                gs_Setup.FeedMode = gu8p_RxdArray[10];
                FRAM_WriteDW(FMAddr_FeedMode, gs_Setup.FeedMode);
            }
            //自动清零间隔
            if (gu8p_RxdArray[11] != gs_Setup.AutoClrZeroInterval)
            {
                gs_Setup.AutoClrZeroInterval = gu8p_RxdArray[11];
                FRAM_WriteDW(FMAddr_AutoClrZeroInterval, gs_Setup.AutoClrZeroInterval);
            }
            //判稳超时时间
            UL_Value.uc[1] = gu8p_RxdArray[12];
            UL_Value.uc[0] = gu8p_RxdArray[13];
            UL_Value.uc[2] = 0;
            UL_Value.uc[3] = 0;
            if (UL_Value.ul != gs_Setup.JudgeStabOverTime)
            {
                gs_Setup.JudgeStabOverTime = UL_Value.ul;
                FRAM_WriteDW(FMAddr_JudgeStabOverTime, gs_Setup.JudgeStabOverTime);
            }
            //副显示内容
            if (gu8p_RxdArray[14] != gs_Setup.AuxDispType)
            {
                gs_Setup.AuxDispType = gu8p_RxdArray[14];
                FRAM_WriteDW(FMAddr_AuxDispType, gs_Setup.AuxDispType);
            }
            //清零范围
            if (gu8p_RxdArray[15] != gs_Setup.ClrZeroRng)
            {
                gs_Setup.ClrZeroRng = gu8p_RxdArray[15];
                FRAM_WriteDW(FMAddr_ClrZeroRng, gs_Setup.ClrZeroRng);
            }
            //判稳范围
            if (gu8p_RxdArray[16] != gs_Setup.JudgeStabRng)
            {
                gs_Setup.JudgeStabRng = gu8p_RxdArray[16];
                FRAM_WriteDW(FMAddr_JudgeStabRng, gs_Setup.JudgeStabRng);
            }
            //零点跟踪范围
            if (gu8p_RxdArray[17] != gs_Setup.TraceZeroRng)
            {
                gs_Setup.TraceZeroRng = gu8p_RxdArray[17];
                FRAM_WriteDW(FMAddr_TraceZeroRng, gs_Setup.TraceZeroRng);
            }
            //滤波参数
            if (gu8p_RxdArray[18] != gs_Setup.AdFilterLevel)
            {
                gs_Setup.AdFilterLevel = gu8p_RxdArray[18];
                FRAM_WriteDW(FMAddr_AdFilterLevel, gs_Setup.AdFilterLevel);
            }
            //卸料滤波参数
            if (gu8p_RxdArray[19] != gs_Setup.AdFilterLevelDisc)
            {
                gs_Setup.AdFilterLevelDisc = gu8p_RxdArray[19];
                FRAM_WriteDW(FMAddr_AdFilterLevelDisc, gs_Setup.AdFilterLevelDisc);
            }
            //定值滤波参数
            if (gu8p_RxdArray[20] != gs_Setup.AdFilterLevelComp)
            {
                gs_Setup.AdFilterLevelComp = gu8p_RxdArray[20];
                FRAM_WriteDW(FMAddr_AdFilterLevelComp, gs_Setup.AdFilterLevelComp);
            }
            //AD采样速度
            if (gu8p_RxdArray[21] != gs_Setup.AdSamplingRate)
            {
                gs_Setup.AdSamplingRate = gu8p_RxdArray[21];
                FRAM_WriteDW(FMAddr_AdSamplingRate, gs_Setup.AdSamplingRate);
                gb_AdOKFlg = AD_Init(gs_Setup.AdSamplingRate);
            }
            //上电自动清零
            if (gu8p_RxdArray[22] != gs_Setup.PowerOnZeroSwitch)
            {
                gs_Setup.PowerOnZeroSwitch = gu8p_RxdArray[22];
                FRAM_WriteDW(FMAddr_PowerOnZeroSwitch, gs_Setup.PowerOnZeroSwitch);
            }
            //手动卸料累计开关
            if (gu8p_RxdArray[23] != gs_Setup.DiscSumSwitch)
            {
                gs_Setup.DiscSumSwitch = gu8p_RxdArray[23];
                FRAM_WriteDW(FMAddr_DiscSumSwitch, gs_Setup.DiscSumSwitch);
            }
            //定值重量保持开关
            if (gu8p_RxdArray[24] != gs_Setup.ResultHoldSwitch)
            {
                gs_Setup.ResultHoldSwitch = gu8p_RxdArray[24];
                FRAM_WriteDW(FMAddr_ResultHoldSwitch, gs_Setup.ResultHoldSwitch);
            }
            //累计查询密码开关
            if (gu8p_RxdArray[25] != gs_Setup.SumCheckCodeSwitch)
            {
                gs_Setup.SumCheckCodeSwitch = gu8p_RxdArray[25];
                FRAM_WriteDW(FMAddr_PowerOnZeroSwitch, gs_Setup.SumCheckCodeSwitch);
            }
            //累计清除密码开关
            if (gu8p_RxdArray[26] != gs_Setup.SumClrCodeSwitch)
            {
                gs_Setup.SumClrCodeSwitch = gu8p_RxdArray[26];
                FRAM_WriteDW(FMAddr_SumClrCodeSwitch, gs_Setup.SumClrCodeSwitch);
            }
            //累计范围扩展开关
            if (gu8p_RxdArray[27] != gs_Setup.SumExtendSwitch)
            {
                gs_Setup.SumExtendSwitch = gu8p_RxdArray[27];
                FRAM_WriteDW(FMAddr_SumExtendSwitch, gs_Setup.SumExtendSwitch);
            }
            //双无斗秤同时松袋：
            if (gu8p_RxdArray[28] != gs_Setup.No2PackOffTwoSwitch)
            {
                gs_Setup.No2PackOffTwoSwitch = gu8p_RxdArray[28];
                FRAM_WriteDW(FMAddr_No2PackOffTwoSwitch, gs_Setup.No2PackOffTwoSwitch);
            }
            //输送机启动延时时间：2Byte
            UL_Value.uc[1] = gu8p_RxdArray[29];
            UL_Value.uc[0] = gu8p_RxdArray[30];
            UL_Value.uc[2] = 0;
            UL_Value.uc[3] = 0;
            if (UL_Value.ul != gs_Setup.No2ConveyorDelayTime)
            {
                gs_Setup.No2ConveyorDelayTime = UL_Value.ul;
                FRAM_WriteDW(FMAddr_No2ConveyorDelayTime, gs_Setup.No2ConveyorDelayTime);
            }
            //输送机运行时间：2Byte
            UL_Value.uc[1] = gu8p_RxdArray[31];
            UL_Value.uc[0] = gu8p_RxdArray[32];
            UL_Value.uc[2] = 0;
            UL_Value.uc[3] = 0;
            if (UL_Value.ul != gs_Setup.No2ConveyorRunTime)
            {
                gs_Setup.No2ConveyorRunTime = UL_Value.ul;
                FRAM_WriteDW(FMAddr_No2ConveyorRunTime, gs_Setup.No2ConveyorRunTime);
            }
            //再次夹袋延时启动加料时间：2Byte
            UL_Value.uc[1] = gu8p_RxdArray[33];
            UL_Value.uc[0] = gu8p_RxdArray[34];
            UL_Value.uc[2] = 0;
            UL_Value.uc[3] = 0;
            if (UL_Value.ul != gs_Setup.No2AddAgainDelayTime)
            {
                gs_Setup.No2AddAgainDelayTime = UL_Value.ul;
                FRAM_WriteDW(FMAddr_No2AddAgainDelayTime, gs_Setup.No2AddAgainDelayTime);
            }
        }
        else    //A报文处理 主站输出到仪表
        {   /* 2AO：输入数据 4字节.高位数据在前,低位数据在后.*/
            for (i = 0; i < 4; i++)
            {   //获取2AO的值
                UL_Value.uc[3-i] = gu8p_RxdArray[1+i];
            }
            // 32DO: 
            /* Byte0:输入值类型，决定2AO的输入数据类型*/
            gu8_WriteErrID = 0;
            switch (gu8p_RxdArray[5])
            {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul == 0)    //写入0，清除对应的配方累计次数和重量
                    {
                        ClrSum_Rec(gu8p_RxdArray[5]);
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 41: //批次数
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 9999)
                    {
                        if (UL_Value.ul != gu32_BatchTime)
                        {
                            gu32_BatchTime = UL_Value.ul;
                            gu32_BatchTimeCnt = gu32_BatchTime;
                            FRAM_WriteDW(FMAddr_BatchTime, gu32_BatchTime);
                            FRAM_WriteDW(FMAddr_BatchTimeCnt, gu32_BatchTimeCnt);
                        }
                        gu8_WriteErrID = 0;
                    }   
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 42://配方号
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 20 && UL_Value.ul >= 1)
                    {
                        if (UL_Value.ul != gu32_RecNum)
                        {
                            gu32_RecNum = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_RecNum, gu32_RecNum);
                            Read_RecipeData(gu32_RecNum);
                        }
                        gu8_WriteErrID = 0;
                    }   
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 80://F1.1
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Target)
                        {
                            if (gb_RunFlg == false)     //停止状态下立即修改配方
                            {
                                gs_Rec.Target = UL_Value.ul;   
                                FRAM_WriteDW(FMAddr_Target(gu32_RecNum), gs_Rec.Target);
                            }
                            else        //运行时修改
                            {
                                gu32_FastSetTemp[0] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x01;
                            }
                        }
                        gu8_WriteErrID = 0;
                    }       
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 81:        //大投提前量
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Large)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Large = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Large(gu32_RecNum), gs_Rec.Large);
                            }
                            else        //运行时修改
                            {
                                gu32_FastSetTemp[1] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x02;
                            }
                        }
                        gu8_WriteErrID = 0;
                    }   
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 82:        //中投提前量
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Middle)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Middle = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Middle(gu32_RecNum), gs_Rec.Middle);
                            }
                            else        //运行时修改
                            {
                                gu32_FastSetTemp[2] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x04;
                            }
                        }
                        gu8_WriteErrID = 0;
                    }       
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 83:        //落差值
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Small)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Small = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Small(gu32_RecNum), gs_Rec.Small);
                            }
                            else        //运行时修改
                            {
                                gu32_FastSetTemp[3] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x08;
                            }
                        }
                        gu8_WriteErrID = 0;
                    }       
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 84:        //近零值
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.NearZero)
                        {
                            gs_Rec.NearZero = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_NearZero(gu32_RecNum), gs_Rec.NearZero);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 85:        //拍袋起始重量
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagWt)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.FlapBagWt = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_FlapBagWt(gu32_RecNum), gs_Rec.FlapBagWt);
                            }
                            else        //运行时修改
                            {
                                gu32_FastSetTemp[5] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x20;
                            }
                        }
                        gu8_WriteErrID = 0;
                    }       
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 86:        //F2
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 1)
                    {
                        if (UL_Value.ul != gs_Rec.OUSwitch)
                        {
                            gs_Rec.OUSwitch = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_OUSwitch(gu32_RecNum), gs_Rec.OUSwitch);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 87:        //F2.1
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.OverValue)
                        {
                            gs_Rec.OverValue = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_OverValue(gu32_RecNum), gs_Rec.OverValue);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 88:        //F2.2
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.UnderValue)
                        {
                            gs_Rec.UnderValue = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_UnderValue(gu32_RecNum), gs_Rec.UnderValue);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 89:        //F2.3
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 1)
                    {
                        if (UL_Value.ul != gs_Rec.OUStopSwitch)
                        {
                            gs_Rec.OUStopSwitch = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_OUStopSwitch(gu32_RecNum), gs_Rec.OUStopSwitch);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 90:        //F3.1
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT1)
                        {
                            gs_Rec.TimeT1 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT1(gu32_RecNum), gs_Rec.TimeT1);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 91:        //F3.2
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT2b)
                        {
                            gs_Rec.TimeT2b = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT2b(gu32_RecNum), gs_Rec.TimeT2b);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 92:        //F3.3
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT3)
                        {
                            gs_Rec.TimeT3 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT3(gu32_RecNum), gs_Rec.TimeT3);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 93:        //F3.4
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT4)
                        {
                            gs_Rec.TimeT4 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT4(gu32_RecNum), gs_Rec.TimeT4);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 94:        //F3.5
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT5)
                        {
                            gs_Rec.TimeT5 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT5(gu32_RecNum), gs_Rec.TimeT5);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 95:        //F3.6
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT6)
                        {
                            gs_Rec.TimeT6 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT6(gu32_RecNum), gs_Rec.TimeT6);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 96:        //F3.7
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT7)
                        {
                            gs_Rec.TimeT7 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT7(gu32_RecNum), gs_Rec.TimeT7);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 97:        //F3.8
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT2a)
                        {
                            gs_Rec.TimeT2a = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT2a(gu32_RecNum), gs_Rec.TimeT2a);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 98:        //F3.9
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT9)
                        {
                            gs_Rec.TimeT9 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT9(gu32_RecNum), gs_Rec.TimeT9);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 99:        //F3.10
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT10)
                        {
                            gs_Rec.TimeT10 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT10(gu32_RecNum), gs_Rec.TimeT10);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 100:        //F3.11
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT11)
                        {
                            gs_Rec.TimeT11 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT11(gu32_RecNum), gs_Rec.TimeT11);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 101:        //F3.12
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT12)
                        {
                            gs_Rec.TimeT12 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT12(gu32_RecNum), gs_Rec.TimeT12);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 102:        //F4
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 1)
                    {
                        if (UL_Value.ul != gs_Rec.CorrectFallSwitch)
                        {
                            gs_Rec.CorrectFallSwitch = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_CorrectFallSwitch(gu32_RecNum), gs_Rec.CorrectFallSwitch);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 103:        //F4.1
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 99)
                    {
                        if (UL_Value.ul != gs_Rec.CorrectFallTime)
                        {
                            gs_Rec.CorrectFallTime = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_CorrectFallTime(gu32_RecNum), gs_Rec.CorrectFallTime);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 104:        //F4.2
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 99)
                    {
                        if (UL_Value.ul != gs_Rec.CorrectFallRng1)
                        {
                            gs_Rec.CorrectFallRng1 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_CorrectFallRng1(gu32_RecNum), gs_Rec.CorrectFallRng1);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 105:        //F4.3
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul <= 3 && UL_Value.ul >= 1)
                    {
                        if (UL_Value.ul != gs_Rec.CorrectFallRng2)
                        {
                            gs_Rec.CorrectFallRng2 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_CorrectFallRng2(gu32_RecNum), gs_Rec.CorrectFallRng2);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 106:        //F5
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 3)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagSwitch)
                        {
                            gs_Rec.FlapBagSwitch = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagSwitch(gu32_RecNum), gs_Rec.FlapBagSwitch);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 107:        //F5.1
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 99)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagAddNum)
                        {
                            gs_Rec.FlapBagAddNum = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagAddNum(gu32_RecNum), gs_Rec.FlapBagAddNum);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 108:        //F5.2
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 99)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagNum)
                        {
                            gs_Rec.FlapBagNum = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagNum(gu32_RecNum), gs_Rec.FlapBagNum);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 109:        //F5.3
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagDelay)
                        {
                            gs_Rec.FlapBagDelay = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagDelay(gu32_RecNum), gs_Rec.FlapBagDelay);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 110:        //F5.4
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagTimeHi)
                        {
                            gs_Rec.FlapBagTimeHi = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagTimeHi(gu32_RecNum), gs_Rec.FlapBagTimeHi);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 111:        //F5.5
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagTimeLow)
                        {
                            gs_Rec.FlapBagTimeLow = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagTimeLow(gu32_RecNum), gs_Rec.FlapBagTimeLow);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 112:        //F5.6
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagExtTime)
                        {
                            gs_Rec.FlapBagExtTime = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagExtTime(gu32_RecNum), gs_Rec.FlapBagExtTime);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 113:        //F6
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 1)
                    {
                        if (UL_Value.ul != gs_Rec.SmallPluseSwitch)
                        {
                            gs_Rec.SmallPluseSwitch = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_SmallPluseSwitch(gu32_RecNum), gs_Rec.SmallPluseSwitch);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 114:        //F6.1
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.SmallPluseHi)
                        {
                            gs_Rec.SmallPluseHi = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_SmallPluseHi(gu32_RecNum), gs_Rec.SmallPluseHi);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 115:        //F6.2
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.SmallPluseLow)
                        {
                            gs_Rec.SmallPluseLow = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_SmallPluseLow(gu32_RecNum), gs_Rec.SmallPluseLow);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 116:        //F7
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 99)
                    {
                        if (UL_Value.ul != gs_Rec.CombinedNum)
                        {
                            gs_Rec.CombinedNum = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_CombinedNum(gu32_RecNum), gs_Rec.CombinedNum);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 117:        //F8
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 1)
                    {
                        if (UL_Value.ul != gs_Rec.DiscShakeSwitch)
                        {
                            gs_Rec.DiscShakeSwitch = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_DiscShakeSwitch(gu32_RecNum), gs_Rec.DiscShakeSwitch);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 118:        //F8.1
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.DiscEffTime)
                        {
                            gs_Rec.DiscEffTime = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_DiscEffTime(gu32_RecNum), gs_Rec.DiscEffTime);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 119:        //F8.2
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.DiscShakeTimeHi)
                        {
                            gs_Rec.DiscShakeTimeHi = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_DiscShakeTimeHi(gu32_RecNum), gs_Rec.DiscShakeTimeHi);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 120:        //F8.3
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.DiscShakeTimeLow)
                        {
                            gs_Rec.DiscShakeTimeLow = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_DiscShakeTimeLow(gu32_RecNum), gs_Rec.DiscShakeTimeLow);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 121:        //F8.4
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 99)
                    {
                        if (UL_Value.ul != gs_Rec.DiscShakeNum)
                        {
                            gs_Rec.DiscShakeNum = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_DiscShakeNum(gu32_RecNum), gs_Rec.DiscShakeNum);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 122:        //F9
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 1)
                    {
                        if (UL_Value.ul != gs_Rec.PrintCodeSwitch)
                        {
                            gs_Rec.PrintCodeSwitch = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_PrintCodeSwitch(gu32_RecNum), gs_Rec.PrintCodeSwitch);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 123:        //F9.1
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.PrintCodeRunDelay)
                        {
                            gs_Rec.PrintCodeRunDelay = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_PrintCodeRunDelay(gu32_RecNum), gs_Rec.PrintCodeRunDelay);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 124:        //F9.2
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.PrintCodeTime)
                        {
                            gs_Rec.PrintCodeTime = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_PrintCodeTime(gu32_RecNum), gs_Rec.PrintCodeTime);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 125:        //F9.3
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 1)
                    {
                        if (UL_Value.ul != gs_Rec.SheildDiscSwitch)
                        {
                            gs_Rec.SheildDiscSwitch = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_SheildDiscSwitch(gu32_RecNum), gs_Rec.SheildDiscSwitch);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                    
                case 150:        //打印总累计
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  == 1)
                    {
                        PrintStart(PRINT_TOTAL_REPORT, 0, 0);
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 151:        //配方设置表打印
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 21)
                    {
                        if (UL_Value.ul == 0)
                        {   //打印当前配方设置表
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = false;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = false;
                            PrintStart(PRINT_REC_SET, gu32_RecNum, 0);
                        }
                        else if (UL_Value.ul >= 1 && UL_Value.ul <= 20)
                        {   //打印对应配方设置表
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = false;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = false;
                            PrintStart(PRINT_REC_SET, UL_Value.ul, 0);
                        }
                        else
                        {   //打印所有配方设置表
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = true;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = true;
                            gu8_Item = PRINT_REC_SET;
                            //打印所有目标值不为0的配方
                            if (gs_Rec.Target != 0)
                            {
                                gu32_PrintRecNumStart = gu32_RecNum;    //保存起始配方号，用来判断是否有所配方都打印完成
                                PrintStart(PRINT_REC_SET, gu32_RecNum, 0);  //从当前配方开始打印
                            }
                            else
                            {   //当前配方目标值为0，获取下一个目标值不为0的配方号
                                gu32_PrintRecNumStart = NextRecNum(gu32_RecNum);
                                if (gu32_PrintRecNumStart != gu32_RecNum)
                                {   //如果获取到的下一个不为0的配方号和目前配方号相等，代表所有配方目标值都为0，不打印
                                    PrintStart(PRINT_REC_SET, gu32_PrintRecNumStart, 0);  //从目标值不为0的配方号开始打印
                                }
                            }
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 152:        //配方累计表打印
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 21)
                    {
                        if (UL_Value.ul == 0)
                        {   //打印当前配方累计报表
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = false;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = false;
                            PrintStart(PRINT_REC_REPORT, gu32_RecNum, 0);
                        }
                        else if (UL_Value.ul >= 1 && UL_Value.ul <= 20)
                        {   //打印对应配方累计报表
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = false;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = false;
                            PrintStart(PRINT_REC_REPORT, UL_Value.ul, 0);
                        }
                        else
                        {   //打印所有配方累计报表
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = true;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = true;
                            gu8_Item = PRINT_REC_REPORT;
                            //打印所有目标值不为0的配方
                            if (gs_Rec.Target != 0)
                            {
                                gu32_PrintRecNumStart = gu32_RecNum;    //保存起始配方号，用来判断是否有所配方都打印完成
                                PrintStart(PRINT_REC_REPORT, gu32_RecNum, 0);  //从当前配方开始打印
                            }
                            else
                            {   //当前配方目标值为0，获取下一个目标值不为0的配方号
                                gu32_PrintRecNumStart = NextRecNum(gu32_RecNum);
                                if (gu32_PrintRecNumStart != gu32_RecNum)
                                {   //如果获取到的下一个不为0的配方号和目前配方号相等，代表所有配方目标值都为0，不打印
                                    PrintStart(PRINT_REC_REPORT, gu32_PrintRecNumStart, 0);  //从目标值不为0的配方号开始打印
                                }
                            }
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                default:
                    gu8_WriteErrID = 3;
            }
            //Byte1:请求数据类型，决定10AI中的“请求数据”传送的数据类型
            gu8_PbusSendType = gu8p_RxdArray[6];
            //Byte2:执行操作1
            for (i = 0; i <= 7; i++ )
            {
                if (((gu8p_RxdArray[7] & (0x01 << i)) != 0) && ((gu8_16DoTempHi & (0x01 << i)) == 0))
                {   //旧缓存状态为0，新接收到的状态为1时才响应相关操作，也就是检测由0到1跳变沿
                    switch(i)
                    {
                        case 0://停止
                            Clr_Alarm();
                            if (gb_RunFlg)
                                gb_EndFlg = true;
                            break;
                        case 1://运行
                            RunStopInit(true);
                            break;
                        case 2://急停
                            Clr_Alarm();
                            RunStopInit(false);
                            break;
                        case 3://清零
                            if (gb_RunFlg == false)
                                gb_ZeroKeyFlg = true;
                            break;
                        case 4://清报警
                            Clr_Alarm();
                            break;
                        case 5://选配方
                            if (gb_RunFlg == false)
                            {
                                gu32_RecNum = NextRecNum(gu32_RecNum);
                                FRAM_WriteDW(FMAddr_RecNum, gu32_RecNum);
                                Read_RecipeData(gu32_RecNum);
                            }
                            break;
                        case 6://夹松袋
                            PackInput();
                            break;
                        case 7://手动卸料
                            if (gb_RunFlg == false)
                                ManualDisc(2);
                            break;
                        default:
                            break;
                    }
                }
                if (((gu8p_RxdArray[8] & (0x01 << i)) != 0) && ((gu8_16DoTempLow & (0x01 << i)) == 0))
                {   //旧缓存状态为0，新接收到的状态为1时才响应相关操作，也就是检测由0到1跳变沿
                    switch(i)
                    {
                        case 0://手动大投
                            if (gb_RunFlg == false)
                                ManualSp123(2);
                            break;
                        case 1://手动加料一次
                            if (gb_RunFlg == false)
                            {
                                gb_GoFlg = true;
                                RunStopInit(true);
                            }
                            break;
                        case 2://清除总累计数据（包括每个配方的累计数据）
                            if (gb_RunFlg == false)
                                ClrSum();
                            break;
                        case 3://手动小投
                            if (gb_RunFlg == false)
                                ManualSp3(2);
                            break;
                        case 7://掉线检测有效
                            gb_ConnectCheckFlg = true;
                            break;
                        default:
                            break;
                    }
                }
                
                if (i == 7)
                {
                    if ((gu8p_RxdArray[8] & (0x01 << i)) == 0)
                    {   //掉线检测无效
                        gb_ConnectCheckFlg = false;
                    }
                    else
                    {   //掉线检测有效
                        gb_ConnectCheckFlg = true;
                    }
                }
            }
            gu8_16DoTempHi = gu8p_RxdArray[7];
            gu8_16DoTempLow = gu8p_RxdArray[8];  //保存16DO本次的状态
        }
    }
    else    //GSD2处理
    {
        if (fu8p_Array[0] & 0x80)  //B报文处理 用户参数
        {
            if (gu32_FollowMasterSwitch == 0)   //工作参数标定参数不受主站控制
                return;
            //分度值
            if (gs_Cal.Div != cu8_DivTab[gu8p_RxdArray[1]])
            {
                lu32_Temp = FRAM_ReadDW(FMAddr_CalDiv);
                //需要调整分度,和追零零点
                gs_Cal.ADK *= (float)cu8_DivTab[gu8p_RxdArray[1]];    //新分度值=原分度值*new dd/old dd
                gs_Cal.ADK /= (float)gs_Cal.Div;
                FRAM_WriteFloat(FMAddr_CalADK, gs_Cal.ADK);
            
                //调整追零零点:调整分度值后,如果是0,仍然显示零
                gu32_FloatZero *= gs_Cal.Div;
                gu32_FloatZero /= cu8_DivTab[gu8p_RxdArray[1]];
                //保存新的分度值
                gs_Cal.Div = cu8_DivTab[gu8p_RxdArray[1]];
                FRAM_WriteDW(FMAddr_CalDiv, gs_Cal.Div);

                //分度值改小后的最大量程处理
                if (gs_Cal.Div < lu32_Temp)                        //分度值由大变小时最大量程需要调整
                {
                    if (gs_Cal.Cap > (gs_Cal.Div * 100000))
                    {
                        gs_Cal.Cap = gs_Cal.Div * 100000;
                        FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                    }
                }
                else if (gs_Cal.Div > gs_Cal.Cap)            //修改分度后量程小于分度，则修改量程为分度值
                {
                    gs_Cal.Cap = gs_Cal.Div;
                    FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                }
            }
            //单位
            if (gs_Cal.Unit != gu8p_RxdArray[2])
            {
                gs_Cal.Unit = gu8p_RxdArray[2];
                FRAM_WriteDW(FMAddr_CalUnit, gs_Cal.Unit);
                ClrSum();
            }
            //小数点
            if (gs_Cal.Point != gu8p_RxdArray[3])
            {
                gs_Cal.Point = gu8p_RxdArray[3];
                FRAM_WriteDW(FMAddr_CalPoint, gs_Cal.Point);
                ClrSum();
            }
            //最大分度数
            UL_Value.uc[1] = gu8p_RxdArray[4];
            UL_Value.uc[0] = gu8p_RxdArray[5];
            UL_Value.ul *= gs_Cal.Div;  //得到最大量程
            if (gs_Cal.Cap != UL_Value.ul)
            {
                gs_Cal.Cap = UL_Value.ul;
                FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
            }
            //零点跟踪范围
            if (gu8p_RxdArray[6] != gs_Setup.TraceZeroRng)
            {
                gs_Setup.TraceZeroRng = gu8p_RxdArray[6];
                FRAM_WriteDW(FMAddr_TraceZeroRng, gs_Setup.TraceZeroRng);
            }
            //判稳范围
            if (gu8p_RxdArray[7] != gs_Setup.JudgeStabRng)
            {
                gs_Setup.JudgeStabRng = gu8p_RxdArray[7];
                FRAM_WriteDW(FMAddr_JudgeStabRng, gs_Setup.JudgeStabRng);
            }
            //清零范围
            if (gu8p_RxdArray[8] != gs_Setup.ClrZeroRng)
            {
                gs_Setup.ClrZeroRng = gu8p_RxdArray[8];
                FRAM_WriteDW(FMAddr_ClrZeroRng, gs_Setup.ClrZeroRng);
            }
            //上电自动清零
            if (gu8p_RxdArray[9] != gs_Setup.PowerOnZeroSwitch)
            {
                gs_Setup.PowerOnZeroSwitch = gu8p_RxdArray[9];
                FRAM_WriteDW(FMAddr_PowerOnZeroSwitch, gs_Setup.PowerOnZeroSwitch);
            }
            //自动清零间隔
            if (gu8p_RxdArray[10] != gs_Setup.AutoClrZeroInterval)
            {
                gs_Setup.AutoClrZeroInterval = gu8p_RxdArray[10];
                FRAM_WriteDW(FMAddr_AutoClrZeroInterval, gs_Setup.AutoClrZeroInterval);
            }
            //滤波参数
            if (gu8p_RxdArray[11] != gs_Setup.AdFilterLevel)
            {
                gs_Setup.AdFilterLevel = gu8p_RxdArray[11];
                FRAM_WriteDW(FMAddr_AdFilterLevel, gs_Setup.AdFilterLevel);
            }
            //秤体模式  注意此处的协议说明和仪表内部变量是相反的，仪表0有斗1无斗
            if ((!gu8p_RxdArray[12]) != gs_Setup.TareMode)
            {
                gs_Setup.TareMode = (!gu8p_RxdArray[12]);
                FRAM_WriteDW(FMAddr_TareMode, gs_Setup.TareMode);
                TareModeChange(gs_Setup.TareMode);
            }
        }
        else    //A报文处理 主站输出到仪表
        {/* 16DO：
            DO15~DO11: 保留.
            DO9 : 当该位由0置为1时,停止状态下,取反手动放料,则返回否定信息.
            DO8 : 当该位由0置为1时,停止状态下,清除累计值,否则不响应.
            DO7 : 当该位由0置为1时,停止状态下,取反手动小投,否则返回否定信息
            DO6 : 当该位由0置为1时,松袋
            DO5 : 当该位由0置为1时, 夹袋
            DO4 : 当该位由0置为1时,停止状态下,跳转到下一配方.
            DO3 : 当该位由0置为1时,清除报警
            DO2 : 当该位由0置为1时,停止状态下,清零.
            DO1 : 当该位由0置为1时,停止.
            DO0 : 当该位由0置为1时,启动.*/
            for (i = 0; i < 8; i++)
            {   //D0~D7的处理
                if (((gu8p_RxdArray[2] & (0x01 << i)) != 0) && ((gu8_16DoTempLow & (0x01 << i)) == 0))
                {   //旧缓存状态为0，新接收到的状态为1时才响应相关操作，也就是检测由0到1跳变沿
                    switch (i)
                    {
                        case 0: //启动
                            RunStopInit(true);
                            break;
                        case 1: //急停
                            Clr_Alarm();
                            RunStopInit(false);
                            break;
                        case 2: //清零
                            if (gb_RunFlg == false)
                                gb_ZeroKeyFlg = true;
                            break;
                        case 3: //清报警
                            Clr_Alarm();
                            break;
                        case 4: //切换配方
                            if (gb_RunFlg == false)
                            {
                                //选配方输入有效调到下目标值不为0的配方
                                gu32_RecNum = NextRecNum(gu32_RecNum);
                                FRAM_WriteDW(FMAddr_RecNum, gu32_RecNum);
                                Read_RecipeData(gu32_RecNum);
                            }
                            break;
                        case 5: //夹袋
                            Pack_JD();
                            break;
                        case 6: //松袋
                            Pack_SD();
                            break;
                        case 7: //手动小投
                            if (gb_RunFlg == false)
                                ManualSp3(2);
                            break;
                        default:
                            break;
                    }
                }
                //D8~D9的处理
                if (((gu8p_RxdArray[1] & (0x01 << i)) != 0) && ((gu8_16DoTempHi & (0x01 << i)) == 0))
                {   //旧缓存状态为0，新接收到的状态为1时才响应相关操作，也就是检测由0到1跳变沿
                    switch (i)
                    {
                        case 0: //清累计
                            if (gb_RunFlg == false)
                                ClrSum();
                            break;
                        case 1: //手动卸料
                            if (gb_RunFlg == false)
                                ManualDisc(2);
                            break;
                        default:
                            break;
                    }
                }
            }
            gu8_16DoTempHi = gu8p_RxdArray[1];
            gu8_16DoTempLow = gu8p_RxdArray[2];  //保存16DO本次的状态
            /*
            1Byte Out:
            D7~D2:2AO写入数据类型
            00000:(0)   保留,不会将2AO中数据写入仪表
            00001:(1)   目标(重量控制参数均应小于最大量程).
            00010:(2)   大投提前量.
            00011:(3)   中投提前量.
            00100:(4)   落差值.
            00101:(5)   零区值.
            00110:(6)   t7 
            00111:(7)   超欠差开关(写入奇数打开,偶数关闭,以下开关类型均类似)
            01000:(8)   超欠差暂停开关
            01001:(9)   超差值.
            01010:(10)  欠差值.
            01011:(11)  t1(时间参数均应在00~99)
            01100:(12)  t2
            01101:(13)  t3
            01110:(14)  t4
            01111:(15)  t5
            10000:(16)  t6
            10001:(17)  自动落差修正开关
            10010:(18)  落差修正次数(00~99)
            10011:(19)  落差修正范围(00~99)
            10100:(20)  落差修正幅度(1~3)
            010101:(21) 拍袋开关
            010110:(22) 拍袋延时
            010111:(23) 拍袋持续时间
            011000:(24) 拍袋次数
            011001:(25) 拍袋有效电平时间
            011010:(26) 拍袋无效电平时间
            011011:(27) 第一次拍袋有效电平时间
            011100:(28) 第一次拍袋无效电平时间
            011101:(29) 点动小投
            011110:(30) 点动小投有效电平时间
            011111:(31) 点动小投无效电平时间
            100000:(32) 连续放料次数
            其 余:     保留,不会将2AO中数据写入仪表
            
            D1~D0:要求2AI传送的数据类型
             00: (0) 当前重量.
             01: (1) 累计重量
             10: (2) 累计次数
             11: (3) 保留*/
            gu8_PbusSendType = (gu8p_RxdArray[3] & 0x03);
            gu8_PbusWriteType = (gu8p_RxdArray[3] >> 2);
            //2AO: 需要写入的数据, 4字节.高位数据在前,低位数据在后.
            for (i = 0; i < 4; i++)
            {   //获取2AO的值
                UL_Value.uc[3-i] = gu8p_RxdArray[4+i];
            }
            switch (gu8_PbusWriteType)
            {   //根据写入类型变量修改对应的参数值
                case 0:
                    break;
                case 1:        //目标值
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Target)
                        {
                            if (gb_RunFlg == false)     //停止状态下立即修改配方
                            {
                                gs_Rec.Target = UL_Value.ul;   
                                FRAM_WriteDW(FMAddr_Target(gu32_RecNum), gs_Rec.Target);
                            }
                            else        //运行时修改
                            {
                                gu32_FastSetTemp[0] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x01;
                            }
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 2:        //大投提前量
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Large)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Large = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Large(gu32_RecNum), gs_Rec.Large);
                            }
                            else        //运行时修改
                            {
                                gu32_FastSetTemp[1] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x02;
                            }
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 3:        //中投提前量
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Middle)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Middle = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Middle(gu32_RecNum), gs_Rec.Middle);
                            }
                            else        //运行时修改
                            {
                                gu32_FastSetTemp[2] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x04;
                            }
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 4:        //落差值
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Small)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Small = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Small(gu32_RecNum), gs_Rec.Small);
                            }
                            else        //运行时修改
                            {
                                gu32_FastSetTemp[3] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x08;
                            }
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 5:        //近零值
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.NearZero)
                        {
                            gs_Rec.NearZero = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_NearZero(gu32_RecNum), gs_Rec.NearZero);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 6: //t7
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT7)
                        {
                            gs_Rec.TimeT7 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT7(gu32_RecNum), gs_Rec.TimeT7);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 7:        //超欠差报警开关
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if ((UL_Value.ul & 0x01) <= 1)
                    {
                        if ((UL_Value.ul & 0x01) != gs_Rec.OUSwitch)
                        {
                            gs_Rec.OUSwitch = (UL_Value.ul & 0x01);
                            FRAM_WriteDW(FMAddr_OUSwitch(gu32_RecNum), gs_Rec.OUSwitch);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 8:        //超欠差暂停开关
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if ((UL_Value.ul & 0x01) <= 1)
                    {
                        if ((UL_Value.ul & 0x01) != gs_Rec.OUStopSwitch)
                        {
                            gs_Rec.OUStopSwitch = (UL_Value.ul & 0x01);
                            FRAM_WriteDW(FMAddr_OUStopSwitch(gu32_RecNum), gs_Rec.OUStopSwitch);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 9:        //超量值
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.OverValue)
                        {
                            gs_Rec.OverValue = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_OverValue(gu32_RecNum), gs_Rec.OverValue);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 10:        //欠量值
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.UnderValue)
                        {
                            gs_Rec.UnderValue = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_UnderValue(gu32_RecNum), gs_Rec.UnderValue);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 11:        //T1
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT1)
                        {
                            gs_Rec.TimeT1 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT1(gu32_RecNum), gs_Rec.TimeT1);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 12:        //T2b
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT2b)
                        {
                            gs_Rec.TimeT2b = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT2b(gu32_RecNum), gs_Rec.TimeT2b);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 13:        //T3
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT3)
                        {
                            gs_Rec.TimeT3 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT3(gu32_RecNum), gs_Rec.TimeT3);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 14:        //T4
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT4)
                        {
                            gs_Rec.TimeT4 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT4(gu32_RecNum), gs_Rec.TimeT4);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 15:        //T5
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT5)
                        {
                            gs_Rec.TimeT5 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT5(gu32_RecNum), gs_Rec.TimeT5);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 16:        //T6
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.TimeT6)
                        {
                            gs_Rec.TimeT6 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_TimeT6(gu32_RecNum), gs_Rec.TimeT6);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 17:        //落差修正开关
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if ((UL_Value.ul & 0x01) <= 1)
                    {
                        if ((UL_Value.ul & 0x01) != gs_Rec.CorrectFallSwitch)
                        {
                            gs_Rec.CorrectFallSwitch = (UL_Value.ul & 0x01);
                            FRAM_WriteDW(FMAddr_CorrectFallSwitch(gu32_RecNum), gs_Rec.CorrectFallSwitch);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 18:        //落差修正次数
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 99)
                    {
                        if (UL_Value.ul != gs_Rec.CorrectFallTime)
                        {
                            gs_Rec.CorrectFallTime = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_CorrectFallTime(gu32_RecNum), gs_Rec.CorrectFallTime);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 19:        //落差修正范围
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 99)
                    {
                        if (UL_Value.ul != gs_Rec.CorrectFallRng1)
                        {
                            gs_Rec.CorrectFallRng1 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_CorrectFallRng1(gu32_RecNum), gs_Rec.CorrectFallRng1);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 20:        //落差修正幅度
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if ((UL_Value.ul >= 1) && (UL_Value.ul <= 3))
                    {
                        if (UL_Value.ul != gs_Rec.CorrectFallRng2)
                        {
                            gs_Rec.CorrectFallRng2 = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_CorrectFallRng2(gu32_RecNum), gs_Rec.CorrectFallRng2);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 21:        //拍袋开关
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 3)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagSwitch)
                        {
                            gs_Rec.FlapBagSwitch = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagSwitch(gu32_RecNum), gs_Rec.FlapBagSwitch);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 22:        //拍袋延时
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagDelay)
                        {
                            gs_Rec.FlapBagDelay = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagDelay(gu32_RecNum), gs_Rec.FlapBagDelay);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 24:        //定值后拍袋次数
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 99)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagNum)
                        {
                            gs_Rec.FlapBagNum = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagNum(gu32_RecNum), gs_Rec.FlapBagNum);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 25:        //拍袋有效电平时间
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagTimeHi)
                        {
                            gs_Rec.FlapBagTimeHi = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagTimeHi(gu32_RecNum), gs_Rec.FlapBagTimeHi);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 26:        //拍袋无效电平时间
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagTimeLow)
                        {
                            gs_Rec.FlapBagTimeLow = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_FlapBagTimeLow(gu32_RecNum), gs_Rec.FlapBagTimeLow);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 29:        //小投点动开关
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if ((UL_Value.ul & 0x01) <= 1)
                    {
                        if ((UL_Value.ul & 0x01) != gs_Rec.SmallPluseSwitch)
                        {
                            gs_Rec.SmallPluseSwitch = (UL_Value.ul & 0x01);
                            FRAM_WriteDW(FMAddr_SmallPluseSwitch(gu32_RecNum), gs_Rec.SmallPluseSwitch);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 30:        //小投点动有效时间
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.SmallPluseHi)
                        {
                            gs_Rec.SmallPluseHi = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_SmallPluseHi(gu32_RecNum), gs_Rec.SmallPluseHi);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 31:        //小投点动无效时间
                    if (gb_RunFlg == true)
                        gu8_WriteErrID = 1;
                    else if (UL_Value.ul <= 999)
                    {
                        if (UL_Value.ul != gs_Rec.SmallPluseLow)
                        {
                            gs_Rec.SmallPluseLow = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_SmallPluseLow(gu32_RecNum), gs_Rec.SmallPluseLow);
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 32:        //F7
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 99)
                    {
                        if (UL_Value.ul != gs_Rec.CombinedNum)
                        {
                            gs_Rec.CombinedNum = UL_Value.ul;
                            FRAM_WriteDW(FMAddr_CombinedNum(gu32_RecNum), gs_Rec.CombinedNum);
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                default:
                    break;
            }
        }
    }
}

/*********************************************************************
函数名称: void Pbus_Recive(void)
功    能: 仪表PROFIBUS接收数据的处理 
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-5-5
修    改:                      时    间: 
*********************************************************************/
void Pbus_Recive(void)
{
    u8 lu8_ErrCode;
    
    if (SE_REQ_IT == false && SE_S_RTS == false && gb_PbusTxdingFlg == false && gb_PbusRxdReadyFlg == true)
    {   //DP总线已经连接上，SE接口板初始化成功，并且允许接收，串口没有发送并且已经接收状态 那么才可以处理接收数据
        gb_PbusRxdReadyFlg = false;
        lu8_ErrCode = Pbus_ReciveCheck(gu8p_RxdArray, gu32_GSDType, gu8_ReqCom);
        if (0 == lu8_ErrCode)
        {   //数据无错误，处理接收到的数据
            Pbus_ReciveDataDeal(gu8p_RxdArray, gu32_GSDType);
        }
    }
}

/*********************************************************************
函数名称: u8 Pbus_Checksum(u8 *fu8p_Array, u8 fu8_i)
功    能: 返回校验和数据
说    明: 
入口参数: fu8p_Array：校验数据首地址    fu8_i：参与校验的个数
返 回 值: 校验和结果
设    计: 丛海旭               时    间: 2014-04-24
修    改:                      时    间: 
*********************************************************************/
u8 Pbus_Checksum(u8 *fu8p_Array, u8 fu8_i)
{
    u8 i, j;
    
    for (i = 0, j = 0; i < fu8_i; i++)
        j += fu8p_Array[i];
    return j;
}

/*********************************************************************
函数名称: void Pbus_StartSend(u8 *fu8_Array, u8 fu8_TxdLen, u8 fu8_RxdLen)
功    能: 启动串口发送数据
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-04-24
修    改:                      时    间: 
*********************************************************************/
void Pbus_StartSend(u8 *fu8_Array, u8 fu8_TxdLen, u8 fu8_RxdLen)
{
    gu8_PbusTxdLen = fu8_TxdLen;    //发送数据个数
    gu8_PbusRxdLen = fu8_RxdLen;    //接收数据个数
    gu8_PbusTxdCnt = 0; //发送计数
    gu8_PbusRxdCnt = 0;
    fu8_Array[fu8_TxdLen-1] = Pbus_Checksum(fu8_Array, fu8_TxdLen - 1);   //校验和
    gb_PbusTxdingFlg = true;    //开始发送
    gu32_TimeTemp = SYS10MS;    //保存发送时刻
    UART_FifoReset(PBUS_UARTX, UART_TxFIFO);         //清空发送队列
    UART_ItConfig(PBUS_UARTX,UART_TxEmpty,ENABLE);   //允许串口发送中断
//    UART_ItConfig(PBUS_UARTX,UART_RxBufNotEmpty,DISABLE);//关闭接收中断
}

/*********************************************************************
函数名称: void Com_Pbus_DP(void)
功    能: PROFIBUS通讯主循环处理
说    明: 放在主循环中每次循环都执行
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-04-29
修    改:                      时    间: 
*********************************************************************/
void Com_Pbus_DP(void)
{
    Pbus_Init();    //初始化
    Pbus_Recive();  //接收数据处理
    Pbus_Send();    //发送数据处理
}



