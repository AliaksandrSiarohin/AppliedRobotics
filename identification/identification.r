setwd("~/apr/AppliedRobotics/motor_data/velocity");

time_responce <- function(t, q, xi, w) {
  if (xi == 1) {
    return (q - xi * q * exp(-xi * w * t) )
  }
  N = q / (2 * sqrt(1 - xi ** 2));
  phi = atan(-xi / sqrt(1 - xi**2) )
  res = q - 2 * N * exp(-xi * w * t) * cos(w * sqrt(1 - xi **2) * t + phi);
  return (res)
}

q_est <- function(speed) {
  return (mean(speed[length(speed)]));
}

xi_est <- function(speed) {
  q = q_est(speed);
  speed_max = max(speed);
  C = abs((speed_max - q)/(speed[1] - q));
  
  xi_est = sqrt(log(C) ^ 2 / (pi ^ 2 + log(C) ^ 2));
  return (xi_est)
}

w_est <- function(speed, time, alpha = 5) {
  xi = xi_est(speed)
  q = q_est(speed)
  
  N_bar = 1 / sqrt(1-xi^2);
  
  speed_upper = q * (alpha/100 + 1);
  speed_lower = q * (-alpha/100 + 1);
  
  setling_i = length(speed);
  for (i in length(speed):1) {
    if (speed[i] > speed_upper) {
      setling_i = i;
      break;
    }
    if (speed[i] < speed_lower) {
      setling_i = i;
      break;
    }
  }
  
  setling_time = time[setling_i];
  
  return ((log(alpha/100) -log(N_bar)) / (-setling_time * xi))
  
}

estimate_params_with_regression <- function(time, speed) {
  library('stats')

  opt_func <- function (params) {
    res = sum((time_responce(df$Time, params[1], params[2], params[3]) - speed) ** 2);
    return (res)
  }
  
  res = optim(c(q_est(speed), xi_est(speed), w_est(speed, time)), opt_func, method = "L-BFGS-B",
              lower = c(-Inf, 0.01, 0.01), upper = c(Inf, 1, Inf))
  
  return (res$par)
}



files = c("20_20000.csv", "30_20000.csv", "40_20000.csv", "50_20000.csv", "60_20000.csv",
          "65_20000.csv", "70_20000.csv", "75_20000.csv", "80_20000.csv", "90_20000.csv")
power = c(20, 30, 40, 50, 60, 65, 70, 75, 80, 90)

regular_estimation = list(q = c(), omega = c(), xi = c());
regression_estimation = list(q = c(), omega = c(), xi = c());

i = 1
for (filename in files) {
  df = read.csv(filename);
  pdf(paste("../plots/", filename));
  
  plot(df, type='l', ylab = "Speed (rad/s)", xlab = "Time (s)");
  
  regular_estimation[['q']][i] = q_est(df$Speed) / power[i];
  regular_estimation[['xi']][i] = xi_est(df$Speed);
  regular_estimation[['omega']][i] = w_est(df$Speed, df$Time);
  
  lines (df$Time, time_responce(df$Time, q = q_est(df$Speed), 
                xi = xi_est(df$Speed), w = w_est(df$Speed, df$Time)), type ='l', col = 'red');
  
  params <- estimate_params_with_regression(df$Time, df$Speed);
  
  regression_estimation[['q']][i] = params[1] / power[i];
  regression_estimation[['xi']][i] = params[2];
  regression_estimation[['omega']][i] = params[3];
  
  lines (df$Time, 
         time_responce(df$Time, q = params[1], xi = params[2], w = params[3]), type = 'l', col = 'blue');
  
  legend(x = "bottomright", col = c('black', 'red', 'blue'), lty = 1, 
         legend = c("Filtered Data", "Regualar Estimation", "Regression Estimation"));
  dev.off();
  i = i + 1;
}

reg_est = data.frame(power = power,regular_estimation)
write.csv(file = "../regular_estimated_params.csv", reg_est, quote = FALSE, row.names = FALSE)
regression_est = data.frame(power = power, regression_estimation)
write.csv(file = "../regression_estimated_params.csv", regression_est, quote = FALSE, row.names = FALSE)

