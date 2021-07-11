/* INCLUDES **************************************************************************************/

#include <encoder.h>
#include <PololuWheelEncoders.h>
#include <Wire.h>
#include <NewPing.h>
#include <message_parser.h>
#include <compass.h>

/* DEFINES ***************************************************************************************/

#define DEBUG				1
#define BAUD_RATE			9600
#define PIN_ECHO			2
#define PIN_TRIG			3
#define PIN_MOTOR_RIGHT_1	6
#define PIN_MOTOR_RIGHT_2	7
#define PIN_MOTOR_LEFT_1	8
#define PIN_MOTOR_LEFT_2	9	
#define PIN_ENCODER_A		19
#define PIN_ENCODER_B		18
#define MSG_TYPE_PR			100
#define MSG_TYPE_MARKER		101

/* MACROS ****************************************************************************************/

#define STR_(val) #val
#define STR(val) STR_(val)

/* GLOBALS ***************************************************************************************/

static NewPing g_sonar(PIN_TRIG, PIN_ECHO, 200);
static Compass g_compass(-158.02f, 164.8f, 530.32f, 1.0f, 1.06f, 0.95f);
static Encoder g_encoder(19, 18);
static uint8_t g_prUpdate[] = { 22, 22, 1, 48, 57, 2, 49, 50, 51, 52, 48, 48, 57, 56, 55, 3, 49, 4 };
static Message g_msg(msg_proc);
static bool g_moving = false;
static float g_distToObj = 0;
static float g_heading = 0;

/* MOVEMENT **************************************************************************************/

void mov_stop()
{
	digitalWrite(PIN_MOTOR_LEFT_1, HIGH);
	digitalWrite(PIN_MOTOR_LEFT_2, HIGH);
	digitalWrite(PIN_MOTOR_RIGHT_1, HIGH);
	digitalWrite(PIN_MOTOR_RIGHT_2, HIGH);
	g_moving = false;
	g_encoder.disable();

#if DEBUG
	Serial.println("Stopped movement");
#endif
}

void mov_forward()
{
#if DEBUG
	Serial.println("Moving forward");
#endif

	g_moving = true;
	g_encoder.enable();
	digitalWrite(PIN_MOTOR_LEFT_1, HIGH);
	digitalWrite(PIN_MOTOR_LEFT_2, LOW);
	digitalWrite(PIN_MOTOR_RIGHT_1, LOW);
	digitalWrite(PIN_MOTOR_RIGHT_2, HIGH);
}

void mov_backward()
{
#if DEBUG
	Serial.println("Moving backward");
#endif

	g_moving = true;
	g_encoder.enable();
	digitalWrite(PIN_MOTOR_LEFT_1, LOW);
	digitalWrite(PIN_MOTOR_LEFT_2, HIGH);
	digitalWrite(PIN_MOTOR_RIGHT_1, HIGH);
	digitalWrite(PIN_MOTOR_RIGHT_2, LOW);
	delay(250);
	mov_stop();
}

void mov_counterclockwise(float angle)
{
#if DEBUG
	Serial.print("Turning counter-clockwise ");
	Serial.print(angle);
	Serial.println(" degrees");
#endif

	float startingHeading = g_heading;
	float targetHeading = startingHeading - angle;

	if (targetHeading < 0) { targetHeading += 360; }

	float targetHeadingMin = targetHeading - 5;
	float targetHeadingMax = targetHeading + 5;

	if (targetHeadingMin < 0) { targetHeadingMin += 360; }
	if (targetHeadingMax > 360) { targetHeading -= 360; }

	g_moving = true;
	g_encoder.disable();
	digitalWrite(PIN_MOTOR_LEFT_1, HIGH);
	digitalWrite(PIN_MOTOR_LEFT_2, LOW);
	digitalWrite(PIN_MOTOR_RIGHT_1, HIGH);
	digitalWrite(PIN_MOTOR_RIGHT_2, LOW);

	delay(10);
	g_heading = g_compass.get_heading();
	while (g_heading < targetHeadingMin || g_heading > targetHeadingMax)
	{
		delay(10);
		g_heading = g_compass.get_heading();
	}

	mov_stop();
}

