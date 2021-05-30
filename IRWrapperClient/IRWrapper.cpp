#pragma once
#include "IRWrapper.h"
#include <Arduino.h>
#include <IRremote.h>
#include <string>
std::string convertDecodingType(decode_type_t &&);
std::unique_ptr<IRrecv> irrecv;
std::unique_ptr<IRsend> irsend;

Wrapper::Wrapper(int RECV_PIN, int SENDV_PIN) {
  irrecv = std::unique_ptr<IRrecv>(new IRrecv(RECV_PIN));
  irsend = std::unique_ptr<IRsend>(new IRsend(SENDV_PIN));
  irrecv->setReceivePin(RECV_PIN);
  irrecv->begin(RECV_PIN,1,13);
  Serial.println("Initialized receiver..");
  printf("Hello World\n");
}

void Wrapper::tryRemote(bool enable) {
  if (enable) {
    Serial.println("trying remote !!");
    irsend->sendNEC(0, 6 , 1);
    delay(500);
    irsend->sendNEC(0, 71, 1);
    delay(500);
    irsend->sendNEC(0, 7, 1);
    delay(500);
    irsend->sendNEC(0, 23, 1);
    enable = false;
  }
}

void Wrapper::readSignal() {
  if (irrecv->decode()) {
    bool nec = irrecv->decodeNEC();
    //data = *irrecv->read();
    //changeLEDState();
    if (nec) Serial.println("decoded protocol correctly ");

    Serial.print("decoded command: " );
    Serial.println((long)irrecv->decodedIRData.command);

    Serial.print("decoded address: " );
    Serial.println(irrecv->decodedIRData.address, HEX);
    std::string typeString = convertDecodingType(std::move(irrecv->decodedIRData.protocol));
	Serial.println(typeString.c_str());
    delay(500);
    irrecv->resume();
  }
}
std::string convertDecodingType(decode_type_t &&type){
	  std::string typeString;
	  switch (type) {
	case NEC: typeString = ("NEC"); break ;
    case SONY: typeString = ("SONY");break;
    case RC5: typeString = ("RC5");break;
    case RC6: typeString = ("RC6");break;
    case DISH: typeString = ("DISH");break;
    case SHARP: typeString =  ("SHARP");break;
    case JVC:  typeString = ("JVC");break;
    case SAMSUNG: typeString =  ("SAMSUNG"); break ;
    case LG:  typeString = ("LG") ; break ;
    case WHYNTER:  typeString = ("WHYNTER"); break ;
    case PANASONIC: typeString =  ("PANASONIC"); break ;
    case DENON: typeString =  ("DENON"); break ;
    default:
    case UNKNOWN: Serial.println("UNKNOWN"); break ;
	  }
	  	return typeString;

}