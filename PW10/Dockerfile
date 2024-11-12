# Базовий образ
FROM gcc:latest

# Встановлення локалі
RUN apt-get update && apt-get install -y locales && \
    locale-gen uk_UA.UTF-8

# Копіювання та компіляція програми
COPY time_program.c /usr/src/app/
WORKDIR /usr/src/app
RUN gcc -o time_program time_program.c

# Установлення команди запуску
CMD ["./time_program"]
