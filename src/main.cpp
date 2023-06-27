/*
 *  Project Final - main.cpp
 */



#include <WiFi.h>
// #include <WiFiClient.h>
#include <WebServer.h>

// const char* ssid = "UA-Alumnos";
// const char* password = "41umn05WLC";
#include "wifi_ruts.h"
WebServer server(80); // Create a webserver object that listens for HTTP request on port 80



const int trigPin = 4; // SR-HC04 trigger pin
const int echoPin = 16; // SR-HC04 echo pin

const int redLedPin = 22;    // Red LED pin
// const int yellowLedPin = 23; // Yellow LED pin
const int greenLedPin = 23;  // Green LED pin
const double sound_speed = 0.0343;

bool isCarParked = false;
bool isReserved = false;

void handleRoot() {
  String status;
  String color;
  
  if (isCarParked) {
    status = "Garage is taken";
    digitalWrite(redLedPin, LOW);
    // digitalWrite(yellowLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    color = "red";
  } else if (isReserved) {
    status = "Garage is reserved";
    digitalWrite(redLedPin, LOW);
    // digitalWrite(yellowLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    color = "yellow";
  } else {
    status = "Garage is free";
    digitalWrite(redLedPin, HIGH);
    // digitalWrite(yellowLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    color = "green";
  }
  
  String html = "<html><head>";
  html += "<style>";
  html += "body { font-family: Arial, Helvetica, sans-serif; }";
  html += ".status { font-size: 24px; font-weight: bold; color: " + color + "; }";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }";
  html += "</style>";
  html += "<script>";
  html += "function refreshPage() {";
  html += "  window.location.reload();";
  html += "}";
  html += "setTimeout(refreshPage, 1000);";  // Refresh the page every 1 second
  html += "</script>";
  html += "</head><body>";
  html += "<h1 class='status'>" + status + "</h1>";
  
  if (!isReserved && !isCarParked) {
    html += "<button class='button' onclick=\"makeReservation()\">Reserve Garage</button>";
    html += "<script>";
    html += "function makeReservation() {";
    html += "  var xhttp = new XMLHttpRequest();";
    html += "  xhttp.onreadystatechange = function() {";
    html += "    if (this.readyState == 4 && this.status == 200) {";
    html += "      location.reload();";  // Refresh the page after making a reservation
    html += "    }";
    html += "  };";
    html += "  xhttp.open('GET', '/reserve', true);";
    html += "  xhttp.send();";
    html += "}";
    html += "</script>";
  }
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleReservation() {
    isReserved = true;
    server.send(200, "text/plain", "Reservation status updated");
    server.sendHeader("Location", "/"); // Add a header to respond with a new location for the browser to go to the home page again
    server.send(303); 
}

void setup() {
    Serial.begin(SERIAL_BAUD); // Start the Serial communication to send messages to the computer
    delay(10);
    Serial.println();
    pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);
  
  pinMode(redLedPin, OUTPUT);
//   pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  
  digitalWrite(redLedPin, HIGH);
//   digitalWrite(yellowLedPin, LOW);
  digitalWrite(greenLedPin, HIGH);
  
    wifi_connect();

  
  server.on("/", handleRoot);
  server.on("/reserve", handleReservation);
  
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
  
  // Read the sensor and update the car status
  long duration;
  double distance;

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * sound_speed / 2;

  Serial.printf("Distance = %7.1f cm\n", distance);
  delay(2000);

//   if (duration < 5000) { // Adjust this threshold according to your sensor
//     isCarParked = true;
//   } else {
//     isCarParked = false;
//   }
  if(round(distance * 10) / 10.0 == 12.0){
    isCarParked = false;
  }
  if(round(distance * 10) / 10.0 == 12.0 && !isReserved){
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    }

    else if(round(distance * 10) / 10.0 < 12.0){
        digitalWrite(greenLedPin, HIGH);
        digitalWrite(redLedPin, LOW);
        isCarParked = true;
        isReserved = false;
    }
}



// #include <Arduino.h>

// #include <WiFi.h>
// #include <WebServer.h>

