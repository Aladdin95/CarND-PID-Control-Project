# **PID Controller**

PID stands for Proportional Integral Derivative controller. 
### P - Proportional  

Proportional component produces a control signal that is  proportional to CTE. This means higher the error bigger is the value of the signal. In simple words,if the car is to the left of the reference trajectory/middle of the road ,we would want to steer to the right. The proportion of the steering angle depends on the proportion of how far off is car on the left.This depends on the propotional gain or KP of the controller. The value of KP decides the responsiveness of the controller. If KP value is high,the system overshoots and the car may drift off the road while a low KP may give us a sluggish system.

### D - Derivative 

The Derivative component produces a control signal that is based on the rate of change of CTE. This component helps in compensating for the overshoot produced by the P component.  It smoothens out the oscillations caused by the P component. Thus the steering angle does not swing the car between right and left of the road.

### I - Integral

Integral component produces a signal based on the sum of the past CTEs. This component is useful to compensate the effect of steady errors like steering drift. This component helps to steer car around sharp curves and turns. High value of KI creates oscillations.


## Tuning Hyperparameters
i tried to implement twiddle algorithm to tune PID parameters but it took too much time to converge so i tried to tune them manually. Following the flow of the lessons,I first started by tuning the P parameter. I and D coefficients were zero. I found the car oscillated a lot between right and left and then drifted outside of the road. The next parameter added was D.  P and D together gave much better response as compared to just P. The oscillations were far more less but not smooth enough. It still drifted out of road,crashed muliple times and did not complete the full course. After a lot of tweaking and tuning the parameters, I settled on the values P = 0.15 and D = 3.05.These values gave good performance. The car did not swing a lot,stayed around middle of the road and completed the full course of road without crashing or leaving the road. The component I was added which give a much smoother performance escpecially around sharp turns. The final values  were (P I D) = (0.15, 0.00025, 3.05).