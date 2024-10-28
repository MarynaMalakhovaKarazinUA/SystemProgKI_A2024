# Вибір базового образу
FROM gcc:latest

# Встановлення робочої директорії
WORKDIR /usr/src/app

# Копіювання файлу з кодом
COPY . .

# Компиляція програми
RUN gcc -o main main.c

# Запуск програми
CMD ["./main"]
