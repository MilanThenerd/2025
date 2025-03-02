library(ggplot2)

iris <- read.csv("2025/COS333/Prac1/iris.data", header = FALSE)
names(iris) <- c("Sepal.Length", "Sepal.Width", "Petal.Length", "Petal.Width", "Species")

plot <- ggplot(iris, aes(x = Petal.Length, y = Petal.Width)) +
  geom_point(color = "blue") +
  labs(title = "Petal Length vs. Petal Width with Polynomial Regression",
       x = "Petal Length",
       y = "Petal Width")

model <- lm(Petal.Width ~ poly(Petal.Length, 3), data = iris)

x_vals <- seq(min(iris$Petal.Length), max(iris$Petal.Length), length.out = 100)
predictions <- predict(model, newdata = data.frame(Petal.Length = x_vals))

regression_df <- data.frame(Petal.Length = x_vals, Petal.Width = predictions)

plot <- plot + geom_line(data = regression_df, aes(x = Petal.Length, y = Petal.Width), color = "red", size = 1)

print(plot)
