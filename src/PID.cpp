#include "PID.h"

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {
  dp[0] = dp[1] = dp[2] = 0.1;
  state = 0;
  best_err = 1234567;
  i = 0;
}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  p[0] = Kp_;
  p[1] = Ki_;
  p[2] = Kd_;
  i_error = 0;
  counter = 0;
  err = 0;
}

void PID::Init() {
  i_error = 0;
  counter = 0;
  err = 0;
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  if(!counter)
    d_error = 0;
  else
    d_error = cte - p_error;
  
  ++counter;
  err += cte*cte;
  i_error += cte; 
  p_error = cte;
}

double PID::get_steer(double cte) {
  this -> UpdateError(cte);
  return -p[0] * p_error - p[1] * i_error - p[2] * d_error;
}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  return 0.0;
  // TODO: Add your total error calc here!
}

double PID::sum_dp() {
  return dp[0] + dp[1] + dp[2];
}