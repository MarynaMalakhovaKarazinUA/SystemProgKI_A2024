# Dockerfile
FROM gcc:latest

# Встановлення робочого каталогу
WORKDIR /usr/src/app

# Копіювання файлів
COPY . .

# Компіляція програми
RUN gcc -o main main.c

# Використання ENTRYPOINT для запуску програми
ENTRYPOINT ["./main"]
