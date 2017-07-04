#pragma pack (1) /* set maximum alignment to 1 */         

/*====================================================================================*/
// EEPROM A0 Table01h
/*====================================================================================*/ 
struct XFPExtIdentifyDef
{
INT8U bit0_2:3;			 //Reserved                  
INT8U bit3:1;            //have CLEI code or not 0: no CLEI code  1:CLEI code in table02
INT8U bit4:1;			 //TX Ref Clock Input required or not  0:required  1:not required
INT8U bit5:1;			 //CDR function or not 0: Module with CDR function  1:Non-CDR of XFP 
INT8U bit6_7:2;			 //Power Level module
};

struct XFPETHERNETCDef			 
{
INT8U bit0:1;			 //Reserved                      
INT8U bit1:1;			 //10GBASE-EW
INT8U bit2:1;		     //10GBASE-LW
INT8U bit3:1;			 //10GBASE-SW
INT8U bit4:1;			 //10GBASE-LRW
INT8U bit5:1;			 //10GBASE-ER  
INT8U bit6:1;			 //10GBASE-LR   
INT8U bit7:1;			 //10GBASE-SR 
};

struct XFPFibreChannelDef
{
INT8U bit0_3:4;			 //Reserved                  
INT8U bit4:1;            //Intermediate Reach 1300 nm FP
INT8U bit5:1;			 //Extended Reach 1550 nm
INT8U bit6:1;			 //1200-SM-LL-L
INT8U bit7:1;			 //1200-MX-SN-I
};

struct XFPLowerSpeedLDef
{
INT8U bit0:1;			 //Reserved                  
INT8U bit1:1;			 //OC-48-LR
INT8U bit2:1;		     //OC-48-IR
INT8U bit3:1;			 //OC-48-SR
INT8U bit4:1;            //2xFC SMF
INT8U bit5:1;			 //2xFC MMF
INT8U bit6:1;			 //1000BASE-LX/1xFC SMF
INT8U bit7:1;			 //1000BASE-SX/1xFC MMF
};

struct XFPSONETCInterCDef
{
INT8U bit0_1:2;			 //Reserved                        
INT8U bit2:1;		     //I-64.5
INT8U bit3:1;			 //I-64.3
INT8U bit4:1;			 //I-64.2
INT8U bit5:1;			 //I-64.2r
INT8U bit6:1;			 //I-64.1
INT8U bit7:1;			 //I-64.1r
};

struct XFPSONETCShortHDef
{
INT8U bit0:1;			 //Reserved                     
INT8U bit1:1;			 //S-64.5b
INT8U bit2:1;		     //S-64.5a
INT8U bit3:1;			 //S-64.3b
INT8U bit4:1;            //S-64.3a
INT8U bit5:1;			 //S-64.2b
INT8U bit6:1;			 //S-64.2a
INT8U bit7:1;			 //S-64.1
};

struct XFPSONETCLongHDef
{
INT8U bit0_1:2;			 //Reserved   
INT8U bit2:1; 			 //G.959.1 P1L1-2D2
INT8U bit3:1;		     //L-64.3
INT8U bit4:1;            //L-64.2c
INT8U bit5:1;			 //L-64.2b
INT8U bit6:1;			 //L-64.2a
INT8U bit7:1;			 //L-64.1
};

struct XFPSONETCVLongHDef
{
INT8U bit0_4:5;			 //Reserved
INT8U bit5:1;			 //V-64.3
INT8U bit6:1;			 //V-64.2b
INT8U bit7:1;			 //V-64.2a
};

struct XFPENCODING
{
INT8U bit0_2:3;			 //Reserved
INT8U bit3:1;		     //RZ
INT8U bit4:1;            //NRZ
INT8U bit5:1;			 //SONET Scrambled
INT8U bit6:1;			 //8B10B
INT8U bit7:1;			 //64B/66B
};

struct XFPDeviceTDef
{
INT8U bit0:1;			 //Transmitter Tunable or not 
INT8U bit1:1;			 //Detector Type 0:PIN 1:APD
INT8U bit2:1; 			 //Cooled transmitter or not
INT8U bit3:1;		     //wavelength control or not
INT8U bit4_7:4;          //Transmitter technology
};

