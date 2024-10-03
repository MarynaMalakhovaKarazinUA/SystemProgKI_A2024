import os
import shutil

# Функція для рекурсивного видалення вмісту каталогу
def delete_directory_contents(directory_path):
    if os.path.exists(directory_path):
        for filename in os.listdir(directory_path):
            file_path = os.path.join(directory_path, filename)
            try:
                if os.path.isfile(file_path) or os.path.islink(file_path):
                    os.unlink(file_path)
                elif os.path.isdir(file_path):
                    shutil.rmtree(file_path)
            except Exception as e:
                print(f'Помилка при видаленні {file_path}. Причина: {e}')

# Функція для створення тестового вмісту в каталозі
def create_test_content(directory_path):
    os.makedirs(directory_path, exist_ok=True)
    
    # Створення файлів
    with open(os.path.join(directory_path, 'file1.txt'), 'w') as f:
        f.write('This is file 1')

    with open(os.path.join(directory_path, 'file2.txt'), 'w') as f:
        f.write('This is file 2')
    
    # Створення підкаталогу
    subdir_path = os.path.join(directory_path, 'subdir')
    os.makedirs(subdir_path, exist_ok=True)
    
    with open(os.path.join(subdir_path, 'file3.txt'), 'w') as f:
        f.write('This is file 3 in subdir')

    print(f"Тестовий вміст створено в каталозі '{directory_path}'")

# Приклад використання
if __name__ == '__main__':
    # Вкажіть каталог для очищення
    directory_to_clean = '/app/tmp'

    # Створюємо тестовий вміст у каталозі
    create_test_content(directory_to_clean)

    # Видаляємо вміст каталогу
    delete_directory_contents(directory_to_clean)
    print(f"Вміст каталогу '{directory_to_clean}' успішно видалено.")
