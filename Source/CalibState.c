/*
Description:            �궨������غ���
Author:                 �Ժ���
Date:                   2013-12-9
*/
#include "include.h"


#define CAL_CHECKCODE   0
#define DISP_CALON      1
#define CAL_UNIT        2
#define CAL_POINT       3
#define CAL_DIV         4
#define CAL_CAP         5
#define CAL_SENSOR      6
#define CAL_MVOUT       7
#define CAL_ZERO_1      8
#define CAL_ZERO_2      9
#define CAL_LOAD_1      10
#define CAL_LOAD_2      11
#define CAL_LOAD_3      12
#define CAL_INPUTCODE   13
#define DISP_CALEND     14
#define CAL_END         15
//����У׼
#define CAL_CORRECT_1   16
#define CAL_CORRECT_2   17


const u32 cu32_MvMax[4] = {0, 17000, 27000, 37000};

//�궨��ز���
struct CALIB gs_Cal;
u32 gu32_CalCode;
u32 gu32_CalLoadAD;
u32 gu32_CalLoadWt;
u32 gu32_DataTemp;

u8 gu8_CalibInTime;
u32 gu32_TimeDelay;             //һ�����ʱ������
bool  gb_GainSig;               //�궨ʱ�ĸ��ű�־
u32 gu32_CalDispWt;     //����У׼������ʾ��������



/*********************************************************************
��������: u32 AdToMv(u32 ful_AdCode)
��    ��: ad��ת���ɺ�����
˵    ��: 
��ڲ���: ful_AdCode = ad��
�� �� ֵ: ad���Ӧ�ĺ�����
��    ��: �Ժ���               ʱ    ��: 2013-10-12
��    ��:                      ʱ    ��: 
*********************************************************************/
u32 AdToMv(u32 ful_AdCode)
{
    u64 lu64_MvData;
    
    if (gs_Cal.Sensor == 1)
    {
        lu64_MvData = (u64)ful_AdCode * SEN1mVMAX + (ADMAX/2);
        lu64_MvData /= ADMAX;
    }
    else if (gs_Cal.Sensor == 3)
    {
        lu64_MvData = (u64)ful_AdCode * SEN3mVMAX + (ADMAX/2);
        lu64_MvData /= ADMAX;
    }
    else
    {
        lu64_MvData = (u64)ful_AdCode * SEN2mVMAX + (ADMAX/2);
        lu64_MvData /= ADMAX;
    }
    
    return (u32)lu64_MvData;
}

/*********************************************************************
��������: u32 MvToAd(u32 ful_MvData)
��    ��: ������ת����ad��
˵    ��: 
��ڲ���: ful_MvData = ������
�� �� ֵ: ��������Ӧ��ad��
��    ��: �Ժ���               ʱ    ��: 2011-3-17
��    ��:                      ʱ    ��: 
*********************************************************************/
u32 MvToAd(u32 ful_MvData)
{
    u64 lu64_AdCode;
    
    if (gs_Cal.Sensor == 1)
    {
        lu64_AdCode = (u64)ful_MvData * ADMAX + (SEN1mVMAX/2);
        lu64_AdCode /= SEN1mVMAX;
    }
    else if (gs_Cal.Sensor == 3)
    {
        lu64_AdCode = (u64)ful_MvData * ADMAX + (SEN3mVMAX/2);
        lu64_AdCode /= SEN3mVMAX;
    }
    else
    {
        lu64_AdCode = (u64)ful_MvData * ADMAX + (SEN2mVMAX/2);
        lu64_AdCode /= SEN2mVMAX;
    }
    
    return (u32)lu64_AdCode;
}

