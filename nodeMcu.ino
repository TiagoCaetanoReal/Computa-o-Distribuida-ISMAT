/*********
  Autoria: Tiago Caetano
  Baseado em:
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
//
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D1, D2);
//

// Replace with your network credentials
const char* ssid     = "";
const char* password = "";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  //
  mySerial.begin(9600);
  //

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  String strs[3];
  int StringCount = 0;
  String msg = mySerial.readStringUntil('\r');

  while (msg.length() > 0)
  {
    int index = msg.indexOf(' ');
    if (index == -1) // No space found
    {
      strs[StringCount++] = msg;
      break;
    }
    else
    {
      strs[StringCount++] = msg.substring(0, index);
      msg = msg.substring(index+1);
    }
  }

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html lang=\"en\" class=\"h-100\">");
            client.println("<head><meta charset=\"utf-8\" name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<title>MyHorta</title>");
            
            //styling
            client.println("<link rel=\"icon\" type=\"image/x-icon\" href=\"https://w1.pngwing.com/pngs/523/470/png-transparent-green-leaf-logo-plants-garden-seedling-flower-garden-symbol-nursery-gardening.png \">");
            client.println("<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css \" rel=\"stylesheet\" integrity=\"sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65\" crossorigin=\"anonymous\">");
            client.println("<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.1/css/all.min.css \"></head>");
          
            // Web Page Heading
            client.println("<body style=\"background-image: linear-gradient(rgb(0, 181, 194), rgb(0, 177, 86));\">");
            client.println("<h1 class=\"text-center py-5 text-white\"><strong>MyHortaData</strong></h1>");
            client.println("<div class=\"container-sm\"><div class=\"row py-5 text-center\"><div class=\"col text-end\">");
            client.println("<i class=\"fa fa-3x fa-droplet text-white\"></i></div> <div class=\"col text-start\">");
            client.println("<h2 class=\"text-white py-2\">- ");
            client.println(strs[0]);
            client.println("%</h2></div></div>");
            client.println("<div class=\"row py-5 text-center\"><div class=\"col text-end\"><i class=\"fa fa-3x fa-sun text-white\"></i>");
            client.println("</div><div class=\"col text-start\"><h2 class=\"text-white py-2\">- ");
            client.println(strs[1]);
            client.println("%</h2></div></div>");
            client.println("<div class=\"row py-5 text-center\"><div class=\"col text-end\">");
            client.println("<i class=\"fa fa-3x fa-temperature-half text-white\"></i></div><div class=\"col text-start\">");
            client.println("<h2 class=\"text-white py-2\">- ");
            client.println(strs[2]);
            client.println("º</h2></div></div>");
            client.println("</h2></div></div>");
            client.println("<div class=\"position-absolute start-50 translate-middle\" style=\"top:90%\">");
            client.println("<img src=\"https://recil.ensinolusofona.pt/image/logo-ismat.png \"></div></body>");
            client.println("<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js \" integrity=\"sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4\" crossorigin=\"anonymous\"></script>");
            client.println("</html>");
          
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}