struct XFPCDRSupportDef
{
INT8U bit0:1;			 //XFI Loopback Mode Supported                    
INT8U bit1:1;			 //Lineside Loopback Mode Supported
INT8U bit2:1;		     //Reserved
INT8U bit3:1;			 //CDR support for 11.1 Gb/s
INT8U bit4:1;            //CDR support for 10.7 Gb/s
INT8U bit5:1;			 //CDR support for 10.5 Gb/s
INT8U bit6:1;			 //CDR support for 10.3 Gb/s
INT8U bit7:1;			 //CDR support for 9.95 Gb/s
};

struct XFPIccmax1Def
{
INT8U bit0_3:4;			 //Maximum current required by +3.3V Supply
INT8U bit4_7:4;			 //Maximum current required by +5V Supply 
};

struct XFPIccmax2Def
{
INT8U bit0_3:4;			 //Maximum current required by -5.2V Supply
INT8U bit4_7:4;			 //Maximum current required by +1.8V Supply 
};

struct XFPDiagnosticMonitorDef
{
INT8U bit0_2:3;			 //Reserved
INT8U bit3:1;		     //Received power measurement type	0:OMA  1:Average Power
INT8U bit4:1;            //Module Respond to FEC BER	0:No BER Support  1:BER Support 
INT8U bit5_7:3;			 //Reserved
};

struct XFPEnhancedOptionDef
{
INT8U bit0:1;			 //Optional CMU Support Mode
INT8U bit1:1;			 //Wavelength tunability implemented
INT8U bit2:1; 			 //Active FEC control functions implemented
INT8U bit3:1;		     //Supports VPS bypassed regulator Mode
INT8U bit4:1;            //Supports VPS LV regulator mode
INT8U bit5:1;			 //Optional Soft P_down implemented
INT8U bit6:1;			 //Optional Soft TX_DISABLE implemented
INT8U bit7:1;			 //Module Supports Optional VPS
};

struct XFPAuxMonitorDef
{
INT8U bit0_3:4;			 //Aux A/D Input 2
INT8U bit4_7:4;          //Aux A/D Input 1
};

struct XFPTable1strA0 		  
{
//BASE ID FIELDS
       //Reserved for SFF8079
  INT8U reserved_array[128];            //pStrA0[0~127] 
      //Type of serial transceiver 
  INT8U  identifier;	                 //pStrA0[128]
      //Extended identifier of type of serial transceiver
  struct XFPExtIdentifyDef  extIdentifier;	                 //pStrA0[129]
      //Code for connector type
  INT8U  connector;		                 //pStrA0[130]
      //Code for electronic compatibility or optical compatibility
  //INT8U  transceiver[8];		         //pStrA0[131~138]
  //Infiniband Compliance Codes
  struct XFPETHERNETCDef ethernetC;
  //Part of SONET Compliance Codes
  struct XFPFibreChannelDef fibreChannel;
  //Part of SONET Compliance Codes
  INT8U  copperLink;	
  //Gigabit Ethernet Compliance Codes
  struct XFPLowerSpeedLDef lowerSpeed;
  //Fiber Channel link length & part of transmitter technology
  struct XFPSONETCInterCDef sonetInterconnect;
  //Part of Fiber Channel transmitter technology
  struct XFPSONETCShortHDef sonetShortH;
  //Fiber Channel Transmission media
  struct XFPSONETCLongHDef sonetLongH;
  //Fiber Channel speed
  struct XFPSONETCVLongHDef sonetVLongH;
  
