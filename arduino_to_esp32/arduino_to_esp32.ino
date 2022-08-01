float a = 6.0;
void setup() {
  Serial.begin(9600);
}
void loop() {

  if(a > 15) a = 9.0;
  a = a+ 0.5;
  
  Serial.println(String(a));
  delay(5000);
}
