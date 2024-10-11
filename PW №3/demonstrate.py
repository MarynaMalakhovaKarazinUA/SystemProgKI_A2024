import os
import atexit

# Функція для демонстрації зміни змінних у дочірньому процесі
def demonstrate_fork():
    variable = 42
    pid = os.fork()
    
    if pid == 0:  # Дочірній процес
        variable = 100
        print(f"Дочірній процес: змінна = {variable}")
    else:  # Батьківський процес
        print(f"Батьківський процес: змінна = {variable}")

# Функція для демонстрації успадкування файлових дескрипторів
def demonstrate_file_descriptors():
    fd = os.open("testfile.txt", os.O_RDWR | os.O_CREAT)
    pid = os.fork()
    
    if pid == 0:  # Дочірній процес
        os.lseek(fd, 0, os.SEEK_END)
        os.write(fd, "Додано дочірнім процесом\n".encode('utf-8'))
        os.close(fd)
    else:  # Батьківський процес
        os.wait()  # Очікуємо завершення дочірнього процесу
        os.lseek(fd, 0, os.SEEK_SET)
        print(os.read(fd, 100).decode('utf-8'))
        os.close(fd)

# Обробники виходу
def exit_handler1():
    print("Обробник виходу 1")

def exit_handler2():
    print("Обробник виходу 2")

# Реєстрація обробників виходу
atexit.register(exit_handler1)
atexit.register(exit_handler2)

if __name__ == "__main__":
    demonstrate_fork()
    demonstrate_file_descriptors()
