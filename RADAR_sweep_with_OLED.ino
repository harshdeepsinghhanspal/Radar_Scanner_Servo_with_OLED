#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <NewPing.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo servo;
NewPing sonar(9, 10, 200); // TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE

int centerX = SCREEN_WIDTH / 2;
int centerY = SCREEN_HEIGHT / 2 + 25; // Shift the semi-circle and sweep line downwards by 25 pixels
int radius = 30;
int detectedX = -1;
int detectedY = -1;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);  // Pause for 2 seconds
  display.clearDisplay(); 

  servo.attach(6); // Attach the servo to pin 6, update with your actual servo pin
  servo.write(0);  // Initialize servo position
}

void loop() {
  display.clearDisplay();

  // Sweep the servo from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle++) {
    servo.write(angle);
    delay(15); // Adjust the delay as needed

    // Measure distance using the ultrasonic sensor
    int distance = sonar.ping_cm();

    // Calculate coordinates for displaying the angle and distance on the OLED
    int x = centerX + int(radius * cos(radians(angle)));
    int y = centerY - int(radius * sin(radians(angle))); // Invert the y-coordinate

    // Draw a line to represent the sweep
    display.drawLine(centerX, centerY, x, y, SSD1306_WHITE);

    if (distance != 0 && distance > 50) {
      // Detected an object within 50 cm, mark the point
      detectedX = x;
      detectedY = y;
    }

    // Draw the line for the detected point if it exists
    if (detectedX != -1 && detectedY != -1) {
      display.drawLine(centerX, centerY, detectedX, detectedY, SSD1306_WHITE);
    }

    int startAngle = 180;  // Start angle in degrees
    int endAngle = 360;  // End angle in degrees
    
    // Draw the semi-circle using lines
    for (int semi_angle = startAngle; semi_angle <= endAngle; semi_angle++) {
      int x_semi = centerX + radius * cos(radians(semi_angle));
      int y_semi = centerY + radius * sin(radians(semi_angle)); // Invert the y-coordinate
      display.drawPixel(x_semi, y_semi, SSD1306_WHITE);
    }

    // Display the angle and distance numerically
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Angle: ");
    display.println(angle);
    display.print("Distance: ");
    display.print(distance);
    display.println(" cm");

    display.display();
    delay(10); // Small delay for display
    display.clearDisplay();
  }

  delay(100); // Pause before starting the next sweep

  // Reset detected point for the next sweep
  detectedX = -1;
  detectedY = -1;

  // Sweep the servo from 180 to 0 degrees
  for (int rev_angle = 180; rev_angle >= 0; rev_angle--) {
    servo.write(rev_angle);
    delay(15); // Adjust the delay as needed

    // Measure distance using the ultrasonic sensor
    int distance = sonar.ping_cm();

    // Calculate coordinates for displaying the angle and distance on the OLED
    int x = centerX + int(radius * cos(radians(rev_angle)));
    int y = centerY - int(radius * sin(radians(rev_angle))); // Invert the y-coordinate

    // Draw a line to represent the sweep
    display.drawLine(centerX, centerY, x, y, SSD1306_WHITE);

    if (distance != 0 && distance < 50) {
      // Detected an object within 50 cm, mark the point
      detectedX = x;
      detectedY = y;
    }

    // Draw the line for the detected point if it exists
    if (detectedX != -1 && detectedY != -1) {
      display.drawLine(centerX, centerY, detectedX, detectedY, SSD1306_WHITE);
    }

    int startAngle = 180;  // Start angle in degrees
    int endAngle = 360;  // End angle in degrees
    
    // Draw the semi-circle using lines
    for (int semi_angle = startAngle; semi_angle <= endAngle; semi_angle++) {
      int x_semi = centerX + radius * cos(radians(semi_angle));
      int y_semi = centerY + radius * sin(radians(semi_angle)); // Invert the y-coordinate
      display.drawPixel(x_semi, y_semi, SSD1306_WHITE);
    }

    // Display the angle and distance numerically
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Angle: ");
    display.println(rev_angle);
    display.print("Distance: ");
    display.print(distance);
    display.println(" cm");

    display.display();
    delay(10); // Small delay for display
    display.clearDisplay();
  }

  delay(100); // Pause before starting the next sweep
}