      //Code for serial encoding algorithm
  struct XFPENCODING  encoding;		                 //pStrA0[139]
      //Minimum bit rate, units of 100 MBits/s.
  INT8U  BR_min;	             //pStrA0[140] 	
      //Maximum bit rate, units of 100 MBits/s.
  INT8U  BR_max;		         //pStrA0[141]
      //Link length supported for SMF fiber in km
  INT8U  lengthSMF_km;		             //pStrA0[142]
      //Link length supported for EBW 50/125 μm fiber, units of 2 m
  INT8U  lengthE50m;		             //pStrA0[143]
      //Link length supported for 50/125 μm fiber, units of 1 m
  INT8U  length50im;		             //pStrA0[144]
      //Link length supported for 62.5/125 μm fiber, units of 1 m
  INT8U  length62p5im;	                 //pStrA0[145]
      //Link length supported for copper, units of 1m
  INT8U  lengthCopper;	                 //pStrA0[146]
      //Device technology
  struct XFPDeviceTDef	 deviceTech;		  //pStrA0[147]
      //Vendor name
  INT8U  vendorName[16];		             //pStrA0[148~163]
      //Reserved
  struct XFPCDRSupportDef  CDRsupport;		     //pStrA0[164]
      //Vendor IEEE company ID
  INT8U  vendorOUI[3];		             //pStrA0[165~167]
      //Part number provided by vendor
  INT8U  vendorPN[16];		             //pStrA0[168~183]
      //Revision level for part number provided by vendor
  INT8U  vendorRev[2];		             //pStrA0[184~185]
      //Laser wavelength
  INT16U waveLength;         			 //pStrA0[186~187]
      //wavelength Tolerance
  INT16U  wavelengthTol;                //pStrA0[188-189]
   //Maximum Case Temperature in Degrees C
  INT8U  maxCaseTemp;                      //pStrA0[190]
      //Check code for Base ID Fields
  INT8U  cc_Base;		                 //pStrA0[191]
  
//EXTENDED ID FIELDS
  //Power supply current requirements and max power dissipation
  INT8U  maxpDissipation;                    //pStrA0[192]
  INT8U  maxpDissipation_pd;                    //pStrA0[193]
  struct XFPIccmax1Def Iccmax1;		//pStrA0[194]
  struct XFPIccmax2Def Iccmax2;		//pStrA0[195]

      //Serial number provided by vendor
  INT8U vendorSN[16];                    //pStrA0[196~211]	
      //Vendor’s manufacturing date code
  INT8U dateCode[8];                     //pStrA0[212~219]
      //Indicates which type of diagnostic monitoring is implemented in the transceiver
  struct XFPDiagnosticMonitorDef diagMonitorType;		 //pStrA0[220]
      //Indicates which optional enhanced features are implemented in the transceiver
  struct XFPEnhancedOptionDef  enhancedOptions; 		 //pStrA0[221]
      //Indicates which revision of SFF-8472 the transceiver complies with
  struct XFPAuxMonitorDef auxMonitor;             		 //pStrA0[222]
      //Check code for the Extended ID Fields		
  INT8U cc_Ext;                          //pStrA0[223]
  
      //VENDOR SPECIFIC ID FIELDS
  INT8U vendorSpecReserved[32];          //pStrA0[224~255]
 
};		

union XFPuA0_Table1
{ 
  struct XFPTable1strA0 sStrA0;
  INT8U  pStrA0[256];
};

union XFPuA0_Table1 Table1_A0;            

			
/////////////////////////////////////////////////////////////////
/*   A0 Lower Memory                                           */
/////////////////////////////////////////////////////////////////

struct FLAG_8082DEF                        
{                                       
INT8U  Tx_P_Low    :1;
INT8U  Tx_P_Hgh    :1;
INT8U  Tx_Bias_Low :1;
INT8U  Tx_Bias_Hgh :1;
INT8U  vcc_Low     :1;
INT8U  vcc_Hgh     :1;
INT8U  Temp_Low    :1;
INT8U  Temp_Hgh    :1;
};

struct FLAG_8183DEF                          
{                                       
INT8U  Reserved1   :1; 	    
INT8U  Reserved2   :1;   
INT8U  AUX2_Low    :1; 
INT8U  AUX2_High   :1; 
INT8U  AUX1_Low    :1;
INT8U  AUX1_High   :1;
INT8U  Rx_P_Low    :1;
INT8U  Rx_P_Hgh    :1;  
};                         
                                   
struct FLAG_84DEF 
{       
INT8U      Reset_Complete  :1; 
INT8U      Mirror_MOD_NR   :1;
INT8U      RX_CDR          :1;
INT8U      RX_LOS          :1;
INT8U      RX_NR           :1;
INT8U      TX_CDR          :1;
INT8U      TX_Fault        :1; 
INT8U      TX_NR           :1;
};
  
struct FLAG_85DEF 
{       
INT8U      Reserved1           :1;	
INT8U      Reserved2           :1;
INT8U      Reserved3           :1;
INT8U      Reserved4           :1;  
INT8U      Reserved5           :1;
INT8U      Wavelength_Unlocked :1;   
INT8U      TEC_Fault           :1; 
INT8U      APD_Supply_Fault    :1;  
};
					
     
struct FLAG_8687DEF
{                             
INT8U      Vee5_Low     :1; 
INT8U      Vee5_High    :1;
INT8U      VCC2_Low     :1;
INT8U      VCC2_High    :1;
INT8U      VCC3_Low     :1;	
INT8U      VCC3_High    :1;	
INT8U      VCC5_Low     :1;
INT8U      VCC5_High    :1;	 
};

