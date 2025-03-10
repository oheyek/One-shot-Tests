# One-shot Tests

A lightweight, customizable GTK-based quiz application that allows users to test their knowledge through multiple-choice questions with time constraints.

![One-shot Tests](https://cdn-icons-png.flaticon.com/256/1581/1581884.png)

## Features

- ✅ Multiple-choice quiz interface
- ⏱️ Configurable time limit per question
- 🔀 Optional shuffling of questions and answers
- 📊 Score tracking and results history
- 🏆 Best score display
- 🔑 Keyboard shortcuts for fast answer selection (1-4)
- 🔄 Restart functionality to retake the quiz

## Prerequisites

To build and run One-shot Tests, you need the following:

- GCC or compatible C compiler
- GTK+ 3.0 development libraries
- Make build system

### On Debian/Ubuntu:

```bash
sudo apt-get install build-essential libgtk-3-dev
```

### On Fedora:

```bash
sudo dnf install gcc gtk3-devel
```

### On macOS (using Homebrew):

```bash
brew install gtk+3
```

## Building from Source

1. Clone the repository:

```bash
git clone https://github.com/oheyek/One-shot-Tests.git
cd One-shot-Tests
```

2. Build the application using make:

```bash
make
```

This will create the executable in the `build` directory.

## Running the Application

After building, you can run the application with:

```bash
make run
```

Or directly:

```bash
./build/one-shot-tests
```

## Configuration

The application can be configured by editing the `config.txt` file:

| Setting | Description | Values |
|---------|-------------|--------|
| question_time_limit | Time allowed for each question (in seconds) | Integer (e.g., 60) |
| shuffle_questions | Whether to randomize the order of questions | 0 (off) or 1 (on) |
| show_timer | Whether to display the countdown timer | 0 (off) or 1 (on) |
| shuffle_answers | Whether to randomize the order of answers | 0 (off) or 1 (on) |

Example configuration:
```
question_time_limit=60
shuffle_questions=1
show_timer=1
shuffle_answers=1
```

## Adding Custom Questions

You can add your own questions by editing the `questions.txt` file. Each question follows this format:

```
Question text
Answer option 1
Answer option 2
Answer option 3
Answer option 4
Correct answer index (0-3)
```

Important notes:
- Each question must have exactly 4 answer options
- The correct answer index is 0-based (0 for the first answer, 1 for the second, etc.)
- Questions are separated without blank lines

## Results

Quiz results are saved in the `results.csv` file in the following format:
```
Name,Score,TotalQuestions
```

The application keeps track of the best score and displays it after each quiz.

## Keyboard Shortcuts

| Key | Action |
|-----|--------|
| 1-4 | Select answer options 1-4 |
| R | Restart the quiz (only works when the quiz is finished) |
| Q | Quit the application |

## Linux Systems

For Linux users, there's an alternative Makefile available:
```bash
cp LinuxMakeFile Makefile
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Author

Made with ❤️ by ohey<br><br>
<a href="https://www.buymeacoffee.com/ohey"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=ohey&button_colour=BD5FFF&font_colour=ffffff&font_family=Lato&outline_colour=000000&coffee_colour=FFDD00" /></a>

---

Happy quizzing!
