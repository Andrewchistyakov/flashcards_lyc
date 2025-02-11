import unittest
import subprocess
import csv

class TestFlashcardProgram(unittest.TestCase):
    def test_privet_command(self):

        # test "privet" command
        result = subprocess.run(["./fcard", "privet"], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0)  # убеждаемся что программа завершается без ошибки
        self.assertEqual(result.stdout.strip(), "Hello world!")  # проверяем аутпут

    def test_add_command(self):
        # test "add" command

        clear_csv("flashcards.csv") # удаляем файл чтобы тестировать с нуля

        result = subprocess.run(["./fcard", "add", "Front of card", "Back of card"], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0)  # убеждаемся что программа завершается без ошибки
        self.assertIn("Flashcard added successfully!", result.stdout)  # проверяем аутпут

        clear_csv("flashcards.csv") # удаляем файл для след тестов

    def test_list_command(self):
        # test "display" command

        # удаляем файл чтобы тестировать с нуля
        clear_csv("flashcards.csv")

        # добавляем карточки
        subprocess.run(["./fcard", "add", "Test Front 1", "Test Back 1"], capture_output=True, text=True)
        subprocess.run(["./fcard", "add", "Test Front 2", "Test Back 2"], capture_output=True, text=True)

        expected_output = """Flashcards:
-----------------------------------
ID: 1
Front: Test Front 1
Back: Test Back 1
-----------------------------------
ID: 2
Front: Test Front 2
Back: Test Back 2
-----------------------------------"""
        # Display all cards
        result = subprocess.run(["./fcard", "list"], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0)  # убеждаемся что программа завершается без ошибки
        self.assertEqual(result.stdout.strip(), expected_output) # проверяем вывод

        clear_csv("flashcards.csv") # удаляем файл для след тестов

def clear_csv(file_path):
    """Clear the contents of a CSV file."""
    with open(file_path, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)

if __name__ == "__main__":
    unittest.main()