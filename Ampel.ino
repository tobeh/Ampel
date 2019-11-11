
/*
    Name:       Ampel.ino
    Created:	09.11.2019 23:40:13
    Author:     Tobias
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts
void setup()
{
	Serial.begin(9600);
	init();
	delay(500);
	pinMode(4, OUTPUT);
	setNodeAddress(0xA6);
}

// Add the main program code into the continuous loop() function
void loop()
{
	digitalWrite(4, HIGH);
	Serial.println(getNodeAddress());
	delay(500);
	digitalWrite(4, LOW);
	delay(500);
}
