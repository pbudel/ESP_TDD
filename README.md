


# 🚀 ESPTDD — ESP32 Firmware TDD Template

Welcome to **ESPTDD**, a modern template for developing ESP32 firmware with [PlatformIO](https://platformio.org/) using the principles of **Test-Driven Development** (TDD). This project is designed to help you kickstart robust and maintainable embedded software projects with confidence and speed.

---

## 🎯 Purpose

This repository provides a clean, organized starting point for:

- ⚙️ **ESP32 development** with PlatformIO
- ✅ **Unit testing and TDD** methodology
- 🧪 Integration of host-based and embedded-target tests
- 📦 Easy CI/CD integration down the line

Whether you're building IoT systems, embedded applications, or device firmware, this template is for you.

---

## 🧰 What’s Inside

- 🏗️ PlatformIO-based project structure
- 🔬 Unit testing framework preconfigured
- 📁 Separated source and test directories
- 🧪 Examples of test-driven development in action
- 🔄 GitHub-friendly `.gitignore` and VSCode configs

---

## 🚦 Getting Started

### Prerequisites

- [VSCode](https://code.visualstudio.com/)
- [PlatformIO Extension](https://platformio.org/install)
- ESP32 toolchain (auto-installed by PlatformIO)

### Clone the Template

```bash
git clone https://github.com/YOUR_USERNAME/ESPTDD.git
cd ESPTDD
```

### Build the Project

```bash
pio run
```

### Run Unit Tests

```bash
pio test
```

You can run tests on the host or target, depending on configuration you have selected

---

## 📁 Project Structure

```text
ESPTDD/
├── include/            # Header files
├── lib/                # External libraries
├── src/                # Source files
├── test/               # Unit and integration tests
├── platformio.ini      # PlatformIO project config
└── README.md           # You're here :)
```

---

## 🧪 Test-Driven Development

TDD encourages writing tests **before** writing code. This repository includes simple examples to help you get started and keep your code base reliable and regression-free.

Recommended cycle:

1. Write a failing test
2. Write just enough code to pass
3. Refactor and improve
4. Repeat 🔁

---

## 💡 Tips

- Use `pio test -e native` for fast feedback on host
- Explore [Unity](https://docs.platformio.org/en/latest/frameworks/unity.html) for writing structured tests
- Split logic into testable components for better design

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

## 🙌 Contributing

Contributions and improvements are welcome! Feel free to open an issue or submit a PR.

---

## ✨ Acknowledgments

Inspired by best practices from the embedded community and the power of PlatformIO.

---

Happy testing! 🧪⚙️