# Використовуємо офіційний образ Ubuntu
FROM ubuntu:latest

# Встановлюємо необхідні бібліотеки
RUN apt-get update && apt-get install -y build-essential libcrypt-dev

# Додаємо користувача з паролем
RUN useradd -m testuser && echo "testuser:testpassword" | chpasswd

# Копіюємо програму в контейнер
COPY auth.c /auth.c

# Компілюємо програму
RUN gcc -o auth auth.c -lcrypt

# Запускаємо програму
CMD ["/auth"]
