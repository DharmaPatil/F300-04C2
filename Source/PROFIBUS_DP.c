/*
Description:            PROFIBUS-DP������ع���
Author:                 �Ժ���
Date:                   2014-4-4
IC:                     PB-OEM2-SEǶ��ʽPROFIBUS������ ������ʵ���¿Ƽ����޹�˾
*/

/*GSD-1�ļ���
��Ӧ����ARMƽ̨��GSD�ļ����������豸�Ŀͻ�ʹ��

; ======================================================
; GSD�ļ���GM8804C-5_ARM�Ǳ�ʹ�� GSD-1
; ��Ʒ�ͺţ�GM8804C-5_ARM��׼��
; �汾�ţ�Version:  V1.1
; �ļ�����04C-5.GSD 
;======================================================
#Profibus_DP
; Prm-Text-Def-List

PrmText=1
Text(1)="�ֶ�ֵ: 1"
Text(2)="�ֶ�ֵ: 2"
Text(5)="�ֶ�ֵ: 5"
Text(10)="�ֶ�ֵ: 10"
Text(20)="�ֶ�ֵ: 20"
Text(50)="�ֶ�ֵ: 50"
EndPrmText

PrmText=2
Text(0)="��λ:  g"
Text(1)="��λ: kg"
Text(2)="��λ:  t"
Text(3)="��λ: lb"
EndPrmText

PrmText=3
Text(0)="0-��С��"
Text(1)="0.0-1λС��"
Text(2)="0.00-2λС��"
Text(3)="0.000-3λС��"
Text(4)="0.0000-4λС��"
EndPrmText

;4 ���ֶ���

PrmText=5
Text(0)="�޼�������װģʽ"
Text(1)="�м�������װģʽ"
Text(2)="˫�ж��ӻ���ģʽA"
Text(3)="˫�ж��ӻ���ģʽB"
Text(4)="˫�޶��ӻ���ģʽA"
Text(5)="˫�޶��ӻ���ģʽB"
EndPrmText

PrmText=6
Text(0)="ë�ذ�װ"
Text(1)="���ذ�װ"
EndPrmText

PrmText=7
Text(0)="��Ϸ�ʽ����"
Text(1)="������ʽ����"
EndPrmText

;8 �Զ�������
;9 ����״̬���ȳ�ʱʱ��

PrmText=10
Text(0)="����ʾ�䷽��"
Text(1)="����ʾ���ۼƴ���"
Text(2)="����ʾ��ǰ�䷽�ۼƴ���"
EndPrmText

;11 ���㷶Χ
;12 ���ȷ�Χ
;13 �����ٷ�Χ
;14 AD�����˲�����
;15 ж���˲�����
;16 ��ֵ�˲�����

PrmText=17
Text(0)="120��/��"
Text(1)="240��/��"
Text(2)="480��/��"
Text(3)="960��/��"
EndPrmText

PrmText=18
Text(1)="ON-��"
Text(0)="OFF-�ر�"
EndPrmText

PrmText=19
Text(1)="ON-��"
Text(0)="OFF-�ر�"
EndPrmText

PrmText=20
Text(1)="ON-��"
Text(0)="OFF-�ر�"
EndPrmText

PrmText=21
Text(1)="ON-��"
Text(0)="OFF-�ر�"
EndPrmText

PrmText=22
Text(1)="ON-��"
Text(0)="OFF-�ر�"
EndPrmText

PrmText=23
Text(1)="ON-��"
Text(0)="OFF-�ر�"
EndPrmText

PrmText=24
Text(0)="OFF-��ͬʱ�ɴ�"
Text(1)="ON1-ͬʱ�ɴ���ͨģʽ"
Text(2)="ON2-ͬʱ�ɴ�����ģʽ"
EndPrmText

;25 ���ͻ�������ʱʱ��
;26 ���ͻ�����ʱ��
;27 �ٴμд���ʱ��������ʱ��


;-----------------------------
ExtUserPrmData=1 "��С�ֶ�ֵ��"
Unsigned8 1 1-50
Prm_Text_Ref=1
EndExtUserPrmData

ExtUserPrmData=2 "��λ��"
Unsigned8 1 0-4
Prm_Text_Ref=2
EndExtUserPrmData

ExtUserPrmData=3 "С��λ����"
Unsigned8 2 0-4
Prm_Text_Ref=3
EndExtUserPrmData

ExtUserPrmData=4 "���ֶ�����"	;�����Ǳ��������
Unsigned32 10000 1-100000
EndExtUserPrmData

ExtUserPrmData=5 "��װ�ӹ���ģʽ��"
Unsigned8 1 0-5
Prm_Text_Ref=5
EndExtUserPrmData

ExtUserPrmData=6 "�޼�������װģʽ��"
Unsigned8 1 0-1
Prm_Text_Ref=6
EndExtUserPrmData

ExtUserPrmData=7 "���Ϸ�ʽ��"
Unsigned8 0 0-1
Prm_Text_Ref=7
EndExtUserPrmData

ExtUserPrmData=8 "�Զ���������"
Unsigned8 0 0-99
EndExtUserPrmData

ExtUserPrmData=9 "����״̬���ȳ�ʱʱ�䣺"
Unsigned16 0 0-999
EndExtUserPrmData

ExtUserPrmData=10 "����ʾ���ݣ�"
Unsigned8 0 0-2
Prm_Text_Ref=10
EndExtUserPrmData

ExtUserPrmData=11 "���㷶Χ��"
Unsigned8 50 1-99
EndExtUserPrmData


ExtUserPrmData=12 "���ȷ�Χ��"
Unsigned8 1 0-99
EndExtUserPrmData

ExtUserPrmData=13 "�����ٷ�Χ��"
Unsigned8 1 0-99
EndExtUserPrmData

ExtUserPrmData=14 "AD�����˲�������"
Unsigned8 5 0-9
EndExtUserPrmData

ExtUserPrmData=15 "ж���˲�������"
Unsigned8 3 0-9
EndExtUserPrmData

ExtUserPrmData=16 "��ֵ�˲�������"
Unsigned8 4 0-9
EndExtUserPrmData

ExtUserPrmData=17 "AD�����ٶȣ�"
Unsigned8 0 0-3
Prm_Text_Ref=16
EndExtUserPrmData

ExtUserPrmData=18 "�ϵ��Զ����㣺"
Unsigned8 0 0-1
Prm_Text_Ref=18
EndExtUserPrmData

ExtUserPrmData=19 "�ֶ�ж���ۼƣ�"
Unsigned8 0 0-1
Prm_Text_Ref=19
EndExtUserPrmData

ExtUserPrmData=20 "��ֵ�������֣�"
Unsigned8 0 0-1
Prm_Text_Ref=20
EndExtUserPrmData

ExtUserPrmData=21 "�ۼƲ�ѯ���룺"
Unsigned8 0 0-1
Prm_Text_Ref=21
EndExtUserPrmData

ExtUserPrmData=22 "�ۼ�������룺"
Unsigned8 0 0-1
Prm_Text_Ref=22
EndExtUserPrmData

ExtUserPrmData=23 "�ۼƷ�Χ��չ��"
Unsigned8 1 0-1
Prm_Text_Ref=23
EndExtUserPrmData

ExtUserPrmData=24 "˫�޶���ͬʱ�ɴ���"
Unsigned8 0 0-2
Prm_Text_Ref=24
EndExtUserPrmData

ExtUserPrmData=25 "���ͻ�������ʱʱ�䣺"
Unsigned16 5 0-999
EndExtUserPrmData

ExtUserPrmData=26 "���ͻ�����ʱ�䣺"
Unsigned16 40 0-999
EndExtUserPrmData

ExtUserPrmData=27 "�ٴμд���ʱ��������ʱ�䣺"
Unsigned16 20 0-999
EndExtUserPrmData


;-----------------------------------------
;General parameters
GSD_Revision    = 1
Vendor_Name     = "���ڽ����Ƽ��ɷ����޹�˾"
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
;12M_supp       = 1	;�ݲ�֧��12M
MaxTsdr_9.6     = 60
MaxTsdr_19.2    = 60
MaxTsdr_45.45   = 250
MaxTsdr_93.75   = 60
MaxTsdr_187.5   = 60
MaxTsdr_500     = 100
MaxTsdr_1.5M    = 150
MaxTsdr_3M      = 250
MaxTsdr_6M      = 450
;MaxTsdr_12M    = 800  ;�ݲ�֧��12M
;Redundancy     = 0
;Repeater_Ctrl_Sig	= 2
;24V_Pins       = 0
Implementation_Type = "spc3"
;Bitmap_Device	="GM8804C-2"      ;λͼ����
; Bitmap_Diag	= "Bitmap1D"
; Bitmap_SF     = "Bitmap1S"
; Slave-Specification:
OrderNumber="GM8804C-2"        ;λͼ���ƣ�
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

Ext_User_Prm_Data_Ref(0)=1  ;�ֶ�ֵ
Ext_User_Prm_Data_Ref(1)=2  ;��λ
Ext_User_Prm_Data_Ref(2)=3  ;С����
Ext_User_Prm_Data_Ref(3)=4	;���ֶ� 4Bytes
Ext_User_Prm_Data_Ref(7)=5  ;��װ�ӹ���ģʽ��
Ext_User_Prm_Data_Ref(8)=6  ;�޼�������װģʽ��
Ext_User_Prm_Data_Ref(9)=7  ;���Ϸ�ʽ
Ext_User_Prm_Data_Ref(10)=8     ;�Զ�������
Ext_User_Prm_Data_Ref(11)=9     ;���ȳ�ʱʱ�� 2Byte
Ext_User_Prm_Data_Ref(13)=10    ;����ʾ���ݣ�
Ext_User_Prm_Data_Ref(14)=11    ;���㷶Χ                    
Ext_User_Prm_Data_Ref(15)=12    ;���ȷ�Χ                    
Ext_User_Prm_Data_Ref(16)=13    ;�����ٷ�Χ                
Ext_User_Prm_Data_Ref(17)=14    ;AD�����˲�����              
Ext_User_Prm_Data_Ref(18)=15    ;ж���˲�����                
Ext_User_Prm_Data_Ref(19)=16    ;��ֵ�˲�����                
Ext_User_Prm_Data_Ref(20)=17    ;AD�����ٶ�                  
Ext_User_Prm_Data_Ref(21)=18    ;�ϵ��Զ����㿪��            
Ext_User_Prm_Data_Ref(22)=19    ;�ֶ�ж���ۼƿ���            
Ext_User_Prm_Data_Ref(23)=20    ;��ֵ�������ֿ���            
Ext_User_Prm_Data_Ref(24)=21    ;�ۼƲ�ѯ���뿪��            
Ext_User_Prm_Data_Ref(25)=22    ;�ۼ�������뿪��            
Ext_User_Prm_Data_Ref(26)=23    ;�ۼƷ�Χ��չ����            
Ext_User_Prm_Data_Ref(27)=24    ;˫�޶���ͬʱ�ɴ���
Ext_User_Prm_Data_Ref(28)=25    ;���ͻ�������ʱʱ�䣺2Byte
Ext_User_Prm_Data_Ref(30)=25     ���ͻ�����ʱ�䣺2Byte
Ext_User_Prm_Data_Ref(32)=25     �ٴμд���ʱ��������ʱ�䣺2Byte
                                    
Module="10AI+24DI+32DI,2AO+32DO" 0x59,0x12,0x13,0x61,0x23

EndModule
*/
/*���Ǳ��������վ��10AI+24DI+32DI====================================================
10AI:
Byte0/1/2/3: ��ǰ����
Byte4/5/6/7: ����δ��
Byte8/9/10/11: ���ۼƴ���
Byte12/1/14/15: ���ۼ�����
Byte16/17/18/19: ��������

24DI+32DI:
Byte0:����״̬
    .2~.0:С����000:��С����
                001:һλС����,0.0
                010:��λС����,0.00
                011��λС����,0.000
                100:��λС����,0.0000
    .4~.3:��λ  00:g
                01:kg
                10:t
                11:lb
    .5:����     1:'-';0:'+'
    .6:�ȶ�     1:�ȶ���0:����
    .7:���     1:�����0:����

Byte1:����δ��

Byte2:���ۼ�����״̬
    .2~.0:С����000:��С����
                001:һλС����,0.0
                010:��λС����,0.00
                011��λС����,0.000
                100:��λС����,0.0000
    .4~.3:��λ  00:g
                01:kg
                10:t
                11:lb
    .7~.5:����δ��

Byte3:ϵͳ״̬1
    .0  0: ֹͣ��1: ���С�
    .1  ����ǰ
    .2  ��Ͷ
    .3  ��Ͷ
    .4  СͶ
    .5  ��ֵ
    .6  ��Ƿ��
    .7  ����
Byte4:ϵͳ״̬2
    .0  �д�
    .1  �Ĵ�
    .2  ж��
    .3  ����
    .4  ����
    .5  �������
    .6  ȱ��
    .7  ж�����
Byte5:ϵͳ״̬3
    .0  ����
    .1  ����
    .2  ���
    .3  �ƴ�
    .4  ���߼��
    .5  ����
    .7~.6  00д�����ݳɹ�
           01д�����ݳ���Χ
           10��ǰ״̬�޷�д�������
           11û��д�����������
    
Byte6:�����������ͣ�����10AI�еġ��������ݡ����͵���������
    0��������
    1~20����Ӧ���䷽1~20���ۼ�����
    21~40����Ӧ���䷽1~20���ۼƴ���
    41��������
    42���䷽��
    43��ʣ������
    44~79������δ��
    80~85��F1.1~F1.6
    86~89��F2~F2.3
    90~101��F3.1~F3.12
    102~105��F4~F4.3
    106~112��F5~F5.6
    113~115��F6~F6.2
    116��F7
    117~121��F8~F8.4
    122~125��F9~F9.3
    126~255������δ��


����վ������Ǳ�2AO+32DO======================================================
2AO: ��������

32DO:
Byte0:����ֵ���ͣ�����2AO��������������
    0����д������
    1~20����������д��0���Ӧ���䷽1~20���ۼ�����
    21~40������δ��
    41��������
    42���䷽��
    43~79������δ��
    80~85��F1.1~F1.6
    86~89��F2~F2.3
    90~101��F3.1~F3.12
    102~105��F4~F4.3
    106~112��F5~F5.6
    113~115��F6~F6.2
    116��F7
    117~121��F8~F8.4
    122~125��F9~F9.3
    126~149������δ��
    150��д��1��ӡ���ۼ�����
    151��д��0��ӡ��ǰ�䷽���ñ�
         д��1~20 ��ӡ��Ӧ�䷽��1~20�����ñ�
         д��21��ӡ�����䷽���ñ�����ӡĿ��ֵΪ0���䷽��
    152��д��0 ��ӡ��ǰ�䷽�ۼƱ���
         д��1~20 ��ӡ��Ӧ�䷽��1~20���ۼƱ���
         д��21��ӡ�����䷽�ۼƱ�������ӡĿ��ֵΪ0���䷽����
    153~255������δ��

Byte1:�����������ͣ�����10AI�еġ��������ݡ����͵���������
    0��������
    1~20����Ӧ���䷽1~20���ۼ�����
    21~40����Ӧ���䷽1~20���ۼƴ���
    41��������
    42���䷽��
    43��ʣ������
    44~79������δ��
    80~85��F1.1~F1.6
    86~89��F2~F2.3
    90~101��F3.1~F3.12
    102~105��F4~F4.3
    106~112��F5~F5.6
    113~115��F6~F6.2
    116��F7
    117~121��F8~F8.4
    122~125��F9~F9.3
    126~255������δ��

    
Byte2:ִ�в���1
    .0 ����δ��
    .1 ����
    .2 ֹͣ
    .3 ����
    .4 �屨��
    .5 ѡ�䷽
    .6 ��ͣ
    .7 �ֶ�ж��
Byte3:ִ�в���2
    .0 �ֶ���Ͷ
    .1 �ֶ�����һ��
    .2 ������ۼ����ݣ�����ÿ���䷽���ۼ����ݣ�
    .3 �ֶ�СͶ
    .4~.6 ����δ��
    .7 ���߼��

*/



