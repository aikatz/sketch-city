byte pos_data[5][4] = { {B00110111, B00100100, B00111011, B00110111},
                        {B00010111, B00010111, B00100111, B00010011},
                        {B00010101, B00000111, B00000011, B00011011},
                        {B00000111, B00001011, B00001011, B00110011},
                        {B00001111, B00101011, B00111010, B00011111} };

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  for (char i = 0; i < 5; i++) {
    for (char j = 0; j < 4; j++) {
      digitalWrite(8, LOW);
      PORTD = (i<<4)|(j<<2);
      delay(500);
      digitalWrite(8,HIGH);
      PORTD = pos_data[i][j]<<2;
      delay(500);
    }
  }
  digitalWrite(8,HIGH);
    PORTD = B11111100;
    delay(5000);
  /*PORTB = B00000000;
  delay(1000);
  PORTB = B00000001;
  delay(1000);
  PORTB = B00000010;
  delay(1000);
  PORTB = B00000011;
  delay(1000);
  
  PORTB = B00000100;
  delay(1000);
  PORTB = B00000101;
  delay(1000);
  PORTB = B00000110;
  delay(1000);
  PORTB = B00000111;
  delay(1000);

  PORTB = B00001000;
  delay(1000);
  PORTB = B00001001;
  delay(1000);
  PORTB = B00001010;
  delay(1000);
  PORTB = B00001011;
  delay(1000);

  PORTB = B00001100;
  delay(1000);
  PORTB = B00001101;
  delay(1000);
  PORTB = B00001110;
  delay(1000);
  PORTB = B00001111;
  delay(1000);

  PORTB = B00010000;
  delay(1000);
  PORTB = B00010001;
  delay(1000);
  PORTB = B00010010;
  delay(1000);
  PORTB = B00010011;
  delay(1000);

  PORTB = B00000000;
  delay(1000);
  PORTB = B00000001;
  delay(1000);
  PORTB = B00000010;
  delay(1000);
  PORTB = B00000011;
  delay(1000); */
}
