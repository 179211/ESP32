// Esp 32

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

bool isVSPI = true;
//constexpr int SPI_SPEED = 16000000;   // 10000000 Default
constexpr int SPI_SPEED = 10000000;
SPIClass *spiVSPI = nullptr;
SPIClass *spiHSPI = nullptr;
RF24 radioVSPI(17, 5, SPI_SPEED);
RF24 radioHSPI(25, 15, SPI_SPEED);
void configureRadio(RF24 &radio, int channel, SPIClass *spi);

const byte address[6] = "00001";
float payload = 0.0;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
  }

  if(isVSPI){
    spiVSPI = new SPIClass(VSPI);
    spiVSPI->begin();
    configureRadio(radioVSPI, 76, spiVSPI);
  }
  else{
    spiHSPI = new SPIClass(HSPI);
    spiHSPI->begin();
    configureRadio(radioHSPI, 76, spiHSPI);
  }
}

void configureRadio(RF24 &radio, int channel, SPIClass *spi)
{
  // 2400 MHz + <channel number> radio.channel = 2  # set the channel to 2 (2402 MHz) 0-125
  // radio.payloadSize = 16  # set the static payload size to 16 bytes
  // Pipe 0 is also used by the writing pipe

  if (radio.begin(spi))
  {
    radio.openWritingPipe(address);
    radio.stopListening();
    radio.setPALevel(RF24_PA_MAX, true);
    //radio.setAutoAck(false);
    //radio.setPayloadSize(sizeof(float));
    //radio.setDataRate(RF24_2MBPS);
    // radio.setRetries(0, 0);
    // radio.setCRCLength(RF24_CRC_DISABLED);
    // radio.startConstCarrier(RF24_PA_HIGH, channel); //Transmission of constant carrier wave with defined frequency and output power
  }
}

void loop()
{
  bool report=false;

  if(isVSPI){
    Serial.println(radioVSPI.getChannel());
    report = radioVSPI.write(&payload, sizeof(float));
  }
  else{
    Serial.println(radioHSPI.getChannel());
    report = radioHSPI.write(&payload, sizeof(float));
  }

  if (report)
  {
    Serial.print(F("Transmission successful! "));
    Serial.print(payload);
    Serial.print("  ");
    Serial.println(sizeof(payload));

    payload += 0.01;
  }
  else
  {
    Serial.println(F("Transmission failed or timed out"));
  }
  delay(1000);
}


