#!/bin/bash

# Шлях для перевірки
DIR_TO_CHECK="/app/test_directory"

# Створюємо тестовий каталог для демонстрації
mkdir -p "$DIR_TO_CHECK"

# Виконуємо перевірку існування каталогу
echo "Checking directory existence:"
./check_dir "$DIR_TO_CHECK"
