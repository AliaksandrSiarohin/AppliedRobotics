setwd("~/apr/AppliedRobotics/motor_data");
#install.packages('plyr');
#install.packages('signal');
library(plyr);
library(signal);
sampling_time = 5;
filter_data <- function(df, plot = TRUE) {
  
  df = ddply(df, .(floor(Time / sampling_time)), summarise, Count = Count[1], 
             Time = floor(Time[1]/ sampling_time) * sampling_time);
  n =  length(df$Count);
  speed = (df$Count[2:n] - df$Count[1:(n-1)]) / (df$Time[2:n] - df$Time[1:(n-1)]);

  b1 <- butter(1, 0.02, type ='low')
  speed.filtered <- filtfilt(b1, speed);
  
  speed.cutted = speed.filtered[1: (n/2)];
  
  
  speed.cutted = (speed.cutted / 180) * pi * 1000;
  time.cutted = df$Time[2:(length(speed.cutted)+1)] / 1000;
  if (plot) {
    plot (time.cutted, speed.cutted, type = 'l', xlab = "Time (s)", ylab = "Speed (rad/s)");
  }
  return (data.frame(Time = time.cutted, Speed = speed.cutted));
} 

files = c("20_20000.csv", "30_20000.csv", "40_20000.csv", "50_20000.csv", "60_20000.csv",
          "65_20000.csv", "70_20000.csv", "75_20000.csv", "80_20000.csv", "90_20000.csv")
#files = "90_20000.csv"
for (filename in files) {
  df = read.csv(filename);
  #pdf(paste("plots/", filename, ".pdf", sep = ""));
  result = filter_data(df, plot = TRUE);
  write.csv(file = paste("velocity/", filename, sep=""), quote = FALSE, result, row.names = FALSE);
  #dev.off();
}

