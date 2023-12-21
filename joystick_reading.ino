#define xAxis 34
#define yAxis 35
#define btn 23


void setup() {
  Serial.begin(115200);

  pinMode(btn, INPUT);  
}

void loop() {
  int xValue = analogRead(xAxis);
  int yValue = analogRead(yAxis); 

  int btnValue = digitalRead(btn);
  
  Serial.printf("Joystick value is %d , %d , %d \n",xValue,yValue,btnValue);
  
  delay(300);  
}