/*GSD-2�ļ���

��GSD�ļ���Ϊ�˼����ϰ汾GM8804C-2��PROFIBUSͨ�ţ������ϰ汾�ı��ĸ�ʽ
16DI(�����ֽ�)��16DO(�����ֽ�) ��1Byte In��1Byte Out��2AI(�ĸ��ֽ�)��2AO(�ĸ��ֽ�)

; ======================================================
; GSD�ļ�����ProfiBus�ӿڵ� ��װ��GM8804C2P�����ڽ����Ƽ����޹�˾
; ��Ʒ�ͺţ�GM8804C2P
; �汾�ţ�Version:  V1.0
; �ļ�����04C2P.GSD
;======================================================
#Profibus_DP
; Prm-Text-Def-List

PrmText=1
Text(0)="�ֶ�ֵ: 1"
Text(1)="�ֶ�ֵ: 2"
Text(2)="�ֶ�ֵ: 5"
Text(3)="�ֶ�ֵ: 10"
Text(4)="�ֶ�ֵ: 20"
Text(5)="�ֶ�ֵ: 50"
EndPrmText

PrmText=2
Text(0)="��λ:  g"
Text(1)="��λ: kg"
Text(2)="��λ:  t"
EndPrmText

PrmText=3
Text(0)="0����С��"
Text(1)="0.0��1λС��"
Text(2)="0.00��2λС��"
Text(3)="0.000��3λС��"
Text(4)="0.0000��4λС��"
EndPrmText

;4 ���ֶ���

PrmText=5
Text(0)="0��������������"
Text(1)="1d��׷�㷶ΧĬ��ֵ"
Text(2)="2d����Χ��������"
Text(3)="3d����Χ��������"
Text(4)="4d����Χ��������"
Text(5)="5d����Χ��������"
Text(6)="6d����Χ��������"
Text(7)="7d����Χ��������"
Text(8)="8d����Χ��������"
Text(9)="9d����Χ��������"
EndPrmText

PrmText=6
Text(1)="1d�����ȷ�Χ"
Text(2)="2d�����ȷ�Χ"
Text(3)="3d�����ȷ�Χ"
Text(4)="4d�����ȷ�Χ"
Text(5)="5d�����ȷ�Χ"
Text(6)="6d�����ȷ�Χ"
Text(7)="7d�����ȷ�Χ"
Text(8)="8d�����ȷ�Χ"
Text(9)="9d�����ȷ�Χ"
EndPrmText

;7 ���㷶Χ 1%-99%

PrmText=8
Text(0)="0���ϵ粻�Զ�����"
Text(1)="1���ϵ��Զ�����"
EndPrmText

;9 ������ 0~99

PrmText=10
Text(0)="0���������ֲ�"
Text(1)="1�������˲�����"
Text(2)="2�������˲�����"
Text(3)="3�������˲�����"
Text(4)="4�������˲�����"
Text(5)="5������(Ĭ��ֵ)"
Text(6)="6�������˲���ǿ"
Text(7)="7�������˲���ǿ"
Text(8)="8�������˲���ǿ"
Text(9)="9�������˲���ǿ"
EndPrmText

PrmText=11
Text(0)="0���޶���װģʽ"
Text(1)="1���ж���װģʽ"
EndPrmText


;-----------------------------
ExtUserPrmData=1 "��С�ֶ�ֵ(�������õ���5)��"
Unsigned8 0 0-5
Prm_Text_Ref=1
EndExtUserPrmData

ExtUserPrmData=2 "��λ��"
Unsigned8 1 0-4
Prm_Text_Ref=2
EndExtUserPrmData

ExtUserPrmData=3 "С��λ����"
Unsigned8 0 0-4
Prm_Text_Ref=3
EndExtUserPrmData

ExtUserPrmData=4 "���ֶ�����" ;�����Ǳ��������
Unsigned16 10000 350-30000
EndExtUserPrmData

ExtUserPrmData=5 "�����ٷ�Χ��"
Unsigned8 1 0-9
Prm_Text_Ref=5
EndExtUserPrmData

ExtUserPrmData=6 "���ȷ�Χ��"
Unsigned8 1 1-9
Prm_Text_Ref=6
EndExtUserPrmData

ExtUserPrmData=7 "���㷶Χ%��"
Unsigned8 50 0-99
EndExtUserPrmData

ExtUserPrmData=8 "�ϵ��Զ����㿪�أ�"
Unsigned8 0 0-1
Prm_Text_Ref=8
EndExtUserPrmData

ExtUserPrmData=9 "����ʱ��������"
Unsigned8 00 00-99
EndExtUserPrmData

ExtUserPrmData=10 "�����˲�������"
Unsigned8 7 0-9
Prm_Text_Ref=10
EndExtUserPrmData

ExtUserPrmData=11 "��װ�ӹ���ģʽ���أ�"
Unsigned8 1 0-1
Prm_Text_Ref=11
EndExtUserPrmData
;-----------------------------------------
;General parameters
GSD_Revision    = 1
Vendor_Name     = "���ڽ����Ƽ����޹�˾"
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
;12M_supp       = 1 ;�ݲ�֧��12M
MaxTsdr_9.6     = 60
MaxTsdr_19.2    = 60
MaxTsdr_45.45   = 250
MaxTsdr_93.75   = 60
MaxTsdr_187.5   = 60
MaxTsdr_500     = 100
MaxTsdr_1.5M    = 150
MaxTsdr_3M      = 250
MaxTsdr_6M      = 450
;MaxTsdr_12M    = 800  ;�ݲ�֧��12M
;Redundancy     = 0
;Repeater_Ctrl_Sig  = 2
;24V_Pins       = 0
Implementation_Type = "spc3"
;Bitmap_Device  ="GM8804C2P"      ;λͼ����
; Bitmap_Diag   = "Bitmap1D"
; Bitmap_SF     = "Bitmap1S"
; Slave-Specification:
OrderNumber="GM8804C2P"        ;λͼ���ƣ�
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
Ext_User_Prm_Data_Ref(3)=4  ;���ֶ� 2Byte
Ext_User_Prm_Data_Ref(5)=5
Ext_User_Prm_Data_Ref(6)=6
Ext_User_Prm_Data_Ref(7)=7
Ext_User_Prm_Data_Ref(8)=8
Ext_User_Prm_Data_Ref(9)=9
Ext_User_Prm_Data_Ref(10)=10
Ext_User_Prm_Data_Ref(11)=11

Module="16DI+1BI+2AI,16DO+1BI+2AO" 0x11,0x10,0x51,0x21,0x20,0x61
;DI:������,�Ǳ����뵽��վ,1Word(2Byte)I
;DO:������,��վ������Ǳ�,1Word(2Byte)
;2AI:��  ��,�Ǳ����뵽��վ,2Word(4Byte)

;1B:�ֽ�,��ʾAO���ݵ����.
;2words out(AO):��  ��,��վ������Ǳ�,2Word(4Byte)

EndModule
*/

