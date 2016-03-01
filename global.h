#ifndef GLOBAL_H
#define GLOBAL_H

ESP8266WebServer server(80);									// The Webserver
int AdminTimeOutCounter = 0;									// Counter for Disabling the AdminMode
boolean Refresh = false; // For Main Loop, to refresh things like GPIO / WS2812
boolean AdminEnabled = false;		// Enable Admin Mode for a given Time


struct strConfig {
	String ssid;
	String password;
	byte  IP[4];
	byte  Netmask[4];
	byte  Gateway[4];
	boolean dhcp;
  String iftttKey;
  String trigger1;
	String trigger2;
	String trigger3;
}   config;


/*
**
** CONFIGURATION HANDLING
**
*/
void ConfigureWifi()
{
	Serial.println("Configuring Wifi");
	WiFi.begin (config.ssid.c_str(), config.password.c_str());
	if (!config.dhcp)
	{
		WiFi.config(IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3] ),  IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ) , IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] ));
	}
}

void WriteConfig()
{

	Serial.println("Writing Config");
	EEPROM.write(0,'C');
	EEPROM.write(1,'F');
	EEPROM.write(2,'G');

	EEPROM.write(16,config.dhcp);
	
	EEPROM.write(32,config.IP[0]);
	EEPROM.write(33,config.IP[1]);
	EEPROM.write(34,config.IP[2]);
	EEPROM.write(35,config.IP[3]);

	EEPROM.write(36,config.Netmask[0]);
	EEPROM.write(37,config.Netmask[1]);
	EEPROM.write(38,config.Netmask[2]);
	EEPROM.write(39,config.Netmask[3]);

	EEPROM.write(40,config.Gateway[0]);
	EEPROM.write(41,config.Gateway[1]);
	EEPROM.write(42,config.Gateway[2]);
	EEPROM.write(43,config.Gateway[3]);

  WriteStringToEEPROM(120,config.iftttKey);
  WriteStringToEEPROM(160,config.trigger1);
  WriteStringToEEPROM(200,config.trigger2);
  WriteStringToEEPROM(240,config.trigger3);
	
	WriteStringToEEPROM(64,config.ssid);
	WriteStringToEEPROM(96,config.password);
	
	EEPROM.commit();
}
boolean ReadConfig()
{

	Serial.println("Reading Configuration");
	if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
	{
		Serial.println("Configurarion Found!");
		config.dhcp = 	EEPROM.read(16);

		config.IP[0] = EEPROM.read(32);
		config.IP[1] = EEPROM.read(33);
		config.IP[2] = EEPROM.read(34);
		config.IP[3] = EEPROM.read(35);
		config.Netmask[0] = EEPROM.read(36);
		config.Netmask[1] = EEPROM.read(37);
		config.Netmask[2] = EEPROM.read(38);
		config.Netmask[3] = EEPROM.read(39);
		config.Gateway[0] = EEPROM.read(40);
		config.Gateway[1] = EEPROM.read(41);
		config.Gateway[2] = EEPROM.read(42);
		config.Gateway[3] = EEPROM.read(43);
				
		config.ssid = ReadStringFromEEPROM(64);
		config.password = ReadStringFromEEPROM(96);
    config.iftttKey = ReadStringFromEEPROM(120);
    config.trigger1 = ReadStringFromEEPROM(160);
    config.trigger2 = ReadStringFromEEPROM(200);
    config.trigger3 = ReadStringFromEEPROM(240);
    	
		
		return true;
		
	}
	else
	{
		Serial.println("Configurarion NOT FOUND!!!!");
		return false;
	}
}


#endif
