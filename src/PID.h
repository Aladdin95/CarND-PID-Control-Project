#ifndef PID_H
#define PID_H

class PID {
 public:
  /**
   * Constructor
   */
  PID();

  /**
   * Destructor.
   */
  virtual ~PID();

  /**
   * Initialize PID.
   * @param (Kp_, Ki_, Kd_) The initial PID coefficients
   */
  double get_steer(double cte);
  void Init(double Kp_, double Ki_, double Kd_);
  void Init();
  /**
   * Update the PID error variables given cross track error.
   * @param cte The current cross track error
   */
  void UpdateError(double cte);

  /**
   * Calculate the total PID error.
   * @output The total PID error
   */
  double TotalError();
  double sum_dp();
  int counter;
  int i;
  int state;
  double dp[3];
  /**
   * PID Errors
   */
  double err;
  double best_err;
  
  double p_error;
  double i_error;
  double d_error;

  /**
   * PID Coefficients
   */ 
  double p[3];
};

#endif  // PID_H