/*���Ǳ��������վ��====================================================
16DI:
DI9:д�����:���ݳ���
DI8:д�����:д������������
DI7~DI5:���Ͻ���
000:  ֹͣ
001:  ����ǰ
010:  ���
011:  �м�
100:  ����
101:  ��ֵ
110:  �Ĵ����ɴ�
111:  ���������
DI4~DI3:ë��/����
00: ë��
01: ����
10: ����(����,�ۼ�������ë����֮��)
11 : ����,����
DI2: �Ǳ����״̬��0������1���
DI1: �Ǳ��ȶ�״̬��0���ȣ�1�ȶ�
DI0: �Ǳ����״̬��0���㣬1���

1Byte In: (����״̬)
D7:����λ,1:��-��; 0:��+��
D6~D5:2AI���ݵ�λ.
00:g
01:kg
10:t
11:����
D4~D2:2AI���ݵ�С����
000:��С����
001:һλС����,0.0
010:��λС����,0.00
011��λС����,0.000
100:��λС����,0.0000
����:����.
D1~D0:��ǰ���͵�2AI��������
 00:��ǰ����.
 01: �ۼ�����
 10: �ۼƴ���
 11: ����
   
2AI : ��Ҫ�������վ������,4�ֽ�.��λ������ǰ,��λ�����ں�.

����վ������Ǳ�======================================================
16DO��
DO15~DO11: ����.
DO9 : ����λ��0��Ϊ1ʱ,ֹͣ״̬��,ȡ���ֶ�����,�򷵻ط���Ϣ.
DO8 : ����λ��0��Ϊ1ʱ,ֹͣ״̬��,����ۼ�ֵ,������Ӧ.
DO7 : ����λ��0��Ϊ1ʱ,ֹͣ״̬��,ȡ���ֶ�СͶ,���򷵻ط���Ϣ
DO6 : ����λ��0��Ϊ1ʱ,�ɴ�
DO5 : ����λ��0��Ϊ1ʱ, �д�
DO4 : ����λ��0��Ϊ1ʱ,ֹͣ״̬��,��ת����һ�䷽.
DO3 : ����λ��0��Ϊ1ʱ,�������
DO2 : ����λ��0��Ϊ1ʱ,ֹͣ״̬��,����.
DO1 : ����λ��0��Ϊ1ʱ,ֹͣ.
DO0 : ����λ��0��Ϊ1ʱ,����.

1Byte Out:
D7~D2:2AOд����������
00000:(0)   ����,���Ὣ2AO������д���Ǳ�
00001:(1)   Ŀ��(�������Ʋ�����ӦС���������).
00010:(2)   ��Ͷ��ǰ��.
00011:(3)   ��Ͷ��ǰ��.
00100:(4)   ���ֵ.
00101:(5)   ����ֵ.
00110:(6)   t7 
00111:(7)   ��Ƿ���(д��������,ż���ر�,���¿������;�����)
01000:(8)   ��Ƿ����ͣ����
01001:(9)   ����ֵ.
01010:(10)  Ƿ��ֵ.
01011:(11)  t1(ʱ�������Ӧ��00~99)
01100:(12)  t2
01101:(13)  t3
01110:(14)  t4
01111:(15)  t5
10000:(16)  t6
10001:(17)  �Զ������������
10010:(18)  �����������(00~99)
10011:(19)  ���������Χ(00~99)
10100:(20)  �����������(1~3)
010101:(21) �Ĵ�����
010110:(22) �Ĵ���ʱ
010111:(23) �Ĵ�����ʱ��
011000:(24) �Ĵ�����
011001:(25) �Ĵ���Ч��ƽʱ��
011010:(26) �Ĵ���Ч��ƽʱ��
011011:(27) ��һ���Ĵ���Ч��ƽʱ��
011100:(28) ��һ���Ĵ���Ч��ƽʱ��
011101:(29) �㶯СͶ
011110:(30) �㶯СͶ��Ч��ƽʱ��
011111:(31) �㶯СͶ��Ч��ƽʱ��
100000:(32) �������ϴ���
�� ��:     ����,���Ὣ2AO������д���Ǳ�

D1~D0:Ҫ��2AI���͵���������
 00: (0) ��ǰ����.
 01: (1) �ۼ�����
 10: (2) �ۼƴ���
 11: (3) ����

2AO: ��Ҫд�������, 4�ֽ�.��λ������ǰ,��λ�����ں�.

*/



#include "include.h"



//===========================��ֲ���ļ�ʱ���޸Ĵ˴�=========================================
//SE_REQ_IT - P2.9       0��SE��ʼ�����         1��SE��ʼ���ȴ�
//SE_S_RTS - P1.11       0��SE�ȴ�����������   1��SE�ܾ�����
//SE_LINK - P1.12     0��SE�Ѿ��ɹ�����DP���� 1��SEδ�ɹ�����DP����

//CUP��PB-OEM2-SE֮��ͨѶ�Ĳ����ʣ����ݷ�ʽ�� 
//UART1����DP�崮�� ������38400 8λ���� żУ�� 1λֹͣλ
#define PBUS_BAUDRATE   38400
#define PBUS_PARITY     UART_EVEN_PARITY
#define PBUS_STOPBITS   UART_1_StopBits
#define PBUS_DATALEN    UARTM_8D_P

#define PBUS_UARTX          UART1               //��SE�ӿڰ�ͨѶʹ�õ�UART����
#define PBUS_UARTX_IRQC     UART1_IRQChannel    //
#define PBUS_UARTX_SIPL     UART1_SIPL          //�����ж����ȼ�

#define USER_PRM_LEN_GSD1       34              //�û���������
#define SE_A_LEN_GSD1           10               //ProfiBus������ݳ���(A����)��վ���Ǳ�
#define SE_B_LEN_GSD1           (USER_PRM_LEN_GSD1+2)  //�û��������ĸ�ʽ����(B����)��վ���Ǳ�
#define SE_IN_LEN_GSD1          29               //ProfiBus�������ݳ���(REQ����)�Ǳ���վ

#define USER_PRM_LEN_GSD2       12              //�û���������
#define SE_A_LEN_GSD2           9               //ProfiBus������ݳ���(A����)��վ���Ǳ�
#define SE_B_LEN_GSD2           (USER_PRM_LEN_GSD2+2)  //�û��������ĸ�ʽ����(B����)��վ���Ǳ�
#define SE_IN_LEN_GSD2          9               //ProfiBus�������ݳ���(REQ����)�Ǳ���վ

//PROFIBUSͨѶʱ�Ĵ����շ������������󳤶ȣ�ע��ϵͳ����ͨѶ�����鳤��һ��Ҫ�������ֵ
#define  PBUS_RXDLEN   49
#define  PBUS_TXDLEN   49

//ϵͳ���������ӿڰ�ͨѶ�Ľ��պͷ��ͻ�������
u8 *gu8p_RxdArray = gu8_RxdArray_UART1;
u8 *gu8p_TxdArray = gu8_TxdArray_UART1;

//�ӻ���
#define PBUS_ADDRESS    gs_Setup.TareNumber_1
//ϵͳ�ж�ʱ��10ms
#define SYS10MS     gu32_Sys10ms
//��ʼ�����ɹ����ٴγ�ʼ���ļ������λ10ms
#define SE_INIT_INTERVAL   50
//SE�ӿڰ���Ǳ�ͨѶ�������,��������ɺ��ٴμ��ʱ����û�н��յ������������ٴ��������ͣ���λ10ms
#define SE_COM_INTERVAL    10

