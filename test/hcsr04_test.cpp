#include "firenet_hcsr04.h"
#include <stdio.h>

static int failures = 0;

static void check(const char* name, int actual, int expected) {
  if (actual == expected) return;
  printf("FAIL %s: got %d, expected %d\n", name, actual, expected);
  failures++;
}

int main() {
  check("empty hopper", firenet::hcsr04FillPercent(60.0f, 5, 60), 0);
  check("half full", firenet::hcsr04FillPercent(32.5f, 5, 60), 50);
  check("full at calibrated distance", firenet::hcsr04FillPercent(5.0f, 5, 60), 100);
  check("full at measured 4.3 cm", firenet::hcsr04FillPercent(4.3f, 5, 60), 100);
  check("closer than full calibration", firenet::hcsr04FillPercent(2.0f, 5, 60), 100);
  check("beyond empty calibration", firenet::hcsr04FillPercent(70.0f, 5, 60), 0);

  firenet::Hcsr04ReadingFilter filter;
  check("first reading accepted", filter.accept(20.0f), true);
  check("large isolated jump rejected", filter.accept(31.0f), false);
  check("return to accepted level", filter.accept(20.5f), true);
  check("first consistent jump candidate rejected", filter.accept(31.0f), false);
  check("second consistent jump accepted", filter.accept(32.0f), true);
  check("small changes accepted immediately", filter.accept(40.0f), true);
  filter.clearCandidate();
  check("candidate after lid-open pause must restart", filter.accept(55.0f), false);
  check("new candidate accepted after confirmation", filter.accept(56.0f), true);

  if (failures) return 1;
  puts("HC-SR04 calibration tests passed");
  return 0;
}
