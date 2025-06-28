## 🛠️ How to Compile and Run the OS Simulator

Follow these steps to compile and run the project using `clang++`.

### 1. 📥 Download the Repository

- **Option 1:** Download as ZIP:
  1. Click the green **"Code"** button on the repository page.
  2. Select **"Download ZIP"**.
  3. Extract the contents after downloading.

- **Option 2:** Clone using Git:
  ```bash
  git clone <repo-url>
  ```

---

### 2. 📂 Navigate to the Project Folder

If you used Git:
```bash
cd <repo-name>
```

If you downloaded the ZIP, open the extracted folder manually.

---

### 3. 🔁 Go to the `csopesy_os_mco` Directory

```bash
cd csopesy_mco_o
```

---

### 4. 🧪 Compile the Code

Use `clang++` to compile all `.cpp` files in the directory:

```bash
clang++ -std=c++20 -I . *.cpp -o my_os.exe
```

**Explanation:**
- `-std=c++20`: Enables C++20 features.
- `-I .`: Adds the current directory to the header include path.
- `*.cpp`: Includes all C++ source files in the current directory.
- `-o my_os.exe`: Sets the output executable file name.

---

### 5. 🚀 Run the Program

```bash
./my_os.exe
```

> On Windows, you can run it by just typing:
> ```
> my_os.exe
> ```

---

Let me know if you'd like a `Makefile` or compilation script included!
