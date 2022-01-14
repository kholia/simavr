// https://microchipsupport.force.com/s/article/Serial-number-in-AVR---Mega-Tiny-devices

#include <avr/boot.h>

void setup()
{
  Serial.begin(9600);
  Serial.println("in setup()");
}

void loop()
{
  delay(1000);
  Serial.println("in loop()");

  unsigned int UniqueIDsize = 9;
  for (size_t i = 0; i < UniqueIDsize; i++)
  {
    byte n = boot_signature_byte_get(0x0E + i + (UniqueIDsize == 9 && i > 5 ? 1 : 0));
    Serial.print(n < 16 ? "0" : "");
    Serial.print(n, HEX);
  }
  Serial.println();
}
