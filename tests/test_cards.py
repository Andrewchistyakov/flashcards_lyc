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
        # тут тестируется что команда просто не падает, сам результат выполнения проверяется в следующем тесте (list)

        clear_csv("flashcards.csv") # удаляем файл чтобы тестировать с нуля

        result = subprocess.run(["./fcard", "add", "Front of card", "Back of card", "-t", "tag1"], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0)  # убеждаемся что программа завершается без ошибки
        self.assertIn("Flashcard added successfully!", result.stdout)  # проверяем аутпут

        clear_csv("flashcards.csv") # удаляем файл для след тестов

    def test_list_command(self):
        # test "display" command

        # удаляем файл чтобы тестировать с нуля
        clear_csv("flashcards.csv")

        # добавляем карточки
        subprocess.run(["./fcard", "add", "Test Front 1", "Test Back 1", "-t", "tag_text_1"], capture_output=True, text=True)
        subprocess.run(["./fcard", "add", "Test Front 2", "Test Back 2", "-t", "tag_text_2"], capture_output=True, text=True)

        expected_output = """Flashcards:
-----------------------------------
ID: 1
Front: Test Front 1
Back: Test Back 1
Tag: tag_text_1
-----------------------------------
ID: 2
Front: Test Front 2
Back: Test Back 2
Tag: tag_text_2
-----------------------------------"""
        # display all cards
        result = subprocess.run(["./fcard", "list"], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0)  # убеждаемся что программа завершается без ошибки
        self.assertEqual(result.stdout.strip(), expected_output) # проверяем вывод

        clear_csv("flashcards.csv") # удаляем файл для след тестов

    def test_remove_command_1(self):
        # test "remove" command

        # удаляем файл чтобы тестировать с нуля
        clear_csv("flashcards.csv")

        # добавляем карточки
        subprocess.run(["./fcard", "add", "Test Front 1", "Test Back 1", "-t", "tag_text_1"], capture_output=True, text=True)
        subprocess.run(["./fcard", "add", "Test Front 2", "Test Back 2", "-t", "tag_text_2"], capture_output=True, text=True)

        # удаляем карточку
        result = subprocess.run(["./fcard", "remove", "1"], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0)  # убеждаемся что программа завершается без ошибки
        self.assertIn("Flashcard removed successfully!", result.stdout)  # проверяем аутпут

        clear_csv("flashcards.csv") # удаляем файл для след тестов

    def test_remove_command_2(self):
        # test "remove" command with results in list and also with invalid ID

        # удаляем файл чтобы тестировать с нуля
        clear_csv("flashcards.csv")

        # добавляем карточки
        subprocess.run(["./fcard", "add", "Test Front 1", "Test Back 1", "-t", "tag_text_1"], capture_output=True, text=True)
        subprocess.run(["./fcard", "add", "Test Front 2", "Test Back 2", "-t", "tag_text_2"], capture_output=True, text=True)

        # удаляем карточку
        result = subprocess.run(["./fcard", "remove", "3"], capture_output=True, text=True)
        self.assertEqual(result.returncode, 1)  # убеждаемся что программа завершается с ошибкой

        subprocess.run(["./fcard", "remove", "2"], capture_output=True, text=True)
        result = subprocess.run(["./fcard", "list"], capture_output=True, text=True)

        expected_output = """Flashcards:
-----------------------------------
ID: 1
Front: Test Front 1
Back: Test Back 1
Tag: tag_text_1
-----------------------------------"""

        self.assertEqual(result.returncode, 0)  # убеждаемся что программа завершается без ошибки
        self.assertEqual(result.stdout.strip(), expected_output) # проверяем вывод

    def test_remove_command_3(self):
        # test remove command if deleting last card

        # удаляем файл чтобы тестировать с нуля
        clear_csv("flashcards.csv")

        # добавляем карточку
        subprocess.run(["./fcard", "add", "Test Front 1", "Test Back 1", "-t", "tag_text_1"], capture_output=True, text=True)

        # удаляем, добавляем и выводим карточки
        subprocess.run(["./fcard", "remove", "1"], capture_output=True, text=True)
        subprocess.run(["./fcard", "add", "Test Front 1", "Test Back 1", "-t", "tag_text_1"], capture_output=True, text=True)
        result = subprocess.run(["./fcard", "list"], capture_output=True, text=True)

        expected_output = """Flashcards:
-----------------------------------
ID: 1
Front: Test Front 1
Back: Test Back 1
Tag: tag_text_1
-----------------------------------"""

        self.assertEqual(result.returncode, 0)  # убеждаемся что программа завершается без ошибки
        self.assertEqual(result.stdout.strip(), expected_output) # проверяем вывод

        clear_csv("flashcards.csv")

    def test_review_all(self):
        # удаляем файл чтобы тестировать с нуля
        clear_csv("flashcards.csv")

        # добавляем карточки
        subprocess.run(["./fcard", "add", "Test Front 1", "Test Back 1", "-t", "tag_text_1"], capture_output=True, text=True)
        subprocess.run(["./fcard", "add", "Test Front 2", "Test Back 2", "-t", "tag_text_2"], capture_output=True, text=True)
        
        output = subprocess.run(["./fcard", "review", "-a"], input=b"\ny\nn", capture_output=True)
        expected_output = "Test Front 1\npress \"y\" if you got it and \"n\" if not\nGood job!\nTest Front 2\npress \"y\" if you got it and \"n\" if not\nTry to memorize it!\n--------------- You finished them all! --------------"
        self.assertEqual(output.stdout.decode().strip(), expected_output) # проверяем вывод

        clear_csv("flashcards.csv")


def clear_csv(file_path):
    """Clear the contents of a CSV file."""
    with open(file_path, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)

if __name__ == "__main__":
    unittest.main()

