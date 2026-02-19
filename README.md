# 🐧 System Programming in Linux

## 📖 Overview

Welcome to the **System Programming in Linux** repository! This is a comprehensive learning roadmap and hands-on practice guide designed to take you from the basics of Linux architecture to advanced system-level concepts.

Whether you are a student, an embedded systems enthusiast, or preparing for technical interviews, this repository provides the **study materials, notes, and practical code examples** you need to master:

- **Linux Architecture & Fundamentals**

- **Process Management & System Calls**

- **Memory Management & File Systems**

- **Shell Implementation**

- **Object-Oriented Programming (OOP) in C**

- **RTOS Concepts & Custom Linux Distro Building**

## 📂 Repository Structure

The content is organized into phases to ensure a structured learning path.

```
System-programming-in-Linux/
│
├── 🔹 Phase_1/ (Fundamentals & Process Management)
│   ├── 00_Material/        # PDFs: Linux basics, OS architecture, build systems
│   └── 01_Code/            # Practical C implementations
│       ├── args/           # Handling argv & envp
│       ├── fork_demo/      # Process creation using fork()
│       ├── zombie_demo/    # Understanding & handling zombie processes
│       ├── mycat, mycp...  # Re-implementing standard Linux utilities
│       ├── Own_Shell/      # Micro/Nano/Pico shell implementations
│       ├── syscall64.nasm  # Assembly x86_64 system call examples
│       └── simple_shell.c  # Basic shell implementation logic
│
├── 🔸 Phase_2/ (Advanced Concepts & Embedded Linux)
│   ├── 00_Material/        # Advanced docs: FS, Memory, RTOS, Git
│   ├── 01_Code/
│   │   ├── ch07_Memory/    # Allocators: malloc, free, sbrk
│   │   ├── ch41_libs/      # Static vs Dynamic libraries linking
│   │   ├── OOP_in_C/       # Implementing OOP concepts using C
│   │   ├── RTOS/           # Introduction to RTOS (STM32CubeWBA)
│   │   └── Simple_Distro/  # Building a Distro: Kernel + BusyBox + Buildroot
│   └── 02_Documents/       # Deep dive notes: FAT32, QEMU, Permissions, MISRA C
```

## 🎯 Learning Objectives

By following this roadmap, you will be able to:

- [x] Understand the internal architecture of the **Linux OS**.

- [x] Master process lifecycle management (**fork, exec, wait, zombies**).

- [x] Interact directly with the kernel using **System Calls**.

- [x] Re-create standard Linux tools (`cat`, `cp`, `mv`, `echo`, `pwd`).

- [x] Build your own **Command Line Shell**.

- [x] Dive deep into **Memory Allocation** internals (`sbrk`, `malloc`).

- [x] Understand **Static vs. Dynamic** linking.

- [x] Apply **Object-Oriented Programming** principles in pure C.

- [x] Build a minimal **Linux Distribution** from scratch.

- [x] Get a solid introduction to **RTOS** concepts.

## 🛠️ Requirements

To run the examples and build the projects, you will need:

- **Operating System:** Linux (Native / VM / WSL).

- **Compiler:** GCC Toolchain.

- **Knowledge:** Basic C programming skills.

- **(Optional):** QEMU (for running custom Linux images/kernels).

## ▶️ How to Build & Run

Here are some examples of how to compile and run the code provided in this repository.

### 1. Basic Utilities (e.g., `mycat`)

Re-implementation of the `cat` command.

```bash
gcc mycat.c -o mycat
./mycat test.txt
```

### 2. Process Management (`fork`)

Demonstration of creating new processes.

```bash
gcc fork_demo.c -o fork_demo
./fork_demo
```

### 3. Running the Shell

Compiling and running the custom shell implementation.

```bash
gcc simple_shell.c -o simple_shell
./simple_shell
# Now you can type commands inside your own shell!
```

> **Note:** Check `README.compile_Commands` inside specific folders for more complex build instructions.

## 📚 Topics Covered

|Category|Topics|
|---|---|
|**Core Linux**|OS Architecture, File Descriptors, Permissions, Users|
|**Processes**|Lifecycle, Fork/Exec, Zombies, Wait, Signals|
|**System Calls**|x86_64 Syscalls, Direct Kernel Interaction|
|**Memory**|Heap Management, `malloc`/`free`, `sbrk`|
|**Libraries**|Static (`.a`) vs Dynamic (`.so`) Linking|
|**Advanced C**|Object-Oriented C, MISRA C Standards|
|**Embedded/OS**|Buildroot, BusyBox, Kernel Building, RTOS Basics, QEMU|

## 🧠 Who Is This For?

- 🎓 **Students** studying Operating Systems or System Programming.

- 🤖 **Embedded Linux Learners** wanting to understand the OS under the hood.

- 💼 **Job Seekers** preparing for Linux/Embedded Systems interviews.

- 💻 **Developers** curious about how Linux works internally.

## 🤝 Contribution

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project.

2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`).

3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`).

4. Push to the Branch (`git push origin feature/AmazingFeature`).

5. Open a Pull Request.

## ⭐ Support

If you found this repository helpful, please consider showing your support:

- Give it a **Star** ⭐ on GitHub!

- Share it with friends and colleagues.

- Use it as a reference for your projects.

## 📜 License

This repository is intended for **educational purposes**. All materials, PDFs, and references belong to their respective owners.

Happy Coding! 🐧