/*********************************************************************
��������: u8 CalDataInputCheck(void)
��    ��: ������������Լ��
˵    ��: 
��ڲ���: 
�� �� ֵ: ���ݴ�������
��    ��: �Ժ���               ʱ    ��: 2011-4-18
��    ��:                      ʱ    ��: 
*********************************************************************/
bool CalDataInputCheck(void)
{
    bool lb_err = false;
    
    switch(gu8_Item)
    {
        case CAL_CAP:               //������̲��ܴ�����С�ֶȵ�100000��
            if ((ulParameter.ul > gs_Cal.Div * 100000) || (ulParameter.ul < gs_Cal.Div))
                lb_err = true;        //error
            break;
        case CAL_ZERO_1:
//            if (gb_OflFlg == true)    //ԭ�ж��������� 
            if (gb_AdOflFlg == true)    //�޸���ʾOFL���ܱ궨�������� v1.01
                lb_err = true;
            break;
        case CAL_ZERO_2:
            if (ulParameter.ul > 12000 * gs_Cal.Sensor || ulParameter.ul == 0)
                lb_err = true;        //Over
            break;
        case CAL_LOAD_1:
            if (ulParameter.ul == 0 || 
                AdToMv(gu32_AdFedValue) > (gs_Cal.Sensor * 12000 + gs_Cal.Sensor * 600))       //�������������Ϊ0������������5%������gu32_CalSensor * 12000��*105%
                lb_err = true;
            break;
        case CAL_LOAD_2:
            if (ulParameter.ul == 0 || 
                ulParameter.ul > (gs_Cal.Sensor * 12000 + gs_Cal.Sensor * 600))
                lb_err = true;
            break;
        case CAL_LOAD_3:           //����궨����
        case CAL_CORRECT_1:
        case CAL_CORRECT_2:
            if (ulParameter.ul == 0)
                lb_err = true;
            break;
        default:
            break;
    }
    
    return lb_err;
}

/*********************************************************************
��������: u32 GetCalibItemValue(u8 fuc_Item)
��    ��: ȡ�궨����ֵ
˵    ��: 
��ڲ���: fuc_Item = �궨������Ŀ
�� �� ֵ: ��Ӧ����Ŀ����ֵ
��    ��: �Ժ���               ʱ    ��: 2011-1-4
��    ��:                      ʱ    ��: 
*********************************************************************/
u32 GetCalibItemValue(u8 fuc_Item)
{
    u32 ful_Value = 0;
    
    switch(fuc_Item)
    {
        case CAL_UNIT:
            ful_Value = gs_Cal.Unit;
            break;
        case CAL_POINT:
            ful_Value = gs_Cal.Point;
            break;
        case CAL_DIV:
            ful_Value = gs_Cal.Div;
            break;
        case CAL_CAP:
            ful_Value = gs_Cal.Cap;
            break;
        case CAL_SENSOR:
            ful_Value = gs_Cal.Sensor;
            break;
        case CAL_LOAD_1:
            if(gu32_AdFedValue >= gs_Cal.Zero)
                ful_Value = gu32_AdFedValue - gs_Cal.Zero;
            break;
        case CAL_INPUTCODE:
            ful_Value = 0;
            break;
        case CAL_CORRECT_1:
            gu32_CalDispWt = 0; //�����ʾ�������뻺��
            ful_Value = 0;
            break;
        default:
            break;
    }
    
    return ful_Value;
}