struct FLAG_110DEF     
{                             
INT8U      Data_Not_Ready :1; //Indicates transceiver has achieved power up and A/D data is ready 
INT8U      LOS            :1; //Indicates Optical Loss of Signal
INT8U      Interrupt      :1; //Digital state of the Interrupt output pin
INT8U      Soft_P_Down    :1; //Optional read/write bit that allows the module to be placed in the power down mode
INT8U      P_Down         :1; //Digital state of the P_Down Pin
INT8U      MOD_NR         :1; //Digital state of the MOD_NR Pin
INT8U      Soft_Tx_Dis    :1; //Optional read/write bit that allows software disable of laser.
INT8U      Tx_Dis         :1; //Digital state of the TX Disable Input Pin 
};


struct FLAG_111DEF                         
{                                        
INT8U      APD_Protected  :1; //jack add 
INT8U      Reserved2      :1;
INT8U      Reserved3      :1;
INT8U      RX_CDR_LOL     :1; //Locked Identifies Loss of Lock in RX path CDR
INT8U      RX_NR          :1; //Identifies Not Ready condition as specific to the TX path
INT8U      TX_CDR_LOL     :1; //Locked Identifies Loss of Lock in TX path CDR  
INT8U      TX_Fault       :1; //Identifies Laser fault condition (Generated by laser safety system)
INT8U      TX_NR          :1; //Identifies Not Ready condition as specific to the TX path 
};                            
                                         

struct XFPLowerstrA0 		  
{
	INT8U  reserved_table[128];		//pStrA0[128~255] 									   
	INT8U  LUTIndex;				//pStrA0[127]   
	INT32U Password;			    //pStrA0[123~126]   
	INT8U  mode;                    //pStrA2[122]，是F2[0]映射到A2[122]的值，只读，即使改写了重上电后恢复原值
	INT8U  version;                 //pStrA2[121]，是大版本号映射到A2[121]的值，只读，即使改写了重上电后恢复原值
	INT8U  INT_i;                   //pStrA2[120]，进RSSI/BEN中断的次数
	INT8U  BAK_i;                   //pStrA2[119]，主循环自动模式下的循环次数      																																	
	//INT32U PasswordSet;				//pStrA0[119~122]   
	INT8U  Packet_Error_Checking;	//pStrA0[118] 
	INT8U  reserved_status[6];		//pStrA0[112~117] 

    struct FLAG_111DEF status_111;	//pStrA0[111] 
    struct FLAG_110DEF status_110;	//pStrA0[110] 
	INT16U AUX2;					//pStrA0[108~109], 1.8V 
	INT16U AUX1;					//pStrA0[106~107], MC_MON
	INT16U rx_Power;				//pStrA0[104~105]
	INT16U tx_Power;				//pStrA0[102~103]
	INT16U tx_Bias;					//pStrA0[100~101]					         
	INT16U vcc;						//pStrA0[98~99], reserved
	INT16S temperature;				//pStrA0[96~97]

	struct FLAG_8687DEF MASK87;			//pStrA0[95]  
	struct FLAG_8687DEF MASK86;			//pStrA0[94]  
	struct FLAG_85DEF   MASK85;			//pStrA0[93]  
	struct FLAG_84DEF   MASK84;			//pStrA0[92]  
	struct FLAG_8183DEF MASK83;			//pStrA0[91]  
	struct FLAG_8082DEF MASK82;			//pStrA0[90]  
	struct FLAG_8183DEF MASK81;			//pStrA0[89]  
	struct FLAG_8082DEF MASK80;			//pStrA0[88]  
	
	struct FLAG_8687DEF Power_Warning;	//pStrA0[87]  
	struct FLAG_8687DEF Power_Alarm;	//pStrA0[86]  
	struct FLAG_85DEF   WL_TEC_APD_Flg; //pStrA0[85]  
	struct FLAG_84DEF   TRx_Flg;		//pStrA0[84]  
	struct FLAG_8183DEF FLAG_WARN_83;	//pStrA0[83]  
	struct FLAG_8082DEF FLAG_WARN_82;   //pStrA0[82]  
	struct FLAG_8183DEF FLAG_ALARM_81;	//pStrA0[81]  
	struct FLAG_8082DEF FLAG_ALARM_80;	//pStrA0[80]  
	
