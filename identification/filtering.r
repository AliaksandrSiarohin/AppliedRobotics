setwd("~/NXT/AppliedRobotics/motor_data");
#install.packages('plyr');
#install.packages('signal');
library(plyr);
library(signal);

df = read.csv("30");

sampling_time = 5;
df = ddply(df, .(floor(Time / sampling_time)), summarise, Count = Count[1], 
            Time = floor(Time[1]/ sampling_time) * sampling_time);
n =  length(df$Count);
speed = (df$Count[2:n] - df$Count[1:(n-1)]) / (df$Time[2:n] - df$Time[1:(n-1)]);
plot(df$Time[2:n], speed, type = 'l');


b1 <- butter(10, 0.020, type ='low');
speed.filtered <- filtfilt(b1, speed);
plot (df$Time[2:n], speed.filtered, type = 'l');