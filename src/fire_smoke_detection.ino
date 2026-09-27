// --- Pin Definitions ---
const int FLAME_SENSOR_PIN = A0;
const int SMOKE_SENSOR_PIN = A1;

const int GREEN_LED_PIN = 2;
const int RED_LED_PIN   = 3;

const int FLAME_BUZZER_PIN = 9;   // Fire buzzer
const int SMOKE_BUZZER_PIN = 10;  // Smoke/Gas buzzer

// --- Calibrated Thresholds ---
const int FLAME_THRESHOLD = 500;  // Below this = Fire
const int SMOKE_THRESHOLD = 110;  // Above this = Smoke/Gas

// --- State Variables ---
bool fireLatched = false;
bool isMuted = false;
unsigned long muteStartTime = 0;
const unsigned long MUTE_DURATION = 60000; // 60 seconds mute

int fireIncidents = 0;
int smokeIncidents = 0;
bool previousSmokeState = false;

// --- Buzzer Cadence Timing ---
unsigned long lastBeepTime = 0;
bool beepState = false;

void setup() {
  Serial.begin(9600);

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(FLAME_BUZZER_PIN, OUTPUT);
  pinMode(SMOKE_BUZZER_PIN, OUTPUT);

  // Startup indication: both LEDs on during 10s warmup
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(FLAME_BUZZER_PIN, LOW);
  digitalWrite(SMOKE_BUZZER_PIN, LOW);

  Serial.println("\n--- SYSTEM BOOTING ---");
  Serial.println("Warming up MQ-2 sensor (10 seconds)...");

  for (int i = 10; i > 0; i--) {
    Serial.print("Stabilizing: ");
    Serial.print(i);
    Serial.println("s remaining...");
    delay(1000);
  }

  digitalWrite(RED_LED_PIN, LOW);
  Serial.println(">> WARM-UP COMPLETE: SYSTEM ARMED <<");
  printHelpMenu();
}

void loop() {
  // Check Bluetooth incoming commands
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    handleBluetoothCommand(cmd);
  }

  // Handle Mute expiration
  if (isMuted && (millis() - muteStartTime >= MUTE_DURATION)) {
    isMuted = false;
    Serial.println(">> MUTE EXPIRED: BUZZERS UNMUTED <<");
  }

  int flameVal = analogRead(FLAME_SENSOR_PIN);
  int smokeVal = analogRead(SMOKE_SENSOR_PIN);

  // Trigger Fire Latch
  if (flameVal < FLAME_THRESHOLD && !fireLatched) {
    fireLatched = true;
    fireIncidents++;
  }

  // Track Smoke Incidents
  bool smokeDetected = (smokeVal > SMOKE_THRESHOLD);
  if (smokeDetected && !previousSmokeState) {
    smokeIncidents++;
  }
  previousSmokeState = smokeDetected;

  // --- Alert State ---
  if (fireLatched || smokeDetected) {
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);

    handleBuzzerCadence(fireLatched, smokeDetected);

    // Live terminal telemetry
    Serial.print("Smoke: ");
    Serial.print(smokeVal);
    Serial.print(" | Flame: ");
    Serial.print(flameVal);
    Serial.print(" -> ");

    if (fireLatched && smokeDetected) {
      Serial.println("ALERT: FIRE AND SMOKE DETECTED!");
    } else if (fireLatched) {
      Serial.println("ALERT: FIRE LATCHED! (Send 'r' to reset)");
    } else {
      Serial.println("ALERT: SMOKE DETECTED!");
    }
  } 
  // --- Safe State ---
  else {
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(FLAME_BUZZER_PIN, LOW);
    digitalWrite(SMOKE_BUZZER_PIN, LOW);

    Serial.print("Smoke: ");
    Serial.print(smokeVal);
    Serial.print(" | Flame: ");
    Serial.print(flameVal);
    Serial.println(" -> STATUS: ALL CLEAR");
  }

  delay(250); // Fluid timing for pulsing buzzer rhythms
}

// --- Pulsing Buzzer Patterns ---
void handleBuzzerCadence(bool fireActive, bool smokeActive) {
  if (isMuted) {
    digitalWrite(FLAME_BUZZER_PIN, LOW);
    digitalWrite(SMOKE_BUZZER_PIN, LOW);
    return;
  }

  unsigned long currentMillis = millis();

  // Rapid Fire pulse (120ms cycle) vs Rhythmic Smoke pulse (250ms cycle)
  int pulseInterval = fireActive ? 120 : 250;

  if (currentMillis - lastBeepTime >= pulseInterval) {
    lastBeepTime = currentMillis;
    beepState = !beepState;
  }

  if (fireActive) {
    digitalWrite(FLAME_BUZZER_PIN, beepState ? HIGH : LOW);
  } else {
    digitalWrite(FLAME_BUZZER_PIN, LOW);
  }

  if (smokeActive) {
    digitalWrite(SMOKE_BUZZER_PIN, beepState ? HIGH : LOW);
  } else {
    digitalWrite(SMOKE_BUZZER_PIN, LOW);
  }
}

// --- Bluetooth Command Handler ---
void handleBluetoothCommand(char cmd) {
  if (cmd == 'r' || cmd == 'R') {
    fireLatched = false;
    Serial.println("\n>> SYSTEM RESET: FIRE ALARM CLEARED <<");
  } 
  else if (cmd == 's' || cmd == 'S') {
    printStatusReport();
  } 
  else if (cmd == 't' || cmd == 'T') {
    runSelfTest();
  } 
  else if (cmd == 'm' || cmd == 'M') {
    isMuted = true;
    muteStartTime = millis();
    Serial.println("\n>> BUZZERS MUTED FOR 60 SECONDS <<");
  }
}

void printStatusReport() {
  Serial.println("\n==================================");
  Serial.println("       SYSTEM HEALTH REPORT       ");
  Serial.println("==================================");
  Serial.print("System Uptime      : ");
  Serial.print(millis() / 1000);
  Serial.println(" seconds");
  Serial.print("Fire Latch State   : ");
  Serial.println(fireLatched ? "TRIGGERED (LATCHED)" : "STANDBY (CLEAR)");
  Serial.print("Fire Incidents     : ");
  Serial.println(fireIncidents);
  Serial.print("Smoke Incidents    : ");
  Serial.println(smokeIncidents);
  Serial.print("Audio Mute Status  : ");
  Serial.println(isMuted ? "MUTED (Timer active)" : "ACTIVE");
  Serial.println("==================================\n");
}

void runSelfTest() {
  Serial.println("\n>> STARTING HARDWARE SELF-TEST <<");
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(FLAME_BUZZER_PIN, HIGH);
  digitalWrite(SMOKE_BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(FLAME_BUZZER_PIN, LOW);
  digitalWrite(SMOKE_BUZZER_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  Serial.println(">> SELF-TEST COMPLETE: HARDWARE OK <<\n");
}

void printHelpMenu() {
  Serial.println("Commands: [r] Reset | [s] Status Report | [t] Self-Test | [m] Mute 60s");
}