	INT8U  reserved2[2];				//pStrA0[78~79]  
	INT8U  PhaseAdjustment;				//pStrA0[77]//Not Support 
	INT8U  Amplitude_Adjustment;		//pStrA0[76]//Not Support 
	
	INT16U Wavelength_Error;			//pStrA0[74~75]//Not Support 
	INT16U Wavelength_SeSet;			//pStrA0[72~73]//Not Support 
	INT16U Actual_BER;					//pStrA0[70~71]//Not Support 
	
	INT8U  reserved1[10];  			    //pStrA0[60~69]
	
	INT16U  Optional_VPS_Control;		//pStrA0[58~59]//Not Support  																							

	INT16U  AUX2LowWarning;				//pStrA0[56~57]//AUX2  1.8V Low Warning  
	INT16U  AUX2HighWarning;			//pStrA0[54~55]//AUX2  1.8V High Warning 
	INT16U  AUX2LowAlarm;				//pStrA0[52~53]//AUX2  1.8V Low Alarm 
	INT16U  AUX2HighAlarm;				//pStrA0[50~51]//AUX2  1.8V High Alarm 
	
	
	INT16U  AUX1LowWarning;				//pStrA0[48~49]//AUX1 VTEMP Low Warning   																									
	INT16U  AUX1HighWarning;			//pStrA0[46~47]//AUX1 VTEMP High Warning   																									
	INT16U  AUX1LowAlarm;				//pStrA0[44~45]//AUX1 VTEMP Low Alarm 				
	INT16U  AUX1HighAlarm;				//pStrA0[42~43]//AUX1 VTEMP High Alarm  							
	
	INT16U  rxPowerLowWarning;			//pStrA0[40~41]//Reserved         																														
	INT16U  rxPowerHighWarning;			//pStrA0[38~39]//RX Power High Warning  																												
	INT16U  rxPowerLowAlarm;			//pStrA0[36~37]//RX Power Low Alarm  																											
	INT16U  rxPowerHighAlarm;			//pStrA0[34~35]//RX Power High Alarm 																											
	
	INT16U  txPowerLowWarning;			//pStrA0[32~33]//TX Power Low Warning   																									
	INT16U  txPowerHighWarning;			//pStrA0[30~31]//TX Power High Warning   																												
	INT16U  txPowerLowAlarm;			//pStrA0[28~29]//TX Power Low Alarm     																													
	INT16U  txPowerHighAlarm;			//pStrA0[26~27]//TX Power High Alarm   																														
	
	INT16U  biasLowWarning;				//pStrA0[24~25]//Bias Low Warning     																									
	INT16U  biasHighWarning;			//pStrA0[22~23]//Bias High Warning   																													
	INT16U  biasLowAlarm;				//pStrA0[20~21]//Bias Low Alarm        																										
	INT16U  biasHighAlarm;				//pStrA0[18~19]//Bias High Alarm       																										
	
	INT16U	voltageLowWarning;	        //pStrA2[16~17]//Voltage Low Warning   																									
	INT16U	voltageHighWarning;	        //pStrA2[14~15]//Voltage High Warning   																									
	INT16U	voltageLowAlarm;	        //pStrA2[12~13]//Voltage Low Alarm 				
	INT16U	voltageHighAlarm;	        //pStrA2[10~11]//Voltage High Alarm  							
	
	INT16S tempLowWarning;				//pStrA0[8~9]  //Temperature Low Warning  																											
	INT16S tempHighWarning;				//pStrA0[6~7]  //Temperature High Warning  																																																				
	INT16S tempLowAlarm;				//pStrA0[4~5]  //Temperature Low Alarm  																																																			
	INT16S tempHighAlarm;				//pStrA0[2~3]  //Temperature High Alarm 																													 
	                                                              
	INT8U  Signal_Conditioner_Control;	//pStrA0[1]    //Nominal bit rate, units of 100 MBits/sec                                                
	
	INT8U  Identifier;					//pStrA0[0]     
}; 
/*
union XFPuA0_Lower
{ 
  struct XFPLowerstrA0 sStr;
  INT8U  pStr[256];
};

union XFPuA0_Lower XFPONU_uA0;
*/
union XFPuA0
{ 
  struct XFPLowerstrA0 sStr;
  INT8U  pStr[256];
};
#pragma pack () /* set maximum alignment to default */
