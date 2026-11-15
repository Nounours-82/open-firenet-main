#pragma once
#include <math.h>

namespace firenet {

inline int hcsr04FillPercent(float distanceCm, int fullDistanceCm, int emptyDistanceCm) {
  const float percent = (emptyDistanceCm - distanceCm) * 100.0f /
                        (emptyDistanceCm - fullDistanceCm);
  if (percent <= 0.0f) return 0;
  if (percent >= 100.0f) return 100;
  return (int)lroundf(percent);
}

class Hcsr04ReadingFilter {
public:
  bool accept(float distanceCm) {
    if (!hasAccepted_ || fabsf(distanceCm - acceptedDistanceCm_) <= 10.0f) {
      acceptedDistanceCm_ = distanceCm;
      hasAccepted_ = true;
      clearCandidate();
      return true;
    }

    if (candidateCount_ && fabsf(distanceCm - candidateDistanceCm_) <= 2.0f) {
      candidateDistanceCm_ = distanceCm;
      candidateCount_++;
    } else {
      candidateDistanceCm_ = distanceCm;
      candidateCount_ = 1;
    }

    if (candidateCount_ < 2) return false;
    acceptedDistanceCm_ = distanceCm;
    clearCandidate();
    return true;
  }

  void clearCandidate() {
    candidateDistanceCm_ = 0.0f;
    candidateCount_ = 0;
  }

  void reset() {
    acceptedDistanceCm_ = 0.0f;
    hasAccepted_ = false;
    clearCandidate();
  }

private:
  float acceptedDistanceCm_ = 0.0f;
  float candidateDistanceCm_ = 0.0f;
  unsigned int candidateCount_ = 0;
  bool hasAccepted_ = false;
};

} // namespace firenet