// #include "wifi_ruts.h"

// WebServer server(80); // Create a webserver object that listens for HTTP request on port 80

// int selectedMinutes = 1;
// boolean isReserved = false;
// const double sound_speed = 0.0343;
// void handleRedLED(void)
// {
//     // If a POST request is made to URI /LED, then toggle LED state
//     Serial.println(__FUNCTION__);

//     digitalWrite(LEDRED, LOW);         // Change the state of the LED
//     digitalWrite(LEDGREEN, HIGH);        // Change the state of the LED
// }
// void handleGreenLED(void)
// {
//     // If a POST request is made to URI /LED, then toggle LED state
//     Serial.println(__FUNCTION__);

//     digitalWrite(LEDRED, HIGH);          // Change the state of the LED
//     digitalWrite(LEDGREEN, LOW);       // Change the state of the LED
// }
// static void
// send_trigger(void)
// {
//     digitalWrite(TRIG_PIN, HIGH);
//     delayMicroseconds(10);
//     digitalWrite(TRIG_PIN, LOW);
// }

// static long
// get_pulse(void)
// {
//     return pulseIn(ECHO_PIN, HIGH); // in microseconds
// }

// void handleRoot()
// {
//     long duration;
//     double distance;

//     send_trigger();
//     duration = get_pulse();

//     distance = duration * sound_speed / 2;
//     String html;
//     if (isReserved)
//     {
//         html = "<html>"
//                "<head>"
//                "<title>Tiempo de espera</title>"
//                "<script>"
//                "var countDownDate = new Date().getTime() + (" +
//                String(selectedMinutes) + " * 60 * 1000);"
//                                          "var x = setInterval(function() {"
//                                          "  var now = new Date().getTime();"
//                                          "  var distance = countDownDate - now;"
//                                          "  var minutes = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));"
//                                          "  var seconds = Math.floor((distance % (1000 * 60)) / 1000);"
//                                          "  document.getElementById('timer').innerHTML = minutes + 'm ' + seconds + 's';"
//                                          "  if (distance < 0) {"
//                                          "      clearInterval(x);"
//                                          "      var xhr = new XMLHttpRequest();"
//                                          "      xhr.open('POST', '/endReservation', true);"
//                                          "      xhr.send();"
//                                          "  }"
//                                          "}, 1000);"
//                                          "</script>"
//                                          "</head>"
//                                          "<body>"
//                                          "  <h1>Ya hay una reserva en curso, vas a tener que esperar: </h1>"
//                                          "  <p id=\"timer\"></p>"
//                                          "</body>"
//                                          "</html>";
//         server.send(200, "text/html", html);
//     }
//     else if (round(distance * 10) / 10.0 == 8.0)
//     {
//         // When URI / is requested, send a web page with a button to navigate to a different page
//         html = "<body>"
//                "  <h1>La cochera esta libre de autos, para reservar toque el siguiente boton: </h1>";
//         html += "<form action=\"/waitingTime\" method=\"GET\">";
//         html += "<input type=\"submit\" value=\"Reservar\" style=\"height:200px; width:500px; font-size: 300%; margin: 0 auto; display: block;\">";
//         html += "</form> </body>";
//         server.send(200, "text/html", html);
//         handleGreenLED();
//     }
//     else
//     {
//         // When URI / is requested, send a web page with a button to navigate to a different page
//         html = "<body> La cochera esta ocupada </body>";
//         server.send(200, "text/html", html);
//         handleRedLED();
//     }
// }

// void handleNavigate()
// {
//     // Handle the navigation to a different page when the button is pressed
//     server.sendHeader("Location", "/waitingTime", true); // Redirect to "/choice" page
//     server.send(302, "text/plain", "");                  // Send the redirect response
// }
// void handleYellowLED(void)
// {
//     // If a POST request is made to URI /LED, then toggle LED state
//     Serial.println(__FUNCTION__);
//     isReserved = true;
//     digitalWrite(LEDRED, LOW);         // Change the state of the LED
//     digitalWrite(LEDGREEN, LOW);       // Change the state of the LED
//     server.sendHeader("Location", "/"); // Add a header to respond with a new location for the browser to go to the home page again
//     server.send(303);                   // Send it back to the browser with an HTTP status 303 (See Other) to redirect
// }
// void handleReservation(void)
// {
//     if (server.method() == HTTP_POST)
//     {
//         selectedMinutes = server.arg("minutes").toInt(); // Obtener los minutos seleccionados del formulario y guardarlos en la variable
//         handleYellowLED();
//     }

