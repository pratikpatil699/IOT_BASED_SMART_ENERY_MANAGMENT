#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
char auth[] = "fe3dbb2cbdad4724b462ce4b530158c9";
char ssid[] = "pratiks";
char pass[] = "###777";
#define EspSerial Serial
#define ESP8266_BAUD 9600
ESP8266 wifi(&EspSerial);

#include <EEPROM.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

char PLS[50];
char UNT[50];
char BIL[50];

long DATA = 0;
long VAL = 0;

int pulse = 0;
int unit = 0;
float bill = 0;
char blue = 0;
int key = 0;
int d1 = 0;
int d2 = 0;
int x = 0;
int y = 0;
#define int1 18

int adc = 0;
int mains = 0;
int watt = 0;
int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
double AMP1 = 0;

void setup()
{
  lcd.begin(16, 2);
  pinMode(int1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(int1), R_PULSE1, LOW);

  if (digitalRead(int1) == LOW)
    for (int i = 0 ; i < EEPROM.length() ; i++) {
      EEPROM.write(i, 0);
    }
  lcd.setCursor(0, 0); lcd.print(" Implementation ");
  lcd.setCursor(0, 1); lcd.print("  Of an Energy  ");
  delay(1000);
  lcd.setCursor(0, 0); lcd.print(" Monitoring And ");
  lcd.setCursor(0, 1); lcd.print(" Control Device ");
  delay(1000);
  lcd.setCursor(0, 0); lcd.print("    Based On    ");
  lcd.setCursor(0, 1); lcd.print("     I.O.T.     ");
  delay(1000);
  lcd.setCursor(0, 0); lcd.print("Please Wait WiFi");
  lcd.setCursor(0, 1); lcd.print("   Initialize   ");
  delay(1000);
  pulse = EEPROM.read(0);
  unit = EEPROM.read(20);
  bill = EEPROM.read(40);
  Serial.begin(9600);
  delay(10);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
}


void loop()
{
  Blynk.run();
  lcd.setCursor(0, 0); lcd.print("P:"); lcd.print(PLS); lcd.print("  U:"); lcd.print(UNT);
  lcd.setCursor(0, 1); lcd.print("BILL:"); lcd.print(bill, 2); lcd.print("/-        ");

  if (d1 == 1 && (digitalRead(int1) == HIGH))
  {
    pulse++;
    x = pulse % 5;
    if (x == 0)
      unit++;
    d1 = 0;
  }
  if(unit<=2)
  
  bill = unit * 6.5;
  else
  bill=unit*7.5;

  DATA = sprintf (PLS, "%05d", pulse);
  DATA = sprintf (UNT, "%05d", unit);

  EEPROM.write(0, pulse);
  EEPROM.write(20, unit);
  EEPROM.write(40, bill);

  Blynk.virtualWrite(V0, pulse);
  Blynk.virtualWrite(V1, unit);
  Blynk.virtualWrite(V2, bill);
}

void R_PULSE1()
{
  d1 = 1;
}
