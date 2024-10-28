# Використовуйте базовий образ з gcc
FROM gcc:latest

# Встановлення inotify-tools для демонізації
RUN apt-get update && apt-get install -y inotify-tools

# Копіюємо всі файли в контейнер
COPY task1.c task1.c
COPY task2.c task2.c
COPY task3.c task3.c
COPY task4.c task4.c

# Компільуємо програми
RUN gcc task1.c -o task1
RUN gcc task2.c -o task2
RUN gcc task3.c -o task3
RUN gcc task4.c -o task4

# Вказуємо команду для запуску
CMD ["bash"]
