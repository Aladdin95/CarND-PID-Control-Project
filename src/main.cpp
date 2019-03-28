#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
    
  uWS::Hub h;

  PID pid;
  /**
   * TODO: Initialize the pid variable.
   */
  pid.Init(0.15, 0.00025, 3.05);
  //pid.Init(0.2, 0.0003, 3);
  //pid.Init(0.1, 0.0002, 3.1);

  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value;
          /**
           * TODO: Calculate steering value here, remember the steering value is
           *   [-1, 1].
           * NOTE: Feel free to play around with the throttle and speed.
           *   Maybe use another PID controller to control the speed!
           */
          
          std::string reset_msg = "42[\"reset\",{}]";
          int max_count = 100;
          
          if(pid.sum_dp() > 0.4 && pid.counter >= max_count)
          {
            if(pid.best_err == 1234567) pid.best_err = pid.err;
            if(pid.state == 0)
            {
              ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);
              //pid.p[i] += pid.dp[i];
              pid.state = 1;
            }
            else if(pid.state == 1)
            {
              if(pid.err < pid.best_err)
              {
                pid.best_err = pid.err;
                pid.dp[pid.i] *= 1.1;
                pid.i = (pid.i + 1)%3;
                pid.p[pid.i] += pid.dp[pid.i];
              }
              else
              {
                pid.p[pid.i] -= 2*pid.dp[pid.i];
                pid.state = 2;
              }
            }
            else if(pid.state == 2)
            {
              if(pid.err < pid.best_err)
              {
                pid.best_err = pid.err;
                pid.dp[pid.i] *= 1.1;
              }
              else
              {
                pid.p[pid.i] += pid.dp[pid.i];
                pid.dp[pid.i] *= 0.9;
              }
              pid.state = 1;
              pid.i = (pid.i + 1)%3;
              pid.p[pid.i] += pid.dp[pid.i];
            }
            pid.Init();
            ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);
          }
          
          steer_value = pid.get_steer(cte);
          // DEBUG
          //std::cout << "CTE: " << cte << " Steering Value: " << steer_value 
          //          << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          //std::cout << "p =" << pid.p[0] << "  i = " <<pid.p[1]<<"  d = " << pid.p[2]<< std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}