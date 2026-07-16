/**
 * WDT_Nano33BLE.ino
 * -----------------
 * Watchdog Timer (WDT) Case Study — Arduino Nano 33 BLE Sense (nRF52840)
 *
 * Demonstrates:
 *   1. Configuring the nRF52840 hardware WDT via bare-metal NRF_WDT registers.
 *   2. Periodically feeding (petting) the WDT to prevent a reset.
 *   3. Simulating a firmware hang that stops feeding the WDT.
 *   4. Automatic system recovery after the WDT timeout expires (~3 s).
 *   5. Detecting the WDT reset on reboot via the POWER_RESETREAS_DOG bit.
 *
 * Wiring : No external components required.
 * Board  : Arduino Nano 33 BLE Sense (mbed OS core)
 * IDE    : Arduino IDE 2.x  |  Serial Monitor @ 115200 baud
 */

#include <nrf.h>

// ─── User-adjustable constants ──────────────────────────────────────────────
#define WDT_TIMEOUT_MS   3000   // WDT fires 3 s after the last feed
#define FEED_INTERVAL_MS 1000   // Feed every 1 s during normal operation
#define HANG_AFTER_FEEDS 10     // Simulate a hang after this many feeds
// ────────────────────────────────────────────────────────────────────────────

uint32_t feedCount = 0;

/**
 * configureWDT()
 *   Programs the nRF52840 Watchdog Timer peripheral.
 *   - Pauses during a debug halt, runs during CPU sleep.
 *   - Sets the Counter Reload Value (CRV) for the desired timeout.
 *   - Enables reload register 0 (RR[0]) and starts the WDT.
 *
 * @param timeoutMs  Desired WDT timeout in milliseconds.
 */
void configureWDT(uint32_t timeoutMs) {
  // Pause WDT when the core is halted by a debugger; keep running during sleep
  NRF_WDT->CONFIG = (WDT_CONFIG_HALT_Pause << WDT_CONFIG_HALT_Pos)
                  | (WDT_CONFIG_SLEEP_Run  << WDT_CONFIG_SLEEP_Pos);

  // CRV register uses a 32 768 Hz clock; formula: CRV = (timeout_ms * 32768) / 1000
  NRF_WDT->CRV = (timeoutMs * 32768ULL) / 1000;

  // Enable reload register 0 — writing WDT_RR_RR_Reload to RR[0] pets the dog
  NRF_WDT->RREN |= WDT_RREN_RR0_Msk;

  // Start the WDT — once started it cannot be stopped without a reset
  NRF_WDT->TASKS_START = 1;
}

/**
 * feedWDT()
 *   Reloads the WDT counter, preventing a reset.
 *   Must be called more frequently than WDT_TIMEOUT_MS.
 */
void feedWDT() {
  NRF_WDT->RR[0] = WDT_RR_RR_Reload;
}

/**
 * wasResetByWDT()
 *   Checks whether the last reset was triggered by the Watchdog Timer.
 *   The DOG bit in POWER->RESETREAS is set by hardware on a WDT reset.
 *
 * @return true if the previous reset was caused by the WDT.
 */
bool wasResetByWDT() {
  return (NRF_POWER->RESETREAS & POWER_RESETREAS_DOG_Msk) != 0;
}

/**
 * clearResetReason()
 *   Clears all bits in POWER->RESETREAS by writing 1 to each set bit.
 *   Should be called after reading the reset reason to avoid stale flags.
 */
void clearResetReason() {
  NRF_POWER->RESETREAS = NRF_POWER->RESETREAS;
}

// ─── Arduino setup() ────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  // Wait up to 3 s for the Serial Monitor to connect (USB CDC)
  while (!Serial && millis() < 3000);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("=========================================");
  Serial.println(" Nano 33 BLE Sense - WDT Case Study Demo ");
  Serial.println("=========================================");

  // ── Detect WDT-caused reset ──────────────────────────────────────────────
  if (wasResetByWDT()) {
    Serial.println(">>> SYSTEM RECOVERED FROM A WATCHDOG RESET! <<<");
    Serial.println(">>> WDT functionality CONFIRMED working.    <<<");
    // Rapid blink for 2 s to signal recovery
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED_BUILTIN, HIGH); delay(100);
      digitalWrite(LED_BUILTIN, LOW);  delay(100);
    }
  } else {
    Serial.println("Normal power-on / manual reset detected.");
  }

  // Clear RESETREAS before starting WDT so flags are fresh next cycle
  clearResetReason();

  // ── Start WDT ────────────────────────────────────────────────────────────
  configureWDT(WDT_TIMEOUT_MS);
  Serial.println("WDT configured with 3000 ms timeout.");
  Serial.println("Feeding WDT every 1000 ms...");
  Serial.println("-----------------------------------------");
}

// ─── Arduino loop() ─────────────────────────────────────────────────────────
void loop() {
  // Pet the watchdog at the top of every loop iteration
  feedWDT();
  feedCount++;

  // Heartbeat blink (150 ms ON)
  digitalWrite(LED_BUILTIN, HIGH);
  delay(150);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("WDT fed. Count = ");
  Serial.println(feedCount);

  // ── Simulate firmware hang after HANG_AFTER_FEEDS feeds ─────────────────
  if (feedCount >= HANG_AFTER_FEEDS) {
    Serial.println("-----------------------------------------");
    Serial.println("Simulating firmware hang (infinite loop)...");
    Serial.println("WDT will NOT be fed from this point onward.");
    Serial.println("System will auto-reset in ~3 seconds.");
    Serial.flush();

    // Deliberately block — WDT will trigger a reset after 3 s
    while (1) {
      /* no feedWDT() here */
    }
  }

  // Wait the remainder of the 1 s feed interval
  delay(FEED_INTERVAL_MS - 150);
}
