
setwd("/home/dilute/Documents/Dev/Chemical/modules/chemical/sim_data")

data <- read.csv("2026-02-19 07:49:06.360826144.csv")

plot(data$tick_index, data$velocity_z, pch=19)

points(data$tick_index, data$velocity_z, pch=19, col="red")

# Eccentricity?
plot(data$acceleration_a, data$velocity_x, pch=19, col="red", xlab="Acceleration",
     ylab="Velocity X, Y, Z", main="Acceleration v Velocity", ylim=c(-40, 30)) 
points(data$acceleration_a, data$velocity_y, pch=19, col="green")
points(data$acceleration_a, data$velocity_z, pch=19, col="blue")

plot(data$velocity_x, data$velocity_y)
plot(data$velocity_y, data$velocity_z)
plot(data$velocity_x, data$velocity_z)

plot(data$distance, data$velocity_z)
