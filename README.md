# Big Integer Project for PPT 2023 course



<details>
<summary>Compile and run C++ on VS Code on Mac</summary>

0. Install C/C++ extension for VS Code: `ms-vscode.cpptools`(extension ID)

0. Check clang/g++ installed
<code>clang --version</code>
<code>g++ --version</code>

1. Install xcode-select (support clang and g++)
<code>xcode-select --install</code>



2. In `.vscode` folder, create
- `tasks.json` file (compiler buuild settings)
- `launch.json` file (debugger settings)
- `c_cpp_properties.json` file (compiler path and IntelliSense setting)

2. Compile code and create an executable file
<code> g++ filename.cpp -o outputname </code>

3. Run code
- Method 1:
    `./outputname`
- Method 2:
    - press F5: open debug tab
    - select "C++ (GDB/LLDB)" as your debugging environment
    - click the green "Play" button to compile and run your code
</details>



