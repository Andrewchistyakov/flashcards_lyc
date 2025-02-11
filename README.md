# fcard
### CLI flashcards tool for students 

This is a HSE lyceum study project of andrew, ilya and sasha.

It is an analog of famous Quizlet app, but in CLI.

It will have features like study mode, adding/deleting flashcards, adding tags to them, viewing stats, importing/exporting sets of cards etc.

Using "Argh!" to manage command line arguments 

Using python "unittest" library for testing in Github Actions

## Currently available commands

`./fcard privet` - outputs "hello world", used to check if program works

`./fcard add <card_front> <card_back> -t <card_tag>` - used to create new cards. cards are added to "flashcards.csv" file, which is located in the same directore as program code. you **can not** add cards without a tag, because it is so inefficient to study without using them. you cam also use `--tag` instead of `-t`. the `-t <card_tag>` may be located anywhere in the query, it will not affect its behavior.

`./fcard list` - lists all existing cards