void mov_clockwise(float angle)
{
#if DEBUG
	Serial.print("Turning clockwise ");
	Serial.print(angle);
	Serial.println(" degrees");
#endif

	float startingHeading = g_heading;
	float targetHeading = startingHeading + angle;

	if (targetHeading > 360) { targetHeading -= 360; }

	float targetHeadingMin = targetHeading - 5;
	float targetHeadingMax = targetHeading + 5;

	if (targetHeadingMin < 0) { targetHeadingMin += 360; }
	if (targetHeadingMax > 360) { targetHeading -= 360; }

	g_moving = true;
	g_encoder.disable();
	digitalWrite(PIN_MOTOR_LEFT_1, LOW);
	digitalWrite(PIN_MOTOR_LEFT_2, HIGH);
	digitalWrite(PIN_MOTOR_RIGHT_1, LOW);
	digitalWrite(PIN_MOTOR_RIGHT_2, HIGH);

	delay(10);
	g_heading = g_compass.get_heading();
	while (g_heading < targetHeadingMin || g_heading > targetHeadingMax)
	{
		delay(10);
		g_heading = g_compass.get_heading();
	}

	mov_stop();
}

bool safe_to_move()
{
	return !(g_distToObj < 10);
}

/* DETECTION *************************************************************************************/

void update_sensor_data()
{
	g_distToObj = g_sonar.get_distance();
	g_heading = g_compass.get_heading();
}

/* COMM ******************************************************************************************/

void set_payload_length(int value, uint8_t* msg)
{
	if (value > 99 || value < 0) { return; }
	static char buff[3];
	itoa(value, buff, 10);
	msg[0] = buff[0];
	msg[1] = buff[1];
}

void set_payload_float(float value, uint8_t* payload)
{
	if (value >= 1000 || value <= -1000) { return; }

	double x = value;

	if (x < 0)
	{
		*payload = (uint8_t)('-');
		x *= -1;
	}
	else
	{
		*payload = (uint8_t)('+');
	}

	payload++;
	uint8_t iX = ((uint8_t)(x / 100)) % 10;
	*payload = iX + 48;
	payload++;
	iX = ((uint8_t)(x / 10)) % 10;
	*payload = iX + 48;
	payload++;
	iX = (uint8_t)x % 10;
	*payload = iX + 48;
	payload++;
	iX = ((uint8_t)(x * 10)) % 10;
	*payload = iX + 48;
	payload++;
	iX = ((uint8_t)(x * 100)) % 10;
	*payload = iX + 48;
}

int get_checksum(uint8_t* payload, int payloadLength)
{
	int checksum = ETX_BYTE;

	for (int i = 0; i < payloadLength; i++)
		checksum ^= payload[i];

	return checksum;
}

void comm_send_pr()
{
	static uint8_t msg[27];
	memset(&msg, 0, sizeof(msg));

	msg[0] = SYN_BYTE;
	msg[1] = SYN_BYTE;
	msg[2] = MSG_TYPE_PR;
	set_payload_length(18, &msg[3]);
	msg[5] = STX_BYTE;
	set_payload_float(5.0f, &msg[6]);
	set_payload_float(2.0f, &msg[12]);
	set_payload_float(g_heading, &msg[18]);
	msg[24] = ETX_BYTE;
	msg[25] = get_checksum(&msg[6], 18);
	msg[26] = EOT_BYTE;

	Serial.write(msg, sizeof(msg));
}

void msg_proc(byte msgtype)
{


}

/* LOOP ******************************************************************************************/

void loop()
{
	static int counter = 0;
	counter++;

	update_sensor_data();

#if DEBUG
	if (Serial.available() > 0)
	{
		char c = Serial.read();

		if (c == 'w') { mov_forward(); }
		if (c == 's') { mov_backward(); }
		if (c == 'q') { mov_counterclockwise(90); }
		if (c == 'a') { mov_counterclockwise(45); }
		if (c == 'd') { mov_clockwise(45); }
		if (c == 'e') { mov_clockwise(90); }
	}

	//Serial.println(g_heading);
	Serial.println(g_encoder.get_count());
#endif

	if (g_moving && !safe_to_move())
		mov_stop();

#if DEBUG == 0
	if (counter % 40 == 0) { comm_send_pr(); }
#endif

	delay(50);
}

/* SETUP *****************************************************************************************/

void setup()
{
	// Initialize serial
	Serial.begin(BAUD_RATE);
#if DEBUG
	Serial.println("Serial initiated at " STR(BAUD_RATE) " baud");
#endif

	// Initialize wire
	Wire.begin();

	// Initialize pins
	pinMode(PIN_ECHO, INPUT);
	pinMode(PIN_TRIG, OUTPUT);
	pinMode(PIN_MOTOR_RIGHT_1, OUTPUT);
	pinMode(PIN_MOTOR_RIGHT_2, OUTPUT);
	pinMode(PIN_MOTOR_LEFT_1, OUTPUT);
	pinMode(PIN_MOTOR_LEFT_2, OUTPUT);

	// Initialize compass
	g_compass.init();

}


