/*********************************************************************
��������: void Key_CalibState(void)
��    ��: �궨����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-12
��    ��:                      ʱ    ��: 
*********************************************************************/
void Key_CalibState(void)
{
    u8 luc_DataLen = 0;      //��������λ��
    static u32 su32_MvZeroTemp;     //��������ʾʱ���������������ֵ
    
    gb_GainSig = false;
    
    if (gu32_KeyCode == K_RIGHT)     //������ʾ�л�����һ��
    {
        if (gu8_Item < DISP_CALEND)
        {
            if (gu8_Item != CAL_CHECKCODE)
                gu8_Item++;
            if (gu8_Item == CAL_ZERO_2)     //�����ֶ������
                gu8_Item++;
            if (gu8_Item == CAL_LOAD_2)     //�����ֶ�������
                gu8_Item += 2;
            if (gu8_Item == CAL_LOAD_3)     //�ֶ��������������ʱ���Ҽ�����������һ��
                gu8_Item--;
            if (gu8_Item == CAL_MVOUT)
                su32_MvZeroTemp = 0;
            if (gb_FastCalFlg && gu8_Item == CAL_INPUTCODE)
            {   //���ٱ궨���������޸Ľ��� v1.01
                gu8_Item = DISP_CALEND;
            }
        }
        ulParameter.ul = GetCalibItemValue(gu8_Item);
        gb_FirstNumKeyFlg = true;
        gu8_DataInLength = 0;
        gu32_TimeDelay = gu32_Sys10ms;
        if (gu8_Item == DISP_CALEND)
            gu32_TimeDelay = gu32_Sys10ms;
    }
    
    //����ÿһ�����ʾ��ֵ
    switch (gu8_Item)
    {
        case CAL_CHECKCODE:             //��������
        case CAL_CAP:                   //�������
        case CAL_ZERO_2:               //�궨���2���ֶ�����������ֵ
        case CAL_LOAD_2:               //�궨����2���ֶ������������ֵ
        case CAL_LOAD_3:               //�궨����3��������������
        case CAL_CORRECT_1:     //����У׼ ������ʾ����
        case CAL_CORRECT_2:     //����У׼ ����ʵ������
            luc_DataLen = 6;            //6λ��������
            break;
        case DISP_CALON:            //��ʾCAL onһ������궨
            gb_FirstNumKeyFlg = false;
            if (gu32_Sys10ms - gu32_TimeDelay >= 100)
            {
                if (gb_FastCalFlg)
                {   //���ٱ궨ֱ�ӽ������궨
                    gu8_Item = CAL_ZERO_1;
                }
                else
                {   //��׼�궨�ӵ�λ��ʼ
                    gu8_Item = CAL_UNIT;
                    ulParameter.ul = gs_Cal.Unit;
                }
                gb_FirstNumKeyFlg = true;
            }
            break;
        case CAL_UNIT:                  //�궨��λ
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 3);      //��λѡ���á����޸�
            break;
        case CAL_POINT:                 //С����
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 4);      //С�����á����޸�
            break;
        case CAL_DIV:                   //��С�ֶ�
            if (gu32_KeyCode == K_INC)        //�ֶ�ֵ�궨�á����޸�
            {
                switch(ulParameter.ul)
                {
                    case 1:
                        ulParameter.ul = 2;
                        break;
                    case 2:
                        ulParameter.ul = 5;
                        break;
                    case 5:
                        ulParameter.ul = 10;
                        break;
                    case 10:
                        ulParameter.ul = 20;
                        break;
                    case 20:
                        ulParameter.ul = 50;
                        break;
                    default:
                        ulParameter.ul = 1;
                        break;
                }
            }
            break;
        case CAL_SENSOR:                //������������
            ulParameter.ul = Key_IncInput(ulParameter.ul, 1, 3);      //�������������á����޸�
            break;
        case CAL_MVOUT:               //��������ʾ
            if(gu32_AdFedValue >= su32_MvZeroTemp)
            {
                ulParameter.ul = gu32_AdFedValue - su32_MvZeroTemp;
                gb_GainSig = false;
            }
            else
            {
                ulParameter.ul = su32_MvZeroTemp - gu32_AdFedValue;
                gb_GainSig = true;
            }
            break;
        case CAL_LOAD_1:               //����궨
            if(gu32_AdFedValue >= gs_Cal.Zero)
            {
                ulParameter.ul = gu32_AdFedValue - gs_Cal.Zero;
                gb_GainSig = false;
            }
            else
            {
                ulParameter.ul = gs_Cal.Zero - gu32_AdFedValue;
                gb_GainSig = true;
            }
            break;
        case CAL_INPUTCODE:             //��������
