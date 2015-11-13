setwd("~/apr/AppliedRobotics/motor_data/velocity");

################# PARAMETER IDENTIFICATION ######################
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
  return (speed[length(speed)]);
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


plot_speed_and_estimations <- function(df, params_regular, params_regresion) {
  plot(df, type='l', ylab = "Speed (rad/s)", xlab = "Time (s)");
  lines (df$Time, time_responce(df$Time, q = params_regular[1], 
                  xi = params_regular[2], w = params_regular[3]), type ='l', col = 'red');
  lines (df$Time, 
         time_responce(df$Time, 
        q = params_regresion[1], xi = params_regresion[2], w = params_regresion[3]), type = 'l', col = 'blue');
  
  legend(x = "bottomright", col = c('black', 'red', 'blue'), lty = 1, 
         legend = c("Filtered Data", "Regualar Estimation", "Regression Estimation"));
}

files = c("20_20000.csv", "30_20000.csv", "40_20000.csv", "50_20000.csv", "60_20000.csv",
          "65_20000.csv", "70_20000.csv", "75_20000.csv", "80_20000.csv", "90_20000.csv")
power = c(20, 30, 40, 50, 60, 65, 70, 75, 80, 90)

regular_estimation = list(q = c(), omega = c(), xi = c());
regression_estimation = list(q = c(), omega = c(), xi = c());

i = 1
for (filename in files) {
  df = read.csv(filename);
  params_regular <- c(q_est(df$Speed), xi_est(df$Speed), w_est(df$Speed, df$Time))
  params_regresion <- estimate_params_with_regression(df$Time, df$Speed);
  pdf(paste("../plots/", filename, ".pdf", sep =""));
  plot_speed_and_estimations(df, params_regular, params_regresion);
  dev.off();

  
  regular_estimation[['q']][i] = params_regular[1] / power[i];
  regular_estimation[['xi']][i] = params_regular[2];
  regular_estimation[['omega']][i] = params_regular[3];
  
  regression_estimation[['q']][i] = params_regresion[1] / power[i];
  regression_estimation[['xi']][i] = params_regresion[2];
  regression_estimation[['omega']][i] = params_regresion[3];
  
  i = i + 1;
}

reg_est = data.frame(power = power,regular_estimation)
write.csv(file = "../regular_estimated_params.csv", reg_est, quote = FALSE, row.names = FALSE)
regression_est = data.frame(power = power, regression_estimation)
write.csv(file = "../regression_estimated_params.csv", regression_est, quote = FALSE, row.names = FALSE)


################### MESAURING PERFOMANCE ############################

compute_square_error_of_all_data <- function(q, omega, xi) {
  values = c()
  for (i in 1:length(files)) {
    df = read.csv(filename);
    
    values[i] = mean((time_responce(df$Time, q = power[i] * q, w = omega, xi = xi) - df$Speed) ** 2)
  }
  return (mean(values));
}

m <- matrix(ncol = 2, nrow = nrow(reg_est))
for (i in 1:nrow(reg_est)) {
  m[i, 1] = compute_square_error_of_all_data(reg_est[i,]$q,
                                             reg_est[i,]$omega, reg_est[i,]$xi)
}

for (i in 1:nrow(regression_est)) {
  m[i, 2] = compute_square_error_of_all_data(regression_est[i,]$q,
                                             regression_est[i,]$omega, regression_est[i,]$xi)
}

squared_error_df = data.frame(power = power, regular = m[, 1], regression = m[,2]) 

write.csv(file = "../square_error.csv", squared_error_df, row.names = FALSE, quote = FALSE);

########################## CHOSING THE BEST MODEL ############################
opt_index = which(m == min(m), arr.ind = TRUE)
if (opt_index[1, 2] == 2) {
  optimal_parameters = regression_est[opt_index[1,1], ]
} else {
  optimal_parameters = reg_est[opt_index[1,1], ]
}

i = 1
for (filename in files) {
  df = read.csv(filename);
  pdf(paste("../plots/", filename, "opt", ".pdf", sep =""));
  plot(df, type='l', ylab = "Speed (rad/s)", xlab = "Time (s)");
  lines (df$Time, 
         time_responce(df$Time, q = optimal_parameters$q * power[i],
                       xi = optimal_parameters$xi, w = optimal_parameters$omega), type = 'l', col = 'blue');
  dev.off()
  
  i = i + 1
}






