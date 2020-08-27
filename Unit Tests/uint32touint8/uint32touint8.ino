uint32_t test32=122500;
uint8_t b[4];
uint32_t u;


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
b[3] = (uint8_t)test32;
b[2] = (uint8_t)(test32>>=8);
b[1] = (uint8_t)(test32>>=8);
b[0] = (uint8_t)(test32>>=8);
u = b[0];
u = (u  << 8) + b[1];
u = (u  << 8) + b[2];
u = (u  << 8) + b[3];
}

void loop() {
  // put your main code here, to run repeatedly:

Serial.println(u);
delay(1000);
}
