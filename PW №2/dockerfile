# Використовуємо офіційний образ Ubuntu як базовий
FROM ubuntu:latest

# Встановлюємо необхідні пакети
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

# Копіюємо вихідний код у контейнер
COPY . /usr/src/myapp

# Встановлюємо робочий каталог
WORKDIR /usr/src/myapp

# Компілюємо програму
RUN gcc -o myprogram myprogram.c

# Вказуємо команду для запуску програми
CMD ["./myprogram"]
