import os
import pickle

# Створюємо кастомну структуру даних (клас)
class CustomData:
    def __init__(self, name, value):
        self.name = name
        self.value = value

    def __repr__(self):
        return f"CustomData(name={self.name}, value={self.value})"


def save_data_to_file(data, filename):
    """Записує кастомні дані в файл"""
    with open(filename, 'wb') as file:
        pickle.dump(data, file)
    print(f"Data has been saved to {filename}")


def load_data_from_file(filename):
    """Зчитує кастомні дані з файлу"""
    if os.path.exists(filename):
        with open(filename, 'rb') as file:
            data = pickle.load(file)
        print(f"Data loaded from {filename}: {data}")
        return data
    else:
        print(f"File {filename} does not exist.")
        return None


def main():
    filename = "custom_data.pkl"
    
    # Створюємо новий екземпляр кастомних даних
    data = CustomData("Example", 42)
    
    # Перевіряємо, чи існує файл перед зчитуванням
    if os.path.exists(filename):
        load_data_from_file(filename)
    else:
        print(f"{filename} does not exist. Saving new data.")
        save_data_to_file(data, filename)


if __name__ == "__main__":
    main()
