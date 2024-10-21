#include <Wire.h>
#include <LiquidCrystal.h>
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#include <LiquidCrystal.h>


#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

char input[12];
int count = 0;
String Item;
int Price;
int a;
String dummy;
int p = 0;
int p1 = 0, p2 = 0, p4 = 0, p5 = 0, p6 = 0;
 
double total = 0;
int count_prod = 0;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);


void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE); 

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["Earphones(Rs.200)"] = p1;
  doc["Coco cola(Rs.40)"] = p2;
  doc["Biscuit(Rs.30)"] = p4;
  doc["Wheat 1kg(Rs.55)"] = p5;
  doc["Apples(Rs.40)"] = p6;
  doc["Total_Products"] =count_prod;
  doc["total"]=total;
  doc["exit_condition"] = dummy;
  
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

void setup ()
{
  pinMode(32, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(12, OUTPUT);
 
  lcd.clear();
  Wire.begin();
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print(" AUTOMATIC BILL");
  delay (2000);
  lcd.setCursor(0, 1);
  lcd.print(" SHOPPING CART ");
  delay (2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO");
  delay (2000);
    lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SUPER MARKET");
  delay (2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Plz Add ITem");
  connectAWS();

}
 
 
 
void loop()
{
  count = 0;
  char input[12];
  while (Serial.available() && count < 12)
  {
    input[count] = Serial.read();
    count++;
    delay(5);
  }
  Serial.print(input);
  int a = digitalRead(32);
  Serial.println(a);
  delay(2000);
 
  if ((strncmp(input, "0B0028141720", 12) == 0) && (a == 1))
  {
    Item = "Earphones";
    Price = 200;
    lcd.setCursor(0, 0);
    lcd.print("Earphones Added       ");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Price :- 200.00      ");
    p1++;
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(12, LOW);
    delay(1000);
    total = total + 200.00;
    count_prod++;
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(12, HIGH);
    delay(1000);
          lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Price :-");
       delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
       delay(100);
      lcd.print(total);   
 
  }
  else if ((strncmp(input, "0B0028141720", 12) == 0) && (a == 0))
  {
    if (p1 > 0)
    {
      Item = "Earphones";
      Price = 200;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Earphones Removed!!!        ");
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(12, LOW);
      delay(1000);
      p1--;
      total = total - 200.00;
      count_prod--;
      lcd.clear();
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(12, HIGH);
                delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Price :-");
       delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
       delay(100);
      lcd.print(total);

    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not in cart!!!        ");
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(12, HIGH);
      delay(2000);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(12, LOW);
      lcd.clear();
    }
  }
 
 
  if ((strncmp(input, "0B00287FBBE7", 12) == 0) && (a == 1))
  {
    Item = "Coca cola";
    Price = 40;
    lcd.setCursor(0, 0);
    lcd.print("Coca cola Added       ");
        delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Price :- 40.00      ");
    p2++;
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(12, LOW);
    delay(1000);
    total = total + 40.00;
    count_prod++;
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(12, HIGH);
        delay(1000);
              lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Price :-");
       delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
       delay(100);
      lcd.print(total);
  }
 
  else if ((strncmp(input, "0B00287FBBE7", 12) == 0) && (a == 0))
  {
    if (p2 > 0)
    {
      Item = "Coca cola";
      Price = 40;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Coca cola Removed!!!        ");
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(12, LOW);
      delay(1000);
      p2--;
      total = total - 40.00;
      count_prod--;
      lcd.clear();
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(12, HIGH);
          delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Price :-");
       delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
       delay(100);
      lcd.print(total);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not in cart!!!        ");
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(12, HIGH);
      delay(2000);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(12, LOW);
      lcd.clear();
    }
  }
 
 
if ((strncmp(input, "0B002821A2A0", 12) == 0) && (a==1))
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Your Bill Items are: ");
    delay(1000);
    String Bill[6] = {"Earphones", "Coca cola", "Biscuit", "Wheat 1kg", "apples", "\0"};
     for( int i=0; i<6; i = i+1){
        if (Bill[i] == "Earphones" && p1> 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Earphones");
          lcd.setCursor(11,0);
          lcd.print(p1);
          delay(1500);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(p1*200);
          delay(1000);
        }
        else if(Bill[i] == "Coca cola" && p2> 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Coca cola");
          lcd.setCursor(11,0);
          lcd.print(p2);
          delay(1500);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(p2*40);
          delay(1000);
          
        }
        else if(Bill[i] == "Biscuit" && p4> 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Biscuit");
          lcd.setCursor(9,0);
          lcd.print(p4);
          delay(1500);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(p4*30);
          delay(1000);
          
        }
         else if(Bill[i] == "Wheat 1kg" && p5> 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Wheat 1kg");
          lcd.setCursor(11,0);
          lcd.print(p5);
          delay(1500);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(p5*55);
          delay(1000);
          
        }
         else if(Bill[i] == "apples" && p6> 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("apples");
          lcd.setCursor(8,0);
          lcd.print(p6);
          delay(1500);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(p6*40);
          delay(1000);
          
        }
        else if(Bill[i] == "\0"){
          break;
        }

     }
     lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your Total bill:- ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(total);
    delay(2000);
     digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(12, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Exiting Shopping Cart");
    delay(2000);
    lcd.clear();
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(12, LOW);
    dummy = "q";
    publishMessage();
    delay(2000);
    exit(0);     
  }
else if ((strncmp(input, "0B002821A2A0", 12) == 0) && (a==0)){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Your Bill Items are: ");
    delay(1000);
    String Bill1[6] = {"Earphones", "Coca cola", "Biscuit", "Wheat 1kg", "Apples", "\0"};
     for( int i=0; i<6; i = i+1){
        if (Bill1[i] == "Earphones" && p1> 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Earphones");
          lcd.setCursor(11,0);
          lcd.print(p1);
          delay(1500);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(p1*200);
          delay(1000);
        }
        else if(Bill1[i] == "Coca cola" && p2> 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Coca cola");
          lcd.setCursor(11,0);
          lcd.print(p2);
          delay(1500);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(p2*40);
          delay(1000);
          
        }
        else if(Bill1[i] == "Biscuit" && p4> 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Biscuit");
          lcd.setCursor(9,0);
          lcd.print(p4);
          delay(1500);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(p4*30);
          delay(1000);
          
        }
         else if(Bill1[i] == "Wheat 1kg" && p5> 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Wheat 1kg");
          lcd.setCursor(11,0);
          lcd.print(p5);
          delay(1500);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(p5*55);
          delay(1000);
          
        }
         else if(Bill1[i] == "Apples" && p6> 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Apples");
          lcd.setCursor(8,0);
          lcd.print(p6);
          delay(1500);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(p6*40);
          delay(1000);
          
        }
        else if(Bill1[i] == "\0"){
          break;
        }

     }
     lcd.clear();
        lcd.setCursor(0,0);
    lcd.print("Plz Add Item");
    delay(1000);
}
if ((strncmp(input, "0B00286A4B02", 12) == 0) && (a == 1))
  {
    Item = "Biscuit";
    Price = 30;
    lcd.setCursor(0, 0);
    lcd.print("Biscuit Added       ");
        delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Price :- 30.00      ");
    p4++;
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(12, LOW);
    delay(1000);
    total = total + 30.00;
    count_prod++;
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(12, HIGH);
        delay(1000);
              lcd.clear();
      lcd.setCursor(0, 0);
        lcd.print("Total Price :-");
       delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
       delay(100);
      lcd.print(total);
 
  }
 
  else if ((strncmp(input, "0B00286A4B02", 12) == 0) && (a == 0))
  {
    if (p4 > 0)
    {
      Item = "Biscuit";
      Price = 30;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Biscuit Removed!!!        ");
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(12, LOW);
      delay(1000);
      p4--;
      total = total - 30.00;
      count_prod--;
      lcd.clear();
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(12, HIGH);
          delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Price :-");
       delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
       delay(100);
      lcd.print(total);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not in cart!!!        ");
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(12, HIGH);
      delay(2000);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(12, LOW);
      lcd.clear();
    }
  }
if ((strncmp(input, "0B002817784C", 12) == 0) && (a == 1))
  {
    Item = "Wheat 1kg";
    Price = 55;
    lcd.setCursor(0, 0);
    lcd.print("Wheat 1kg Added       ");
        delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Price :- 55.00      ");
    p5++;
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(12, LOW);
    delay(1000);
    total = total + 55.00;
    count_prod++;
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(12, HIGH);
        delay(1000);
           lcd.clear();
      lcd.setCursor(0, 0);
       lcd.print("Total Price :-");
       delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
       delay(100);
      lcd.print(total);
 
  }
 
  else if ((strncmp(input, "0B002817784C", 12) == 0) && (a == 0))
  {
    if (p5 > 0)
    {
      Item = "Wheat 1kg";
      Price = 55;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wheat 1kg Removed!!!        ");
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(12, LOW);
      delay(1000);
      p5--;
      total = total - 55.00;
      count_prod--;
      lcd.clear();
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(12, HIGH);
          delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Price :-");
       delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
       delay(100);
      lcd.print(total);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not in cart!!!        ");
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(12, HIGH);
      delay(2000);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(12, LOW);
      lcd.clear();
    }
  }
if ((strncmp(input, "0B00287D91CF", 12) == 0) && (a == 1))
  {
    Item = "Apples";
    Price = 40;
    lcd.setCursor(0, 0);
    lcd.print("Apples Added       ");
        delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Price :- 40.00      ");
    p6++;
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(12, LOW);
    delay(1000);
    total = total + 40.00;
    count_prod++;
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(12, HIGH);
        delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);        
         lcd.print("Total Price :-");
       delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
       delay(100);
      lcd.print(total);
 
  }
 
  else if ((strncmp(input, "0B00287D91CF", 12) == 0) && (a == 0))
  {
    if (p6 > 0)
    {
      Item = "Apples";
      Price = 40;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Apples Removed!!!        ");
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(12, LOW);
      delay(1000);
      p6--; 
      total = total - 40.00;
      count_prod--;
      lcd.clear();
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(12, HIGH);
          delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Price :-");
       delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
       delay(100);
      lcd.print(total);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not in cart!!!        ");
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(12, HIGH);
      delay(2000);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(12, LOW);
      lcd.clear();
    }
  }
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(12, LOW);
  publishMessage();
  client.loop();
  delay(1000); // Update readings every second
  
}
