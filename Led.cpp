#include <iostream>
#include <string>
#include "pico_sdk.h"

// Define a function to initialize the GPIO pins
void init_gpio(int pin) {
  // Initialize the pin
  gpio_init(pin);
  // Set the pin as an output
  gpio_dir(pin, OUTPUT);
}

// Define a function to generate an HTML form
std::string generate_form() {
  std::string form = "<form method=\"post\" action=\"/led\">\n";
  form += "  <label for=\"on\">On</label>\n";
  form += "  <input type=\"radio\" name=\"state\" value=\"on\" id=\"on\">\n";
  form += "  <label for=\"off\">Off</label>\n";
  form += "  <input type=\"radio\" name=\"state\" value=\"off\" id=\"off\">\n";
  form += "  <input type=\"submit\" value=\"Submit\">\n";
  form += "</form>\n";
  return form;
}

// Define a function to handle incoming requests
int handle_request(const std::string& request, std::string& response) {
  // Check if the request is a GET request to the root endpoint
  if (request.find("GET / HTTP/1.1") != std::string::npos) {
    // Set the response body to the HTML form
    response = generate_form();
  } else if (request.find("POST /led HTTP/1.1") != std::string::npos) {
    // Parse the request body to get the state of the LED
    std::size_t pos = request.find("\r\n\r\n");
    if (pos != std::string::npos) {
      std::string body = request.substr(pos + 4);
      if (body == "on") {
        // Turn the LED on
        std::cout << "Turning LED on" << std::endl;
        gpio_write(LED_PIN, HIGH);
      } else if (body == "off") {
        // Turn the LED off
        std::cout << "Turning LED off" << std::endl;
        gpio_write(LED_PIN, LOW);
      }
    }
    // Set the response body
    response = "OK";
  } else {
    // Set the response body
    response = "Invalid request";
  }
  // Return a 200 OK status code
  return 200;
}

int main() {
  // Initialize the LED pin
  init_gpio(LED_PIN);
  // Create a new PicoServer instance
  PicoServer server;
  // Set the request handler function
  server.set_request_handler(handle_request);
  // Start the server
  server.start();
  // Run the server loop
  server.serve_forever();
  return 0;
}