//�ӿڰ��ʼ��������ݣ������ѯSE�ӿڰ����ñ������鲻������һ���ֽڴӻ���
//�ֱ���GSD1��GSD2��Ӧ�ĳ�ʼ������
const u8 cu8_PbusInit[2][48] = {
//byte1     byte2        3                4     5 6 7 ����                     24             25              26          27����
//ID�Ÿ�λ ID�ŵ�λ IO�������ݳ���20 IO��������1 2 3 4 ����                  �������ݳ���113 �������ݳ���113 �û���������41  ���� 
{0xC2,0xFC,0x05,0x59,0x12,0x13,0x61,0x23,   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SE_IN_LEN_GSD1,SE_A_LEN_GSD1,USER_PRM_LEN_GSD1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0xC2,0xFC,0x06,0x11,0x10,0x51,0x21,0x20,0x61,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SE_IN_LEN_GSD2,SE_A_LEN_GSD2,USER_PRM_LEN_GSD2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

//==========================================================================================
u8 gu8_ExtType;     //��չ������
u32 gu32_GSDType;   //GSD�ļ�
u32 gu32_FollowMasterSwitch;    //���������궨��������վ���ƿ���

//�������
volatile bool gb_PbusRxdReadyFlg;    //���ڽ����Ѿ���ɱ�־
volatile bool gb_PbusTxdingFlg;      //�������ڷ��ͱ�־
volatile bool gb_PbusRxdingFlg;      //�������ڽ��ձ�־
u8 gu8_PbusTxdLen;          //�������ݳ���
u8 gu8_PbusRxdLen;          //�������ݳ���
u8 gu8_PbusTxdCnt;          //����ʱ�����ݸ�����¼
u8 gu8_PbusRxdCnt;          //����ʱ�����ݸ�����¼
//SE�ӿڰ��CPU��ͨѶ�������
u8 gu8_ReqCom;      
/*������������req_com
D7~D2������
D1��0����Ч 1����ӿڰ��û�����������վ���±��AB_status
D0��0������ӿڻش�PROFIBUS������ݣ�A���ģ� 1������ӿڻش�PROFIBUS��վ���µ��û��������ݣ�B���ģ�*/

/*�������ͼ��ӿ�״̬AB_status
D7��0��A���� 1��B����
D6~D3������
D2~D1��00���ӿڰ�����û��屨���޴�
       01���ӿڰ�����û��屨���ַ���żУ���
       10���ӿڰ�����û��屨����У��ʹ�
       11������
D0��0���ӿڰ廹û�н��յ���վ���͵��û�����������վû�и����û�����
    1���ӿڰ��û���������������վ����*/
    
#define ERRDATA         0x01
#define ERRCANNOTWT     0x02

u8 gu8_WriteErrID;      //д���ݴ�������
u8 gu8_PbusSendType;    //PROFIBUS������������
u8 gu8_PbusWriteType;   //PROFIBUSд����������
u8 gu8_16DoTempHi;      //16DO�ĸ�λ����
u8 gu8_16DoTempLow;     //16DO�ĵ�λ����
u32 gu32_TimeTemp;
bool gb_ConnectCheckFlg;   //���߼���־

/*********************************************************************
��������: bool Pbus_CheckExt(void)
��    ��: ����Ƿ�����DP��չ��
˵    ��: 
��ڲ���: 
�� �� ֵ: true��������DP��չ��  false��δ����DP��չ��
��    ��: �Ժ���               ʱ    ��: 2014-4-4
��    ��:                      ʱ    ��: 
*********************************************************************/
bool Pbus_CheckExt(void)
{
    //����PBUS_REQ_IT��PBUS_S_RTS��PBUS_LINKΪTTL��ƽ����
    GPIO_Config(GPIO2, (1<<9), GPIO_IN_TRI_TTL);    //SE_REQ_IT
    GPIO_Config(GPIO1, (1<<11), GPIO_IN_TRI_TTL);   //SE_S_RTS 
    GPIO_Config(GPIO1, (1<<12), GPIO_IN_TRI_TTL);   //SE_LINK  
    
    /*
    δ����PBUS_UART�壬�ϵ�ʱ��������״̬��
    SE_REQ_IT = false;
    SE_S_RTS = true;
    SE_LINK = true;
    ����PBUS_UART�壬�ϵ�ʱ��������״̬��
    SE_REQ_IT = true;
    SE_S_RTS = false;
    SE_LINK = true;
    
    ���Ը���PBUS_S_RTS��PBUS_LINK��״̬���жϵ�ǰ�Ǳ��Ƿ�������DPͨѶ����չ��
    */
    
    if (SE_REQ_IT == true && SE_S_RTS == false)   //for test
        return true;
    else
        return false;
}

/*********************************************************************
��������: void UARTInit_Pbus(void)
��    ��: ��SE�ӿڰ����ͨѶǰ���ڳ�ʼ��
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-04-16
��    ��:                      ʱ    ��: 
*********************************************************************/
void UARTInit_Pbus(void)
{
    gb_PbusRxdReadyFlg = false;
    gb_PbusTxdingFlg = false;
    gb_PbusRxdingFlg = false;
    gu8_PbusTxdLen = 0;          //�������ݳ���
    gu8_PbusRxdLen = 0;          //�������ݳ���
    gu8_PbusTxdCnt = 0;          //����ʱ�����ݸ�����¼
    gu8_PbusRxdCnt = 0;          //����ʱ�����ݸ�����¼
    
    UART_Init(PBUS_UARTX);               //��λ
    UART_GuardTimeConfig(PBUS_UARTX,0); //����Txempty��λ�ȴ���ʱ
    UART_FifoConfig(PBUS_UARTX,DISABLE);//�ر�FIFO
    UART_LoopBackConfig(PBUS_UARTX,DISABLE);//�رջ�д

    //���ݴ���������ó�ʼ������ ������ У�鷽ʽ
    UART_Config(PBUS_UARTX,PBUS_BAUDRATE,PBUS_PARITY,PBUS_STOPBITS,PBUS_DATALEN);
    UART_RxConfig(PBUS_UARTX,ENABLE);//ʹ�ܽ���
    UART_FifoReset(PBUS_UARTX, UART_RxFIFO);
    UART_OnOffConfig(PBUS_UARTX,ENABLE);//ʹ�ܴ���
    UART_ItConfig(PBUS_UARTX,UART_TxEmpty,DISABLE);   //�رմ��ڷ����ж�
//    UART_ItConfig(PBUS_UARTX,UART_RxBufNotEmpty,DISABLE);//�رս����ж�
    UART_ItConfig(PBUS_UARTX,UART_RxBufNotEmpty,ENABLE);//ʹ�ܽ����ж�

    EIC_IRQChannelPriorityConfig(PBUS_UARTX_IRQC, PBUS_UARTX_SIPL);
    EIC_IRQChannelConfig(PBUS_UARTX_IRQC, ENABLE);
}

/*********************************************************************
��������: void IRQ_PBUS_UART1(void)
��    ��: PROFIBUSͨѶ�Ĵ����жϴ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-4-16
��    ��:                      ʱ    ��: 
*********************************************************************/
void IRQ_Pbus(void)
{
    u8 lu8_RxdData;
    
    //�����ж�
    if(UART_FlagStatus(PBUS_UARTX) & (UART_TxEmpty|UART_TxHalfEmpty))
    {
        if (gb_PbusTxdingFlg)   //���ڷ��ͱ�־
        {   //��������
            if (gu8_PbusTxdCnt < gu8_PbusTxdLen)
            {
                UART_ByteSend(PBUS_UARTX,&gu8p_TxdArray[gu8_PbusTxdCnt++]);
            }
            else
            {
                gb_PbusTxdingFlg = false;   //�������
                gb_PbusRxdingFlg = true;    //���ڽ���
                gu8_PbusTxdCnt = 0;
                UART_ItConfig(PBUS_UARTX,UART_TxEmpty,DISABLE); //�ط����ж�
            }
        }
        else
        {   //�������
            gu8_PbusTxdCnt = 0;
            UART_ItConfig(PBUS_UARTX,UART_TxEmpty,DISABLE); //�ط����ж�
        }
    }
    //�����ж�
    if(UART_FlagStatus(PBUS_UARTX) & (UART_RxBufNotEmpty))
    {
        lu8_RxdData = (u8)(PBUS_UARTX->RxBUFR);
        UART_FifoReset(PBUS_UARTX, UART_RxFIFO);
        if (gb_PbusTxdingFlg == false)
        {   //�Ѿ�������ɣ��������
            gu8p_RxdArray[gu8_PbusRxdCnt++] = lu8_RxdData;
            if (gu8_PbusRxdCnt >= gu8_PbusRxdLen)
            {
                gb_PbusRxdReadyFlg = true;  //�������
                gb_PbusRxdingFlg = false;   //�������
            }
            if (gu8_PbusRxdCnt >= (PBUS_RXDLEN+1))    //��ֹ�������
                gu8_PbusRxdCnt = 0;
        }
    }
}

/*********************************************************************
��������: void Pbus_Init(void)
��    ��: SE�ӿڰ��ʼ��
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-04-17
��    ��:                      ʱ    ��: 
*********************************************************************/
void Pbus_Init(void)
{
    static bool sb_Initlg = false;
    static u32 su32_Time = 0;
    
    if (SE_REQ_IT)    //gb_PbusInitOKFlg�Ƿ�ֹ��ʼ�����Ǳ���յ���ʼ������������Ϊû�г�ʼ���ɹ�
    {   //DP��չ���г�ʼ������
        if (SE_S_RTS == false)
        {   //DP��չ��������� ���ͳ�ʼ������
            if (gb_PbusTxdingFlg == false)
            {   //����û�����ݷ���
                //����պͷ��ͻ���
                if (su32_Time == 0 || SYS10MS - su32_Time >= SE_INIT_INTERVAL)
                {   //��ʼ�����ɹ����ٴγ�ʼ���ļ��
                    sb_Initlg = true;
                    memset(gu8p_RxdArray, 0, PBUS_RXDLEN);
                    memset(gu8p_TxdArray, 0, PBUS_TXDLEN);
                    gu8p_TxdArray[0] = PBUS_ADDRESS; //�ӻ���
                    memcpy(&gu8p_TxdArray[1], &cu8_PbusInit[gu32_GSDType][0], PBUS_TXDLEN - 1);   //��ʼ������
                    Pbus_StartSend(gu8p_TxdArray, PBUS_TXDLEN, PBUS_RXDLEN);   //��ʼ����
                    su32_Time = SYS10MS;
                }
            }
        }
    }
    else
    {
        if (sb_Initlg)
        {   //�Ѿ����͹���ʼ�����ģ�����SE_REQ_IT�Ѿ���͵�ƽ
            //��ִ�е�����֤��SE�ӿڰ��Ѿ���ʼ���ɹ�
            sb_Initlg = false;
            gu8_ReqCom = 0;     //��ʼ������������A����
            gu8_WriteErrID = 0; //��д���ݴ����־
            gu8_PbusSendType = 0;
            gu8_PbusWriteType = 0;
            gu8_16DoTempHi = 0;
            gu8_16DoTempLow = 0;
            BuzzerStart(5);
        }
    }
}

/*********************************************************************
��������: void Pbus_SendDataDeal(u8 *fu8p_Array, u32 fu32_GSDType)
��    ��: PROFIBUS�������ݵĹ��죬���Ǳ��������վ������
˵    ��: 
��ڲ���: fu8p_Array�����������׵�ַ fu32_GSDType��GSD�ļ�����
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-05-04
��    ��:                      ʱ    ��: 
*********************************************************************/
void Pbus_SendDataDeal(u8 *fu8p_Array, u32 fu32_GSDType)
{
    ULDATA UL_Value;
    u8 i;
    
    if (fu32_GSDType == GSD_1)
    {   //GSD1��Ӧ�����ݹ���
        fu8p_Array[0] = gu8_ReqCom;     //������������
        //10AI:
        //Byte0~3:��ǰ����================================================
        if (gs_Setup.ResultHoldSwitch != 0 && gb_DiscWtGetedFlg == true && gb_RunFlg == true)
            UL_Value.ul = gu32_HoldWt;    //��ֵ��������ʱ��ʾ������
        else
            UL_Value.ul = Modbus_WtOrMvData(CurWt);
        //��Ҫ�������վ������,4�ֽ�.��λ������ǰ,��λ�����ں�.
        for (i = 0; i < 4; i++)
            fu8p_Array[1+i] = UL_Value.uc[3-i];
        //Byte4~7:����δ��================================================
        UL_Value.ul = 0;
        //��Ҫ�������վ������,4�ֽ�.��λ������ǰ,��λ�����ں�.
        for (i = 0; i < 4; i++)
            fu8p_Array[5+i] = UL_Value.uc[3-i];
        //Byte8~11:���ۼƴ���================================================
        UL_Value.ul = gu32_SumNum;
        //��Ҫ�������վ������,4�ֽ�.��λ������ǰ,��λ�����ں�.
        for (i = 0; i < 4; i++)
            fu8p_Array[9+i] = UL_Value.uc[3-i];
        //Byte12~15:���ۼ�================================================
        UL_Value.ul = gu32_SumWt;
        //��Ҫ�������վ������,4�ֽ�.��λ������ǰ,��λ�����ں�.
        for (i = 0; i < 4; i++)
            fu8p_Array[13+i] = UL_Value.uc[3-i];
        //Byte16~19:��������================================================
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
            case 20://�䷽�ۼ�����
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
            case 40://�䷽�ۼƴ���
                UL_Value.ul = gu32_SumNum_Rec[gu8_PbusSendType-20];
                break;
            case 41: //������
                UL_Value.ul = gu32_BatchTime;
                break;
            case 42://�䷽��
                UL_Value.ul = gu32_RecNum;
                break;
            case 43://ʣ��������
                UL_Value.ul = gu32_BatchTimeCnt;
                break;
            case 80://F1.1
                UL_Value.ul = gs_Rec.Target;
                break;
            case 81:        //��Ͷ��ǰ��
                UL_Value.ul = gs_Rec.Large;
                break;
            case 82:        //��Ͷ��ǰ��
                UL_Value.ul = gs_Rec.Middle;
                break;
            case 83:        //���ֵ
                UL_Value.ul = gs_Rec.Small;
                break;
            case 84:        //����ֵ
                UL_Value.ul = gs_Rec.NearZero;
                break;
            case 85:        //�Ĵ���ʼ����
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
        //��Ҫ�������վ������,4�ֽ�.��λ������ǰ,��λ�����ں�.
        for (i = 0; i < 4; i++)
            fu8p_Array[17+i] = UL_Value.uc[3-i];
        //24DI+32DI:
        //Byte0:����״̬====================================================
        fu8p_Array[21] |= gs_Cal.Point;
        fu8p_Array[21] |= gs_Cal.Unit << 3;
        fu8p_Array[21] |= (gb_SigWtFlg ? (1 << 5) : 0);
        fu8p_Array[21] |= (gb_StabFlg ? (1 << 6) : 0);
        fu8p_Array[21] |= (gb_OflFlg ? (1 << 7) : 0);
        //Byte1:����δ��====================================================
        fu8p_Array[22] = 0;
        //Byte2:���ۼ�����״̬==============================================
        fu8p_Array[23] |= gu32_SumWtPoint;
        fu8p_Array[23] |= gu32_SumWtUnit << 3;
        //Byte3:ϵͳ״̬1===================================================
              fu8p_Array[24] = 0;
/*����    */  fu8p_Array[24] |= (gb_RunFlg ? (1 << 0) : 0);
/*����ǰ  */  fu8p_Array[24] |= ((gu32_Process == T1PROCESS && gb_RunFlg == true) ? (1 << 1) : 0);
/*��Ͷ    */  fu8p_Array[24] |= (Sp_Disc_State(SP1_STATE) ? (1 << 2) : 0);
/*��Ͷ    */  fu8p_Array[24] |= (Sp_Disc_State(SP2_STATE) ? (1 << 3) : 0);
/*СͶ    */  fu8p_Array[24] |= (Sp_Disc_State(SP3_STATE) ? (1 << 4) : 0);
/*��ֵ    */  fu8p_Array[24] |= (gb_CompFlg ? (1 << 5) : 0);
/*��Ƿ��  */  fu8p_Array[24] |= ((gb_OverFlg || gb_UnderFlg || gb_OUOverFlg || gb_OUUnderFlg) ? (1 << 6) : 0);
/*����    */  fu8p_Array[24] |= (gb_AlarmFlg ? (1 << 7) : 0);
        //Byte4:ϵͳ״̬2===================================================
              fu8p_Array[25] = 0;
/*�д�    */  fu8p_Array[25] |= (gb_PackFlg ? (1 << 0) : 0);
/*�Ĵ�    */  fu8p_Array[25] |= ((gb_FlapBagOutFlg || gb_FlapAllHighFlg) ? (1 << 1) : 0);
/*ж��    */  fu8p_Array[25] |= (Sp_Disc_State(DISC_STATE) ? (1 << 2) : 0);
/*����    */  fu8p_Array[25] |= (gb_NzFlg ? (1 << 3) : 0);
/*����    */  fu8p_Array[25] |= (gb_FillFlg ? (1 << 4) : 0);
/*�������*/  fu8p_Array[25] |= (gb_BatchOverFlg ? (1 << 5) : 0);
/*ȱ��    */  fu8p_Array[25] |= (gb_LackFlg ? (1 << 6) : 0);
/*ж�����*/  fu8p_Array[25] |= (gb_DiscShakeFlg ? (1 << 7) : 0);
        //Byte5:ϵͳ״̬3===================================================
              fu8p_Array[26] = 0;
/*����    */  fu8p_Array[26] |= (gb_PrintCodeOutFlg ? (1 << 0) : 0);
/*����    */  fu8p_Array[26] |= (gb_Sp3Flg ? (1 << 1) : 0);
/*���    */  fu8p_Array[26] |= (gb_LiftBagFlg ? (1 << 2) : 0);
/*�ƴ�    */  fu8p_Array[26] |= (gb_PushBagFlg ? (1 << 3) : 0);
/*���߼��*/  fu8p_Array[26] |= (gb_ConnectCheckFlg ? (1 << 4) : 0);
/*����д��*/  fu8p_Array[26] |= gu8_WriteErrID << 6;
        //Byte6:������������
        fu8p_Array[27] |= gu8_PbusSendType;
        Pbus_StartSend(fu8p_Array, SE_IN_LEN_GSD1, (gu8_ReqCom & 0x01 ? SE_B_LEN_GSD1 : SE_A_LEN_GSD1));
    }
    else
    {   //GSD2��Ӧ�����ݹ���
        fu8p_Array[0] = gu8_ReqCom;     //������������
        /*16DI��
        DI9:д�����:���ݳ���
        DI8:д�����:д������������
        DI7~DI5:���Ͻ���
        000:  ֹͣ
        001:  ����ǰ
        010:  ���
        011:  �м�
        100:  ����
        101:  ��ֵ
        110:  �Ĵ����ɴ�
        111:  ���������
        DI4~DI3:ë��/����
        00: ë��
        01: ����
        10: ����(����,�ۼ�������ë����֮��)
        11 : ����,����
        DI2: �Ǳ����״̬��0������1���
        DI1: �Ǳ��ȶ�״̬��0���ȣ�1�ȶ�
        DI0: �Ǳ����״̬��0���㣬1���*/
        fu8p_Array[1] = gu8_WriteErrID; //д���ݴ�������
        gu8_WriteErrID = 0;     //�����������
        fu8p_Array[2] |= (gb_ZeroPosition ? 0x01 : 0x00);   //��λ
        fu8p_Array[2] |= (gb_StabFlg ? 0x02 : 0x00);        //�ȶ�
        fu8p_Array[2] |= ((gb_OflFlg || gb_AdOflFlg) ? 0x04 : 0x00);        //���
        fu8p_Array[2] |= (gb_DispNetFlg ? 0x08 : 0x00);        //����
        fu8p_Array[2] |= (gu8_PbusSendType ? 0x10 : 0x00);        //��������
        if (gb_RunFlg)
        {
            if (gb_BatchOverFlg)    //�������
                fu8p_Array[2] |= 0xE0;
            else if (gu32_Process == T1PROCESS) //����ǰ
                fu8p_Array[2] |= 0x20;
            else if (gu32_Process == LARGEPROCESS)  //���
                fu8p_Array[2] |= 0x40;
            else if (gu32_Process == MIDDLEPROCESS) //�м�
                fu8p_Array[2] |= 0x60;
            else if (gu32_Process == SMALLPROCESS)  //����
                fu8p_Array[2] |= 0x80;
            else if (gu32_Process == T3PROCESS || gu32_Process == T4PROCESS)    //��ֵ
            {
                if ((gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B) && (gb_FlapWaitFlg || gb_FlapStartFlg))  //�޶� �Ĵ�
                    fu8p_Array[2] |= 0xC0;
                else
                    fu8p_Array[2] |= 0xA0;
            }
            else if (gu32_Process == FALLPROCESS || gu32_Process == T5PROCESS)
            {
                if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)    //�ж� ж��
                    fu8p_Array[2] |= 0xC0;
            }
        }
        /*
        1Byte In: (����״̬)
        D7:����λ,1:��-��; 0:��+��
        D6~D5:2AI���ݵ�λ.
        00:g
        01:kg
        10:t
        11:����
        D4~D2:2AI���ݵ�С����
        000:��С����
        001:һλС����,0.0
        010:��λС����,0.00
        011��λС����,0.000
        100:��λС����,0.0000
        ����:����.
        D1~D0:��ǰ���͵�2AI��������
        00: ��ǰ����
        01: �ۼ�����
        10: �ۼƴ���
        11: ����*/
        switch (gu8_PbusSendType)
        {
            case 1: //���ۼ�����
                //����
                fu8p_Array[3] |= gu32_SumWtUnit << 5;  //��λ
                fu8p_Array[3] |= gu32_SumWtPoint << 2;  //С����
                UL_Value.ul = gu32_SumWt;  //�ۼ�����
                break;
            case 2: //���ۼƴ���
                //����
                fu8p_Array[3] |= 3 << 5;  //�ۼƴ���û�е�λ���ɳ������3ʱû�е�λ�������³���3����lb
                fu8p_Array[3] |= 0 << 2;  //С����
                UL_Value.ul = gu32_SumNum;  //�ۼƴ���
                break;
            case 0:
            default:    //��ǰ����
                fu8p_Array[3] |= (gb_SigWtFlg ? 0x80 : 0x00);   //����
                fu8p_Array[3] |= gs_Cal.Unit << 5;  //��λ
                fu8p_Array[3] |= gs_Cal.Point << 2;  //С����
                UL_Value.ul = gu32_Wt;  //��ǰ����
                break;
        }
        fu8p_Array[3] |= (gu8_PbusSendType & 0x03);
        //2AI : ��Ҫ�������վ������,4�ֽ�.��λ������ǰ,��λ�����ں�.
        for (i = 0; i < 4; i++)
        {
            fu8p_Array[4+i] = UL_Value.uc[3-i];
        }
        Pbus_StartSend(fu8p_Array, SE_IN_LEN_GSD2, (gu8_ReqCom & 0x01 ? SE_B_LEN_GSD2 : SE_A_LEN_GSD2));
    }
}

/*********************************************************************
��������: void Pbus_Send(void)
��    ��: �Ǳ�PROFIBUS�������ݴ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-05-04
��    ��:                      ʱ    ��: 
*********************************************************************/
void Pbus_Send(void)
{
    if (SE_REQ_IT == false && SE_S_RTS == false && gb_PbusTxdingFlg == false)
    {   //SE�ӿڰ��ʼ���ɹ�������������գ�����û�з��ͺͽ��� ��ô�ſ��Դ���������
        if (gb_PbusRxdReadyFlg == false && gb_PbusRxdingFlg == false)
        {
            //���շ�����
            memset(gu8p_RxdArray, 0, PBUS_RXDLEN);
            memset(gu8p_TxdArray, 0, PBUS_TXDLEN);
            //���췢�����ݵ�����
            Pbus_SendDataDeal(gu8p_TxdArray, gu32_GSDType);
        }
        else
        {
            if (SYS10MS - gu32_TimeTemp >= SE_COM_INTERVAL)
            {   //���ͺ󳬹�SE_COM_INTERVALʱ���û�н��յ��������������·���
                gb_PbusTxdingFlg = false;
                gb_PbusRxdReadyFlg = false;
                gb_PbusRxdingFlg = false;
            }
        }
    }
}

/*********************************************************************
��������: u8 Pbus_ReciveCheck(u8 *fu8p_Array, u32 fu32_GSDType, u8 fu8_ReqCom)
��    ��: 
˵    ��: 
��ڲ���: gu8_RxdArray�����������׵�ַ fu32_GSDType��GSD�ļ����� fu8_ReqCom��������������
�� �� ֵ: 
!=0���������
0x84 ���ȴ���
0x81 У���A��
0x82 У���B��
0x85 ����A���ĸ�ʽ��
0x86 ����B���ĸ�ʽ��
0x90 ���յ��ǳ�ʼ�����ص�����,������ȷ
0x91 ���յ��ǳ�ʼ�����ص�����,IO�������ݴ���
0x92 ���յ��ǳ�ʼ�����ص�����,IO�������ݸ�������20
0x93 ���յ��ǳ�ʼ�����ص�����,վ��ַ����126
0x94 ���յ��ǳ�ʼ�����ص�����,�����ַ�У��ʹ�
0x95 ���յ��ǳ�ʼ�����ص�����,�����ַ���żУ�����
0x96 ���յ��ǳ�ʼ�����ص�����,�����ַ�����<49

D2~D1��00���ӿڰ�����û��屨���޴�
       01���ӿڰ�����û��屨���ַ���żУ���
       10���ӿڰ�����û��屨����У��ʹ�
       11������

 0����������
��    ��: �Ժ���               ʱ    ��: 2014-05-05
��    ��:                      ʱ    ��: 
*********************************************************************/
u8 Pbus_ReciveCheck(u8 *fu8p_Array, u32 fu32_GSDType, u8 fu8_ReqCom)
{
    u8 lu8_ErrCode = 0;
    u8 lu8_SeLen;
    
    if (gu8_PbusRxdCnt == PBUS_RXDLEN)
    {   //���յ��ǳ�ʼ�����ر���
        lu8_ErrCode = gu8p_RxdArray[1];
        lu8_ErrCode |= 0x90;
    }
    else
    {   //�жϽ��յ���A��B����
        if (fu32_GSDType == GSD_1)
            lu8_SeLen = ((fu8_ReqCom & 0x01) ? SE_B_LEN_GSD1 : SE_A_LEN_GSD1);
        else
            lu8_SeLen = ((fu8_ReqCom & 0x01) ? SE_B_LEN_GSD2 : SE_A_LEN_GSD2);
                
        if (gu8_PbusRxdCnt != lu8_SeLen)
        {   //���ȴ���
            lu8_ErrCode = 0x84;
        }
        else
        {
            if (fu8p_Array[lu8_SeLen-1] == Pbus_Checksum(fu8p_Array, lu8_SeLen - 1))
            {   //У�����ȷ
                if ((fu8p_Array[0] & 0x80) != ((fu8_ReqCom & 0x01) ? 0x80 : 0))
                {   //����A��ʽ���󣬴�������B����
                    lu8_ErrCode = ((fu8_ReqCom & 0x01) ? 0x86 : 0x85);
                }
                else if ((fu8p_Array[0] & 0x06) != 0)
                {   //SE�����û������д���
                    lu8_ErrCode = fu8p_Array[0];
                }
                else
                {   //�޴��󣬸�ֵ������������
                    gu8_ReqCom = ((fu8_ReqCom & 0x01) ? 0x02 : (fu8p_Array[0] & 0x01));
                }
            }
            else
            {   //У��ʹ���
                lu8_ErrCode = ((fu8_ReqCom & 0x01) ? 0x82 : 0x81);
            }
        }
    }
    return lu8_ErrCode;
}

/*********************************************************************
��������: void Pbus_ReciveDataDeal(u8 *fu8p_Array, u32 fu32_GSDType)
��    ��: 
˵    ��: 
��ڲ���: fu8p_Array�����������׵�ַ fu32_GSDTypee��GSD�ļ����� fu8_ReqCom��������������
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-05-06
��    ��:                      ʱ    ��: 
*********************************************************************/
void Pbus_ReciveDataDeal(u8 *fu8p_Array, u32 fu32_GSDType)
{
    ULDATA UL_Value;
    u8 i;
    u32 lu32_Temp;
    const u8 cu8_DivTab[] = {1,2,5,10,20,50};
    
    UL_Value.ul = 0;
    
    if (SE_LINK)   //PROFIBUS�����Ѿ���ͨ���ܽ��������������
        return;
    
    if (fu32_GSDType == GSD_1)  //GSD1����
    {
        if (fu8p_Array[0] & 0x80)  //B���Ĵ��� �û�����
        {
            if (gu32_FollowMasterSwitch == 0)   //���������궨����������վ����
                return;
            //�ֶ�ֵ
            if (gs_Cal.Div != gu8p_RxdArray[1])
            {
                lu32_Temp = FRAM_ReadDW(FMAddr_CalDiv);
                //��Ҫ�����ֶ�,��׷�����
                gs_Cal.ADK *= (float)gu8p_RxdArray[1];    //�·ֶ�ֵ=ԭ�ֶ�ֵ*new dd/old dd
                gs_Cal.ADK /= (float)gs_Cal.Div;
                FRAM_WriteFloat(FMAddr_CalADK, gs_Cal.ADK);
            
                //����׷�����:�����ֶ�ֵ��,�����0,��Ȼ��ʾ��
                gu32_FloatZero *= gs_Cal.Div;
                gu32_FloatZero /= gu8p_RxdArray[1];
                //�����µķֶ�ֵ
                gs_Cal.Div = gu8p_RxdArray[1];
                FRAM_WriteDW(FMAddr_CalDiv, gs_Cal.Div);

                //�ֶ�ֵ��С���������̴���
                if (gs_Cal.Div < lu32_Temp)                        //�ֶ�ֵ�ɴ��Сʱ���������Ҫ����
                {
                    if (gs_Cal.Cap > (gs_Cal.Div * 100000))
                    {
                        gs_Cal.Cap = gs_Cal.Div * 100000;
                        FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                    }
                }
                else if (gs_Cal.Div > gs_Cal.Cap)            //�޸ķֶȺ�����С�ڷֶȣ����޸�����Ϊ�ֶ�ֵ
                {
                    gs_Cal.Cap = gs_Cal.Div;
                    FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                }
            }
            //��λ
            if (gs_Cal.Unit != gu8p_RxdArray[2])
            {
                gs_Cal.Unit = gu8p_RxdArray[2];
                FRAM_WriteDW(FMAddr_CalUnit, gs_Cal.Unit);
                ClrSum();
            }
            //С����
            if (gs_Cal.Point != gu8p_RxdArray[3])
            {
                gs_Cal.Point = gu8p_RxdArray[3];
                FRAM_WriteDW(FMAddr_CalPoint, gs_Cal.Point);
                ClrSum();
            }
            //���ֶ���
            UL_Value.uc[3] = gu8p_RxdArray[4];
            UL_Value.uc[2] = gu8p_RxdArray[5];
            UL_Value.uc[1] = gu8p_RxdArray[6];
            UL_Value.uc[0] = gu8p_RxdArray[7];
            UL_Value.ul *= gs_Cal.Div;  //�õ��������
            if (gs_Cal.Cap != UL_Value.ul)
            {
                gs_Cal.Cap = UL_Value.ul;
                FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
            }
            //��װ�ӹ���ģʽ
            if (gu8p_RxdArray[8] != gs_Setup.TareMode)
            {
                gs_Setup.TareMode = gu8p_RxdArray[8];
                FRAM_WriteDW(FMAddr_TareMode, gs_Setup.TareMode);
            }
            //�޼�������װģʽ��
            if (gu8p_RxdArray[9] != gs_Setup.GorNMode)
            {
                gs_Setup.GorNMode = gu8p_RxdArray[9];
                FRAM_WriteDW(FMAddr_GorNMode, gs_Setup.GorNMode);
            }
            //���Ϸ�ʽ
            if (gu8p_RxdArray[10] != gs_Setup.FeedMode)
            {
                gs_Setup.FeedMode = gu8p_RxdArray[10];
                FRAM_WriteDW(FMAddr_FeedMode, gs_Setup.FeedMode);
            }
            //�Զ�������
            if (gu8p_RxdArray[11] != gs_Setup.AutoClrZeroInterval)
            {
                gs_Setup.AutoClrZeroInterval = gu8p_RxdArray[11];
                FRAM_WriteDW(FMAddr_AutoClrZeroInterval, gs_Setup.AutoClrZeroInterval);
            }
            //���ȳ�ʱʱ��
            UL_Value.uc[1] = gu8p_RxdArray[12];
            UL_Value.uc[0] = gu8p_RxdArray[13];
            UL_Value.uc[2] = 0;
            UL_Value.uc[3] = 0;
            if (UL_Value.ul != gs_Setup.JudgeStabOverTime)
            {
                gs_Setup.JudgeStabOverTime = UL_Value.ul;
                FRAM_WriteDW(FMAddr_JudgeStabOverTime, gs_Setup.JudgeStabOverTime);
            }
            //����ʾ����
            if (gu8p_RxdArray[14] != gs_Setup.AuxDispType)
            {
                gs_Setup.AuxDispType = gu8p_RxdArray[14];
                FRAM_WriteDW(FMAddr_AuxDispType, gs_Setup.AuxDispType);
            }
            //���㷶Χ
            if (gu8p_RxdArray[15] != gs_Setup.ClrZeroRng)
            {
                gs_Setup.ClrZeroRng = gu8p_RxdArray[15];
                FRAM_WriteDW(FMAddr_ClrZeroRng, gs_Setup.ClrZeroRng);
            }
            //���ȷ�Χ
            if (gu8p_RxdArray[16] != gs_Setup.JudgeStabRng)
            {
                gs_Setup.JudgeStabRng = gu8p_RxdArray[16];
                FRAM_WriteDW(FMAddr_JudgeStabRng, gs_Setup.JudgeStabRng);
            }
            //�����ٷ�Χ
            if (gu8p_RxdArray[17] != gs_Setup.TraceZeroRng)
            {
                gs_Setup.TraceZeroRng = gu8p_RxdArray[17];
                FRAM_WriteDW(FMAddr_TraceZeroRng, gs_Setup.TraceZeroRng);
            }
            //�˲�����
            if (gu8p_RxdArray[18] != gs_Setup.AdFilterLevel)
            {
                gs_Setup.AdFilterLevel = gu8p_RxdArray[18];
                FRAM_WriteDW(FMAddr_AdFilterLevel, gs_Setup.AdFilterLevel);
            }
            //ж���˲�����
            if (gu8p_RxdArray[19] != gs_Setup.AdFilterLevelDisc)
            {
                gs_Setup.AdFilterLevelDisc = gu8p_RxdArray[19];
                FRAM_WriteDW(FMAddr_AdFilterLevelDisc, gs_Setup.AdFilterLevelDisc);
            }
            //��ֵ�˲�����
            if (gu8p_RxdArray[20] != gs_Setup.AdFilterLevelComp)
            {
                gs_Setup.AdFilterLevelComp = gu8p_RxdArray[20];
                FRAM_WriteDW(FMAddr_AdFilterLevelComp, gs_Setup.AdFilterLevelComp);
            }
            //AD�����ٶ�
            if (gu8p_RxdArray[21] != gs_Setup.AdSamplingRate)
            {
                gs_Setup.AdSamplingRate = gu8p_RxdArray[21];
                FRAM_WriteDW(FMAddr_AdSamplingRate, gs_Setup.AdSamplingRate);
                gb_AdOKFlg = AD_Init(gs_Setup.AdSamplingRate);
            }
            //�ϵ��Զ�����
            if (gu8p_RxdArray[22] != gs_Setup.PowerOnZeroSwitch)
            {
                gs_Setup.PowerOnZeroSwitch = gu8p_RxdArray[22];
                FRAM_WriteDW(FMAddr_PowerOnZeroSwitch, gs_Setup.PowerOnZeroSwitch);
            }
            //�ֶ�ж���ۼƿ���
            if (gu8p_RxdArray[23] != gs_Setup.DiscSumSwitch)
            {
                gs_Setup.DiscSumSwitch = gu8p_RxdArray[23];
                FRAM_WriteDW(FMAddr_DiscSumSwitch, gs_Setup.DiscSumSwitch);
            }
            //��ֵ�������ֿ���
            if (gu8p_RxdArray[24] != gs_Setup.ResultHoldSwitch)
            {
                gs_Setup.ResultHoldSwitch = gu8p_RxdArray[24];
                FRAM_WriteDW(FMAddr_ResultHoldSwitch, gs_Setup.ResultHoldSwitch);
            }
            //�ۼƲ�ѯ���뿪��
            if (gu8p_RxdArray[25] != gs_Setup.SumCheckCodeSwitch)
            {
                gs_Setup.SumCheckCodeSwitch = gu8p_RxdArray[25];
                FRAM_WriteDW(FMAddr_PowerOnZeroSwitch, gs_Setup.SumCheckCodeSwitch);
            }
            //�ۼ�������뿪��
            if (gu8p_RxdArray[26] != gs_Setup.SumClrCodeSwitch)
            {
                gs_Setup.SumClrCodeSwitch = gu8p_RxdArray[26];
                FRAM_WriteDW(FMAddr_SumClrCodeSwitch, gs_Setup.SumClrCodeSwitch);
            }
            //�ۼƷ�Χ��չ����
            if (gu8p_RxdArray[27] != gs_Setup.SumExtendSwitch)
            {
                gs_Setup.SumExtendSwitch = gu8p_RxdArray[27];
                FRAM_WriteDW(FMAddr_SumExtendSwitch, gs_Setup.SumExtendSwitch);
            }
            //˫�޶���ͬʱ�ɴ���
            if (gu8p_RxdArray[28] != gs_Setup.No2PackOffTwoSwitch)
            {
                gs_Setup.No2PackOffTwoSwitch = gu8p_RxdArray[28];
                FRAM_WriteDW(FMAddr_No2PackOffTwoSwitch, gs_Setup.No2PackOffTwoSwitch);
            }
            //���ͻ�������ʱʱ�䣺2Byte
            UL_Value.uc[1] = gu8p_RxdArray[29];
            UL_Value.uc[0] = gu8p_RxdArray[30];
            UL_Value.uc[2] = 0;
            UL_Value.uc[3] = 0;
            if (UL_Value.ul != gs_Setup.No2ConveyorDelayTime)
            {
                gs_Setup.No2ConveyorDelayTime = UL_Value.ul;
                FRAM_WriteDW(FMAddr_No2ConveyorDelayTime, gs_Setup.No2ConveyorDelayTime);
            }
            //���ͻ�����ʱ�䣺2Byte
            UL_Value.uc[1] = gu8p_RxdArray[31];
            UL_Value.uc[0] = gu8p_RxdArray[32];
            UL_Value.uc[2] = 0;
            UL_Value.uc[3] = 0;
            if (UL_Value.ul != gs_Setup.No2ConveyorRunTime)
            {
                gs_Setup.No2ConveyorRunTime = UL_Value.ul;
                FRAM_WriteDW(FMAddr_No2ConveyorRunTime, gs_Setup.No2ConveyorRunTime);
            }
            //�ٴμд���ʱ��������ʱ�䣺2Byte
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
        else    //A���Ĵ��� ��վ������Ǳ�
        {   /* 2AO���������� 4�ֽ�.��λ������ǰ,��λ�����ں�.*/
            for (i = 0; i < 4; i++)
            {   //��ȡ2AO��ֵ
                UL_Value.uc[3-i] = gu8p_RxdArray[1+i];
            }
            // 32DO: 
            /* Byte0:����ֵ���ͣ�����2AO��������������*/
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
                    else if (UL_Value.ul == 0)    //д��0�������Ӧ���䷽�ۼƴ���������
                    {
                        ClrSum_Rec(gu8p_RxdArray[5]);
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 41: //������
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
                case 42://�䷽��
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
                            if (gb_RunFlg == false)     //ֹͣ״̬�������޸��䷽
                            {
                                gs_Rec.Target = UL_Value.ul;   
                                FRAM_WriteDW(FMAddr_Target(gu32_RecNum), gs_Rec.Target);
                            }
                            else        //����ʱ�޸�
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
                case 81:        //��Ͷ��ǰ��
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Large)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Large = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Large(gu32_RecNum), gs_Rec.Large);
                            }
                            else        //����ʱ�޸�
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
                case 82:        //��Ͷ��ǰ��
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Middle)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Middle = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Middle(gu32_RecNum), gs_Rec.Middle);
                            }
                            else        //����ʱ�޸�
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
                case 83:        //���ֵ
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Small)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Small = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Small(gu32_RecNum), gs_Rec.Small);
                            }
                            else        //����ʱ�޸�
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
                case 84:        //����ֵ
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
                case 85:        //�Ĵ���ʼ����
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.FlapBagWt)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.FlapBagWt = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_FlapBagWt(gu32_RecNum), gs_Rec.FlapBagWt);
                            }
                            else        //����ʱ�޸�
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
                    
                case 150:        //��ӡ���ۼ�
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
                case 151:        //�䷽���ñ��ӡ
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 21)
                    {
                        if (UL_Value.ul == 0)
                        {   //��ӡ��ǰ�䷽���ñ�
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = false;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = false;
                            PrintStart(PRINT_REC_SET, gu32_RecNum, 0);
                        }
                        else if (UL_Value.ul >= 1 && UL_Value.ul <= 20)
                        {   //��ӡ��Ӧ�䷽���ñ�
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = false;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = false;
                            PrintStart(PRINT_REC_SET, UL_Value.ul, 0);
                        }
                        else
                        {   //��ӡ�����䷽���ñ�
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = true;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = true;
                            gu8_Item = PRINT_REC_SET;
                            //��ӡ����Ŀ��ֵ��Ϊ0���䷽
                            if (gs_Rec.Target != 0)
                            {
                                gu32_PrintRecNumStart = gu32_RecNum;    //������ʼ�䷽�ţ������ж��Ƿ������䷽����ӡ���
                                PrintStart(PRINT_REC_SET, gu32_RecNum, 0);  //�ӵ�ǰ�䷽��ʼ��ӡ
                            }
                            else
                            {   //��ǰ�䷽Ŀ��ֵΪ0����ȡ��һ��Ŀ��ֵ��Ϊ0���䷽��
                                gu32_PrintRecNumStart = NextRecNum(gu32_RecNum);
                                if (gu32_PrintRecNumStart != gu32_RecNum)
                                {   //�����ȡ������һ����Ϊ0���䷽�ź�Ŀǰ�䷽����ȣ����������䷽Ŀ��ֵ��Ϊ0������ӡ
                                    PrintStart(PRINT_REC_SET, gu32_PrintRecNumStart, 0);  //��Ŀ��ֵ��Ϊ0���䷽�ſ�ʼ��ӡ
                                }
                            }
                        }
                        gu8_WriteErrID = 0;
                    }
                    else
                        gu8_WriteErrID = 1;
                    break;
                case 152:        //�䷽�ۼƱ��ӡ
                    if (gb_RunFlg)
                        gu8_WriteErrID = 2;
                    else if (UL_Value.ul  <= 21)
                    {
                        if (UL_Value.ul == 0)
                        {   //��ӡ��ǰ�䷽�ۼƱ���
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = false;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = false;
                            PrintStart(PRINT_REC_REPORT, gu32_RecNum, 0);
                        }
                        else if (UL_Value.ul >= 1 && UL_Value.ul <= 20)
                        {   //��ӡ��Ӧ�䷽�ۼƱ���
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = false;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = false;
                            PrintStart(PRINT_REC_REPORT, UL_Value.ul, 0);
                        }
                        else
                        {   //��ӡ�����䷽�ۼƱ���
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = true;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = true;
                            gu8_Item = PRINT_REC_REPORT;
                            //��ӡ����Ŀ��ֵ��Ϊ0���䷽
                            if (gs_Rec.Target != 0)
                            {
                                gu32_PrintRecNumStart = gu32_RecNum;    //������ʼ�䷽�ţ������ж��Ƿ������䷽����ӡ���
                                PrintStart(PRINT_REC_REPORT, gu32_RecNum, 0);  //�ӵ�ǰ�䷽��ʼ��ӡ
                            }
                            else
                            {   //��ǰ�䷽Ŀ��ֵΪ0����ȡ��һ��Ŀ��ֵ��Ϊ0���䷽��
                                gu32_PrintRecNumStart = NextRecNum(gu32_RecNum);
                                if (gu32_PrintRecNumStart != gu32_RecNum)
                                {   //�����ȡ������һ����Ϊ0���䷽�ź�Ŀǰ�䷽����ȣ����������䷽Ŀ��ֵ��Ϊ0������ӡ
                                    PrintStart(PRINT_REC_REPORT, gu32_PrintRecNumStart, 0);  //��Ŀ��ֵ��Ϊ0���䷽�ſ�ʼ��ӡ
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
            //Byte1:�����������ͣ�����10AI�еġ��������ݡ����͵���������
            gu8_PbusSendType = gu8p_RxdArray[6];
            //Byte2:ִ�в���1
            for (i = 0; i <= 7; i++ )
            {
                if (((gu8p_RxdArray[7] & (0x01 << i)) != 0) && ((gu8_16DoTempHi & (0x01 << i)) == 0))
                {   //�ɻ���״̬Ϊ0���½��յ���״̬Ϊ1ʱ����Ӧ��ز�����Ҳ���Ǽ����0��1������
                    switch(i)
                    {
                        case 0://ֹͣ
                            Clr_Alarm();
                            if (gb_RunFlg)
                                gb_EndFlg = true;
                            break;
                        case 1://����
                            RunStopInit(true);
                            break;
                        case 2://��ͣ
                            Clr_Alarm();
                            RunStopInit(false);
                            break;
                        case 3://����
                            if (gb_RunFlg == false)
                                gb_ZeroKeyFlg = true;
                            break;
                        case 4://�屨��
                            Clr_Alarm();
                            break;
                        case 5://ѡ�䷽
                            if (gb_RunFlg == false)
                            {
                                gu32_RecNum = NextRecNum(gu32_RecNum);
                                FRAM_WriteDW(FMAddr_RecNum, gu32_RecNum);
                                Read_RecipeData(gu32_RecNum);
                            }
                            break;
                        case 6://���ɴ�
                            PackInput();
                            break;
                        case 7://�ֶ�ж��
                            if (gb_RunFlg == false)
                                ManualDisc(2);
                            break;
                        default:
                            break;
                    }
                }
                if (((gu8p_RxdArray[8] & (0x01 << i)) != 0) && ((gu8_16DoTempLow & (0x01 << i)) == 0))
                {   //�ɻ���״̬Ϊ0���½��յ���״̬Ϊ1ʱ����Ӧ��ز�����Ҳ���Ǽ����0��1������
                    switch(i)
                    {
                        case 0://�ֶ���Ͷ
                            if (gb_RunFlg == false)
                                ManualSp123(2);
                            break;
                        case 1://�ֶ�����һ��
                            if (gb_RunFlg == false)
                            {
                                gb_GoFlg = true;
                                RunStopInit(true);
                            }
                            break;
                        case 2://������ۼ����ݣ�����ÿ���䷽���ۼ����ݣ�
                            if (gb_RunFlg == false)
                                ClrSum();
                            break;
                        case 3://�ֶ�СͶ
                            if (gb_RunFlg == false)
                                ManualSp3(2);
                            break;
                        case 7://���߼����Ч
                            gb_ConnectCheckFlg = true;
                            break;
                        default:
                            break;
                    }
                }
                
                if (i == 7)
                {
                    if ((gu8p_RxdArray[8] & (0x01 << i)) == 0)
                    {   //���߼����Ч
                        gb_ConnectCheckFlg = false;
                    }
                    else
                    {   //���߼����Ч
                        gb_ConnectCheckFlg = true;
                    }
                }
            }
            gu8_16DoTempHi = gu8p_RxdArray[7];
            gu8_16DoTempLow = gu8p_RxdArray[8];  //����16DO���ε�״̬
        }
    }
    else    //GSD2����
    {
        if (fu8p_Array[0] & 0x80)  //B���Ĵ��� �û�����
        {
            if (gu32_FollowMasterSwitch == 0)   //���������궨����������վ����
                return;
            //�ֶ�ֵ
            if (gs_Cal.Div != cu8_DivTab[gu8p_RxdArray[1]])
            {
                lu32_Temp = FRAM_ReadDW(FMAddr_CalDiv);
                //��Ҫ�����ֶ�,��׷�����
                gs_Cal.ADK *= (float)cu8_DivTab[gu8p_RxdArray[1]];    //�·ֶ�ֵ=ԭ�ֶ�ֵ*new dd/old dd
                gs_Cal.ADK /= (float)gs_Cal.Div;
                FRAM_WriteFloat(FMAddr_CalADK, gs_Cal.ADK);
            
                //����׷�����:�����ֶ�ֵ��,�����0,��Ȼ��ʾ��
                gu32_FloatZero *= gs_Cal.Div;
                gu32_FloatZero /= cu8_DivTab[gu8p_RxdArray[1]];
                //�����µķֶ�ֵ
                gs_Cal.Div = cu8_DivTab[gu8p_RxdArray[1]];
                FRAM_WriteDW(FMAddr_CalDiv, gs_Cal.Div);

                //�ֶ�ֵ��С���������̴���
                if (gs_Cal.Div < lu32_Temp)                        //�ֶ�ֵ�ɴ��Сʱ���������Ҫ����
                {
                    if (gs_Cal.Cap > (gs_Cal.Div * 100000))
                    {
                        gs_Cal.Cap = gs_Cal.Div * 100000;
                        FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                    }
                }
                else if (gs_Cal.Div > gs_Cal.Cap)            //�޸ķֶȺ�����С�ڷֶȣ����޸�����Ϊ�ֶ�ֵ
                {
                    gs_Cal.Cap = gs_Cal.Div;
                    FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                }
            }
            //��λ
            if (gs_Cal.Unit != gu8p_RxdArray[2])
            {
                gs_Cal.Unit = gu8p_RxdArray[2];
                FRAM_WriteDW(FMAddr_CalUnit, gs_Cal.Unit);
                ClrSum();
            }
            //С����
            if (gs_Cal.Point != gu8p_RxdArray[3])
            {
                gs_Cal.Point = gu8p_RxdArray[3];
                FRAM_WriteDW(FMAddr_CalPoint, gs_Cal.Point);
                ClrSum();
            }
            //���ֶ���
            UL_Value.uc[1] = gu8p_RxdArray[4];
            UL_Value.uc[0] = gu8p_RxdArray[5];
            UL_Value.ul *= gs_Cal.Div;  //�õ��������
            if (gs_Cal.Cap != UL_Value.ul)
            {
                gs_Cal.Cap = UL_Value.ul;
                FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
            }
            //�����ٷ�Χ
            if (gu8p_RxdArray[6] != gs_Setup.TraceZeroRng)
            {
                gs_Setup.TraceZeroRng = gu8p_RxdArray[6];
                FRAM_WriteDW(FMAddr_TraceZeroRng, gs_Setup.TraceZeroRng);
            }
            //���ȷ�Χ
            if (gu8p_RxdArray[7] != gs_Setup.JudgeStabRng)
            {
                gs_Setup.JudgeStabRng = gu8p_RxdArray[7];
                FRAM_WriteDW(FMAddr_JudgeStabRng, gs_Setup.JudgeStabRng);
            }
            //���㷶Χ
            if (gu8p_RxdArray[8] != gs_Setup.ClrZeroRng)
            {
                gs_Setup.ClrZeroRng = gu8p_RxdArray[8];
                FRAM_WriteDW(FMAddr_ClrZeroRng, gs_Setup.ClrZeroRng);
            }
            //�ϵ��Զ�����
            if (gu8p_RxdArray[9] != gs_Setup.PowerOnZeroSwitch)
            {
                gs_Setup.PowerOnZeroSwitch = gu8p_RxdArray[9];
                FRAM_WriteDW(FMAddr_PowerOnZeroSwitch, gs_Setup.PowerOnZeroSwitch);
            }
            //�Զ�������
            if (gu8p_RxdArray[10] != gs_Setup.AutoClrZeroInterval)
            {
                gs_Setup.AutoClrZeroInterval = gu8p_RxdArray[10];
                FRAM_WriteDW(FMAddr_AutoClrZeroInterval, gs_Setup.AutoClrZeroInterval);
            }
            //�˲�����
            if (gu8p_RxdArray[11] != gs_Setup.AdFilterLevel)
            {
                gs_Setup.AdFilterLevel = gu8p_RxdArray[11];
                FRAM_WriteDW(FMAddr_AdFilterLevel, gs_Setup.AdFilterLevel);
            }
            //����ģʽ  ע��˴���Э��˵�����Ǳ��ڲ��������෴�ģ��Ǳ�0�ж�1�޶�
            if ((!gu8p_RxdArray[12]) != gs_Setup.TareMode)
            {
                gs_Setup.TareMode = (!gu8p_RxdArray[12]);
                FRAM_WriteDW(FMAddr_TareMode, gs_Setup.TareMode);
                TareModeChange(gs_Setup.TareMode);
            }
        }
        else    //A���Ĵ��� ��վ������Ǳ�
        {/* 16DO��
            DO15~DO11: ����.
            DO9 : ����λ��0��Ϊ1ʱ,ֹͣ״̬��,ȡ���ֶ�����,�򷵻ط���Ϣ.
            DO8 : ����λ��0��Ϊ1ʱ,ֹͣ״̬��,����ۼ�ֵ,������Ӧ.
            DO7 : ����λ��0��Ϊ1ʱ,ֹͣ״̬��,ȡ���ֶ�СͶ,���򷵻ط���Ϣ
            DO6 : ����λ��0��Ϊ1ʱ,�ɴ�
            DO5 : ����λ��0��Ϊ1ʱ, �д�
            DO4 : ����λ��0��Ϊ1ʱ,ֹͣ״̬��,��ת����һ�䷽.
            DO3 : ����λ��0��Ϊ1ʱ,�������
            DO2 : ����λ��0��Ϊ1ʱ,ֹͣ״̬��,����.
            DO1 : ����λ��0��Ϊ1ʱ,ֹͣ.
            DO0 : ����λ��0��Ϊ1ʱ,����.*/
            for (i = 0; i < 8; i++)
            {   //D0~D7�Ĵ���
                if (((gu8p_RxdArray[2] & (0x01 << i)) != 0) && ((gu8_16DoTempLow & (0x01 << i)) == 0))
                {   //�ɻ���״̬Ϊ0���½��յ���״̬Ϊ1ʱ����Ӧ��ز�����Ҳ���Ǽ����0��1������
                    switch (i)
                    {
                        case 0: //����
                            RunStopInit(true);
                            break;
                        case 1: //��ͣ
                            Clr_Alarm();
                            RunStopInit(false);
                            break;
                        case 2: //����
                            if (gb_RunFlg == false)
                                gb_ZeroKeyFlg = true;
                            break;
                        case 3: //�屨��
                            Clr_Alarm();
                            break;
                        case 4: //�л��䷽
                            if (gb_RunFlg == false)
                            {
                                //ѡ�䷽������Ч������Ŀ��ֵ��Ϊ0���䷽
                                gu32_RecNum = NextRecNum(gu32_RecNum);
                                FRAM_WriteDW(FMAddr_RecNum, gu32_RecNum);
                                Read_RecipeData(gu32_RecNum);
                            }
                            break;
                        case 5: //�д�
                            Pack_JD();
                            break;
                        case 6: //�ɴ�
                            Pack_SD();
                            break;
                        case 7: //�ֶ�СͶ
                            if (gb_RunFlg == false)
                                ManualSp3(2);
                            break;
                        default:
                            break;
                    }
                }
                //D8~D9�Ĵ���
                if (((gu8p_RxdArray[1] & (0x01 << i)) != 0) && ((gu8_16DoTempHi & (0x01 << i)) == 0))
                {   //�ɻ���״̬Ϊ0���½��յ���״̬Ϊ1ʱ����Ӧ��ز�����Ҳ���Ǽ����0��1������
                    switch (i)
                    {
                        case 0: //���ۼ�
                            if (gb_RunFlg == false)
                                ClrSum();
                            break;
                        case 1: //�ֶ�ж��
                            if (gb_RunFlg == false)
                                ManualDisc(2);
                            break;
                        default:
                            break;
                    }
                }
            }
            gu8_16DoTempHi = gu8p_RxdArray[1];
            gu8_16DoTempLow = gu8p_RxdArray[2];  //����16DO���ε�״̬
            /*
            1Byte Out:
            D7~D2:2AOд����������
            00000:(0)   ����,���Ὣ2AO������д���Ǳ�
            00001:(1)   Ŀ��(�������Ʋ�����ӦС���������).
            00010:(2)   ��Ͷ��ǰ��.
            00011:(3)   ��Ͷ��ǰ��.
            00100:(4)   ���ֵ.
            00101:(5)   ����ֵ.
            00110:(6)   t7 
            00111:(7)   ��Ƿ���(д��������,ż���ر�,���¿������;�����)
            01000:(8)   ��Ƿ����ͣ����
            01001:(9)   ����ֵ.
            01010:(10)  Ƿ��ֵ.
            01011:(11)  t1(ʱ�������Ӧ��00~99)
            01100:(12)  t2
            01101:(13)  t3
            01110:(14)  t4
            01111:(15)  t5
            10000:(16)  t6
            10001:(17)  �Զ������������
            10010:(18)  �����������(00~99)
            10011:(19)  ���������Χ(00~99)
            10100:(20)  �����������(1~3)
            010101:(21) �Ĵ�����
            010110:(22) �Ĵ���ʱ
            010111:(23) �Ĵ�����ʱ��
            011000:(24) �Ĵ�����
            011001:(25) �Ĵ���Ч��ƽʱ��
            011010:(26) �Ĵ���Ч��ƽʱ��
            011011:(27) ��һ���Ĵ���Ч��ƽʱ��
            011100:(28) ��һ���Ĵ���Ч��ƽʱ��
            011101:(29) �㶯СͶ
            011110:(30) �㶯СͶ��Ч��ƽʱ��
            011111:(31) �㶯СͶ��Ч��ƽʱ��
            100000:(32) �������ϴ���
            �� ��:     ����,���Ὣ2AO������д���Ǳ�
            
            D1~D0:Ҫ��2AI���͵���������
             00: (0) ��ǰ����.
             01: (1) �ۼ�����
             10: (2) �ۼƴ���
             11: (3) ����*/
            gu8_PbusSendType = (gu8p_RxdArray[3] & 0x03);
            gu8_PbusWriteType = (gu8p_RxdArray[3] >> 2);
            //2AO: ��Ҫд�������, 4�ֽ�.��λ������ǰ,��λ�����ں�.
            for (i = 0; i < 4; i++)
            {   //��ȡ2AO��ֵ
                UL_Value.uc[3-i] = gu8p_RxdArray[4+i];
            }
            switch (gu8_PbusWriteType)
            {   //����д�����ͱ����޸Ķ�Ӧ�Ĳ���ֵ
                case 0:
                    break;
                case 1:        //Ŀ��ֵ
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Target)
                        {
                            if (gb_RunFlg == false)     //ֹͣ״̬�������޸��䷽
                            {
                                gs_Rec.Target = UL_Value.ul;   
                                FRAM_WriteDW(FMAddr_Target(gu32_RecNum), gs_Rec.Target);
                            }
                            else        //����ʱ�޸�
                            {
                                gu32_FastSetTemp[0] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x01;
                            }
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 2:        //��Ͷ��ǰ��
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Large)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Large = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Large(gu32_RecNum), gs_Rec.Large);
                            }
                            else        //����ʱ�޸�
                            {
                                gu32_FastSetTemp[1] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x02;
                            }
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 3:        //��Ͷ��ǰ��
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Middle)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Middle = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Middle(gu32_RecNum), gs_Rec.Middle);
                            }
                            else        //����ʱ�޸�
                            {
                                gu32_FastSetTemp[2] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x04;
                            }
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 4:        //���ֵ
                    if (UL_Value.ul  <= gs_Cal.Cap)
                    {
                        if (UL_Value.ul != gs_Rec.Small)
                        {
                            if (gb_RunFlg == false)
                            {
                                gs_Rec.Small = UL_Value.ul;
                                FRAM_WriteDW(FMAddr_Small(gu32_RecNum), gs_Rec.Small);
                            }
                            else        //����ʱ�޸�
                            {
                                gu32_FastSetTemp[3] = UL_Value.ul;
                                gu8_FastSetSigTemp |= 0x08;
                            }
                        }
                    }
                    else
                        gu8_WriteErrID = 2;
                    break;
                case 5:        //����ֵ
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
                case 7:        //��Ƿ�������
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
                case 8:        //��Ƿ����ͣ����
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
                case 9:        //����ֵ
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
                case 10:        //Ƿ��ֵ
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
                case 17:        //�����������
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
                case 18:        //�����������
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
                case 19:        //���������Χ
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
                case 20:        //�����������
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
                case 21:        //�Ĵ�����
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
                case 22:        //�Ĵ���ʱ
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
                case 24:        //��ֵ���Ĵ�����
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
                case 25:        //�Ĵ���Ч��ƽʱ��
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
                case 26:        //�Ĵ���Ч��ƽʱ��
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
                case 29:        //СͶ�㶯����
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
                case 30:        //СͶ�㶯��Чʱ��
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
                case 31:        //СͶ�㶯��Чʱ��
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
��������: void Pbus_Recive(void)
��    ��: �Ǳ�PROFIBUS�������ݵĴ��� 
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-5-5
��    ��:                      ʱ    ��: 
*********************************************************************/
void Pbus_Recive(void)
{
    u8 lu8_ErrCode;
    
    if (SE_REQ_IT == false && SE_S_RTS == false && gb_PbusTxdingFlg == false && gb_PbusRxdReadyFlg == true)
    {   //DP�����Ѿ������ϣ�SE�ӿڰ��ʼ���ɹ�������������գ�����û�з��Ͳ����Ѿ�����״̬ ��ô�ſ��Դ����������
        gb_PbusRxdReadyFlg = false;
        lu8_ErrCode = Pbus_ReciveCheck(gu8p_RxdArray, gu32_GSDType, gu8_ReqCom);
        if (0 == lu8_ErrCode)
        {   //�����޴��󣬴�����յ�������
            Pbus_ReciveDataDeal(gu8p_RxdArray, gu32_GSDType);
        }
    }
}