//            gb_FirstNumKeyFlg = false;
            if (gu32_KeyCode == K_SET)           //������������״̬
            {
                gb_SetFlg = true;
                gb_FirstNumKeyFlg = true;
            }
            if (gb_SetFlg)
            {
                if (gu32_KeyCode == K_ESC)       //�˳���������״̬
                {
                    gb_SetFlg = false;
                    gu32_KeyCode = 0;
                    gu8_CalibInTime = 0;
                    gu8_DataInLength = 0;
                }
                else
                    luc_DataLen = 6;
            }
            break;
        case DISP_CALEND:               //�궨����
            if (gu32_Sys10ms - gu32_TimeDelay >= 100)
            {
                gu32_SystemState = STOPSTATE;
//                Read_CalibData();
//                FluxInit();     //����������λ��С����
            }
            break;
        default:
            break;
    }
    
    //��������
    ulParameter.ul = Key_NumInput(ulParameter.ul, luc_DataLen);
    
    //ÿһ���µİ�����������
    switch(gu32_KeyCode)
    {
        case K_SET:     //��������У׼
            if (gu8_Item >= CAL_UNIT && gu8_Item <= CAL_LOAD_3)
            {
                gu8_Item = CAL_CORRECT_1;
                ulParameter.ul = GetCalibItemValue(gu8_Item);
            }
            break;
        case K_ZERO:
            if (gu8_Item == CAL_MVOUT)
                su32_MvZeroTemp = gu32_AdFedValue;
            break;
        case K_INC:             //set��������
            //���궨1������궨1ʱ��set���������궨2������궨2���ֶ����������
            if (gu8_Item == CAL_ZERO_1 || gu8_Item == CAL_LOAD_1)
            {
                gu8_Item++;
                ulParameter.ul = 0;
            }
            break;
        case K_ESC:             //�˳�������
            if ((gu8_Item == CAL_MVOUT) && su32_MvZeroTemp != 0)
            {
                su32_MvZeroTemp = 0;
                break;
            }
            if (gu8_Item == CAL_LOAD_2 || gu8_Item == CAL_LOAD_3)        //�ֶ���������水ESC�˳������������ʾ״̬
                gu8_Item = CAL_LOAD_1;
            else if (gu8_Item == CAL_ZERO_2)
                gu8_Item = CAL_ZERO_1;
            else
            {
                gu8_Item = DISP_CALEND;
                gu32_TimeDelay = gu32_Sys10ms;
            }
            break;
        case K_ENTER:           //ȷ�ϼ�����
            if (CalDataInputCheck())      //�������ݵĺ����Լ��
            {
                ErrorDeal(0, 100);
                ulParameter.ul = GetCalibItemValue(gu8_Item);   //���ݴ���󷵻�ԭֵ
                return;
            }
            switch(gu8_Item)
            {
                case CAL_CHECKCODE:     //��������
                    gu8_CalibInTime++;
                    if ((ulParameter.ul == gu32_CalCode && gu8_DataInLength == 6) || 
                        (ulParameter.ul == SUPERCODE  && gu8_DataInLength == 6))
                    {
                        gu8_CalibInTime = 0;
                        ulParameter.ul = 0;
                    }
                    else if (gu8_CalibInTime >= 3)      //����������󳬹�3��
                    {
                        ErrorDeal(4, 0);        //��ʾerror4
                        Disp_Error();
                        VFD_CopyDispArryToDispRam();
                        VFD_WriteDispRamArry();
                        while(1);
                    }
                    else            //�������������ʾerror
                    {
                        gb_FirstNumKeyFlg = true;
                        ErrorDeal(0, 100);
                        ulParameter.ul = 0;
                        gu8_DataInLength = 0;
                        return;
                    }
                    break;
                case CAL_UNIT:
                    if (gs_Cal.Unit != ulParameter.ul)
                    {   //�޸ĵ�λ������ۼ�
                        gs_Cal.Unit = ulParameter.ul;
                        FRAM_WriteDW(FMAddr_CalUnit, gs_Cal.Unit);  //���浥λ
                        ClrSum();
                    }
                    break;
                case CAL_POINT:
                    if (gs_Cal.Point != ulParameter.ul)
                    {   //�޸�С���������ۼ�
                        gs_Cal.Point = ulParameter.ul;
                        FRAM_WriteDW(FMAddr_CalPoint, gs_Cal.Point);   //����С����
                        ClrSum();
                    }
                    break;
                case CAL_DIV:       //�޸ķֶ�ֵ���޸�adk �������
//                    if (gs_Cal.Div != ulParameter.ul)
//                    {
//                        UpdateRecF1(ulParameter.ul);  //�ֶ��޸ĸ����䷽F1
//                    }
                    gs_Cal.ADK = FRAM_ReadFloat(FMAddr_CalADK);    //������adk
                    gs_Cal.ADK /= (float)gs_Cal.Div;                     //���Էֶ�ֵ
                    gu32_FloatZero *= gs_Cal.Div;
                    gs_Cal.Div = ulParameter.ul;                    //�õ��·ֶ�ֵ
                    gs_Cal.ADK *= (float)gs_Cal.Div;                     //������adk
                    gu32_FloatZero /= gs_Cal.Div;                  //������㴦��
                    FRAM_WriteDW(FMAddr_CalDiv, gs_Cal.Div);        //�����·ֶ�ֵ
                    FRAM_WriteFloat(FMAddr_CalADK, gs_Cal.ADK);;    //������adk
                    
                    if (gs_Cal.Cap > gs_Cal.Div * 100000)            //������̴��ڷֶ�ֵ��30000�����޸��������
                    {
                        gs_Cal.Cap = gs_Cal.Div * 100000;
                        FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                    }
                    break;
                case CAL_CAP:
                    gs_Cal.Cap = ulParameter.ul;
                    FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);    //�����������
                    break;
                case CAL_SENSOR:
                    gs_Cal.Sensor = ulParameter.ul;
                    FRAM_WriteDW(FMAddr_CalSensor, gs_Cal.Sensor);      //���洫����������
                    break;
                case CAL_ZERO_1:
                    if (gb_MvStabFlg == false)      //���ȶ����ܱ����
                        return;
                    gs_Cal.Zero = gu32_AdFedValue;
                    FRAM_WriteDW(FMAddr_CalZero, gs_Cal.Zero);  //�������
                    gu32_FloatZero = 0;
                    break;
                case CAL_ZERO_2:
                    gs_Cal.Zero = MvToAd(ulParameter.ul);       //�������ֵ��Ӧ��ad��
                    FRAM_WriteDW(FMAddr_CalZero, gs_Cal.Zero);  //�������
                    gu32_FloatZero = 0;
                    break;
                case CAL_LOAD_1:
                    if (gb_GainSig == true || gb_MvStabFlg == false)
                        return;
                    gu32_CalLoadAD = ulParameter.ul;
                    gu32_FloatZero = 0;
                    break;
                case CAL_LOAD_2:
                    gu32_CalLoadAD = MvToAd(ulParameter.ul);       //�������ֵ��Ӧ��ad��
                    gu32_FloatZero = 0;
                    break;
                case CAL_LOAD_3:
                    gu32_CalLoadWt = ulParameter.ul;
                    //         ADc * (2^13)
                    //  ADK = ------------------
                    //         (Wc / dd) * 4
//                    gs_Cal.ADK = RoundOff(gu32_CalLoadAD << 13, gu32_CalLoadWt * 4) * gs_Cal.Div;
                    gs_Cal.ADK = ((float)gu32_CalLoadAD)/((float)gu32_CalLoadWt * 4.0f) * (float)gs_Cal.Div;
                    FRAM_WriteFloat(FMAddr_CalADK, gs_Cal.ADK);    //����ADK
                    gu32_FloatZero = 0;
                    break;
                case CAL_INPUTCODE:
                    if (gb_SetFlg == true)
                    {
                        gu8_CalibInTime++;
                        if (gu8_CalibInTime >= 2)
                        {
                            if (ulParameter.ul == gu32_DataTemp && gu8_DataInLength == 6)         //�������������һ��
                            {
                                gu8_CalibInTime = 0;
                                gb_SetFlg = false;
                                gu32_CalCode = ulParameter.ul;
                                FRAM_WriteDW(FMAddr_CalCode, gu32_CalCode);          //��������
                            }
                            else
                            {
                                gu8_CalibInTime = 0;
                                ErrorDeal(0, 100);
                                gb_FirstNumKeyFlg = true;
                                ulParameter.ul = 0;
                                gu8_DataInLength = 0;
                                return;
                            }
                        }
                        else
                        {
                            if (gu8_DataInLength == 6)      //�����������6λ
                            {
                                gu32_DataTemp = ulParameter.ul;      //�ݴ��һ����������ֵ
                                ulParameter.ul = 0;
                                gu8_DataInLength = 0;
                                return;
                            }
                            else
                            {
                                gu8_CalibInTime = 0;
                                ErrorDeal(0, 100);
                                gb_FirstNumKeyFlg = true;
                                ulParameter.ul = 0;
                                gu8_DataInLength = 0;
                                return;
                            }
                        }
                    }
                    break;
                case CAL_CORRECT_1:     //����У׼ �����������ʾ����
                    gu32_CalDispWt = ulParameter.ul;
                    gu8_Item++;
                    break;
                case CAL_CORRECT_2:     //���������ʵ����������ʾ�������¼���ADK
                    gs_Cal.ADK = ((float)gu32_CalDispWt)*gs_Cal.ADK/((float)ulParameter.ul);
                    FRAM_WriteFloat( FMAddr_CalADK, gs_Cal.ADK );    //����ADK
                    gu32_FloatZero = RoundOff(gu32_FloatZero * ulParameter.ul, gu32_CalDispWt);
                    gu32_CalDispWt = 0;
                    gu8_Item = CAL_INPUTCODE;
                    break;
                default:
                    break;
            }
            gu8_Item++;
            if (gu8_Item == CAL_ZERO_2 || gu8_Item == CAL_LOAD_2)
                gu8_Item++;
            if (gu8_Item >= CAL_END)
                gu8_Item--;
            if (gu8_Item == CAL_MVOUT)
                su32_MvZeroTemp = 0;
            if (gb_FastCalFlg && gu8_Item == CAL_INPUTCODE)
            {   //���ٱ궨���������޸Ľ��� v1.01
                gu8_Item = DISP_CALEND;
            }
            ulParameter.ul = GetCalibItemValue(gu8_Item);
            gu8_DataInLength = 0;
            gb_FirstNumKeyFlg = true;
            if (gu8_Item == DISP_CALEND || gu8_Item == DISP_CALON)  //��ʾCALON��CALENDʱ�ļ�ʱ
                gu32_TimeDelay = gu32_Sys10ms;
            break;
        default:
            break;
    }
}

