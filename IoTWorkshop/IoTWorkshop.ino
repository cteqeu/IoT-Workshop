

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

/* Set these to your desired credentials. */
const char *ssid = "IoTWorkshop";
const char *password = "IoTWorkshop";
String decodedMsg= "Welcome...";
ESP8266WebServer server(80);


//String value;
//int z=0;
//int pixelsInText;
//int r,g,b;
int x;
#define PIN 2

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 1, PIN,
NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
NEO_GRB + NEO_KHZ800);
int R= 255;
int G= 0;
int B=0;
//uint16_t colors[] = {matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };
uint16_t colors[] = {matrix.Color(255, 0, 0)};
 
void handleRoot() {
  server.send(200, "text/html", "<h1>More info: www.pxl.be</h1>");
}

void ScrollText(String value)
{
  int pixelsInText = (value.length()*7)+8;
  while(x+17 > (matrix.width() - pixelsInText)){
      matrix.fillScreen(0);
      matrix.setCursor(--x, 0);
      matrix.print(value);
      matrix.show();
      delay(100);
}
   x    = matrix.width();
}

String decodeMsg(String value)
{
  // Restore special characters that are misformed to %char by the client browser
  value.replace("+", " ");
  value.replace("%20", " ");
  value.replace("%21", "!");
  value.replace("%22", "");
  value.replace("%23", "#");
  value.replace("%24", "$");
  value.replace("%25", "%");
  value.replace("%26", "&");
  value.replace("%27", "'");
  value.replace("%28", "(");
  value.replace("%29", ")");
  value.replace("%2A", "*");
  value.replace("%2B", "+");
  value.replace("%2C", ",");
  value.replace("%2F", "/");
  value.replace("%3A", ":");
  value.replace("%3B", ";");
  value.replace("%3C", "<");
  value.replace("%3D", "=");
  value.replace("%3E", ">");
  value.replace("%3F", "?");
  value.replace("%40", "@");

  return value;
}

void handle_msg() 
{  
  String msg = server.arg("msg");
 
//  Serial.println(msg);
  matrix.setTextColor(colors[0]);
    ScrollText(decodeMsg(msg));
  //ScrollText(msg);
  server.send(200, "text/html", "<h1>Message: " + msg + "</h1>");
}



void handle_rgb() 
{
                                        // clears oled
   // int rgb = server.arg("rgb").toInt();
    String rgb = server.arg("rgb");
    int r_rgb,b_rgb, g_rgb=255;
    r_rgb= rgb.substring(0,3).toInt();
    g_rgb=rgb.substring(4,7).toInt();
    b_rgb=rgb.substring(8,11).toInt();
    colors[0]=matrix.Color(r_rgb, g_rgb,b_rgb);
 //   Serial.println(rgb);
 //   Serial.print(r_rgb);
 //   Serial.print(g_rgb);
 //   Serial.print(b_rgb);
    matrix.setTextColor(matrix.Color(r_rgb, g_rgb, b_rgb));
    //ScrollText(decodeMsg(rgb));
   // ScrollText("Color Changed");
    server.send(200, "text/html", "<h1>Color Changed to " + rgb + "</h1>");
}


void handle_pix() 
{
                                        // clears oled
   // int rgb = server.arg("rgb").toInt();
    String pix = server.arg("pix");
    int x,y;
    x=  pix.substring(0,1).toInt();
    y=  pix.substring(1,2).toInt();
    matrix.drawPixel(x,y,colors[0]);
    matrix.show();
    server.send(200, "text/html", "<h1>PIX changed to " + pix + "</h1>");
}

void handle_clear() 
{
                                        // clears oled
   // int rgb = server.arg("rgb").toInt();
    String pix = server.arg("clear");
   // int x,y;
   // x=  pix.substring(0,1).toInt();
   // y=  pix.substring(1,2).toInt();
    matrix.clear();
    matrix.show();
    server.send(200, "text/html", "<h1>Matrix Cleared</h1>");
}


void setup() {
  matrix.begin(); 
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
  delay(1000);
   x    = matrix.width();
  Serial.begin(115200);
  //Serial.println();
  //Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.on("/", handleRoot);
  server.on("/msg", handle_msg);
  server.on("/rgb", handle_rgb);
  server.on("/pix", handle_pix);
  server.on("/clear", handle_clear);
  server.begin();
  
//  Serial.println("HTTP server started");
  
  delay(2000);
  String value="PXL PXL PXL";
  
  ScrollText(decodeMsg(value));
}

void loop() {
  server.handleClient();
}