/*********************************************************************
��������: u8 Pbus_Checksum(u8 *fu8p_Array, u8 fu8_i)
��    ��: ����У�������
˵    ��: 
��ڲ���: fu8p_Array��У�������׵�ַ    fu8_i������У��ĸ���
�� �� ֵ: У��ͽ��
��    ��: �Ժ���               ʱ    ��: 2014-04-24
��    ��:                      ʱ    ��: 
*********************************************************************/
u8 Pbus_Checksum(u8 *fu8p_Array, u8 fu8_i)
{
    u8 i, j;
    
    for (i = 0, j = 0; i < fu8_i; i++)
        j += fu8p_Array[i];
    return j;
}

/*********************************************************************
��������: void Pbus_StartSend(u8 *fu8_Array, u8 fu8_TxdLen, u8 fu8_RxdLen)
��    ��: �������ڷ�������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-04-24
��    ��:                      ʱ    ��: 
*********************************************************************/
void Pbus_StartSend(u8 *fu8_Array, u8 fu8_TxdLen, u8 fu8_RxdLen)
{
    gu8_PbusTxdLen = fu8_TxdLen;    //�������ݸ���
    gu8_PbusRxdLen = fu8_RxdLen;    //�������ݸ���
    gu8_PbusTxdCnt = 0; //���ͼ���
    gu8_PbusRxdCnt = 0;
    fu8_Array[fu8_TxdLen-1] = Pbus_Checksum(fu8_Array, fu8_TxdLen - 1);   //У���
    gb_PbusTxdingFlg = true;    //��ʼ����
    gu32_TimeTemp = SYS10MS;    //���淢��ʱ��
    UART_FifoReset(PBUS_UARTX, UART_TxFIFO);         //��շ��Ͷ���
    UART_ItConfig(PBUS_UARTX,UART_TxEmpty,ENABLE);   //�����ڷ����ж�
//    UART_ItConfig(PBUS_UARTX,UART_RxBufNotEmpty,DISABLE);//�رս����ж�
}

/*********************************************************************
��������: void Com_Pbus_DP(void)
��    ��: PROFIBUSͨѶ��ѭ������
˵    ��: ������ѭ����ÿ��ѭ����ִ��
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-04-29
��    ��:                      ʱ    ��: 
*********************************************************************/
void Com_Pbus_DP(void)
{
    Pbus_Init();    //��ʼ��
    Pbus_Recive();  //�������ݴ���
    Pbus_Send();    //�������ݴ���
}