/*********************************************************************
��������: void Disp_CalibState(void)
��    ��: �궨����ʾ����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-12
��    ��:                      ʱ    ��: 
*********************************************************************/
void Disp_CalibState(void)
{
    u32 lu32_Point = 0;
    u32 lu32_Unint = 0;
    bool  lb_Ofl = false;
    
    
    gb_FlashFlg = false;
    
    switch (gu8_Item)
    {
        case CAL_CHECKCODE:         //����У��
            Disp_InputCode();
            gb_FlashFlg = true;
            break;
        case DISP_CALON:        //����궨��ʾCAL on
            VFD_MainDisp_Str("CAL ON");
            break;
        case CAL_UNIT:
            VFD_AuxDisp_Str("-1-");
            break;
        case CAL_POINT:
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("-2-");
            break;
        case CAL_DIV:           //�궨�ֶ�ֵ
            lu32_Point = gs_Cal.Point;
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("-3-");
            break;
        case CAL_CAP:           //�������
            gb_FlashFlg = true;
            lu32_Point = gs_Cal.Point;
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("-4-");
            break;
        case CAL_SENSOR:
            lu32_Unint = UNIT_none;
            VFD_AuxDisp_Str("-5-");
            break;
        case CAL_MVOUT:       //��������ʾ
            lu32_Point = 3;
            ulParameter.ul = AdToMv(ulParameter.ul);
            if (AdToMv(gu32_AdFedValue) >= cu32_MvMax[gs_Cal.Sensor])
                lb_Ofl = true;
            lu32_Unint = UNIT_none;
            VFD_AuxDisp_Str("-6-");
            VFD_FlgDisp_STAB(gb_MvStabFlg);
            break;
        case CAL_ZERO_1:       //�궨�����ʾZEro
            lu32_Point = 3;
            lu32_Unint = UNIT_none;
            lb_Ofl = gb_AdOflFlg;
            ulParameter.ul = AdToMv(gu32_AdFedValue);
            VFD_AuxDisp_Str("-7-");
            VFD_FlgDisp_STAB(gb_MvStabFlg);
            VFD_FlgDisp_ZERO(true);
            break;
        case CAL_ZERO_2:       //�ֶ��궨��� ���������
            gb_FlashFlg = true;
            lu32_Point = 3;
            VFD_AuxDisp_Str("-7-");
            break;
        case CAL_LOAD_1:       //�궨������ʾLoAd
            lb_Ofl = gb_AdOflFlg;
            lu32_Point = 3;
            lu32_Unint = UNIT_none;
            ulParameter.ul = AdToMv(ulParameter.ul);
            VFD_AuxDisp_Str("-8-");
            VFD_FlgDisp_STAB(gb_MvStabFlg);
            break;
        case CAL_LOAD_2:
            gb_FlashFlg = true;
            lu32_Point = 3;
            VFD_AuxDisp_Str("-8-");
            break;
        case CAL_LOAD_3:
            gb_FlashFlg = true;
            lu32_Point = gs_Cal.Point;
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("-9-");
            break;
        case CAL_INPUTCODE:
            Disp_InputCode();       //����������ʾ����
            VFD_AuxDisp_Str("PAS");
            if (gb_SetFlg == true)
                gb_FlashFlg = true;
            break;
        case DISP_CALEND:           //�궨�����ʾCALEnd
            VFD_MainDisp_Str("CALEND");
            break;
        case CAL_CORRECT_1:         //����У׼1������ʾ����
            gb_FlashFlg = true;
            lu32_Point = gs_Cal.Point;
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("C-1");
            break;
        case CAL_CORRECT_2:         //����У׼2����ʵ������
            gb_FlashFlg = true;
            lu32_Point = gs_Cal.Point;
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("C-2");
            break;
        default:
            break;
    }
    
    switch (gu8_Item)
    {
        case CAL_CHECKCODE:
        case CAL_INPUTCODE:
            Flash_MainDisp(0x01);                //��˸����
            break;
        case DISP_CALON:
        case DISP_CALEND:
            break;
        case CAL_UNIT:
            VFD_UnitDisp(ulParameter.ul);            //��ʾ��λ
            break;
        case CAL_POINT:
            Disp_Main_Data(0, ulParameter.ul, 6, true, false);               //��ʾС����λ��
            VFD_UnitDisp(lu32_Unint);                  //��λ��ʾ
            break;
        case CAL_ZERO_2:
        case CAL_LOAD_2:
            Disp_Main_Data(ulParameter.ul, lu32_Point, 6, false, false);       //A��ʾֵ
            Flash_MainDisp(0x01);                //��˸����
            break;
        default:
            Disp_Main_Data(ulParameter.ul, lu32_Point, 6, true, lb_Ofl);       //��ʾֵ
            VFD_MainSigDisp(gb_GainSig);          //��ʾ����
            Flash_MainDisp(0x01);                //��˸����
            VFD_UnitDisp(lu32_Unint);                  //��λ��ʾ
            break;
    }
}