//     String html = "<head> <title>Reserva de minutos</title>"
//                   "<script>"
//                   "function updateSelectedMinutes() {"
//                   "  var rangeInput = document.getElementById('rangeInput');"
//                   "  var outputElement = document.getElementById('selectedMinutes');"
//                   "  outputElement.innerHTML = rangeInput.value;"
//                   "}"
//                   "</script>"
//                   "</head>"
//                   "<body>"
//                   "  <h1>Por cuantos minutos quiere reservar?</h1>"
//                   "  <form action=\"/reservation\" method=\"POST\">"
//                   "    <input type=\"range\" id=\"rangeInput\" name=\"minutes\" min=\"1\" max=\"10\" step=\"1\" oninput=\"updateSelectedMinutes()\">"
//                   "    <br>"
//                   "    <output id=\"selectedMinutes\" for=\"rangeInput\">" +
//                   String(selectedMinutes) + "</output> minutos"
//                                             "    <br>"
//                                             "    <input type=\"submit\" value=\"Reservar\">"
//                                             "  </form>"
//                                             "</body>";

//     server.send(200, "text/html", html);
// }

// void handleEndReservation(void)
// {
//     Serial.println(__FUNCTION__);
//     long duration;
//     double distance;
//     isReserved = false;

//     send_trigger();
//     duration = get_pulse();

//     distance = duration * sound_speed / 2;
//     if (round(distance * 10) / 10.0 != 8.0)
//     {
//         handleRedLED();
//     }
//     else
//     {
//         handleGreenLED();
//     }
//     server.sendHeader("Location", "/"); // Add a header to respond with a new location for the browser to go to the home page again
//     server.send(303); 

// }

// void handleNotFound(void)
// {
//     Serial.println(__FUNCTION__);

//     server.send(404, "text/plain",
//                 "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
// }

// void setup(void)
// {
    // Serial.begin(SERIAL_BAUD); // Start the Serial communication to send messages to the computer
    // delay(10);
    // Serial.println();

//     pinMode(LEDGREEN, OUTPUT);
//     pinMode(LEDRED, OUTPUT);

    // wifi_connect();

//     server.on("/", HTTP_GET, handleRoot);
//     // server.on("/reservation", HTTP_GET, handleReservation); // Add this line
//     server.on("/reservation", HTTP_POST, handleYellowLED);
//     server.on("/endReservation", HTTP_POST, handleEndReservation);
//     server.on("/waitingTime", HTTP_GET, handleReservation);
//     server.onNotFound(handleNotFound); // When a client requests an unknown

//     server.begin(); // Actually start the server
//     Serial.println("HTTP server started");

//     pinMode(TRIG_PIN, OUTPUT);   // trigPin as output
//     digitalWrite(TRIG_PIN, LOW); // trigPin to low
//     pinMode(ECHO_PIN, INPUT);    // echoPin as input
    

//     Serial.begin(BAUD); 
// }

// void loop(void)
// {
//     server.handleClient(); // Listen for HTTP requests from clients
//     long duration;
//     double distance;

//     send_trigger();
//     duration = get_pulse();

//     distance = duration * sound_speed / 2;
//     Serial.printf("Distance = %7.1f cm\n", distance);
//     delay(2000);
//     Serial.printf("Red: %b", digitalRead(LEDRED));
//     Serial.printf("Green: %b", digitalRead(LEDGREEN));
//     if(round(distance * 10) / 10.0 == 8.0 && !isReserved){
//         handleGreenLED();
//     }
//     else if(round(distance * 10) / 10.0 == 8.0){
//         handleRedLED();
//     }
// }
