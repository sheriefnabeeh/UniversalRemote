#include <EEPROM.h>
#include "EEWrapper.h"
#include <Arduino.h>

#define REMOTEEND 0xFFFF

RemoteData EEWrapper::initRemotes()
{
  int remoteLimit{1};
  IRData irData{};
  RemoteData remote{};
  int irAddress{1};
  bool remoteFound = false;
  Status = EEPROM.read(EEAddress, &dataRead);
  if (Status == 0)
  {
    remoteFound = true;
  }
  Serial.print("Beginning parsing data from memory at address: ");
    
  while (remoteLimit <= 10 && Status == 0)
  {
    delay(10);
    //TODO: handle the address of EEaddress case here
    if (Status != 0)
    {
      Serial.print("EEPROM: reading incorrect address, or no data saved with address equal");
      Serial.println(EEAddress, DEC);
      break;
    }
    if (dataRead == REMOTEEND)
    {
      Serial.println("reached ending data.");
      EEAddress += 1;
      Serial.println("finished parsing");
      return remote;
    }
    if (irAddress == PROTOCOLINDEX)
    {
      irData.protocol = protocolmap[dataRead];
    }
    if (irAddress == ADDRESSINDEX)
    {
      irData.address = dataRead;
    }
    if (irAddress == COMMANDINDEX)
    {

      irData.command = dataRead;
      remote.signals[remoteLimit] = irData;
      remote.setName(protocol_to_string(irData.protocol));
 
      ++remoteLimit;
      irAddress = 0;
    }
    
    ++irAddress;
    ++EEAddress;
    Status = EEPROM.read(EEAddress, &dataRead);
  }
  if (remoteFound)
  {
    EEAddress++;
  }
  else
  {
    Serial.println("no remote found");
  }
  Serial.println("finished parsing");
  return remote;
}
std::vector<RemoteData> EEWrapper::fetchRemotes()
{
  uint16_t Data{0};
  EEAddress = 0x0001;
  uint16_t addressRead = EEAddress;

  std::vector<RemoteData> remoteList;
  while (EEPROM.read(EEAddress, &Data) == 0 )
  {
    addressRead = EEAddress;
    RemoteData data = initRemotes();
    if (data.getName() != "")
    {
      Serial.println("Remote will be added to list");
      remoteList.push_back(data);
    }
  }
  return remoteList;
}
int EEWrapper::writeSignal(IRData data)
{
  Serial.print("writing data to memory at address :");
  Serial.println(EEAddress);
  delay(20);
  if (remoteSignalsCounter != REMOTESIZELIMIT)
  {
    uint16_t Status = EEPROM.read(EEAddress, &dataRead);

    if (Status == 0)
    {
      Serial.println("trying to overwrite an already written address.. abort");
      return 1;
    }
    Serial.print("begin writing signal at: ");
    Serial.println(EEAddress);
    ++remoteSignalsCounter;
    Status = EEPROM.write(EEAddress, (as_integer(data.protocol)));
    Status = EEPROM.write(++EEAddress, data.address);
    Status = EEPROM.write(++EEAddress, data.command);
    ++EEAddress;
    Serial.print("finished signal and incremented at: ");
    Serial.println(EEAddress);
  }
  else
  {
    endRemoteSeries();
  }
  return 0;
}
int EEWrapper::writeRemote(const IRData irList[], int size)
{
  Serial.println("entering write Remote");

  for (int i = 0; i < size; i++)
  {
    writeSignal(irList[i]);
  }
}
void EEWrapper::endRemoteSeries()
{
  Serial.println("ending writing");
  Serial.print(", at address: ");
  Serial.println(EEAddress);

  if (remoteSignalsCounter != 0)
  {
    Status = EEPROM.write(EEAddress, 0xFFFF);
    remoteSignalsCounter = 0;
    ++EEAddress;
  }
}
uint16_t EEWrapper::formatMemory()
{
  uint16_t returnValue = EEPROM.format();
  Serial.println("Memory Formatted.");
  return returnValue;
}
uint16_t EEWrapper::getRemoteSignalCounter()
{
  return remoteSignalsCounter;
}
uint16_t EEWrapper::getUniversalEECounter()
{
  return EEAddress;
}
std::string EEWrapper::protocol_to_string(const PROTOCOL &protocol)
{
  std::string protocolString{};

  switch (protocol)
  {
  case PROTOCOL::BOSE:
    protocolString = "BOSE";
    break;
  case PROTOCOL::Denon:
    protocolString = "Denon";
    break;
  case PROTOCOL::JVC:
    protocolString = "JVC";
    break;
  case PROTOCOL::KASEIKYO:
    protocolString = "KASEIKYO";
    break;
  case PROTOCOL::LEGALPOWER:
    protocolString = "LEGALPOWER";
    break;
  case PROTOCOL::LG:
    protocolString = "LG";
    break;
  case PROTOCOL::MAGI:
    protocolString = "MAGI";
    break;
  case PROTOCOL::NEC:
    protocolString = "NEC";
    break;
  case PROTOCOL::RC5:
    protocolString = "RC5";
    break;
  case PROTOCOL::RC6:
    protocolString = "RC6";
    break;
  case PROTOCOL::SAMSUNG:
    protocolString = "SAMSUNG";
    break;
  case PROTOCOL::SHARP:
    protocolString = "SHARP";
    break;
  case PROTOCOL::SONY:
    protocolString = "SONY";
    break;
  }

  return protocolString;
}
